//
// Socket.hh

#ifndef Socket_hh
#define Socket_hh

#include "BSD.hh"

class BSD::Socket : private _BSD_ {

public: // Methods

   inline bool Valid() const;

   void Close();

protected: // Methods

   Socket(int domain, int type, int protocol=0);

   Socket(Socket &socket);

protected: // Variables

   int socket;

private: // Enums

   enum { Invalid = -1 };

}; // Socket

inline bool BSD::Socket::Valid() const {
   return socket!=Invalid;
} // Socket::Valid()

#endif // Socket_hh
