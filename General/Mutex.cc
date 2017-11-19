//
// Mutex.cc
//

#include "Mutex.hh"

Mutex::Mutex() {
   pthread_mutex_init(&mutex, nullptr);
} // Mutex::Mutex()

Mutex::~Mutex() {
   pthread_mutex_destroy(&mutex);
} // Mutex::~Mutex()
