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

public: // Static methods

   static bool Start();

   static void Reconfigure();

   static unsigned Total();

   static unsigned Logged();

public: // Variables

#pragma pack(push,1)

   PCap::Packet pcap;

   byte buffer[1500]; // Ethernet packet size

#pragma pack(pop)

}; // Packet

#endif // Packet_hh
