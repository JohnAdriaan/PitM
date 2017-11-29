//
// Semaphore.cc
//

#include <fcntl.h>    // For O_*
#include <sys/stat.h> // For S_*

#include "Semaphore.hh"

//static const int share = S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH;
static const int share = S_IREAD|S_IWRITE;

using namespace MT;

Semaphore::Semaphore(unsigned start/*=0*/) :
           name(nullptr),
           sem(),
           semaphore(&sem) {
   ::sem_init(semaphore, 0, start);
} // Semaphore::Semaphore(start)

Semaphore::Semaphore(const char *name,
                     Creates create/*=Exists*/,
                     unsigned start/*=0*/) :
           name(name),
           sem(),
           semaphore(SEM_FAILED) {
   int oflags;
   switch (create) {
   case Exists :
      oflags = O_RDWR;
      break;
   case Create :
      oflags = O_RDWR|O_CREAT;
      break;
   case Exclusive :
      oflags = O_RDWR|O_CREAT|O_EXCL;
      break;
   } // switch
   semaphore = ::sem_open(name, oflags, share, start);
} // Semaphore::Semaphore(name,create,start)

Semaphore::Semaphore(const char *name, bool &already) :
           Semaphore(name, Exclusive) { // Attempt to create
   already = false;
   if (Valid()) {
      return;
   } // if
   semaphore = ::sem_open(name, O_RDWR);     // Attempt to open existing
   if (Valid()) {
      already = true;
      return;
   } // if
   semaphore = ::sem_open(name, O_RDWR|O_CREAT, share, 0); // Something's odd...
} // Semaphore::Semaphore(name,already)

bool Semaphore::Post() {
   if (!Valid()) {
      return false;
   } // if
   return ::sem_post(semaphore)==0;
} // Semaphore::Post()

bool Semaphore::Try() {
   if (!Valid()) {
      return false;
   } // if
   return ::sem_trywait(semaphore)==0;
} // Semaphore::Try()

void Semaphore::Wait() {
   if (!Valid()) {
      return;
   } // if
   ::sem_wait(semaphore);
} // Semaphore::Wait()

void Semaphore::Unlink() {
   if (name==nullptr) {
      return;
   } // if
   ::sem_unlink(name);
} // Semaphore::Unlink()

void Semaphore::Close() {
   sem_t *temp = semaphore;
   semaphore = SEM_FAILED;
   if (temp==SEM_FAILED) {
      return;
   } // if
   if (temp!=&sem) { // Named?
      ::sem_close(temp);
   } // if
   else {
      ::sem_destroy(temp);
   } // else
} // Semaphore::Close()

Semaphore::~Semaphore() {
   Close();
} // Semaphore::~Semaphore()
