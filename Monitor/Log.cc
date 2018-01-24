//
// Log.cc
//

#include "Packet.hh"
#include "Log.hh"

using namespace PitM;

unsigned Monitor::Log::logged = 0;

Monitor::Log Monitor::Log::log;

Monitor::Log::Log() :
              Thread(),
              queue() {
} // Log::Log()

void Monitor::Log::Stop() {
   queue.Quit();
} // Log::Stop()

void *Monitor::Log::Run() {
   for (ever) {
      Packet *packet = queue.Pop();
      if (packet==nullptr) {
         break;
      } // if
      // TODO
      Packet::pool.Push(*packet);
   } // while
   return nullptr;
} // Log::Run()
