//
// Config.cc
//

#include "../Packet/Packet.hh"
#include "../Server/Server.hh"

#include "Config.hh"

using namespace PitM;

Config config;

const Config &Config::master = config;

Config::Config() {
   if (this==&master) {
      Load();
   } // if
} // Config::Config()

void Config::Load() { // TODO
} // Config::Load()

void Config::Set(const Config &newConfig) {
   bool packetChanged = false;
   if (config.left!=newConfig.left) {
      config.left = newConfig.left;
      packetChanged = true;
   } // if
   if (config.right!=newConfig.right) {
      config.right = newConfig.right;
      packetChanged = true;
   } // if

   bool configChanged = false;
   if (config.protocol!=newConfig.protocol) {
      config.protocol = newConfig.protocol;
      configChanged = true;
   } // if
   if (config.icmp!=newConfig.icmp) {
      config.icmp = newConfig.icmp;
      configChanged = true;
   } // if
   if (config.ports!=newConfig.ports) {
      config.ports = newConfig.ports;
      configChanged = true;
   } // if

   bool serverChanged = false;
   if (config.server!=newConfig.server) {
      config.server = newConfig.server;
      serverChanged = true;
   } // if
   if (config.port!=newConfig.port) {
      config.port = newConfig.port;
      serverChanged = true;
   } // if

   if (!packetChanged &&
       !configChanged &&
       !serverChanged) {
      return;
   } // if
   config.Save();
   if (packetChanged) {
      Packet::Start();
   } // if
   else if (configChanged) { // If the former, no need to do this!
      Packet::Reconfigure();
   } // else if
   if (serverChanged) {
      Server::Start();
   } // if
} // Config::Set(newConfig)

void Config::Save() { // TODO
} // Config::Save()
