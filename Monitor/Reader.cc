//
// Reader.cc
//

#include <net/ethernet.h>

#include <Socket/Address.hh>

#include "Packet.hh"
#include "Reader.hh"

using namespace PitM;

Monitor::Reader::Reader(Monitor &monitor, const String &interface) :
                 BSD::Raw(interface, Protocols::All),
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
} // Reader::Reader(Monitor, interface)

bool Monitor::Reader::Send(const Monitor::Packet &packet) {
   Size wrote;
   BSD::Address to(All, index, packet.buffer, ETHER_ADDR_LEN);
   return Raw::Send(packet.buffer,packet.Size(),wrote,to);
} // Reader::Send(Packet)

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
      if (!Recv(packet->buffer, sizeof packet->buffer, read,
                packet->address, MSG_TRUNC)) {
         break;
      } // if
      IOCtl(SIOCGSTAMP, packet->Stamp(read, sizeof packet->buffer));
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
