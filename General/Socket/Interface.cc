//
// Interface.cc
//

#include <arpa/inet.h>

#include "Interfaces.hh"

using namespace BSD;

Interface::Interface(Interfaces &list,
                     const char *name,
                     sockaddr *addr,
                     Protocols protocol,
                     States state) :
           name(name),
           addr(addr),
           protocol(protocol),
           state(state),
           node(list, *this) {
} // Interface::Interface(list, name, protocol, state)

const in_addr Interface::IPv4Address() const {
   static const in_addr null = {};
   return addr!=nullptr ?
          ((sockaddr_in *)addr)->sin_addr :
          null;
} // Interface::IPv4address()

String Interface::IPv4String() const {
   char str[INET_ADDRSTRLEN];
   in_addr addr = IPv4Address();
   inet_ntop(AF_INET, &addr, str, sizeof str);
   return str;
} // Interface::IPv4String()

const in6_addr Interface::IPv6Address() const {
   static const in6_addr null = {};
   return addr!=nullptr ?
         ((sockaddr_in6 *)addr)->sin6_addr :
         null;
} // Interface::IPv6Address()

String Interface::IPv6String() const {
   char str[INET6_ADDRSTRLEN];
   in6_addr addr = IPv6Address();
   inet_ntop(AF_INET6, &addr, str, sizeof str);
   return str;
} // Interface::IPv6String()
