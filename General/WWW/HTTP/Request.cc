//
// Request.cc
//

#include "Request.hh"

using namespace WWW;

using namespace HTTP;

static HTTP::Versions Version(const String &version) {
   if (version=="HTTP/1.1") {
      return HTTP::HTTP11;
   } // if
   if (version=="HTTP/1.0") {
      return HTTP::HTTP10;
   } // if
   return HTTP::Versions::Unknown;
} // Version(version)

static Request::Methods Method(const String &method) {
   if (method=="GET") {
      return Request::GET;
   } // if
   if (method=="HEAD") {
      return Request::HEAD;
   } // if
   if (method=="POST") {
      return Request::POST;
   } // if
   if (method=="PUT") {
      return Request::PUT;
   } // if
   if (method=="DELETE") {
      return Request::DELETE;
   } // if
   if (method=="TRACE") {
      return Request::TRACE;
   } // if
   if (method=="OPTIONS") {
      return Request::OPTIONS;
   } // if
   if (method=="CONNECT") {
      return Request::CONNECT;
   } // if
   if (method=="PATCH") {
      return Request::PATCH;
   } // if
   return Request::Methods::Unknown;
} // Method(string)
