//
// Packet.cc
//

#include <sys/time.h>

#include "Packet.hh"

using namespace PitM;

Packets Packet::pool;

Packet::Packet() :
        Node() {
} // Packet::Packet()

bool Packet::Stamp(Size length) {
   pcap.incl_len = length;
   pcap.orig_len = length;
   return ::gettimeofday(&time, nullptr)==0;
} // Packet::Stamp(length)
