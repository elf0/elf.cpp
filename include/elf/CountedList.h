#ifndef COUNTEDLIST_H
#define COUNTEDLIST_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "Type.h"
#include "List.h"

class CountedList: private List{
public:
  U Count(){
    return _uCount;
  }

  void Reset(){
    List::Reset();
    _uCount = 0;
  }

  bool Empty(){
    return List::Empty();
  }

  bool NotEmpty(){
    return List::NotEmpty();
  }

  DoubleNode *Front(){
    return List::Front();
  }

  DoubleNode *Back(){
    return List::Back();
  }

  bool First(DoubleNode *pNode){
    return List::First(pNode);
  }

  bool NotFirst(DoubleNode *pNode){
    return List::NotFirst(pNode);
  }

  bool Last(DoubleNode *pNode){
    return List::Last(pNode);
  }

  bool NotLast(DoubleNode *pNode){
    return List::NotLast(pNode);
  }

  void Insert(DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    List::Insert(pNode, pPrev, pNext);
    ++_uCount;
  }

  void Remove(DoubleNode *pNode){
    List::Remove(pNode);
    --_uCount;
  }

  //push back
  void Push(DoubleNode *pNode){
    List::Push(pNode);
    ++_uCount;
  }

  //pop front
  DoubleNode *Pop(){
    DoubleNode *pNode = List::Pop();
    --_uCount;
    return pNode;
  }

  void PopTo(CountedList *pTarget){
    List::PopTo(pTarget);
    --_uCount;
    ++pTarget->_uCount;
  }

  void PushFront(DoubleNode *pNode){
    List::PushFront(pNode);
    ++_uCount;
  }

  DoubleNode *PopBack(){
    DoubleNode *pNode = List::PopBack();
    --_uCount;
    return pNode;
  }

  void MoveNodeToBack(DoubleNode *pNode){
    List::MoveNodeToBack(pNode);
  }

  void MoveNodeToFront(DoubleNode *pNode){
    List::MoveNodeToFront(pNode);
  }

  //move to back
  void MoveTo(CountedList *pTarget){
    List::MoveTo(pTarget);
    pTarget->_uCount += _uCount;
    _uCount = 0;
  }

  void MoveToFront(CountedList *pTarget){
    List::MoveToFront(pTarget);
    pTarget->_uCount += _uCount;
    _uCount = 0;
  }

  void Swap(CountedList *pTarget){
    List::Swap(pTarget);
    U nTmp = _uCount;
    _uCount = pTarget->_uCount;
    pTarget->_uCount = nTmp;
  }
private:
  U _uCount = 0;
};

#endif //COUNTEDLIST_H
