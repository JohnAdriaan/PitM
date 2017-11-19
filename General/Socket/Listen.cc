//
// Listen.cc
//

#include "Listen.hh"

using namespace BSD;

Listen::Listen(Port port) :
        Socket() {
} // Listen::Listen(Port)

Listen::Listen(const Address &address, Port port) :
        Socket() {
} // Listen::Listen(Address,Port)

Listen::~Listen() {
} // Listen::~Listen()
