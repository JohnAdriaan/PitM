//
// Monitor.cc
//

#include "Monitor.hh"

using namespace PitM;

static unsigned total = 0;

static unsigned logged = 0;

bool Monitor::Start() { // TODO
   return true;
} // Monitor::Start()

void Monitor::Reconfigure() {
} // Monitor::Reconfigure()

unsigned Monitor::Total() {
   return total;
} // Monitor::Total()

unsigned Monitor::Logged() {
   return logged;
} // Monitor::Logged()
