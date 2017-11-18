//
// Interface.cc
//

#include <sys/types.h>
#include <ifaddrs.h>

#include "Interface.hh"

using namespace BSD;

Interface *Interface::head = nullptr;

void Interface::Populate(Protocols protocols, States states) {
   while (head!=nullptr) {
      Interface *next = head->next;
      delete head;
      head = next;
   } // while

   ifaddrs *list;
   if (getifaddrs(&list)!=0) {
      return;
   } // if
   for (ifaddrs *interface=list;
        interface!=nullptr;
        interface = interface->ifa_next) {
      switch (protocols) {
      case IPv4  :
         break;
      case IPv6  :
         break;
      case IPv46 :
         break;
      case Any   :
         break;
      } // switch
      switch (states) {
      case Down :
         break;
      case Up   :
         break;
      case All  :
         break;
      } // switch
      printf("%s:\t%08X\n",interface->ifa_name,interface->ifa_flags);
   } // for
   freeifaddrs(list);
} // Interface::Populate()

Interface::Interface(const char *name) :
           name(name),
           next(nullptr) {
} // Interface::Interface(name)
