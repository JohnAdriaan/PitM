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
   return true;  // If configuration is bad, don't not Start()
} // Monitor::Start()

bool Monitor::Reconfigure() {
   bool valid = true;
   if (Config::master.left.empty()) {
      left.Stop();
      valid = false;
   } // if
   if (Config::master.right.empty()) {
      right.Stop();
      valid = false;
   } // if
   if (Config::master.left==Config::master.right) {
      valid = false;
   } // if
   if (!valid) {
      return false;
   } // if
   new Reader(left,  Config::master.left);
   new Reader(right, Config::master.right);
   return true;
} // Monitor::Reconfigure()

unsigned Monitor::NumLeft() {
   return left.count;
} // Monitor::NumLeft()

unsigned Monitor::NumRight() {
   return right.count;
} // Monitor::NumRight()

unsigned Monitor::NumTotal() {
   return left.count+right.count;
} // Monitor::NumTotal()

unsigned Monitor::NumDropped() {
   return left.dropped+right.dropped;
} // Monitor::NumDropped()

unsigned Monitor::NumLogged() {
   return Log::logged;
} // Monitor::NumLogged()

void Monitor::Quit() {
   left.queue.Quit();
   right.queue.Quit();
   left.Stop();
   right.Stop();
   Log::log.Stop();
} // Monitor::Quit()

Monitor::Monitor(Monitor &other) :
         Thread(),
         other(other),
         queue(),
         reader(),
         count(0),
         dropped(0) {
} // Monitor::Monitor(other)

void Monitor::Transmit(Packet &packet) {
   Reader *r = reader;
   if (r==nullptr ||
       !r->Socket::Valid()) {
      ++dropped;
      Packet::pool.Push(packet);
      return;
   } // if
   queue.Push(packet);
} // Monitor::Transmit(Packet)

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
      Log::log.queue.Push(*packet); // Save as processed packet
   } // for
   return nullptr;
} // Monitor::Run()

void Monitor::Stop(Reader *swap) {
   swap = reader.Swap(swap);
   if (swap!=nullptr) {
      swap->Close();
   } // if
} // Monitor::Stop(Reader)
