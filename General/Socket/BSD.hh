//
// BSD.hh
//

#ifndef BSD_hh
#define BSD_hh

#include "../General.hh"

namespace BSD {

enum Protocols {
   NoProtocol,
   IPv4,
   IPv6,
   IPv46
}; // Protocols

enum States {
   NoState,
   Loopback,
   Down,
   Up
}; // States

class _BSD_;

class Interface;

class Interfaces;

class Socket;

class Listen;

class TCP;

class Raw;

} // BSD

class BSD::_BSD_ : private _General_ {
}; // _BSD_

#endif // BSD_hh
