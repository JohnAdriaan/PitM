//
// main.cc
//

#include "Packet/Packet.hh"
#include "Server/Server.hh"

MT::Semaphore PitM::quit;

const String &PitM::Version() {
   static const String version = "0.1.0.0";
   return version;
} // PitM::Version()

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
