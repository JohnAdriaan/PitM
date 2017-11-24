//
// Config.cc
//

#include "../Packet/Packet.hh"
#include "../Server/Server.hh"

#include "Config.hh"

using namespace PitM;

Config Config::config;

Config::Config() {
   Load();
} // Config::Config()

void Config::Load() { // TODO
} // Config::Load()

void Config::Set(const String &newLeft,
                 const String &newRight,
                 const String &newServer,
                 const String &newPort) {
   bool packetChanged = false;
   if (left!=newLeft) {
      left = newLeft;
      packetChanged = true;
   } // if
   if (right!=newRight) {
      right = newRight;
      packetChanged = true;
   } // if

   bool serverChanged = false;
   if (server!=newServer) {
      server = newServer;
      serverChanged = true;
   } // if
   BSD::Port newP = atoi(newPort.c_str());
   if (port!=newP) {
      port = newP;
      serverChanged = true;
   } // if

   if (!packetChanged && !serverChanged) {
      return;
   } // if
   Save();
   if (packetChanged) {
      Packet::Start();
   } // if
   if (serverChanged) {
      Server::Start();
   } // if
} // Config::Set(newLeft,newRight,newServer,newPort)

void Config::Save() { // TODO
} // Config::Save()
