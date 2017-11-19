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

   Address local(address, port);
   if (::bind(fd, local, local.Length())==-1) {
      Close();
      return;
   } // if
   if (::listen(fd, 5)==-1) {
      Close();
      return;
   } // if
} // Listen::Listen(Address,Port)

void Listen::Readable() {
   Address address;
   socklen_t length = address.Length();
   FD::Type heard = ::accept(fd, address, &length);
   if (heard==-1) {
      Close();
      return;
   } // if
   TCP tcp(heard);
   Heard(tcp, address);
} // Listen::Readable()

Listen::~Listen() {
   Close();
} // Listen::~Listen()
