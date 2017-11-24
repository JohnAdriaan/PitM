//
// HTTP.cc
//

#include "HTTP.hh"

using namespace WWW;

const String HTTP::EOL = "\r\n";
const String HTTP::ContentLength = "Content-Length";
const String HTTP::Connection = "Connection";
const String HTTP::KeepAlive = "keep-alive";
const String HTTP::Close = "close";
const String HTTP::Allow = "Allow";
const String HTTP::Location = "Location";
const String HTTP::CacheControl = "Cache-Control";
const String HTTP::MaxAge = "max-age=";

String HTTP::Version(HTTP::Versions version) {
   switch (version) {
   case HTTP::HTTP11 :
      return "HTTP/1.1";
   case HTTP::HTTP10 :
      return "HTTP/1.0";
   case HTTP::Versions::Unknown :
   default :
      return "";
   } // switch
} // Version(version)

HTTP::Versions HTTP::Version(const String &version) {
   if (version=="HTTP/1.1") {
      return HTTP::HTTP11;
   } // if
   if (version=="HTTP/1.0") {
      return HTTP::HTTP10;
   } // if
   if (version=="") {
      return HTTP::HTTP10;
   } // if
   return HTTP::Versions::Unknown;
} // Version(version)
