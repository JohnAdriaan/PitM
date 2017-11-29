//
// Reader.cc
//

#include "Packet.hh"
#include "Reader.hh"

using namespace PitM;

Monitor::Reader::Reader(Monitor &monitor) :
                 BSD::Raw(),
                 MT::Thread(),
                 monitor(monitor) {
   monitor.Stop(this);
   if (!Raw::Valid()) {
      delete this;
      return;
   } // if
   if (!Thread::Start()) {
      delete this;
      return;
   } // if
} // Reader::Reader(Monitor)

void *Monitor::Reader::Run() {
   Monitor::Packet *packet;
   for (;;) {
      if (!Monitor::Packet::pool.TryPop(packet)) {
         packet = new Monitor::Packet();
      } // if
      if (packet==nullptr) {
         break;
      } // if
      Size read;
      if (!Read(packet->buffer, sizeof packet->buffer, read)) {
         break;
      } // if
      packet->Stamp(read);
      if (read==0) {
         break;
      } // if
      monitor.other.queue.Push(*packet);
   } // for
   Socket::Close();
   if (packet!=nullptr) {
      Packet::pool.Push(*packet);
   } // if
   delete this;
   return nullptr;
} // Reader::Run()

Monitor::Reader::~Reader() {
   monitor.reader.Swap(nullptr, this); // Set nullptr, but only if this
} // Reader::~Reader()
