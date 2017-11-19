//
// Mutex.Lock.hh
//

#ifndef Mutex_Lock_hh
#define Mutex_Lock_hh

#include "Mutex.hh"

class Mutex::Lock : private Mutex::_Lock_ {

public: // Methods

   explicit Lock(Mutex &mutex);

   ~Lock();

private: // Methods

   Lock(const Lock &) = delete;

private: // Variables

   Mutex &mutex;

}; // Mutex::Lock

#endif // Mutex_Lock_hh
