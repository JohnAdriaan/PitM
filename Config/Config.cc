//
// Config.cc
//

#include <fstream>

#include <File/File.hh>

#include "../Monitor/Monitor.hh"

#include "../Server/Server.hh"

#include "Config.hh"

using namespace PitM;

static const String fileName = File::Expand("~/.PitM");

Config config;

const Config &Config::master = config;

Config::Config() {
   if (this==&master) {
      Load();
   } // if
} // Config::Config()

void Config::Load() {
   String line;
   std::ifstream f(fileName);

   while (std::getline(f,line)) {
      Pos pos = line.find('=');
      String field = line.substr(0,pos);
      String value = line.substr(pos+1);
      if (field=="Left") {
         left = value;
         continue;
      } // if
      if (field=="Right") {
         right = value;
         continue;
      } // if
      if (field=="Protocol") {
         protocol = value;
         continue;
      } // if
      if (field=="ICMP") {
         icmp = (value=="Y");
         continue;
      } // if
      if (field=="Server") {
         server = value;
         continue;
      } // if
      if (field=="Port") {
         port = ToNumber(value);
         continue;
      } // if
      if (field.find("Port")==0) {
         ports.push_back(value);
         continue;
      } // if
   } // while
} // Config::Load()

void Config::Set(const Config &newConfig) {
   bool monitorChanged = false;
   if (config.left!=newConfig.left) {
      config.left = newConfig.left;
      monitorChanged = true;
   } // if
   if (config.right!=newConfig.right) {
      config.right = newConfig.right;
      monitorChanged = true;
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

   if (!monitorChanged &&
       !configChanged &&
       !serverChanged) {
      return;
   } // if
   config.Save();
   if (monitorChanged) {
      Monitor::Start();
   } // if
   else if (configChanged) { // If the former, no need to do this!
      Monitor::Reconfigure();
   } // else if
   if (serverChanged) {
      Server::Start();
   } // if
} // Config::Set(newConfig)

void Config::Save() {
   std::ofstream f(fileName, std::ofstream::trunc);

   f << "Left="     << left     << std::endl;
   f << "Right="    << right    << std::endl;
   f << "Protocol=" << protocol << std::endl;
   f << "ICMP="     << (icmp ?
                        'Y' :
                        'N')    << std::endl;
   f << "Server="   << server   << std::endl;
   f << "Port="     << port     << std::endl;

   unsigned i = 1;
   for (const auto &p : ports) {
      f << "Port" << i++ << '=' << (String)p << std::endl;
   } // for
} // Config::Save()
