//
// Interfaces.hh
//

#ifndef Interfaces_hh
#define Interfaces_hh

#include "Interface.hh"

class BSD::Interfaces : public List<Interface> {

public: // Methods

   Interfaces(Protocols protocols, States states);

   // Returns nullptr if name is not found
   const Interface *Find(const String &name);

}; // Interfaces

#endif // Interfaces_hh
