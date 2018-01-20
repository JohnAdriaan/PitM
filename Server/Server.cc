//
// Server.cc
//

#include <Socket/Address.hh>

#include "Client.hh"
#include "Server.hh"

using namespace PitM;

MT::Atomic<Server *> Server::server;

bool Server::Start() {
   Server *s = new Server();
   return server==s;
} // Server::Start()

void Server::Quit(Server *swap) {
   swap = server.Swap(swap);
   if (swap!=nullptr) {
      swap->Listen::Close();
   } // if
} // Server::Quit(swap)

Server::Server() :
        MT::Thread(),
        BSD::Listen(BSD::Address::any4.Family()) {
   Quit(this);             // Close any existing server for this one
   if (!Listen::Valid()) {
      delete this;
      return;
   } // if
   if (!Thread::Start()) {
      delete this;
      return;
   } // if
} // Server::Server()

void Server::Heard(BSD::TCP &client, const BSD::Address &address) {
   new Client(client, address);
} // Server::Heard(TCP, Address)

void *Server::Run() {
   if (Hear(BSD::Address::any4, Config::master.server)) {
      while (Accept()) {
      } // while
   } // if
   delete this;
   return nullptr;
} // Server::Run()

Server::~Server() {
   server.Swap(nullptr, this); // Set nullptr, but only if this
} // Server::~Server()
