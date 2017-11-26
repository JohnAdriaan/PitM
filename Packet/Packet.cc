//
// Packet.cc
//

#include <random> // TODO

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
   return total = 100 + logged*2 + rand() % 100; // TODO
} // Packet::Total()

unsigned Packet::Logged() {
   return logged += rand() % 100; // TODO
} // Packet::Logged()
