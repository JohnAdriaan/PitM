//
// URI.cc
//

#include "URI.hh"

using namespace WWW::HTTP;

//static const String defaultProtocol = "http";
static const String defaultPath = "/";

static URI::Parameters ParseParameters(String params) {
   URI::Parameters parameters;

   for (Pos start = 0;
        start<params.length();
        /*Nothing*/) {
      Pos ampersand = params.find('&', start);
      String pair = params.substr(start, ampersand-start);

      Pos equals = pair.find('=');
      String left = pair.substr(0,equals);
      String right;
      if (equals!=String::npos) {
         right = pair.substr(equals+1);
      } // if
      parameters[left] = right;

      start += pair.length()+1; // Note NOT ampersand+1 - that'll become 0
   } // for

   return parameters;
} // ParseParameters(params)

// This helper function breaks a string into its component parts,
// then uses them to construct a full URI.
// It does assume that the passed-in string passes URI::Valid(string);
static URI Parse(String uri) {
   String protocol;
   String hostname;
   String port;
   String path;
   URI::Parameters parameters;
   String anchor;

   Pos colonSlashSlash = uri.find("://");
   if (colonSlashSlash!=String::npos) {
      protocol = uri.substr(0,colonSlashSlash);
      uri.erase(0,colonSlashSlash+3);
   } // if

   Pos slash = uri.find('/');
   if (slash!=String::npos) {
      hostname = uri.substr(0,slash);
      uri.erase(0,slash+1);

      // Now look for the end of the path
      Pos questionMark = uri.find('?');
      Pos hash = uri.find('#');
      Pos pos = std::min(questionMark, hash);
      path = uri.substr(0,pos);

      if (questionMark<hash) {
         parameters = ParseParameters(uri.substr(questionMark+1, hash-questionMark));
      } // if

      if (hash!=String::npos) {
         anchor = uri.substr(hash+1);
      } // if
   } // if

   return URI(protocol,hostname,port,path,parameters,anchor);
} // Parse(uri)

bool URI::Valid(const String &uri) {
   Pos colonSlashSlash = uri.find("://");

   // Check for an earlier occurrence of '/' than above!
   Pos slash = uri.find('/');
   if (slash==colonSlashSlash+1) {
      slash = uri.find('/', colonSlashSlash+3);
   } // if
   if (colonSlashSlash!=String::npos) {
      if (slash<colonSlashSlash) {
         return false;
      } // if
   } // if

   Pos questionMark = uri.find('?');
   Pos hash = uri.find('#');
   Pos pos = std::min(questionMark,hash);
   if (pos<slash) {
      return false;
   } // if

   return true;
} // URI::Valid(uri)

URI::URI(const String &uri) :
     URI(Parse(uri)) {
} // URI::URI(uri)

URI::URI(const String &hostname,
         const String &path) :
     hostname(hostname),
     path(path) {
} // URI::URI(hostname,path)

URI::URI(const String &protocol,
         const String &hostname,
         const String &port,
         const String &path,
         Parameters &parameters,
         const String &anchor) :
     protocol(protocol),
     hostname(hostname),
     port(port),
     path(!path.empty() ? path : defaultPath),
     parameters(parameters),
     anchor(anchor) {
} // URI::URI(protocol,hostname,port,path,parameters,anchor)

URI::operator String() const {
   String s;
   s.reserve(2048);

   if (!protocol.empty()) {
      s += protocol;
      s += "://";
   } // if

   s += hostname;

   if (!port.empty()) {
      s += ':';
      s += port;
   } // if

   s += path;

   char sep = '?'; // Start separator
   for (const auto i : parameters) {
      s += sep;
      s += i.first;
      s += '=';
      s += i.second;
      sep = '&'; // Middle separator
   } // for

   if (!anchor.empty()) {
      s += '#';
      s += anchor;
   } // if

   return s;
} // URI::operator String()
