//
// Log.cc
//

#include "Packet.hh"
#include "Log.hh"

using namespace PitM;

Log Log::log;

Log::Log() :
     Thread(),
     queue() {
} // Log::Log()

void *Log::Run() {
   for (;;) {
      Packet *packet = queue.Pop();
      if (packet==nullptr) {
         break;
      } // if

      Packet::pool.Push(*packet);
   } // while
   return nullptr;
} // Log::Run()
