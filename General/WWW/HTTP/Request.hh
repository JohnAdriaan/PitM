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

public: // Static methods

   static Request *Parse(String line);

public: // Methods

   Request(Methods method, const String &path, Versions version);

   void Append(const String &header);

public: // Variables

   const Methods method;

   const String path;

   const Versions version;

   const MapSet &headers;

private: // Variables

   MapSet headerSet;

}; // Request

#endif // Request_hh
