//
// Thread.hh
//

#ifndef Thread_hh
#define Thread_hh

#include <pthread.h>

#include "MT.hh"

class MT::Thread : private _MT_ {

protected: // Methods

   Thread() = default;

   void Start();

   virtual ~Thread() = default;

private: // Hooks

   hook void Run() pure;

private: // Static methods

   static void *Run(void *parameter);

}; // Thread

#endif // Thread_hh
