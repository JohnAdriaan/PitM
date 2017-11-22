//
// URI.cc
//

#include "URI.hh"

using namespace WWW::HTTP;

//static const String defaultScheme = "http";
static const BSD::Port defaultPort = 0;
static const String defaultPath = "/";

static WWW::Map ParseQuery(String s) {
   WWW::Map query;

   for (Pos start = 0;
        start<s.length();
        /*Nothing*/) {
      Pos delim = s.find_first_of("&;", start); // Either/or
      String pair = s.substr(start, delim-start);

      Pos equals = pair.find('=');
      String left = pair.substr(0,equals);
      String right;
      if (equals!=String::npos) {
         right = pair.substr(equals+1);
      } // if
      query[left] = right;

      start += pair.length()+1; // Note NOT delim+1 - that'll become 0
   } // for

   return query;
} // ParseQuery(s)

// This helper function breaks a string into its component parts,
// then uses them to construct a full URI.
// It does assume that the passed-in string passes URI::Valid(string);
static URI Parse(String uri) {
   String scheme;
   String user;
   String password;
   String host;
   BSD::Port port = defaultPort;
   String path;
   WWW::Map query;
   String fragment;

   Pos colonSlashSlash = uri.find("://"); // Not strictly true! TODO
   if (colonSlashSlash!=String::npos) {
      scheme = uri.substr(0,colonSlashSlash);
      uri.erase(0,colonSlashSlash+3);
   } // if

   Pos colon = uri.find(':');
   Pos slash = uri.find('/');
   Pos min = std::min(colon, slash);
   host = uri.substr(0,min);
   if (colon<slash) {
      String service = uri.substr(colon+1, slash-colon);
      port = atoi(service.c_str());
   } // if

   if (slash!=String::npos) {
      uri.erase(0, slash);
      // Now look for the end of the path
      Pos questionMark = uri.find('?');
      Pos hash = uri.find('#');
      Pos min = std::min(questionMark, hash);
      path = uri.substr(0,min);

      if (questionMark<hash) {
         query = ParseQuery(uri.substr(questionMark+1, hash-questionMark));
      } // if

      if (hash!=String::npos) {
         fragment = uri.substr(hash+1);
      } // if
   } // if

   return URI(scheme,user,password,host,port,path,query,fragment);
} // Parse(uri)

bool URI::Valid(const String &uri) {
   Pos colonSlashSlash = uri.find("://"); // Not strictly true! TODO

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

URI::URI(const String &host,
         const String &path) :
     host(host),
     port(defaultPort),
     path(path) {
} // URI::URI(host,path)

URI::URI(const String &scheme,
         const String &user,
         const String &password,
         const String &host,
         BSD::Port port,
         const String &path,
         Map &query,
         const String &fragment) :
     scheme(scheme),
     user(user),
     password(password),
     host(host),
     port(port),
     path(!path.empty() ? path : defaultPath),
     query(query),
     fragment(fragment) {
} // URI::URI(scheme,user,password,host,port,path,query,fragment)

URI::operator String() const {
   String s;
   s.reserve(2048);

   if (!scheme.empty()) {
      s += scheme;
      s += "://";
   } // if

   if (!user.empty()) {
      s += user;
      if (!password.empty()) {
         s += ':';
         s += password;
      } // if
      s += '@';
   } // if
   s += host;

   if (port!=defaultPort) {
      s += ':';
      s += port;
   } // if

   s += path;

   char sep = '?'; // Start separator
   for (const auto i : query) {
      s += sep;
      s += i.first;
      if (!i.second.empty()) {
         s += '=';
         s += i.second;
      } // if
      sep = '&'; // Middle separator
   } // for

   if (!fragment.empty()) {
      s += '#';
      s += fragment;
   } // if

   return s;
} // URI::operator String()
