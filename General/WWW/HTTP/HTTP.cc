//
// HTTP.cc
//

#include "HTTP.hh"

using namespace WWW;

const String HTTP::EOL = "\r\n";
const String HTTP::Connection = "Connection";
const String HTTP::KeepAlive = "keep-alive";
const String HTTP::Close = "close";
const String HTTP::ContentLength = "Content-Length";


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
   return HTTP::Versions::Unknown;
} // Version(version)
