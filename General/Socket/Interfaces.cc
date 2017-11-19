//
// Interfaces.cc
//

#include <ifaddrs.h>
#include <net/if.h>

#include "Interfaces.hh"

BSD::Interfaces::Interfaces(Protocols protocols,
                            States states) :
                 List<Interface>(&Interface::node) {
   ifaddrs *list;
   if (getifaddrs(&list)!=0) {
      return;
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

      States state = states;
      short flags = ifaddr->ifa_flags;
      bool loopback = (flags & IFF_LOOPBACK)==IFF_LOOPBACK;
      bool up = (flags & IFF_UP)==IFF_UP;
      switch (state) {
      case NoState :
         break;
      case Loopback :
         if (!loopback) {
            continue;
         } // if
         break;
      case Down :
         if (loopback || up) {
            continue;
         } // if
         break;
      case Up   :
         if (loopback || !up) {
            continue;
         } // if
         break;
      } // switch

      new Interface(*this, ifaddr->ifa_name, ifaddr->ifa_addr, protocol, state);
   } // for
   freeifaddrs(list);
} // Interfaces::Interfaces(Protocols,States)

const BSD::Interface *BSD::Interfaces::Find(const String &name) {
   for (const Interface *interface = Head();
        interface!=nullptr;
        interface = Next(interface)) {
      if (interface->name==name) {
         return interface;
      } // if
   } // for
   return nullptr;
} // Interfaces::Find(name)
