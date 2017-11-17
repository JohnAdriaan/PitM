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

class NoMutex::Lock : private NoMutex::_Lock_ {

public: // Methods

   inline Lock(NoMutex &);

   inline ~Lock() = default;

private: // Methods

   Lock(const Lock &) = delete;

}; // NoMutex::Lock

inline NoMutex::Lock::Lock(NoMutex &) {
} // Lock::Lock(NoMutex)

#endif // NoMutex_hh
