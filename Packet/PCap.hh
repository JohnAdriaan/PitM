//
// PCap.hh
//

#ifndef PCap_hh
#define PCap_hh

#include "../General/General.hh"

namespace PCap {

typedef unsigned short guint16;
typedef unsigned       guint32;
typedef int            gint32;

#pragma pack(push,1)
struct Global {
   guint32 magic_number;   /* magic number */
   guint16 version_major;  /* major version number */
   guint16 version_minor;  /* minor version number */
   gint32  thiszone;       /* GMT to local correction */
   guint32 sigfigs;        /* accuracy of timestamps */
   guint32 snaplen;        /* max length of captured packets, in octets */
   guint32 network;        /* data link type */
}; // Global
#pragma pack(pop)

#pragma pack(push,1)
struct Packet {
   guint32 ts_sec;         /* timestamp seconds */
   guint32 ts_usec;        /* timestamp microseconds */
   guint32 incl_len;       /* number of octets of packet saved in file */
   guint32 orig_len;       /* actual length of packet */
}; // Packet
#pragma pack(pop)
} // PCap

#endif // PCap_hh
