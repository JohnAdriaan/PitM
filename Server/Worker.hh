//
// Worker.hh
//

#ifndef Worker_hh
#define Worker_hh

#include "../General/Thread/Thread.hh"

#include "../General/Socket/TCP.hh"

class Server;

class Worker : public BSD::TCP,
               private MT::Thread {

friend Server;

private: // Methods

   Worker(BSD::TCP &client, const BSD::Address &address);

   virtual ~Worker() = default;

private: // Thread overrides

   overrides void *Run();

}; // TCP

#endif // Worker_hh
