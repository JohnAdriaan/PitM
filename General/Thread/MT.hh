//
// MT.hh
//

#ifndef MT_hh
#define MT_hh

#include "../General.hh"

namespace MT {

   class _MT_;

   class _Mutex_;

   class Mutex;

   class NoMutex;

   class Semaphore;

   class Thread;

   class _Queue_;

   template <typename Element, class Mutex, class Lock>
   class Queue;

} // MT

class MT::_MT_ : private _General_ {
}; // _MT_

class MT::_Mutex_ : private _MT_ {
protected:
   class _Lock_;
}; // _Mutex_

class MT::_Mutex_::_Lock_ : private _Mutex_ {
}; // _Mutex_::_Lock_

#endif // MT_hh
