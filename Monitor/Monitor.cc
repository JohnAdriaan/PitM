//
// Monitor.cc
//

#include "Log.hh"
#include "Packet.hh"
#include "Reader.hh"

using namespace PitM;

static MT::Atomic<unsigned> total(0);

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
   new Reader(left);
   new Reader(right);
} // Monitor::Reconfigure()

unsigned Monitor::Total() {
   return total;
} // Monitor::Total()

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
         reader() {
} // Monitor::Monitor(other)

void *Monitor::Run() {
   for (;;) {
      Packet *packet = queue.Pop();
      if (packet==nullptr) {
         break;
      } // if
      Reader *r = reader;
      if (r!=nullptr) {
         Size read;
         r->Write(packet->buffer,sizeof packet->buffer,read);
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
      swap->Raw::Close();
   } // if
} // Monitor::Stop(Reader)
