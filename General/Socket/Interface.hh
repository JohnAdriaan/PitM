//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include "../List.tt"
#include "Address.hh"

class BSD::Interface : private _BSD_ {

public: // Variables

   const String name;

   const Address address;

   const Protocols protocol;

   const States state;

private: // Methods

   friend Interfaces;

   Interface(Interfaces &list,
             const char *name,
             const sockaddr *addr,
             Protocols protocol,
             States state);

private: // Variables

   List<Interface>::Node node;

}; // Interface

#endif // Interface_hh
