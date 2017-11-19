//
// Server.cc
//

#include "Worker.hh"
#include "Server.hh"

Server::Server() :
        BSD::Listen(BSD::Address::any4, Config::config.Port()) {
} // Server::Server()

void Server::Heard(BSD::TCP &client) {
   new Worker(client);
} // Server::Heard(TCP)
