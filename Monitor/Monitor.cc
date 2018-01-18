//
// Monitor.cc
//

#include <Socket/Address.hh>

#include "../Config/Config.hh"

#include "Log.hh"
#include "Packet.hh"
#include "Reader.hh"

using namespace PitM;

Monitor Monitor::left(right);

Monitor Monitor::right(left);

bool Monitor::Start() {
   if (!left.Thread::Start()) {
      return false;
   } // if
   if (!right.Thread::Start()) {
      return false;
   } // if
   Reconfigure();
   return true;
} // Monitor::Start()

void Monitor::Reconfigure() {
   new Reader(left,  Config::master.left);
   new Reader(right, Config::master.right);
} // Monitor::Reconfigure()

unsigned Monitor::Left() {
   return left.count;
} // Monitor::Left()

unsigned Monitor::Right() {
   return right.count;
} // Monitor::Right()

unsigned Monitor::Total() {
   return left.count+right.count;
} // Monitor::Total()

unsigned Monitor::Dropped() {
   return left.dropped+right.dropped;
} // Monitor::Dropped()

unsigned Monitor::Logged() {
   return Log::logged;
} // Monitor::Logged()

void Monitor::Quit() {
   left.Stop();
   right.Stop();
   Log::log.Stop();
} // Monitor::Quit()

Monitor::Monitor(Monitor &other) :
         other(other),
         queue(),
         reader(),
         count(0),
         dropped(0) {
} // Monitor::Monitor(other)

void *Monitor::Run() {
   for (;;) {
      Packet *packet = queue.Pop();
      if (packet==nullptr) {
         break;
      } // if
      ++count;

      Reader *r = reader;
      if (r==nullptr ||
          !r->Send(*packet)) {
         ++dropped;
         Packet::pool.Push(*packet); // Return to free pool
         continue;
      } // if
      Log::log.queue.Push(*packet);
   } // for
   return nullptr;
} // Monitor::Run()

void Monitor::Stop(Reader *swap) {
   if (swap==nullptr) {
      queue.Quit();
   } // if
   swap = reader.Swap(swap);
   if (swap!=nullptr) {
      swap->Close();
   } // if
} // Monitor::Stop(Reader)
