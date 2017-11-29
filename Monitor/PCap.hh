//
// PCap.hh
//

#ifndef PCap_hh
#define PCap_hh

#include "../General/General.hh"

namespace PCap {

enum LinkTypes {
   Loopback   = 0,
   Ethernet   = 1,
   TokenRing  = 6,
   SLIP       = 8,
   PPP        = 9,
   PPPoE      = 51,
   Raw        = 101,
   IEEE802_11 = 105,
   RawIPv4    = 228,
   RawIPv6    = 229
}; // LinkTypes

typedef int            gint32;
typedef unsigned       guint32;
typedef unsigned short guint16;

#pragma pack(push,1)
struct Global {
   guint32 magic_number  = 0xA1B2C3D4; // Magic number
   guint16 version_major = 2;          // Major version number
   guint16 version_minor = 4;          // Minor version number
   gint32  thiszone      = 0;          // GMT to local correction
   guint32 sigfigs       = 0;          // Accuracy of timestamps
   guint32 snaplen       = 65535;      // Max length of captured packets, in octets
   guint32 network;                    // Data link type
}; // Global
#pragma pack(pop)

#pragma pack(push,1)
struct Packet {
   guint32 ts_sec;         // Timestamp seconds
   guint32 ts_usec;        // Timestamp microseconds
   guint32 incl_len;       // Number of octets of packet saved in file
   guint32 orig_len;       // Actual length of packet
}; // Packet
#pragma pack(pop)

} // PCap

#endif // PCap_hh
