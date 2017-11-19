//
// NoMutex.Lock.hh
//

#ifndef NoMutex_Lock_hh
#define NoMutex_Lock_hh

#include "NoMutex.hh"

class NoMutex::Lock : private NoMutex::_Lock_ {

public: // Methods

   inline Lock(NoMutex &);

   inline ~Lock() = default;

private: // Methods

   Lock(const Lock &) = delete;

}; // NoMutex::Lock

inline NoMutex::Lock::Lock(NoMutex &) {
} // Lock::Lock(NoMutex)

#endif // NoMutex_Lock_hh
