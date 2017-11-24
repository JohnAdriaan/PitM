//
// PitM.hh
//

#ifndef PitM_hh
#define PitM_hh

#include "General/Thread/Semaphore.hh"

namespace PitM {

   class _PitM_;

   class Config;

   class Packet;

   class Server;

   class Worker;

   const String &Version();

   extern MT::Semaphore quit;

} // PitM

class PitM::_PitM_ : private _General_ {
}; // _PitM_

#endif // PitM_hh
