//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include <netinet/in.h> // For in_addr and in6_addr

#include "../List.tt"
#include "BSD.hh"

class BSD::Interface : private _BSD_ {

friend Interfaces;

public: // Methods

   inline const String &Name() const;

   const in_addr IPv4Address() const;

   String IPv4String() const;

   const in6_addr IPv6Address() const;

   String IPv6String() const;

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

   List<Interface>::Node node;

}; // Interface

inline const String &BSD::Interface::Name() const {
   return name;
} // Interface::Name()

inline BSD::Protocols BSD::Interface::Protocol() const {
   return protocol;
} // Interface::Protocol()

inline BSD::States BSD::Interface::State() const {
   return state;
} // Interface::State()

#endif // Interface_hh
