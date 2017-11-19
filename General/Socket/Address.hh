//
// Address.hh
//

#ifndef Address_hh
#define Address_hh

#include "BSD.hh"

class BSD::Address : private _BSD_ {

public: // Methods

   implicit Address(const sockaddr *addr);

   Address(const Address &address, Port port);

   inline operator const sockaddr *() const;

   operator String() const;

   Port Service() const;

private: // Variables

   union {
      sockaddr         address;
      sockaddr_in      ipv4;
      sockaddr_in6     ipv6;
      sockaddr_storage storage;
   }; // union

}; // Address

inline BSD::Address::operator const sockaddr *() const {
   return &address;
} // Address::operator const sockaddr *()

#endif // Address_hh
