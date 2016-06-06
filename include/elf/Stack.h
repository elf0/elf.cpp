#ifndef STACK_H
#define STACK_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

class Stack{
public:
  struct Node{
    struct Node *pNext;
  };

  void Clear(){
    _pTop = (Node*)this;
  }

  bool Empty(){
    return _pTop == (Node*)this;
  }

  bool NotEmpty(){
    return _pTop != (Node*)this;
  }

  Node *Top(){
    return _pTop;
  }

  void Push(Node *pNode){
    pNode->pNext = _pTop;
    _pTop = pNode;
  }

  Node *Pop(){
    Node *pNode = _pTop;
    _pTop = pNode->pNext;
    return pNode;
  }
private:
  Node *_pTop = (Node*)this;
};

#endif //STACK_H
