//
// main.cc
//

#include "General/Socket/Interface.hh"

#include "Server/Server.hh"

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   BSD::Interface::Populate(BSD::Interface::IPv4, BSD::Interface::Up);
   return 0;
} // main(argc, argv, env)
