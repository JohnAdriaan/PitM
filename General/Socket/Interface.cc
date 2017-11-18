//
// Interface.cc
//

#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <net/if.h>

#include "Interface.hh"

using namespace BSD;

Interface::Interfaces Interface::GetList(Protocols protocols, States states) {
   Interfaces interfaces(&Interface::node);

   ifaddrs *list;
   if (getifaddrs(&list)!=0) {
      return interfaces;
   } // if

   for (ifaddrs *interface=list;
        interface!=nullptr;
        interface = interface->ifa_next) {
      unsigned short family = interface->ifa_addr!=nullptr ?
                              interface->ifa_addr->sa_family :
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
      short flags = interface->ifa_flags;
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

      new Interface(interfaces, interface->ifa_name, protocol, state);
   } // for
   freeifaddrs(list);

   return interfaces;
} // Interface::GetList()

Interface::Interface(Interfaces &list,
                     const char *name,
                     Protocols protocol,
                     States state) :
           name(name),
           protocol(protocol),
           state(state),
           node(list, *this) {
} // Interface::Interface(list, name, protocol, state)
