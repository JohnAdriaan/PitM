//
// Listen.hh
//

#ifndef Listen_hh
#define Listen_hh

#include "Socket.hh"
#include "Address.hh"

class BSD::Listen : public Socket {

protected: // Methods

   Listen(const Address &address, Port port);

   bool Accept();

   virtual ~Listen();

private: // Overrides

   overrides void Readable();

private: // Hooks

   hook void Heard(TCP &client, const Address &address) pure;

}; // Listen

#endif // Listen_hh
