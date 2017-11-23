//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include "Address.hh"

class BSD::Interface : private _BSD_ {

public: // Static methods

   static Interfaces List(Protocols protocol, States state);

public: // Variables

   const String name;

   const Address address;

   const Protocols protocol;

   const States state;

private: // Methods

   Interface(const char *name,
             const sockaddr *addr,
             Protocols protocol,
             States state);

}; // Interface

#endif // Interface_hh
