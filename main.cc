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
   static const String version = "0.4.0.0";
   return version;
} // PitM::Version()

void PitM::Quit(bool graceful/*=true*/) {
   if (graceful) {
      Server::Quit();
      Monitor::Quit();
   } // if
   quit.Unlink(); // No more PitMs
   quit.Post();
} // PitM::Quit(graceful)

static void TakeOver() {
   quit.Post(); // Tell other one to quit
   std::cout << "Shutting down existing PitM..." << std::flush;
   ::sleep(1);
   while (quit.Try()) { // After a second, it oughta've finished!
      quit.Post();      // ...but it looks like I took it. Try again
      std::cout << '!' << std::flush;
      ::sleep(1);
   } // while
   std::cout << std::endl;
} // TakeOver()

static void Handler(int sig) {
   switch (sig) {
   case SIGINT :
      PitM::Quit(true);  // Try shutting down everything
      return;
   case SIGTERM :
      PitM::Quit(false); // That didn't work. Just shut down the minimum
      return;
// case SIGKILL : // Ah, if only!
   } // switch
} // Handler(sig)

int main(int argc,
         char *argv[],
         char * /*env*/[]) {
   std::cout << "PitM " << PitM::Version() << std::endl;
   if (!quit.Valid()) {
      return 1;
   } // if
   if (::geteuid()!=0) {
      std::cerr << "PitM needs to be run as root." << std::endl;
      return 2;
   } // if
   ::signal(SIGINT, &Handler);
   if (alreadyPitM) {
      if (argc<2 || argv[1][0]!='!') {
         std::cerr << "PitM is already running. Use 'PitM !' to override." << std::endl;
         return 3;
      } // if
      TakeOver();
   } // if
   if (!PitM::Server::Start()) {
      return 4;
   } // if
   if (!PitM::Monitor::Start()) {
      return 5;
   } // if
   quit.SetUnlink(false); // Don't automatically Unlink quit any more
   quit.Wait();
   return 0;
} // main(argc, argv, env)
