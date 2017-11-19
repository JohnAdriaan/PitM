//
// Listen.hh
//

#ifndef Listen_hh
#define Listen_hh

#include "Socket.hh"
#include "Address.hh"

class BSD::Listen : public Socket {

protected: // Methods

   explicit Listen(Port port);

   Listen(const Address &address, Port port);

   virtual ~Listen();

private: // Hooks

   hook void Heard(TCP &client) pure;

}; // Listen

#endif // Listen_hh
