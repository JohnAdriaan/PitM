//
// Listen.hh
//

#ifndef Listen_hh
#define Listen_hh

#include "Socket.hh"

class BSD::Listen : public Socket {

protected: // Methods

   virtual ~Listen();

private: // Hooks

   hook void Heard(TCP &client) pure;

}; // Listen

#endif // Listen_hh
