//
// Packet.hh
//

#ifndef Packet_hh
#define Packet_hh

#include <Socket/Address.hh>

#include "PCap.hh"

#include "Monitor.hh"

class PitM::Monitor::Packet : public Packets::Node {

public: // Static variables

   // Pool of used packets
   static Packets pool;

public: // Methods

   Packet();

   inline ::Size Size() const;

   // Encode length, and return pointer to timestamp
   ::timeval *Stamp(::Size length, ::Size max);

public: // Variables

   BSD::Address address;

#pragma pack(push,1) // Ensure header and data are adjacent

   union {
      ::timeval    time;
      PCap::Packet pcap;
   }; // Unnamed

   byte buffer[1500]; // Ethernet packet size

#pragma pack(pop)

}; // Packet

inline ::Size PitM::Monitor::Packet::Size() const {
   return pcap.incl_len;
} // Packet::Size()

#endif // Packet_hh
