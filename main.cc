//
// main.cc
//

#include <arpa/inet.h>

#include "General/Socket/Interface.hh"

#include "Server/Server.hh"

void ShowInterfaces(bool ipv4, bool up) {
   using namespace BSD;
   printf("Interfaces that are %s:\n", up ? "Up" : "Down");

   Interface::Interfaces interfaces = Interface::GetList(up ? ipv4 ? Interface::IPv4 : Interface::IPv6 : Interface::NoProtocol,
                                                         up ? Interface::Up : Interface::Down);
   for (Interface *interface = interfaces.Head();
        interface!=nullptr;
        interface = interfaces.Next(interface)) {
      if (up) {
         char str[INET6_ADDRSTRLEN];
         if (ipv4) {
            in_addr addr = interface->IPv4Address();
            inet_ntop(AF_INET, &addr, str, sizeof str);
         } // if
         else {
            in6_addr addr = interface->IPv6Address();
            inet_ntop(AF_INET6, &addr, str, sizeof str);
         } // else
         printf("%s:\t%s\n", interface->Name().c_str(), str);
      } // if
      else {
         printf("%s\n", interface->Name().c_str());
      } // else
   } // for
   printf("\n");
} // ShowInterfaces(up)

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   ShowInterfaces(false,false);
   ShowInterfaces(true,true);
   ShowInterfaces(false,true);
   return 0;
} // main(argc, argv, env)
