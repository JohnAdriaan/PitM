//
// Mutex.hh
//

#ifndef Mutex_hh
#define Mutex_hh

#include <pthread.h>

#include "General.hh"

class Mutex : private _Mutex_ {

public: // Classes

   class Lock; friend Lock;

public: // Methods

   Mutex();

   ~Mutex();

private: // Methods

   Mutex(const Mutex &) = delete;

private: // Variables

   pthread_mutex_t mutex;

}; // Mutex

#endif // Mutex_hh
