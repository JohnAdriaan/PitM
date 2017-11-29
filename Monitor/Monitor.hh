//
// Monitor.hh
//

#ifndef Monitor_hh
#define Monitor_hh

#include <Thread/Queue.tt>

#include "../PitM.hh"

class PitM::Monitor : private _PitM_ {

public: // Static methods

   static bool Start();

   static void Reconfigure();

   static unsigned Total();

   static unsigned Logged();

   static void Quit();

private: // Typedefs and classes

   class Log;

   class Packet;

   typedef MT::Queue<Packet,MT::Mutex> Packets;

private: // Methods

   // Not a move constructor - just looks like one
   explicit Monitor(Monitor &other);

private: // Variables

   Monitor &other;

   Packets queue;

private: // Static variables

   static Monitor left;

   static Monitor right;

}; // Monitor

#endif // Monitor_hh
