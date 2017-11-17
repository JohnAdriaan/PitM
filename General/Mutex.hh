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

class Mutex::Lock : private Mutex::_Lock_ {

public: // Methods

   explicit Lock(Mutex &mutex);

   ~Lock();

private: // Methods

   Lock(const Lock &) = delete;

private: // Variables

   Mutex &mutex;

}; // Mutex::Lock

#endif // Mutex_hh
