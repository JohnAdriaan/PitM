//
// Server.cc
//

#include "Worker.hh"
#include "Server.hh"

Server::Server() :
        BSD::Listen(BSD::Address::any4, Config::config.Port()) {
} // Server::Server()

void Server::Heard(BSD::TCP &client, const BSD::Address &address) {
   new Worker(client, address);
} // Server::Heard(TCP, Address)
