//
// Socket.hh

#ifndef Socket_hh
#define Socket_hh

#include "BSD.hh"

class BSD::Socket : private _BSD_ {

protected: // Methods

   Socket();

   Socket(Socket &socket);

}; // Socket

#endif // Socket_hh
