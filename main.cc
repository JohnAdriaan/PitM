//
// main.cc
//

#include "Packet/Packet.hh"
#include "Server/Server.hh"

const String PitM::version = "0.1.0.0";

MT::Semaphore PitM::quit;

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   if (!PitM::Server::Start()) {
      return 1;
   } // if
   if (!PitM::Packet::Start()) {
      return 2;
   } // if
   PitM::quit.Wait();
   return 0;
} // main(argc, argv, env)
