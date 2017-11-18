//
// Interface.hh
//

#ifndef Interface_hh
#define Interface_hh

#include <string>

#include "BSD.hh"

class BSD::Interface : private _BSD_ {

public: // Enumerations

   enum Protocols {
      IPv4,
      IPv6,
      IPv46,
      Any
   }; // Protocols

   enum States {
      Down,
      Up,
      All
   }; // States

public: // Static methods

   static void Populate(Protocols protocols, States states);

   static Interface *Find(const char *name);

public: // Methods

   const char *Name() const;

private: // Methods

   Interface(const char *name);

private: // Variables

   std::string name;

   Interface *next;

private: // Static variables

   static Interface *head;

}; // Interface

#endif // Interface_hh
