//
// Address.cc
//

#include <arpa/inet.h> // For inet_ntop

#include "Address.hh"

using namespace BSD;

Address::Address(const sockaddr *addr) :
         storage() {
   switch (addr->sa_family) {
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

Address::Address(const Address &rhs, Port port) :
         storage(rhs.storage) {
   switch (address.sa_family) {
   case AF_INET :
      ipv4.sin_port = port;
      break;
   case AF_INET6 :
      ipv6.sin6_port = port;
      break;
   default :
      break;
   } // switch
} // Address::Address(Address, Port)

Address::operator String() const {
   const void *data;

   switch (address.sa_family) {
   case AF_INET :
      data = &ipv4.sin_addr;
      break;
   case AF_INET6 :
      data = &ipv6.sin6_addr;
      break;
   default :
      data = address.sa_data;
      break;
   } // switch

   char str[INET6_ADDRSTRLEN];
   inet_ntop(address.sa_family, data, str, sizeof str);
   return str;
} // Address::operator String()

Port Address::Service() const {
   switch (address.sa_family) {
   case AF_INET :
      return ipv4.sin_port;
   case AF_INET6 :
      return ipv6.sin6_port;
   default :
      return 0;
   } // switch
} // Address::Service()
