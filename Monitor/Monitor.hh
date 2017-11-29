//
// Monitor.hh
//

#ifndef Monitor_hh
#define Monitor_hh

#include "../General/Thread/Queue.tt"

#include "Packet.hh"

class PitM::Monitor : private _PitM_ {

public: // Static methods

   static bool Start();

   static void Reconfigure();

   static unsigned Total();

   static unsigned Logged();

public: // Methods

   Monitor();

}; // Monitor

#endif // Monitor_hh
