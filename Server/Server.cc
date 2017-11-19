//
// Server.cc
//

#include "Worker.hh"
#include "Server.hh"

Server::Server() :
        BSD::Listen(BSD::Address::any4, Config::config.Port()),
        MT::Thread(),
        pool() {
   if (!Listen::Valid()) {
      delete this;
      return;
   } // if
   pool.Add(*this);
   Start();
} // Server::Server()

void Server::Heard(BSD::TCP &client, const BSD::Address &address) {
   new Worker(pool, client, address);
} // Server::Heard(TCP, Address)

void Server::Run() {
   while (pool.Poll()) {
   } // while
} // Server::Run()

Server::~Server() {
} // Server::~Server()
