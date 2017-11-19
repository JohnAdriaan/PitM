//
// Listen.cc
//

#include "Listen.hh"

using namespace BSD;

Listen::Listen(const Address &address, Port port) :
        Socket(address.Family(), SOCK_STREAM) {
   if (!Valid()) {
      return;
   } // if

   Address local(address, port);
   if (::bind(socket, local, local.Length())==-1) {
      Close();
      return;
   } // if
   if (::listen(socket, 5)==-1) {
      Close();
      return;
   } // if
} // Listen::Listen(Address,Port)

Listen::~Listen() {
   Close();
} // Listen::~Listen()
