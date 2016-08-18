#ifndef RBTREE_H
#define RBTREE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#define RBTREE_NODE_PARENT(pNode) (*(Node**)pNode)
#define RBTREE_NODE_SET_PARENT(pNode, pParent) (RBTREE_NODE_PARENT(pNode) = pParent)

template<typename TNode>
class RBTree{
public:
  struct Node{
    Node *pParent;
    Node *pLeft;
    Node *pRight;
    B bRed;
  };

  typedef E8 (*ForEach_f) (void *pContext, Node *pNode);
  typedef E8 (RBTree::*Add_f) (const Byte *pKey, U32 uKey, void *pContext, Node **ppNode);

  inline RBTree();
  inline ~RBTree();
  inline Node *Find(const Byte *pKey, U32 uKey);
  inline E8 ForEach(ForEach_f onEach, void *pContext);
  inline B Add(const Byte *pKey, U32 uKey, void *pContext, Node **ppNode);
  inline void Remove(Node *pNode);

private:
  inline E8 ForEach(Node *pNode, ForEach_f onEach, void *pContext);


  inline E8 AddRoot(const Byte *pKey, U32 uKey
                    , void *pContext, Node **ppNode);
  inline E8 AddChild(const Byte *pKey, U32 uKey
                     , void *pContext, Node **ppNode);
  inline void Balance(Node *pNode);
  static inline void DeleteNode(Node *pNode);

  Node     *_pRoot = 0;
  Add_f   _fAdd = &RBTree::AddRoot;
};

template<typename TNode>
RBTree<TNode>::RBTree(){
}

template<typename TNode>
void RBTree<TNode>::DeleteNode(RBTree<TNode>::Node *pNode){
  if(pNode->pLeft)
    DeleteNode(pNode->pLeft);

  if(pNode->pRight)
    DeleteNode(pNode->pRight);

  TNode::Delete((TNode*)pNode);
}

template<typename TNode>
RBTree<TNode>::~RBTree(){
  if(_pRoot)
    DeleteNode(_pRoot);
}

template<typename TNode>
typename RBTree<TNode>::Node *RBTree<TNode>::Find(const Byte *pKey, U32 uKey){
  Node *pNode = _pRoot;
  I8 iCompare;
  while(pNode){
    iCompare = TNode::Compare(pKey, uKey, pNode);
    if(iCompare < 0)
      pNode = pNode->pLeft;
    else if(iCompare > 0)
      pNode = pNode->pRight;
    else
      return pNode;
  }

  return 0;
}

template<typename TNode>
E8 RBTree<TNode>::ForEach(ForEach_f onEach, void *pContext){
  if(_pRoot)
    return ForEach(_pRoot, onEach, pContext);

  return 0;
}

template<typename TNode>
E8 RBTree<TNode>::ForEach(Node *pNode, ForEach_f onEach, void *pContext){
  E8 e;

  if(pNode->pLeft){
    e = ForEach(pNode->pLeft, onEach, pContext);
    if(e)
      return e;
  }

  e = onEach(pContext, pNode);
  if(e)
    return e;

  if(pNode->pRight){
    e = ForEach(pNode->pRight, onEach, pContext);
    if(e)
      return e;
  }

  return 0;
}

template<typename TNode>
E8 RBTree<TNode>::Add(const Byte *pKey, U32 uKey
                      , void *pContext, Node **ppNode){
  return (this->*_fAdd)(pKey, uKey, pContext, ppNode);
}

template<typename TNode>
E8 RBTree<TNode>::AddRoot(const Byte *pKey, U32 uKey
                          , void *pContext, Node **ppNode){
  Node *pNew = (Node*)TNode::New(pContext, pKey, uKey);
  pNew->pLeft = 0;
  pNew->pRight = 0;
  RBTREE_NODE_SET_PARENT(pNew, 0);
  pNew->bRed = 0;

  _pRoot = pNew;
  _fAdd = &RBTree::AddChild;

  *ppNode = pNew;
  return 0;
}

template<typename TNode>
E8 RBTree<TNode>::AddChild(const Byte *pKey, U32 uKey
                           , void *pContext, Node **ppNode){
  Node *pNew;
  Node *pNode = _pRoot;
  I8 iCompare;
  while(1){
    iCompare = TNode::Compare(pKey, uKey, (TNode*)pNode);
    if(iCompare < 0){
      if(pNode->pLeft)
        pNode = pNode->pLeft;
      else{
        pNode->pLeft = pNew = (Node*)TNode::New(pContext, pKey, uKey);
        break;
      }
    }
    else if(iCompare > 0){
      if(pNode->pRight)
        pNode = pNode->pRight;
      else{
        pNode->pRight = pNew = (Node*)TNode::New(pContext, pKey, uKey);
        break;
      }
    }
    else{
      *ppNode = pNode;
      return 1;
    }
  }

  RBTREE_NODE_SET_PARENT(pNew, pNode);
  pNew->pLeft = 0;
  pNew->pRight = 0;
  pNew->bRed = 1;

  Balance(pNew);
  *ppNode = pNew;
  return 0;
}

