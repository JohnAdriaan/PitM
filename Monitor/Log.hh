//
// Log.hh
//

#ifndef Log_hh
#define Log_hh

#include "../General/File/File.hh"

#include "../General/Thread/Thread.hh"

#include "../PitM.hh"

class PitM::Log : private MT::Thread {

friend Monitor;

private: // Methods

   Log();

private: // Thread overrides

   overrides void *Run();

private: // Variables

   PitM::Packets queue;

private: // Classes

   static Log log;

}; // Log

#endif // Log_hh
