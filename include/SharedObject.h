#pragma once
//License: Public Domain
//Author: elf
//EMail: elf198012@gmail.com

namespace elf{

template<typename T>
class SharedPointer;

template<typename T>
class SharedObject{
public:
 SharedObject(){
  fprintf(stderr, "SharedObject(): %lu\n", _nReferences);
 }
private:
 ~SharedObject(){
  fprintf(stderr, "~SharedObject(): %lu\n", _nReferences);
 }

 void Reference(){
  ++_nReferences;
  fprintf(stderr, "Reference: %lu\n", _nReferences);
 }

 void Dereference(){
  fprintf(stderr, "Dereference: %lu\n", _nReferences);
  if(--_nReferences == 0){
   get()->~T();
   delete this;
  }
 }

 T *get()const{
  return (T*)_object;
 }

 Byte _object[sizeof(T)];
 size_t _nReferences = 1;
 friend class SharedPointer<T>;
};

template<typename T>
class SharedPointer{
public:
// SharedPointer(): _pObject(new SharedObject<T>()){
// }

 SharedPointer(SharedObject<T> *pObject): _pObject(pObject){
 }

 SharedPointer(const SharedPointer &other)
  : _pObject(other._pObject){
  fprintf(stderr, "SharedPointer: Copy constructor\n");
  Reference();
 }

 ~SharedPointer(){
  Dereference();
 }

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

 T *get()const{
  return (T*)_pObject;
 }

 T &operator*()const{
  return *get();
 }

 T *operator->()const{
  return get();
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
 void Reference(){
  if(_pObject)
   _pObject->Reference();
 }

 void Dereference(){
  if(_pObject)
   _pObject->Dereference();
 }
 SharedObject<T> *_pObject = nullptr;
};

#define GET_OVERLOAD_NAME(_1,_2,NAME,...) NAME
#define NewShared(...) GET_OVERLOAD_NAME(__VA_ARGS__, NewShared2, NewShared1)(__VA_ARGS__)

#define NewShared1(TYPE)\
 elf::SharedPointer<TYPE>((elf::SharedObject<TYPE>*)(new (new elf::SharedObject<TYPE>()) TYPE()))

#define NewShared2(TYPE, CONSTRUCTOR_PARAMETERS)\
 elf::SharedPointer<TYPE>((elf::SharedObject<TYPE>*)(new (new elf::SharedObject<TYPE>()) TYPE(CONSTRUCTOR_PARAMETERS)))

}//namespace elf
