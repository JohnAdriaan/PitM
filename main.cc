//
// main.cc
//

#include <unistd.h>
#include <iostream>

#include <Thread/Semaphore.hh>

#include "Monitor/Monitor.hh"
#include "Server/Server.hh"

static bool alreadyPitM;
static MT::Semaphore quit("/PitM", alreadyPitM);

const String &PitM::Version() {
   static const String version = "0.4.0.0";
   return version;
} // PitM::Version()

void PitM::Quit() {
   Server::Quit();
   Monitor::Quit();
   quit.Unlink(); // No more PitMs
   quit.Post();
} // PitM::Quit()

static void TakeOver() {
   quit.Post(); // Tell other one to quit
   std::cout << "Shutting down existing PitM..." << std::flush;
   ::sleep(1);
   while (quit.Try()) { // After a second, it oughta've finished!
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
   if (!PitM::Monitor::Start()) {
      return 2;
   } // if
   quit.Wait();
   return 0;
} // main(argc, argv, env)