//Do NOT use "Remove()"! Not implement yet
template<typename TNode>
void RBTree<TNode>::Remove(Node *pNode){

}

template<typename TNode>
void RBTree<TNode>::Balance(Node *pNode){
  Node *pGreatGrandparent;
  Node *pGrandparent;
  Node *pUncle;
  Node *pParent;
  Node *pLeft;
  Node *pRight;
  while((pParent = RBTREE_NODE_PARENT(pNode))->bRed){
    pGrandparent = RBTREE_NODE_PARENT(pParent);
    pGreatGrandparent = pGrandparent->pParent;
    if(pParent == pGrandparent->pLeft){
      pUncle = pGrandparent->pRight;
      if(pUncle && pUncle->bRed){
        pUncle->bRed = 0;
        if(pGreatGrandparent)
          pNode = pGrandparent;
        else{
          pParent->bRed = 0;
          break;
        }
      }
      else{
        Node  *pParentRight = pParent->pRight;
        if(pNode == pParentRight){
          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pLeft)
              pGreatGrandparent->pLeft = pNode;
            else
              pGreatGrandparent->pRight = pNode;
          }
          else
            _pRoot = pNode;

          RBTREE_NODE_SET_PARENT(pGrandparent, pNode);
          pRight = pNode->pRight;
          pGrandparent->pLeft = pRight;
          if(pRight)
            RBTREE_NODE_SET_PARENT(pRight, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pNode);
          pLeft = pNode->pLeft;
          pParent->pRight = pLeft;
          if(pLeft)
            RBTREE_NODE_SET_PARENT(pLeft, pParent);

          pNode->bRed = 0;
          RBTREE_NODE_SET_PARENT(pNode, pGreatGrandparent);
          pNode->pLeft = pParent;
          pNode->pRight = pGrandparent;
        }
        else{
          RBTREE_NODE_SET_PARENT(pGrandparent, pParent);
          pGrandparent->pLeft = pParentRight;

          if(pParentRight)
            RBTREE_NODE_SET_PARENT(pParentRight, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pGreatGrandparent);
          pParent->pRight = pGrandparent;

          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pLeft)
              pGreatGrandparent->pLeft = pParent;
            else
              pGreatGrandparent->pRight = pParent;
          }
          else
            _pRoot = pParent;

          pParent->bRed = 0;
        }
        pGrandparent->bRed = 1;
        break;
      }
    }
    else{
      pUncle = pGrandparent->pLeft;
      if(pUncle && pUncle->bRed){
        pUncle->bRed = 0;
        if(pGreatGrandparent)
          pNode = pGrandparent;
        else{
          pParent->bRed = 0;
          break;
        }
      }
      else{
        Node  *pParentLeft = pParent->pLeft;
        if(pNode == pParentLeft){
          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pLeft)
              pGreatGrandparent->pLeft = pNode;
            else
              pGreatGrandparent->pRight = pNode;
          }
          else
            _pRoot = pNode;

          RBTREE_NODE_SET_PARENT(pGrandparent, pNode);
          pLeft = pNode->pLeft;
          pGrandparent->pRight = pLeft;
          if(pLeft)
            RBTREE_NODE_SET_PARENT(pLeft, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pNode);
          pRight = pNode->pRight;
          pParent->pLeft = pRight;
          if(pRight)
            RBTREE_NODE_SET_PARENT(pRight, pParent);

          pNode->bRed = 0;
          RBTREE_NODE_SET_PARENT(pNode, pGreatGrandparent);
          pNode->pLeft = pGrandparent;
          pNode->pRight = pParent;

        }
        else{
          RBTREE_NODE_SET_PARENT(pGrandparent, pParent);
          pGrandparent->pRight = pParentLeft;

          if(pParentLeft)
            RBTREE_NODE_SET_PARENT(pParentLeft, pGrandparent);

          RBTREE_NODE_SET_PARENT(pParent, pGreatGrandparent);
          pParent->pLeft = pGrandparent;

          if(pGreatGrandparent){
            if(pGrandparent == pGreatGrandparent->pRight)
              pGreatGrandparent->pRight = pParent;
            else
              pGreatGrandparent->pLeft = pParent;
          }
          else
            _pRoot = pParent;

          pParent->bRed = 0;
        }
        pGrandparent->bRed = 1;
        break;
      }
    }
    pParent->bRed = 0;
    pGrandparent->bRed = 1;
  }
}

#endif // RBTREE_H

