//
// Monitor.cc
//

#include "Log.hh"
#include "Monitor.hh"

using namespace PitM;

static unsigned total = 0;

static unsigned logged = 0;

Monitor Monitor::left(right);

Monitor Monitor::right(left);

bool Monitor::Start() { // TODO
   return true;
} // Monitor::Start()

void Monitor::Quit() {
   left.queue.Quit();
   right.queue.Quit();
   Log::log.queue.Quit();
} // Monitor::Quit()

void Monitor::Reconfigure() {
} // Monitor::Reconfigure()

unsigned Monitor::Total() {
   return total;
} // Monitor::Total()

unsigned Monitor::Logged() {
   return logged;
} // Monitor::Logged()

Monitor::Monitor(Monitor &other) :
         other(other),
         queue() {
} // Monitor::Monitor(other)
