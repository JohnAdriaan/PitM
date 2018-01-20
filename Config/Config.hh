//
// Config.hh
//

#ifndef Config_hh
#define Config_hh

#include <vector>

#include <Socket/BSD.hh>

#include "../PitM.hh"

// There is only one Config::master.
// You can have as many Configs as you want (especially to change Config::master)
// but the only one that is actually used by the system is Config::master. See Config::Set()
class PitM::Config : private _PitM_ {

public: // Typedefs

   // The order of Ports in the list is important for the user interface only
   typedef std::vector<String> Ports;

public: // Static variables

   static const BSD::Port MaxPort = BSD::MaxPort; // BSD::ReservedPorts;

   // You can look at Config::master - but you can only Config::Set() it
   static const Config &master;

public: // Static methods

   // Use this to update Config::master
   static void Set(const Config &newConfig);

public: // Methods

   // You want your own Config? Fine - but there's only one Config::master
   Config();

   // You can create Config copies - but there's only one Config::master
   Config(const Config &config) = default;

   // You can copy Configs - but there's only one Config::master.
   Config &operator =(const Config &config) = default;

public: // Variables

   // Server port
   BSD::Port server = 8080;

   // These are the names of the Interfaces, with their defaults.
   // Note that from the middle, there's no real difference between the two sides...
   String left   = "eth0";
   String right  = "eth1";

   String protocol = "Ethernet";

   // Monitor ICMP?
   bool icmp = true;

   // All the ports to monitor
   Ports ports;

private: // Methods

   // Load Config::master
   void Load();

   // Save Config::master
   void Save();

}; // Config

#endif // Config_hh
