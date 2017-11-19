//
// Worker.hh
//

#ifndef Worker_hh
#define Worker_hh

#include "../General/FD/Pool.hh"

#include "../General/Socket/TCP.hh"

class Server;

class Worker : public BSD::TCP {

friend Server;

private: // Methods

   Worker(Pool &pool, BSD::TCP &client, const BSD::Address &address);

   virtual ~Worker();

private: // FD overrides

   overrides void Readable();

   overrides void Writable();

}; // TCP

#endif // Worker_hh
