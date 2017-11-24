//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include "Address.hh"

class BSD::Interface : private _BSD_ {

public: // Static methods

   static Interfaces List(Protocols protocol, States state);

public: // Methods

   inline operator const String &() const;

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

inline BSD::Interface::operator const String &() const {
   return name;
} // Interface::operator String()

#endif // Interface_hh
