//
// HTTP.hh
//

#ifndef HTTP_hh
#define HTTP_hh

#include "../WWW.hh"

namespace WWW {

   namespace HTTP {

      class _HTTP_;

      class URI;

      // This is just the Request Header - the body (if any) is separate
      class Request;

      // This is just the Response Header - the body (if any) is separate
      class Response;

   } // HTTP

} // WWW

class WWW::HTTP::_HTTP_ : private _WWW_ {
}; // _HTTP_

#endif // HTTP_hh
