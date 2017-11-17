//
// Mutex.cc
//

#include "Mutex.hh"

Mutex::Lock::Lock(Mutex &mutex) :
             mutex(mutex) {
   pthread_mutex_lock(&mutex.mutex);
} // Lock::Lock(Mutex)

Mutex::Lock::~Lock() {
   pthread_mutex_unlock(&mutex.mutex);
} // Lock::~Lock()

Mutex::Mutex() {
   pthread_mutex_init(&mutex, nullptr);
} // Mutex::Mutex()

Mutex::~Mutex() {
   pthread_mutex_destroy(&mutex);
} // Mutex::~Mutex()
