//
// Semaphore.cc
//

#include "Semaphore.hh"

using namespace MT;

Semaphore::Semaphore(unsigned start/*=0*/) {
   sem_init(&semaphore, 0, start);
} // Semaphore::Semaphore(start)

bool Semaphore::Post() {
   return sem_post(&semaphore)==0;
} // Semaphore::Post()

bool Semaphore::Try() {
   return sem_trywait(&semaphore)==0;
} // Semaphore::Try()

void Semaphore::Wait() {
   sem_wait(&semaphore);
} // Semaphore::Wait()

Semaphore::~Semaphore() {
   sem_destroy(&semaphore);
} // Semaphore::~Semaphore()
