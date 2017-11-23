//
// Config.cc
//

#include "Config.hh"

using namespace PitM;

Config Config::config;

Config::Config() {
   Load();
} // Config::Config()

void Config::Load() {
} // Config::Load()

void Config::SetLeft(const String &newLeft) {
   left = newLeft;
   Save();
} // Config::SetLeft(newLeft)

void Config::SetRight(const String &newRight) {
   right = newRight;
   Save();
} // Config::SetRight(newRight)

void Config::SetServer(const String &newServer) {
   server = newServer;
   Save();
} // Config::SetServer(newServer)

void Config::SetPort(BSD::Port newPort) {
   port = newPort;
   Save();
} // Config::SetPort(newPort)

void Config::Save() {
} // Config::Save()
