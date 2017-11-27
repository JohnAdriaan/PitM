//
// BSD.hh
//

#ifndef BSD_hh
#define BSD_hh

#include <set>
#include <map>
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

   static const Port NoPort = 0;
   static const Port ReservedPorts = 1023;
   static const Port MaxPort = 65535;

   typedef in_addr IPv4Addr;

   typedef in6_addr IPv6Addr;

   typedef sa_family_t Family;

   class _BSD_;

   class Address;

   class Interface;

   typedef std::list<Interface> Interfaces;

   class Socket;

   // A named Port
   class Service;

   // Service names are unique
   typedef std::map<String, Service, NoCase> Services;

   // Service ports are not unique.
   // There may be a number of Services for each port
   typedef std::map<Port, Services> Ports;

   class Listen;

   class TCP;

   class Raw;

} // BSD

class BSD::_BSD_ : private _General_ {
}; // _BSD_

#endif // BSD_hh
