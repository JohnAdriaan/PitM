//
// Worker.hh
//

#ifndef Worker_hh
#define Worker_hh

#include "../General/Socket/TCP.hh"

class Server;

class Worker : public BSD::TCP {

friend Server;

private: // Methods

   Worker(BSD::TCP &client, const BSD::Address &address);

}; // TCP

#endif // Worker_hh
