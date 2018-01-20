//
// Server.hh
//

#ifndef Server_hh
#define Server_hh

#include <Socket/Listen.hh>
#include <Thread/Thread.hh>
#include <Thread/Atomic.tt>

#include "../Config/Config.hh"

class PitM::Server : private MT::Thread, // Thread first, so destructed last
                     public BSD::Listen {

public: // Static methods

   static bool Start();

   static void Quit(Server *swap=nullptr);

private: // Methods

   Server();

   virtual ~Server(); // Ensure can only be created by new

private: // Listen overrides

   overrides void Heard(BSD::TCP &client, const BSD::Address &address);

private: // Thread overrides

   overrides void *Run();

private: // Classes

   class Client;

private: // Static variables

   static MT::Atomic<Server *> server;

}; // Server

#endif // Server_hh
