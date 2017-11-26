//
// Packet.cc
//

#include "Packet.hh"

using namespace PitM;

static unsigned total = 0;

static unsigned logged = 0;

bool Packet::Start() { // TODO
   return true;
} // Packet::Start()

void Packet::Reconfigure() {
} // Packet::Reconfigure()

unsigned Packet::Total() {
   return total;
} // Packet::Total()

unsigned Packet::Logged() {
   return logged;
} // Packet::Logged()
