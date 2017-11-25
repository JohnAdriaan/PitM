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

Request Request::Parse(String line) {
   String method;
   String path;
   String version;

   Pos notSpace = 0;
   Pos space = line.find(' ', notSpace);
   method = line.substr(notSpace, space-notSpace);

   if (space!=String::npos) {
      notSpace = line.find_first_not_of(' ', space);
      space = line.find(' ', notSpace);
      path = line.substr(notSpace, space-notSpace);

      if (space!=String::npos) {
         notSpace = line.find_first_not_of(' ', space);
         version = line.substr(notSpace);
      } // if
   } // if
   return Request(::Method(method), path, HTTP::Version(version));
} // Parse(line)

Request::Request() :
         method(Methods::Unknown),
         path(),
         version(Versions::Unknown),
         headers() {
} // Request::Request()

Request::Request(Methods method, const String &path, Versions version) :
         method(method),
         path(path),
         version(version),
         headers() {
} // Request::Request(method, path, version)

void Request::Append(const String &header) {
   Pos delim = header.find(':');
   String key = header.substr(0, delim);
   auto i = headers.find(key);
   Set &set = i==headers.end() ?
              headers[key] = Set() : // Need new one
              i->second;             // Use existing one

   while (delim!=String::npos) {
      Pos start = header.find_first_not_of(' ', delim+1);
      delim = header.find(',', start);
      set.insert(header.substr(start, delim-start));
   } // while
} // Request::Append(header)

void Request::Body(const String &body) {
   this->body = body;
} // Request::Body(body)

String Request::Get(const String &field, const String &none) const {
   Pos pos = body.find(field);
   if (pos==String::npos) {
      return String();
   } // if
   pos += field.length();
   Pos end = body.find('&', pos);
   String value = body.substr(pos, end-pos);
   if (value==none) {
      return String();
   } // if
   return value;
} // Request::Get(field, none)
