//
// NoMutex.hh
//

#ifndef NoMutex_hh
#define NoMutex_hh

#include "MT.hh"

class MT::NoMutex : private _Mutex_ {

public: // Classes

   class Lock; friend Lock;

public: // Methods

   inline NoMutex() = default;

   inline ~NoMutex() = default;

private: // Methods

   NoMutex(const NoMutex &) = delete;

}; // NoMutex

#endif // NoMutex_hh
