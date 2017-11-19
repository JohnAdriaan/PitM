//
// Mutex.Lock.cc
//

#include "Mutex.Lock.hh"

Mutex::Lock::Lock(Mutex &mutex) :
             mutex(mutex) {
   pthread_mutex_lock(&mutex.mutex);
} // Lock::Lock(Mutex)

Mutex::Lock::~Lock() {
   pthread_mutex_unlock(&mutex.mutex);
} // Lock::~Lock()
