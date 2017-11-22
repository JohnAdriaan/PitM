//
// URI.hh
//

#ifndef URI_hh
#define URI_hh

#include <map>

#include "HTTP.hh"

class WWW::HTTP::URI : private _HTTP_ {

public: // Typedefs

   typedef std::map<String, String> Parameters;

public: // Static methods

   static bool Valid(const String &uri);

public: // Methods

   explicit URI(const String &uri);

   // Minimum URI is hostname and path
   URI(const String &hostname,
       const String &path);

   URI(const String &protocol,
       const String &hostname,
       const String &port,
       const String &path,
       Parameters &parameters,
       const String &anchor);

   operator String() const;

   inline BSD::Port Port() const;

public: // Variables

   // e.g. http or https
   const String protocol;

   // e.g. www.google.com.au
   const String hostname;

   // e.g. 80 (blank means protocol's default)
   const String port;

   // e.g. / or /index.html
   const String path;

   // e.g. ?user=John&icon=mandelbrot.png
   const Parameters parameters;

   // e.g. #Heading
   const String anchor;

}; // URI

inline BSD::Port WWW::HTTP::URI::Port() const {
   return ::atoi(port.c_str());
} // URI::Port()

#endif // URI_hh
