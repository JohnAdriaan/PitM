//
// Reader.hh
//

#ifndef Reader_hh
#define Reader_hh

#include <Socket/Raw.hh>

#include "Monitor.hh"

class PitM::Monitor::Reader : public BSD::Raw,
                              private MT::Thread {

friend Monitor;

private: // Methods

   Reader(Monitor &monitor, const String &interface);

   bool Send(const Monitor::Packet &packet);

   virtual ~Reader();

private: // Thread overrides

   overrides void *Run();

private: // Variables

   Monitor &monitor;

}; // Reader

#endif // Reader_hh