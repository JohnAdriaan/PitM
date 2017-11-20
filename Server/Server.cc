//
// Server.cc
//

#include "Worker.hh"
#include "Server.hh"

MT::Atomic<Server *> Server::current;

bool Server::Start() {
   Server *s = new Server();
   return current==s;
} // Server::Start()

Server::Server() :
        BSD::Listen(BSD::Address::any4, Config::config.Port()),
        MT::Thread() {
   if (!Listen::Valid()) {
      delete this;
      return;
   } // if
   if (!Thread::Start()) {
      delete this;
      return;
   } // if

   Server *old = current.Swap(this);
   if (old!=nullptr) {
      old->Close();
   } // if
} // Server::Server()

void Server::Heard(BSD::TCP &client, const BSD::Address &address) {
   new Worker(client, address);
} // Server::Heard(TCP, Address)

void *Server::Run() {
   while (Accept()) {
   } // while
   delete this;
   return nullptr;
} // Server::Run()

Server::~Server() {
   current.Swap(this, nullptr);
} // Server::~Server()
