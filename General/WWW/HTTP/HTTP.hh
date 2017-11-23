//
// HTTP.hh
//

#ifndef HTTP_hh
#define HTTP_hh

#include "../WWW.hh"

namespace WWW {

   namespace HTTP {

      enum Versions {
         Unknown,
         HTTP10,
         HTTP11
      }; // Versions

      static const String EOL = "\r\n";

      // This is the header field for Keep-Alive
      static const String Connection = "Connection";

      // ...and this is the value
      static const String KeepAlive = "keep-alive";

      // This header is signficant to Keep-Alive and HTTP/1.1
      static const String ContentLength = "Content-Length";

      Versions Version(const String &version);

      String Version(Versions version);

      class _HTTP_;

      class URI;

      // This is just the Request Message Header, consisting of
      //    Request Line
      //    Request Headers
      // The body (if any) is separate
      class Request;

      // This is just the Response Header, consisting of
      //   Rwsponse Line
      //   Response Headers
      // The body (if any) is separate
      class Response;

   } // HTTP

} // WWW

class WWW::HTTP::_HTTP_ : private _WWW_ {
}; // _HTTP_

#endif // HTTP_hh
