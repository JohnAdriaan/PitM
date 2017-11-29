//
// Monitor.hh
//

#ifndef Monitor_hh
#define Monitor_hh

#include <Thread/Queue.tt>

#include "Packet.hh"

class PitM::Monitor : private _PitM_ {

public: // Static methods

   static bool Start();

   static void Reconfigure();

   static unsigned Total();

   static unsigned Logged();

   static void Quit();

private: // Methods

   explicit Monitor(Monitor &other);

private: // Variables

   Monitor &other;

   Packets queue;

private: // Static variables

   static Monitor left;

   static Monitor right;

}; // Monitor

#endif // Monitor_hh
