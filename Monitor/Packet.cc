//
// Packet.cc
//

#include <sys/time.h>

#include "Packet.hh"

using namespace PitM;

Monitor::Packets Monitor::Packet::pool;

Monitor::Packet::Packet() :
                 Node() {
} // Packet::Packet()

::timeval *Monitor::Packet::Stamp(::Size length, ::Size max) {
   pcap.incl_len = length<max ? length : max;
   pcap.orig_len = length;
   return &time;
} // Packet::Stamp(length, max)
