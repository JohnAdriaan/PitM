//
// Server.cc
//

#include <iostream>

#include <Thread/Atomic.tt>
#include <Socket/Address.hh>

#include "Client.hh"
#include "Server.hh"

using namespace PitM;

static MT::Atomic<Server *> server; // Only one server 'live' at a time

bool Server::Start() {
   Server *s = new Server();
   return server==s;
} // Server::Start()

void Server::Quit() {
   Replace(nullptr);
} // Server::Quit()

void Server::Replace(Server *swap) {
   swap = server.Swap(swap);
   if (swap!=nullptr) {
      swap->Listen::Close();
   } // if
} // Server::Replace(swap)

Server::Server() :
        MT::Thread(),
        BSD::Listen(BSD::Address::any4.Family()) {
   Replace(this);          // Close any existing server for this one
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
   Listen::Close();
   server.Swap(nullptr, this); // Set nullptr, but only if ==this
} // Server::~Server()
