//
// Config.hh
//

#ifndef Config_hh
#define Config_hh

#include "../General/Socket/BSD.hh"

#include "../PitM.hh"

// There is only one Config::master.
// You can have as many Configs as you want (especially to change Config::master)
// but the only one that is actually used by the system is Config::master.
class PitM::Config : private _PitM_ {

public: // Static variables

   static const Config &master;

public: // Static methods

   static void Set(const Config &newConfig);

public: // Methods

   Config();

private: // Methods

   void Load();

   void Save();

public: // Variables

   // These are the names of the Interfaces, with their defaults.
   // Note that from the middle, there's no real difference between the two sides...
   String left   = "eth0";
   String right  = "eth1";

   String protocol = "Ethernet";

   String server = "wlan0";

   BSD::Port port = 8080;

}; // Config

#endif // Config_hh
