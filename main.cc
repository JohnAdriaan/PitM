//
// main.cc
//

#include <signal.h>
#include <unistd.h>

#include <iostream>

#include <Thread/Semaphore.Global.hh>

#include "Monitor/Monitor.hh"
#include "Server/Server.hh"

static bool alreadyPitM;
static MT::Semaphore::Global quit("/PitM", alreadyPitM);

const String &PitM::Version() {
   static const String version = "0.5.0.0";
   return version;
} // PitM::Version()

void PitM::Quit(bool graceful/*=true*/) {
   if (graceful) {
      Monitor::Quit();
      Server::Quit();
      quit.Post();  // Quit nicely
   } // if
   else {
      quit.Close(); // Less nice!
   } // else
   quit.SetUnlink(true); // No more PitMs
} // PitM::Quit(graceful)

static bool TakeOver() {
   std::cout << "Shutting down existing PitM..." << std::flush;
   if (!quit.Post()) { // Tell other one to quit
      return false;
   } // if
   ::sleep(1);
   while (quit.Try()) {   // After a second, it oughta've finished!
      if (!quit.Post()) { // ...but it looks like I took it. Try again
         return false;
      } // if
      std::cout << '!' << std::flush;
      ::sleep(1);
   } // while
   std::cout << std::endl;
   return quit.Valid();
} // TakeOver()

static void Handler(int sig) {
   switch (sig) {
   case SIGINT :
      static bool first = true;
      if (first) {
         first = false;
         PitM::Quit(true);  // Try shutting down everything
         return;
      } // if
   // Fall through
   case SIGTERM :
      static bool second = true;
      if (second) {
         PitM::Quit(false); // That didn't work. Just shut down the minimum
         return;
      } // if
   // Fall through
   case SIGKILL : // Ah, if only! Doesn't mean we can't pretend...
      exit(-1);
      return;
   } // switch
} // Handler(sig)

int main(int argc,
         char *argv[],
         char * /*env*/[]) {
   std::cout << "PitM: version " << PitM::Version() << std::endl;
   if (::geteuid()!=0) {
      std::cerr << "PitM needs to be run as root." << std::endl;
      return 1;
   } // if
   if (!quit.Valid()) {
      return 2;
   } // if
   ::signal(SIGINT, &Handler);
   if (alreadyPitM) {
      if (argc<2 || argv[1][0]!='!') {
         std::cerr << "PitM is already running. Use 'PitM !' to shut it down." << std::endl;
         return 3;
      } // if
      quit.SetUnlink(true); // Shutting down, so I'm allowed to Unlink quit too
      if (!TakeOver()) {
         return 4;
      } // if
      return 0;
   } // if
   if (!PitM::Server::Start()) {
      return 5;
   } // if
   if (!PitM::Monitor::Start()) {
      return 6;
   } // if
   quit.SetUnlink(false); // Don't automatically Unlink quit any more
   quit.Wait();
   return 0;
} // main(argc, argv, env)
