//
// main.cc
//

#include <iostream>

#include "General/Socket/Interfaces.hh"

#include "Server/Server.hh"

void ShowInterfaces(bool ipv4, bool up) {
   std::cout << "Interfaces that are "<< (up ? "Up" : "Down") << std::endl;

   using namespace BSD;
   Interfaces interfaces(up ? ipv4 ? IPv4 :
                                     IPv6 :
                              NoProtocol,
                         up ? Up :
                              Down);
   for (Interface *interface = interfaces.Head();
        interface!=nullptr;
        interface = interfaces.Next(interface)) {
      std::cout << interface->name;
      if (up) {
         std::cout << '\t' << (String)interface->address;
      } // if
      std::cout << std::endl;
   } // for
   std::cout << std::endl;
} // ShowInterfaces(ipv4,up)

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   ShowInterfaces(false,false);
   ShowInterfaces(true,true);
   ShowInterfaces(false,true);
   return 0;
} // main(argc, argv, env)
