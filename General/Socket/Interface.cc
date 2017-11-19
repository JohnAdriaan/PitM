//
// Interface.cc
//

#include "Interfaces.hh"

using namespace BSD;

Interface::Interface(Interfaces &list,
                     const char *name,
                     const sockaddr *addr,
                     Protocols protocol,
                     States state) :
           name(name),
           address(addr),
           protocol(protocol),
           state(state),
           node(list, *this) {
} // Interface::Interface(list,name,addr,protocol,state)
