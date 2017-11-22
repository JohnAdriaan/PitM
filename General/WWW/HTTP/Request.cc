//
// Request.cc
//

#include "Request.hh"

using namespace WWW;

using namespace HTTP;

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

Request *Request::Parse(String line) {
   String method;
   String path;
   String version;

   Pos notSpace = 0;
   Pos space = line.find(' ', notSpace);
   method = line.substr(notSpace, space);

   if (space!=String::npos) {
      notSpace = line.find_first_not_of(' ', space);
      space = line.find(' ', notSpace);
      path = line.substr(notSpace, space);

      if (space!=String::npos) {
         notSpace = line.find_first_not_of(' ', space);
         version = line.substr(notSpace);
      } // if
   } // if
   return new Request(Method(method), path, Version(version));
} // Parse(line)

Request::Request(Methods method, const String &path, Versions version) :
         method(method),
         path(path),
         version(version),
         headers(headerSet),
         headerSet() {
} // Request::Request(method, path, version)

void Request::Append(const String &header) {
   Pos delim = header.find(':');
   String key = header.substr(0, delim);
   Set values;
   auto i = headerSet.find(key);
   Set &set = i!=headerSet.end() ?
              i->second :
              values;

   while (delim!=String::npos) {
      Pos start = header.find_first_not_of(' ', delim+1);
      delim = header.find(',', start);
      set.insert(header.substr(start, delim-start));
   } // while

   if (&set==&values) {
      headerSet[key] = values;
   } // if
} // Request::Append(header)