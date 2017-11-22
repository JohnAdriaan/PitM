//
// Request.hh
//

#ifndef Request_hh
#define Request_hh

#include "HTTP.hh"

class WWW::HTTP::Request {

public: // Enums

   enum Methods {
      Unknown,
      GET,
      HEAD,
      POST,
      PUT,
      DELETE,
      TRACE,
      OPTIONS,
      CONNECT,
      PATCH
   }; // Methods

public: // Methods

   explicit Request(String request);

   bool Valid() const;

public: // Variables

   const Methods method;

   const String path;

   const Versions version;

   const MapSet headers;

private: // Variables

   bool valid;

}; // Request

#endif // Request_hh
