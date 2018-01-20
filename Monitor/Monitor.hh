//
// Monitor.hh
//

#ifndef Monitor_hh
#define Monitor_hh

#include <Thread/Queue.tt>
#include <Thread/Atomic.tt>
#include <Thread/Thread.hh>

#include "../PitM.hh"

class PitM::Monitor : private MT::Thread {

public: // Static methods

   static bool Start();

   static void Reconfigure();

   static unsigned NumLeft();

   static unsigned NumRight();

   static unsigned NumTotal();

   static unsigned NumDropped();

   static unsigned NumLogged();

   static void Quit();

private: // Typedefs and classes

   class Log;

   class Packet;

   class Reader; friend Reader;

   typedef MT::Queue<Packet,MT::Mutex> Packets;

private: // Methods

   // Not a move constructor - just looks like one
   explicit Monitor(Monitor &other);

   void Transmit(Packet &packet);

   void Stop(Reader *swap=nullptr);

   virtual ~Monitor() = default;

private: // Thread overrides

   overrides void *Run();

private: // Variables

   Monitor &other;

   Packets queue;

   MT::Atomic<Reader *> reader;

   unsigned count;

   unsigned dropped;

private: // Static variables

   static Monitor left;

   static Monitor right;

}; // Monitor

#endif // Monitor_hh
