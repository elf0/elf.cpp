#ifndef HASHSET_H
#define HASHSET_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "RBTree.h"

//SET_HASH_WIDTH must be 2, 4, 8, 16 , 32, ..
#ifndef SET_HASH_WIDTH
#define SET_HASH_WIDTH 16
#endif

#define SET_HASH_INDEX(size) (size & (SET_HASH_WIDTH - 1))

template<typename TNode>
class HashSet{
public:
  typedef typename RBTree<TNode>::Node Node;
  typedef E8 (*ForEach_f) (void *pContext, Node *pNode);

  inline HashSet();
  inline ~HashSet();
  inline  Node *Find(const Byte *pKey, U32 uKey);
  inline E8 ForEach(ForEach_f onEach, void *pContext);
  inline  E8 Add(const Byte *pKey, U32 uKey
                 , void *pContext, Node **ppNode);
  inline void Remove(U32 uKey, Node *pNode);
  U32 Count(){
    return _uCount;
  }

private:
  RBTree<TNode> _trees[SET_HASH_WIDTH];
  U32 _uCount = 0;
};

template<typename TNode>
HashSet<TNode>::HashSet(){
}

template<typename TNode>
HashSet<TNode>::~HashSet(){
}

template<typename TNode>
typename HashSet<TNode>::Node *HashSet<TNode>::Find(const Byte *pKey, U32 uKey){
  return (Node *)_trees[SET_HASH_INDEX(uKey)].Find(pKey, uKey);
}

template<typename TNode>
E8 HashSet<TNode>::ForEach(ForEach_f onEach, void *pContext){
  E8 e;
  RBTree<TNode> *pTree = _trees;
  RBTree<TNode> *ptEnd = pTree + SET_HASH_WIDTH;
  while(pTree != ptEnd){
    e = pTree++->ForEach(onEach, pContext);
    if(e)
      return e;
  }

  return 0;
}

template<typename TNode>
E8 HashSet<TNode>::Add(const Byte *pKey, U32 uKey
                       , void *pContext, Node **ppNode){
  E8 e = _trees[SET_HASH_INDEX(uKey)].Add(pKey, uKey
                                          , pContext, (typename RBTree<TNode>::Node**)ppNode);
  if(e)
    return e;

  ++_uCount;
  return 0;
}

//Do NOT use "Remove()"! RBTree_Remove not implement yet
template<typename TNode>
void HashSet<TNode>::Remove(U32 uKey, Node *pNode){
  _trees[SET_HASH_INDEX(uKey)].Remove((typename RBTree<TNode>::Node*)pNode);
  --_uCount;
}

#endif // HASHSET_H
