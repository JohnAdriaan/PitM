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

   static Request Parse(String line);

public: // Methods

   Request();

   Request(Methods method, const String &path, Versions version);

   Request &operator =(const Request &rhs) = default;

   void Append(const String &header);

   void Body(const String &body);

   // Get "field=" from POST body. If value==none, return empty string
   String Get(const String &field, const String &none=String()) const;

   inline Methods Method() const;

   inline const String &Path() const;

   inline Versions Version() const;

   inline const MapSet &Headers() const;

   inline const String &Body() const;

private: // Variables

   Methods method;

   String path;

   Versions version;

   MapSet headers;

   String body;

}; // Request

inline WWW::HTTP::Request::Methods WWW::HTTP::Request::Method() const {
   return method;
} // Request::Method()

inline const String &WWW::HTTP::Request::Path() const {
   return path;
} // Request::Path()

inline WWW::HTTP::Versions WWW::HTTP::Request::Version() const {
   return version;
} // Request::Version()

inline const WWW::MapSet &WWW::HTTP::Request::Headers() const {
   return headers;
} // Request::Headers()

inline const String &WWW::HTTP::Request::Body() const {
   return body;
} // Request::Body()

#endif // Request_hh
