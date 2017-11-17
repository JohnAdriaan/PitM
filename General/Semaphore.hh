//
// Semaphore.hh
//

#ifndef Semaphore_hh
#define Semaphore_hh

#include <semaphore.h>

#include "General.hh"

class Semaphore : private _General_ {

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
