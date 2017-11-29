//
// Semaphore.hh
//

#ifndef Semaphore_hh
#define Semaphore_hh

#include <semaphore.h>

#include "MT.hh"

class MT::Semaphore : private _MT_ {

public: // Enums

   enum Creates {
      Exists,   // Open only if exists
      Create,   // Create if doesn't exist
      Exclusive // Only open if doesn't exist
   }; // Creates

public: // Methods

   explicit Semaphore(unsigned start=0);

   explicit Semaphore(const char *name, Creates create=Exists, unsigned start=0);

   Semaphore(const char *name, bool &already);

   inline bool Valid() const;

   bool Post();

   bool Try();

   void Wait();

   void Close();

   // Remove named Semaphore from future Opens
   void Unlink();

   ~Semaphore();

private: // Methods

   explicit Semaphore(const Semaphore &) = delete;

private: // Variables

   const char *name;

   sem_t sem;

   sem_t *semaphore;

}; // Semaphore

inline bool MT::Semaphore::Valid() const {
   return semaphore!=SEM_FAILED;
} // Semaphore::Valid()

#endif // Semaphore_hh
