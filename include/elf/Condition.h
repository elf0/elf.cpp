#ifndef CONDITION_H
#define CONDITION_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#ifdef __linux__
#include <pthread.h>
#else
#endif

namespace thread{

class Condition{
public:
  Condition(){
#ifdef __linux__
    _condition = PTHREAD_COND_INITIALIZER;
#else
#endif
  }

  ~Condition(){
#ifdef __linux__
    pthread_cond_destroy(&_condition);
#else
#endif
  }

  void Wait(Mutex *pMutex){
#ifdef __linux__
    pthread_cond_wait(&_condition, &pMutex->_mutex);
#else
#endif
  }

  void Signal(){
#ifdef __linux__
    pthread_cond_signal(&_condition);
#else
#endif
  }

  void SignalAll(){
#ifdef __linux__
    pthread_cond_broadcast(&_condition);
#else
#endif
  }
private:
#ifdef __linux__
  pthread_condition_t _condition;
#else
#endif
};

} //namespace thread

#endif // CONDITION_H
