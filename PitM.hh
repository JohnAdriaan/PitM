//
// PitM.hh
//

#ifndef PitM_hh
#define PitM_hh

#include <Thread/Queue.tt>

namespace PitM {

   class _PitM_;

   class Config;

   class Monitor;

      class Packet;

      class Log;

      typedef MT::Queue<Packet,MT::Mutex> Packets;

   class Server;

      class Worker;

   const String &Version();

   void Quit();

} // PitM

class PitM::_PitM_ : private _General_ {
}; // _PitM_

#endif // PitM_hh
