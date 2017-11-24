//
// TCP.hh
//

#ifndef TCP_hh
#define TCP_hh

#include "Socket.hh"

class BSD::TCP : public Socket {

protected: // Methods

   explicit TCP(TCP &tcp) = default;

private: // Listen hooks

   friend Listen;

   explicit TCP(FD::Type client);

}; // TCP

#endif // TCP_hh
