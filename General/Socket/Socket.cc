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

Socket::Socket(FD::Type fd) :
        FD(fd) {
} // Socket::Socket(fd)

bool Socket::Set(int level, int option, bool on) {
   int value = on ? 1 : 0;
   return Set(level, option, &value, sizeof value);
} // Socket::Set(level, option, on)

bool Socket::Set(int level, int option, void *data, socklen_t size) {
   return ::setsockopt(fd, level, option, data, size)==0;
} // Socket::Set(option, data, size)
