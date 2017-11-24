//
// Socket.hh

#ifndef Socket_hh
#define Socket_hh

#include "../FD/FD.hh"

#include "BSD.hh"

class BSD::Socket : public FD {

protected: // Methods

   explicit Socket(FD::Type fd);

   Socket(int domain, int type, int protocol=0);

   explicit Socket(Socket &socket) = default;

   bool Set(int level, int option, bool on);

   bool Set(int level, int option, void *data, socklen_t size);

}; // Socket

#endif // Socket_hh
