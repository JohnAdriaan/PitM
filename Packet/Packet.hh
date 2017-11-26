//
// Packet.hh
//

#ifndef Packet_hh
#define Packet_hh

#include "PCap.hh"

#include "../PitM.hh"

class PitM::Packet : private _PitM_ {

public: // Static methods

   static bool Start();

   static void Reconfigure();

   static unsigned Total();

   static unsigned Logged();

private: // Variables

#pragma pack(push,1)

   PCap::Packet pcap;

   byte buffer[1500]; // Ethernet packet size

#pragma pack(pop)

}; // Packet

#endif // Packet_hh
