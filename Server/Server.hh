//
// Server.hh
//

#ifndef Server_hh
#define Server_hh

#include "../General/Socket/Listen.hh"

#include "../Config/Config.hh"

class Server : public BSD::Listen {

public: // Methods

   Server();

private: // Overrides

   overrides void Heard(BSD::TCP &client);

}; // Server

#endif // Server_hh
