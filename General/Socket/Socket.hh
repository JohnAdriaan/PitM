//
// Socket.hh

#ifndef Socket_hh
#define Socket_hh

#include "../FD/FD.hh"

#include "BSD.hh"

class BSD::Socket : public FD {

protected: // Methods

   Socket(int domain, int type, int protocol=0);

   Socket(Socket &socket);

}; // Socket

#endif // Socket_hh
