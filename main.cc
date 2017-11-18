//
// main.cc
//

#include "General/Socket/Interface.hh"

#include "Server/Server.hh"

void ShowInterfaces(bool up) {
   using namespace BSD;
   printf("Interfaces that are %s:\n", up ? "Up" : "Down");

   Interface::Interfaces interfaces = Interface::GetList(up ? Interface::IPv4 : Interface::NoProtocol,
                                                         up ? Interface::Up : Interface::Down);
   for (Interface *interface = interfaces.Head();
        interface!=nullptr;
        interface = interfaces.Next(interface)) {
      printf("%s\n", interface->Name().c_str());
   } // for
   printf("\n");
} // ShowInterfaces(up)

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   ShowInterfaces(false);
   ShowInterfaces(true);
   return 0;
} // main(argc, argv, env)
