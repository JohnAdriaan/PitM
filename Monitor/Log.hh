//
// Log.hh
//

#ifndef Log_hh
#define Log_hh

#include <File/File.hh>
#include <Thread/Thread.hh>

#include "Monitor.hh"

class PitM::Monitor::Log : private MT::Thread {

friend Monitor;

private: // Methods

   Log();

   void Stop();

private: // Thread overrides

   overrides void *Run();

private: // Variables

   Packets queue;

private: // Static variables

   static Log log;

   static unsigned logged;

}; // Log

#endif // Log_hh
