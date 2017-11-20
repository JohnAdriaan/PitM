//
// Listen.cc
//

#include "TCP.hh"
#include "Listen.hh"

using namespace BSD;

Listen::Listen(const Address &address, Port port) :
        Socket(address.Family(), SOCK_STREAM) {
   if (!Valid()) {
      return;
   } // if

   Set(SOL_SOCKET, SO_REUSEADDR, true);

   Address local(address, port);
   if (::bind(fd, local, local.Length())!=0) {
      Close();
      return;
   } // if
   if (::listen(fd, 5)!=0) {
      Close();
      return;
   } // if
} // Listen::Listen(Address,Port)

bool Listen::Accept() {
   if (!Valid()) {
      return false;
   } // if

   Address address;
   socklen_t length = address.Length();
   FD::Type heard = ::accept(fd, address, &length);
   if (heard==-1) {
      return false;
   } // if

   TCP tcp(heard);
   Heard(tcp, address);
   return true;
} // Listen::Accept()

void Listen::Readable() {
   if (!Accept()) {
      Close();
   } // if
} // Listen::Readable()

Listen::~Listen() {
   Close();
} // Listen::~Listen()
