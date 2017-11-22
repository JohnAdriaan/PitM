//
// URI.hh
//

#ifndef URI_hh
#define URI_hh

#include "HTTP.hh"

class WWW::HTTP::URI : private _HTTP_ {

public: // Static methods

   static bool Valid(const String &uri);

public: // Methods

   explicit URI(const String &uri);

   // Minimum URI is host and path
   URI(const String &host,
       const String &path);

   URI(const String &scheme,
       const String &user,
       const String &password,
       const String &host,
       BSD::Port    port,
       const String &path,
       Map          &query,
       const String &fragment);

   operator String() const;

public: // Variables

   // e.g. "http" or "https"
   const String scheme;

   // e.g. "John"
   const String user;

   // e.g. "Password1"
   const String password;

   // e.g. "www.google.com.au"
   const String host;

   // e.g. 80 (0 means protocol's default)
   const BSD::Port port;

   // e.g. "/" or "/sub/index.html"
   const String path;

   // e.g. "?user=John&icon=mandelbrot.png"
   const Map query;

   // e.g. "#Heading"
   const String fragment;

}; // URI

#endif // URI_hh
