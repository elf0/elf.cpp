#ifndef MUTEX_H
#define MUTEX_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#ifdef __linux__
#include <pthread.h>
#else
#endif

namespace thread{

class Mutex{
public:
  Mutex(){
#ifdef __linux__
    _mutex = PTHREAD_MUTEX_INITIALIZER;
#else
#endif
  }

  ~Mutex(){
#ifdef __linux__
    pthread_mutex_destroy(&_mutex);
#else
#endif
  }

  void Lock(){
#ifdef __linux__
    pthread_mutex_lock(&_mutex);
#else
#endif
  }

  void Unlock(){
#ifdef __linux__
    pthread_mutex_unlock(&_mutex);
#else
#endif
  }

private:
#ifdef __linux__
  pthread_mutex_t _mutex;
#else
#endif
  friend class Condition;
};

} //namespace thread

#endif // MUTEX_H
