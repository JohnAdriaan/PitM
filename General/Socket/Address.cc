//
// Address.cc
//

#include <arpa/inet.h> // For inet_ntop

#include "Address.hh"

using namespace BSD;

static const sockaddr_in any4 = { AF_INET, htons(0), INADDR_ANY };

static const sockaddr_in6 any6 = { AF_INET6, htons(0), 0, IN6ADDR_ANY_INIT };

const Address Address::any4 = (sockaddr *)&::any4;

const Address Address::any6 = (sockaddr *)&::any6;

Address::Address() :
         storage( { 0 } ) {
} // Address::Address()

Address::Address(const sockaddr *addr) :
         address( { addr->sa_family }) {
   switch (Family()) {
   case AF_INET :
      ipv4 = *(const sockaddr_in *)addr;
      break;
   case AF_INET6 :
      ipv6 = *(const sockaddr_in6 *)addr;
      break;
   default :
      address = *addr;
      break;
   } // switch
} // Address::Address(sockaddr*)

Address::Address(const Address &rhs, BSD::Port port) :
         storage(rhs.storage) {
   switch (Family()) {
   case AF_INET :
      ipv4.sin_port = htons(port);
      break;
   case AF_INET6 :
      ipv6.sin6_port = htons(port);
      break;
   default :
      break;
   } // switch
} // Address::Address(Address, Port)

Address::operator String() const {
   const void *data;

   switch (Family()) {
   case AF_INET :
      data = &ipv4.sin_addr;
      break;
   case AF_INET6 :
      data = &ipv6.sin6_addr;
      break;
   default :
      data = &address.sa_data;
      break;
   } // switch

   char str[INET6_ADDRSTRLEN];
   ::inet_ntop(address.sa_family, data, str, sizeof str);
   return str;
} // Address::operator String()

socklen_t Address::Length() const {
   switch (Family()) {
   case 0 :
      return sizeof storage;
   case AF_INET :
      return sizeof ipv4;
   case AF_INET6 :
      return sizeof ipv6;
   default :
      return sizeof address;
   } // switch
} // Address::Length()

BSD::Port Address::Port() const {
   switch (Family()) {
   case AF_INET :
      return ntohs(ipv4.sin_port);
   case AF_INET6 :
      return ntohs(ipv6.sin6_port);
   default :
      return 0;
   } // switch
} // Address::Port()
