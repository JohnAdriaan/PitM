//
// Server.hh
//

#ifndef Server_hh
#define Server_hh

#include <Socket/Listen.hh>

#include <Thread/Thread.hh>
#include <Thread/Atomic.tt>

#include "../Config/Config.hh"

class PitM::Server : public BSD::Listen,
                     private MT::Thread {

public: // Static methods

   static bool Start();

   static void Quit(Server *swap=nullptr);

public: // Methods

   Server();

private: // Methods

   virtual ~Server();

private: // Listen overrides

   overrides void Heard(BSD::TCP &client, const BSD::Address &address);

private: // Thread overrides

   overrides void *Run();

private: // Classes

   class Worker;

private: // Static variables

   static MT::Atomic<Server *> current;

}; // Server

#endif // Server_hh
