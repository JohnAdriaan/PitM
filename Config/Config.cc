//
// Config.cc
//

#include "Config.hh"

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

void Config::Save() {
} // Config::Save()
