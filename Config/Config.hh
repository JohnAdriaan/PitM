//
// Config.hh
//

#ifndef Config_hh
#define Config_hh

#include "../PitM.hh"

struct Config {

   // These are the names of the Interfaces, with their defaults.
   // Note that from the middle, there's no real difference between the two sides...
   String left   = "eth0";
   String right  = "eth1";

   String server = "wlan0";

}; // Config

#endif // Config_hh
