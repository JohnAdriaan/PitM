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

   Address();

   implicit Address(const sockaddr *addr);

   Address(const Address &address, BSD::Port port);

   inline BSD::Family Family() const;

   inline operator const sockaddr *() const;

   operator String() const;

   socklen_t Length() const;

   BSD::Port Port() const;

private: // Methods

   friend Listen;

   inline operator sockaddr *();

private: // Variables

   union {
      sockaddr         address;
      sockaddr_in      ipv4;
      sockaddr_in6     ipv6;
      sockaddr_storage storage;
   }; // union

}; // Address

inline BSD::Address::operator sockaddr *() {
   return &address;
} // Address::operator sockaddr *()

inline BSD::Address::operator const sockaddr *() const {
   return &address;
} // Address::operator const sockaddr *()

inline BSD::Family BSD::Address::Family() const {
   return address.sa_family;
} // Address:Family()

#endif // Address_hh
