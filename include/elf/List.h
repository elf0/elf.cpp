#ifndef LIST_H
#define LIST_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "DoubleNode.h"

class List{
public:
  List(){
  }

  void Reset(){
    _entry.Reset();
  }

  bool Empty(){
    return _entry.Alone();
  }

  bool NotEmpty(){
    return _entry.NotAlone();
  }

  DoubleNode *Front(){
    return _entry.Next();
  }

  DoubleNode *Back(){
    return _entry.Prev();
  }

  bool First(DoubleNode *pNode){
    return pNode->Prev() == &_entry;
  }

  bool NotFirst(DoubleNode *pNode){
    return pNode->Prev() != &_entry;
  }

  bool Last(DoubleNode *pNode){
    return pNode->Next() == &_entry;
  }

  bool NotLast(DoubleNode *pNode){
    return pNode->Next() != &_entry;
  }

  void Insert(DoubleNode *pNode, DoubleNode *pPrev, DoubleNode *pNext){
    pNode->Link(pPrev, pNext);
  }

  void Remove(DoubleNode *pNode){
    pNode->Unlink();
  }

  //push back
  void Push(DoubleNode *pNode){
    pNode->Link(_entry.Prev(), &_entry);
  }

  //pop front
  DoubleNode *Pop(){
    DoubleNode *pNode = _entry.Next();
    pNode->Unlink();
    return pNode;
  }

  void PushFront(DoubleNode *pNode){
    pNode->Link(&_entry, _entry.Next());
  }

  DoubleNode *PopBack(){
    DoubleNode *pNode = _entry.Prev();
    pNode->Unlink();
    return pNode;
  }

  void MoveNodeToFront(DoubleNode *pNode){
    Remove(pNode);
    PushFront(pNode);
  }

  //move to back
  void MoveTo(List *pTarget){
    _entry.MoveBuddiesTo(&pTarget->_entry);
  }

  void MoveToFront(List *pTarget){
    _entry.MoveBuddiesToFront(&pTarget->_entry);
  }

  void Swap(List *pTarget){
    _entry.SwapBuddies(&pTarget->_entry);
  }
private:
  DoubleNode _entry;
};

#endif //LIST_H

