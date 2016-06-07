#ifndef CONDITION_H
#define CONDITION_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#ifdef __linux__
#include <pthread.h>
#else
#include <uv.h>
#endif

namespace thread{

class Condition{
public:
  Condition(){
#ifdef __linux__
    _condition = PTHREAD_COND_INITIALIZER;
#else
    UV_EXTERN int uv_cond_init(&_condition);
#endif
  }

  ~Condition(){
#ifdef __linux__
    pthread_cond_destroy(&_condition);
#else
    uv_cond_destroy(&_condition);
#endif
  }

  void Wait(Mutex *pMutex){
#ifdef __linux__
    pthread_cond_wait(&_condition, &pMutex->_mutex);
#else
    uv_cond_wait(&_condition, &pMutex->_mutex);
#endif
  }

  void Signal(){
#ifdef __linux__
    pthread_cond_signal(&_condition);
#else
    uv_cond_signal(&_condition);
#endif
  }

  void SignalAll(){
#ifdef __linux__
    pthread_cond_broadcast(&_condition);
#else
    uv_cond_broadcast(&_condition);
#endif
  }
private:
#ifdef __linux__
  pthread_cond_t _condition;
#else
  uv_cond_t _condition;
#endif
};

} //namespace thread

#endif // CONDITION_H
