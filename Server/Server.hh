//
// Server.hh
//

#ifndef Server_hh
#define Server_hh

#include "../General/FD/Pool.hh"

#include "../General/Socket/Listen.hh"

#include "../General/Thread/Thread.hh"

#include "../Config/Config.hh"

class Server : public BSD::Listen,
               private MT::Thread {

public: // Methods

   Server();

private: // Methods

   virtual ~Server();

private: // Listen overrides

   overrides void Heard(BSD::TCP &client, const BSD::Address &address);

private: // Thread overrides

   overrides void Run();

private: // Variables

   Pool pool;

}; // Server

#endif // Server_hh
