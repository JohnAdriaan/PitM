//
// main.cc
//

#include "Packet/Packet.hh"
#include "Server/Server.hh"

MT::Semaphore quit;

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   if (!Server::Start()) {
      return 1;
   } // if
   if (!Packet::Start()) {
      return 2;
   } // if
   quit.Wait();
   return 0;
} // main(argc, argv, env)
