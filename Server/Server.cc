//
// Server.cc
//

#include "Worker.hh"
#include "Server.hh"

void Server::Heard(BSD::TCP &client) {
   new Worker(client);
} // Server::Heard(TCP)
