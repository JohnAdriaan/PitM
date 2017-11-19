//
// Semaphore.hh
//

#ifndef Semaphore_hh
#define Semaphore_hh

#include <semaphore.h>

#include "MT.hh"

class MT::Semaphore : private _MT_ {

public: // Methods

   explicit Semaphore(unsigned start=0);

   bool Post();

   bool Try();

   void Wait();

   ~Semaphore();

private: // Methods

   Semaphore(const Semaphore &) = delete;

private: // Variables

   sem_t semaphore;

}; // Semaphore

#endif // Semaphore_hh
