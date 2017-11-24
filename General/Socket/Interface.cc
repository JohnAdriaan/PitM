//
// Interface.cc
//

#include <ifaddrs.h>
#include <net/if.h>

#include "Interface.hh"

using namespace BSD;

Interfaces Interface::List(Protocols protocols,
                           States states) {
   Interfaces interfaces;

   ifaddrs *list;
   if (getifaddrs(&list)!=0) {
      return interfaces;
   } // if

   for (ifaddrs *ifaddr=list;
        ifaddr!=nullptr;
        ifaddr = ifaddr->ifa_next) {
      unsigned short family = ifaddr->ifa_addr!=nullptr ?
                              ifaddr->ifa_addr->sa_family :
                              0;
      Protocols protocol = protocols;
      switch (protocol) {
      case IPv46 :      // Either...
      case NoProtocol : // ...or none
         if (family==AF_INET) {
            protocol = IPv4;
         } // if
         else if (family==AF_INET6) {
            protocol = IPv6;
         } // else if
         if (protocol==IPv46) { // Protocol not found?
            continue;
         } // else
         break;
      case IPv4  :
         if (family!=AF_INET) {
            continue;
         } // if
         break;
      case IPv6  :
         if (family!=AF_INET6) {
            continue;
         } // if
         break;
      } // switch

      short flags = ifaddr->ifa_flags;
      bool up = (flags & IFF_UP)==IFF_UP;
      bool loopback = (flags & IFF_LOOPBACK)==IFF_LOOPBACK;

      States state = states;
      switch (state) {
      case NoState :
         break;
      case Loopback :
         if (!loopback) {
            continue;
         } // if
         break;
      case Up   :
         if (loopback || !up) {
            continue;
         } // if
         break;
      case Down :
         if (loopback || up) {
            continue;
         } // if
         break;
      case UpDown :
         if (loopback) {
            continue;
         } // if
         break;
      } // switch

      Interface interface(ifaddr->ifa_name, ifaddr->ifa_addr, protocol, state);
      interfaces.push_back(interface);
   } // for
   freeifaddrs(list);
   return interfaces;
} // Interface::List(Protocols,States)

Interface::Interface(const char *name,
                     const sockaddr *addr,
                     Protocols protocol,
                     States state) :
           name(name),
           address(addr),
           protocol(protocol),
           state(state) {
} // Interface::Interface(name,addr,protocol,state)
