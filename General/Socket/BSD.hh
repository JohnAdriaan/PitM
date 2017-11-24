//
// BSD.hh
//

#ifndef BSD_hh
#define BSD_hh

#include <list>

#include <netinet/in.h> // For in_port_t, in_addr and in6_addr

#include "../General.hh"

namespace BSD {

   enum Protocols {
      NoProtocol,
      IPv4,
      IPv6,
      IPv46
   }; // Protocols

   enum States {
      NoState,
      Loopback,
      Up,
      Down,
      UpDown
   }; // States

   typedef in_port_t Port;

   typedef in_addr IPv4Addr;

   typedef in6_addr IPv6Addr;

   typedef sa_family_t Family;

   class _BSD_;

   class Address;

   class Interface;

   typedef std::list<Interface> Interfaces;

   class Socket;

   class Listen;

   class TCP;

   class Raw;

} // BSD

class BSD::_BSD_ : private _General_ {
}; // _BSD_

#endif // BSD_hh
