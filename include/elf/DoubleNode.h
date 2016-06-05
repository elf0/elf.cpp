#ifndef DOUBLENODE_H
#define DOUBLENODE_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

class DoubleNode{
public:
  DoubleNode()
    : _pPrev(this)
    , _pNext(this)
  {}

  DoubleNode(DoubleNode *pPrev, DoubleNode *pNext)
    : _pPrev(pPrev)
    , _pNext(pNext)
  {}

  DoubleNode *Next(){
    return _pNext;
  }

  DoubleNode *Prev(){
    return _pPrev;
  }

  void LinkPrev(DoubleNode *pPrev){
    _pPrev = pPrev;
    pPrev->_pNext = this;
  }

  void LinkNext(DoubleNode *pNext){
    _pNext = pNext;
    pNext->_pPrev = this;
  }

  void Link(DoubleNode *pPrev, DoubleNode *pNext){
    LinkPrev(pPrev);
    LinkNext(pNext);
  }

  void Unlink(){
    _pPrev->_pNext = _pNext;
    _pNext->_pPrev = _pPrev;
  }

  void Reset(){
    _pPrev = _pNext = this;
  }

  bool Alone(){
    return _pNext == this;
  }

  bool NotAlone(){
    return _pNext != this;
  }

  void MoveBuddiesTo(DoubleNode *pTargetNode){
    _pNext->LinkPrev(pTargetNode->_pPrev);
    _pPrev->LinkNext(pTargetNode);
    Reset();
  }

  void MoveBuddiesToFront(DoubleNode *pTargetNode){
    _pPrev->LinkNext(pTargetNode->_pNext);
    _pNext->LinkPrev(pTargetNode);
    Reset();
  }

  void SwapBuddies(DoubleNode *pTargetNode){
    DoubleNode tmp = *this;
    *this = *pTargetNode;
    *pTargetNode = tmp;
  }
private:
  DoubleNode *_pNext;
  DoubleNode *_pPrev;
};

#endif //DOUBLENODE_H

