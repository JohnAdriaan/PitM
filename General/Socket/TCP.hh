//
// TCP.hh
//

#ifndef TCP_hh
#define TCP_hh

#include "Socket.hh"

class BSD::TCP : public Socket {

protected: // Methods

   TCP(TCP &tcp);

private: // Listen hooks

   friend Listen;

   TCP(FD::Type client);

}; // TCP

#endif // TCP_hh
