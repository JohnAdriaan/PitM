//
// Config.hh
//

#ifndef Config_hh
#define Config_hh

#include "../PitM.hh"

class Config : private _PitM_ {

public: // Static variables

   static Config config;

public: // Methods

   inline const String &Left() const;

   inline const String &Right() const;

   inline const String &Server() const;

   void SetLeft(const String &newLeft);

   void SetRight(const String &newRight);

   void SetServer(const String &newServer);

private: // Methods

   // Prevent any more Configs, other than this one
   Config();

   void Load();

   void Save();

private: // Variables

   // These are the names of the Interfaces, with their defaults.
   // Note that from the middle, there's no real difference between the two sides...
   String left   = "eth0";
   String right  = "eth1";

   String server = "wlan0";

}; // Config

inline const String &Config::Left() const {
   return left;
} // Config::Left()

inline const String &Config::Right() const {
   return right;
} // Config::Right()

inline const String &Config::Server() const {
   return server;
} // Config::Server()

#endif // Config_hh
