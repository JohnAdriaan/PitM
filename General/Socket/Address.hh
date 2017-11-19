//
// Address.hh
//

#ifndef Address_hh
#define Address_hh

#include "BSD.hh"

class BSD::Address : private _BSD_ {

public: // Static variables

   static const Address any4;

   static const Address any6;

public: // Methods

   implicit Address(const sockaddr *addr);

   Address(const Address &address, BSD::Port port);

   inline Families Family() const;

   inline operator const sockaddr *() const;

   operator String() const;

   socklen_t Length() const;

   BSD::Port Port() const;

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

inline BSD::Families BSD::Address::Family() const {
   return address.sa_family;
} // Address:Family()

#endif // Address_hh
