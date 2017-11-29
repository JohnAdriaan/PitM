//
// main.cc
//

#include <unistd.h>

#include <iostream>

#include "Packet/Packet.hh"
#include "Server/Server.hh"

bool alreadyPitM;
MT::Semaphore PitM::quit("/PitM", alreadyPitM);

const String &PitM::Version() {
   static const String version = "0.4.0.0";
   return version;
} // PitM::Version()

static void TakeOver() {
   PitM::quit.Post(); // Tell other one to quit
   std::cout << "Shutting down existing PitM..." << std::flush;
   ::sleep(1);
   while (PitM::quit.Try()) { // After a second, it oughta've finished!
      std::cout << '!';
   } // while
   std::cout << std::endl;
} // TakeOver()

int main(int /*argc*/,
         char * /*argv*/[],
         char * /*env*/[]) {
   if (alreadyPitM) {
      TakeOver();
   } // if
   if (!PitM::Server::Start()) {
      return 1;
   } // if
   if (!PitM::Packet::Start()) {
      return 2;
   } // if
   PitM::quit.Wait();
   return 0;
} // main(argc, argv, env)
