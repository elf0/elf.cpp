#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{

template<typename T>
class SharedObject;

template<typename T>
class SharedPointer{
public:
 SharedPointer(){};

 SharedPointer(const SharedPointer &other)
  : _pObject(other._pObject){
  //fprintf(stderr, "Copy constructor\n");
  Reference();
 }

 ~SharedPointer(){Dereference();}

 SharedPointer &operator=(const SharedPointer &other){
  //fprintf(stderr, "operator=()\n");
  Dereference();
  _pObject = other._pObject;
  Reference();
  return *this;
 }

 bool SameObject(const SharedPointer &other)const{
  return _pObject == other._pObject;
 }

 bool NotSameObject(const SharedPointer &other)const{
  return _pObject != other._pObject;
 }

 T &operator*()const{
  return *(T*)_pObject;
 }

 T *operator->()const{
  return _pObject;
 }

 bool operator==(const SharedPointer &other)const{
  return *this == *other;
 }

 bool operator!=(const SharedPointer &other)const{
  return *this != *other;
 }

//for container
 bool operator<(const SharedPointer &other)const{
  return *this < *other;
 }

 bool operator>(const SharedPointer &other)const{
  return *this > *other;
 }

 bool operator<=(const SharedPointer &other)const{
  return *this <= *other;
 }

 bool operator>=(const SharedPointer &other)const{
  return *this >= *other;
 }

private:
 SharedPointer(SharedObject<T> *pObject): _pObject(pObject){
//  fprintf(stderr, "SharedPointer(SharedObject<T> *pObject)\n");
 }

 void Reference(){
  if(_pObject)
   _pObject->Reference();
 }

 void Dereference(){
  if(_pObject)
   _pObject->Dereference();
 }
 SharedObject<T> *_pObject = nullptr;
 friend class SharedObject<T>;
};

template<typename T>
class SharedObject: public T{
public:
 static SharedPointer<T> New(const T &object){
  return SharedPointer<T>(new SharedObject(object));
 }

 void Reference(){
  ++_nCount;
  //fprintf(stderr, "Reference: %lu\n", _nCount);
 }

 void Dereference(){
  //fprintf(stderr, "Dereference: %lu\n", _nCount);
  if(--_nCount == 0)
   delete this;
 }

private:
 SharedObject(const T &object): T(object){}
 ~SharedObject(){}

 size_t _nCount = 1;
};

}//namespace elf
