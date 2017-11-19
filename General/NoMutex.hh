//
// NoMutex.hh
//

#ifndef NoMutex_hh
#define NoMutex_hh

#include "General.hh"

class NoMutex : private _Mutex_ {

public: // Classes

   class Lock; friend Lock;

public: // Methods

   inline NoMutex() = default;

   inline ~NoMutex() = default;

private: // Methods

   NoMutex(const NoMutex &) = delete;

}; // NoMutex

#endif // NoMutex_hh
