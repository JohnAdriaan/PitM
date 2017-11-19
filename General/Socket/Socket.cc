//
// Socket.cc
//

#include "Socket.hh"

using namespace BSD;

Socket::Socket(int domain, int type, int protocol/*=0*/) :
        FD(::socket(domain,type,protocol)) {
} // Socket::Socket(domain, type, protocol)

Socket::Socket(Socket &socket) :
        FD(socket) {
} // Socket::Socket(Socket)
