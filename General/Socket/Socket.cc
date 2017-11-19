//
// Socket.cc
//

#include <unistd.h>

#include "Socket.hh"

using namespace BSD;

Socket::Socket(int domain, int type, int protocol/*=0*/) :
        socket(::socket(domain,type,protocol)) {
} // Socket::Socket(domain, type, protocol)

Socket::Socket(Socket &socket) :
        socket(socket.socket) {
   socket.socket = Invalid;
} // Socket::Socket(Socket)

void Socket::Close() {
   if (!Valid()) {
      return;
   } // if
   ::close(socket);
} // Socket::Close()
