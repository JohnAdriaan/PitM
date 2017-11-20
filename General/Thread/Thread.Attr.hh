//
// Thread.Attr.hh
//

#ifndef Thread_Attr_hh
#define Thread_Attr_hh

#include "Thread.hh"

class MT::Thread::Attr : private _MT_ {

public: // Enums

   enum States {
      Joinable = PTHREAD_CREATE_JOINABLE, // Default
      Detached = PTHREAD_CREATE_DETACHED
   }; // States

public: // Methods

   inline Attr();

   inline explicit Attr(States state);

   inline operator const pthread_attr_t *() const;

   inline void Set(States state);

   inline ~Attr();

private: // Variables

   pthread_attr_t attr;

}; // Thread::Attr

inline MT::Thread::Attr::Attr() {
   ::pthread_attr_init(&attr);
} // Attr::Attr()

inline MT::Thread::Attr::Attr(States state) :
                         Attr() {
   Set(state);
} // Attr::Attr(States)

inline MT::Thread::Attr::operator const pthread_attr_t *() const {
   return &attr;
} // Attr::operator const pthread_attr_t *

inline void MT::Thread::Attr::Set(States state) {
   ::pthread_attr_setdetachstate(&attr, state);
} // Attr::Set(States)

inline MT::Thread::Attr::~Attr() {
   ::pthread_attr_destroy(&attr);
} // Attr::~Attr()

#endif // Thread_Attr_hh
