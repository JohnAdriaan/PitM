//
// PitM.hh
//

#ifndef PitM_hh
#define PitM_hh

#include <General.hh>

namespace PitM {

   class _PitM_;

   class Config;

   class Monitor;

   class Server;

   const String &Version();

   // Shut everything down gracefully.
   // Otherwise, let everything collapse on its own
   void Quit(bool graceful=true);

} // PitM

class PitM::_PitM_ : private _General_ {
}; // _PitM_

#endif // PitM_hh
