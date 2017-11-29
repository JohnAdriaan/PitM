//
// Server.cc
//

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
        BSD::Listen(BSD::Address::any4,
                    Config::master.port),
        MT::Thread() {
   if (!Listen::Valid()) {
      delete this;
      return;
   } // if
   if (!Thread::Start()) {
      delete this;
      return;
   } // if
   Quit(this);
} // Server::Server()

void Server::Heard(BSD::TCP &client, const BSD::Address &address) {
   new Client(client, address);
} // Server::Heard(TCP, Address)

void *Server::Run() {
   while (Accept()) {
   } // while
   delete this;
   return nullptr;
} // Server::Run()

Server::~Server() {
   server.Swap(nullptr, this); // If this, set null
} // Server::~Server()
