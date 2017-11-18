//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include <netinet/in.h>

#include "../List.tt"
#include "BSD.hh"

class BSD::Interface : private _BSD_ {

public: // Enumerations

   enum Protocols {
      NoProtocol,
      IPv4,
      IPv6,
      IPv46
   }; // Protocols

   enum States {
      NoState,
      Loopback,
      Down,
      Up
   }; // States

public: // Typedefs

   typedef List<Interface> Interfaces;

public: // Static methods

   // If protocols=NoProtocol, or states=NoState, then ignore that aspect
   static Interfaces GetList(Protocols protocols, States states);

public: // Methods

   inline const String &Name() const;

   const in_addr IPv4Address() const;

   const in6_addr IPv6Address() const;

   inline Protocols Protocol() const;

   inline States State() const;

private: // Methods

   Interface(Interfaces &list,
             const char *name,
             sockaddr *addr,
             Protocols protocol,
             States state);

private: // Variables

   String name;

   sockaddr *addr;

   Protocols protocol;

   States state;

   Interfaces::Node node;

}; // Interface

inline const String &BSD::Interface::Name() const {
   return name;
} // Interface::Name()

inline BSD::Interface::Protocols BSD::Interface::Protocol() const {
   return protocol;
} // Interface::Protocol()

inline BSD::Interface::States BSD::Interface::State() const {
   return state;
} // Interface::State()

#endif // Interface_hh
