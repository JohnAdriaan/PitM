//
// Packet.hh
//

#ifndef Packet_hh
#define Packet_hh

#include "../General/Thread/Queue.tt"

#include "PCap.hh"

#include "../PitM.hh"

typedef MT::Queue<PitM::Packet,MT::Mutex> Packets;

class PitM::Packet : public Packets::Node {

public: // Static variables

   // Pool of used packets
   static Packets pool;

public: // Methods

   Packet();

   // Encode timestamp and length
   bool Stamp(Size length);

public: // Variables

#pragma pack(push,1) // Ensure header and data are adjacent

   union {
      ::timeval    time;
      PCap::Packet pcap;
   }; // Unnamed

   byte buffer[1500]; // Ethernet packet size

#pragma pack(pop)

}; // Packet

#endif // Packet_hh
