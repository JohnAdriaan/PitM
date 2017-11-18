//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include <string>

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

   static Interfaces GetList(Protocols protocols, States states);

public: // Methods

   inline const std::string &Name() const;

   inline Protocols Protocol() const;

   inline States State() const;

private: // Methods

   Interface(Interfaces &list,
             const char *name,
             Protocols protocol,
             States state);

private: // Variables

   std::string name;

   Protocols protocol;

   States state;

   Interfaces::Node node;

}; // Interface

inline const std::string &BSD::Interface::Name() const {
   return name;
} // Interface::Name()

#endif // Interface_hh
