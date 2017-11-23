//
// Worker.cc
//

#include <iostream> // TODO

#include "../General/File/File.hh"

#include "../General/Socket/Interface.hh"

#include "../General/WWW/HTTP/Response.hh"

#include "../Config/Config.hh"

#include "Worker.hh"

extern const char favicon[];

extern char faviconSize; // Get the ADDRESS of this!!!

using namespace WWW;

using namespace WWW::HTTP;

using namespace PitM;

Worker::Worker(BSD::TCP &client, const BSD::Address &address) :
        TCP(client),
        Thread(),
        request(nullptr),
        state(RequestLine),
        line(),
        buffer(),
        read(),
        pos(),
        start(),
        contentLength() {
   line.reserve(2048);
   if (!TCP::Valid()) {
      delete this;
      return;
   } // if
   if (!Start()) {
      delete this;
      return;
   } // if
} // Worker::Worker(client, address)

bool Worker::GET(bool head) {
   std::cout << "GET " << request->path << std::endl;
   if (request->path=="/") {
      return SendHomePage(head);
   } // if
   if (request->path=="/favicon.ico") {
      return SendObj(head,favicon, &faviconSize);
   } // if
   if (request->path=="/config") {
      return SendConfigPage(head);
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Worker::GET(head)

bool Worker::SendHomePage(bool head) {
   static const String body =
      "<!DOCTYPE html>\n"
      "<html lang=en-AU>\n"
      "<head>\n"
      "<meta charset=UTF-8 />\n"
      "<meta name=viewport content=\"width=device-width, initial-scale=1.0\" />\n"
      "<title>Pi in the Middle (PitM)</title>\n"
      "</head>\n"
      "<body>\n"
      "<h1>\n"
      "<img src=favicon.ico alt=\"John Burger\" height=64 width=64 />"
      "&nbsp;Pi in the Middle (PitM)<br />\n"
      "<small><small><small><small>"
      "Ver: " + PitM::version + ", by John Burger"
      "</small></small></small></small>\n"
      "</h1>\n"
      "<h3><a href=\"/status\">Status</a></h3>\n"
      "<h3><a href=\"/stats\">Statistics</a></h3>\n"
      "<h3><a href=\"/config\">Configuration</a></h3>\n"
      "<h3><a href=\"/logs\">Logs</a></h3>\n"
      "<form action=\"/quit\" method=\"POST\""
      " onsubmit=\"return confirm('Are you sure you want to quit?')\">\n"
      "<input type=submit value=\"Quit\" style=\"color:red\" />\n"
      "</form>\n"
      "</body>\n"
      "</html>";

   Response response(HTTP11, Response::OK, body.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Worker::SendHomePage(head)

static String Selection(const BSD::Interfaces &interfaces,
                        const String &label,
                        String current) {
   String selection;
   selection.reserve(256);
   selection += "<br />";
   selection += "<label for=" + label + ">" + label + ": </label>\n";
   selection += "<select name=" + label + " id=" + label + ">\n";
   selection += "  <option value=\"None\"";
   if (current.empty()) {
      selection += " selected";
   } // if
   selection += ">None</option>\n";
   for (const auto &i : interfaces) {
      selection += "  <option value=\"" + i.name + '"';
      if (current==i.name) {
         selection += " selected";
      } // if
      selection += ">" + i.name + "</option>\n";
   } // for
   selection += "</select>\n";
   return selection;
} // Selection(Interfaces, label, current)

bool Worker::SendConfigPage(bool head) {
   BSD::Interfaces interfaces = BSD::Interface::List(BSD::IPv4, BSD::Up);

   const String body =
         "<!DOCTYPE html>\n"
         "<html lang=en-AU>\n"
         "<head>\n"
         "<meta charset=UTF-8 />\n"
         "<meta name=viewport content=\"width=device-width, initial-scale=1.0\" />\n"
         "<title>PitM: Configuration</title>\n"
         "</head>\n"
         "<body>\n"
         "<h1>\n"
         "<img src=favicon.ico alt=\"John Burger\" height=64 width=64 />"
         "&nbsp;PitM: Configuration"
         "</h1>\n"
         "<form method=\"POST\">\n" // action="/config" is assumed
         + Selection(interfaces, "Left", Config::config.Left())
         + Selection(interfaces, "Right", Config::config.Right())
         + Selection(interfaces, "Server", Config::config.Server()) +
         "<br />Port: <input type=number name=Port value=" + std::to_string(Config::config.Port()) + ">\n"
         "<br /><input type=submit>\n"
         "</form>\n"
         "</body>\n"
         "</html>";

   Response response(HTTP11, Response::OK, body.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Worker::SendConfigPage(head)

bool Worker::SendFile(bool head,const char *path) {
   File file(path);
   if (!file.Valid()) {
      return false;
   } // if
   Size length = file.Size();

   Response response(HTTP11, Response::OK, length);

   if (!Write(response)) {
      return false;
   } // if
   if (!head && !FD::SendFile(file)) {
      return false;
   } // if
   return true;
} // Worker::SendFile(head,path)

bool Worker::SendObj(bool head, const void *obj, const void *size) {
   Size length = (Size)size; // Convert (absolute) address to size

   Response response(HTTP11, Response::OK, length);
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !FD::Write(obj,length)) {
      return false;
   } // if
   return true;
} // Worker::SendObj(head,obj,size)

bool Worker::POST() {
   std::cout << "POST " << request->path << std::endl;
   std::cout << line << std::endl;
   if (request->path=="/config") {
      return Config();
   } // if
   if (request->path=="/quit") {
      return Quit();
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Worker::POST()

bool Worker::Config() {
   // Decode line into Config parameters // TODO
   Response response(HTTP11, Response::SeeOther);
   response.Add(Location, "/config");
   return Write(response);
} // Worker::Config()

bool Worker::Quit() {
   static const String body =
      "<!DOCTYPE html>\n"
      "<html lang=en-AU>\n"
      "<head>\n"
      "<meta charset=UTF-8 />\n"
      "<meta name=viewport content=\"width=device-width, initial-scale=1.0\" />\n"
      "<title>Pi in the Middle (PitM)</title>\n"
      "</head>\n"
      "<body>\n"
      "<h1>\n"
      "Pi in the Middle (PitM)<br />\n"
      "<small><small><small><small>"
      "Ver: " + PitM::version + ", by John Burger"
      "</small></small></small></small>\n"
      "</h1>\n"
      "<p>Thank you for using PitM!</p>\n"
      "</body>\n"
      "</html>\n";
   Response response(HTTP11, Response::OK, body);
   response.Add(HTTP::Connection, HTTP::Close);
   Write(response);
   PitM::quit.Post();
   return false; // Might as well close; shutting down anyway!
} // Worker::Quit()

bool Worker::Parse() {
   for (;;) {
      while (pos<read) {
         // Don't manipulate CR/LF in Request Body...
         if (state==RequestBody) {
            ++pos;
            if (--contentLength>0) {
               continue;
            } // if
            line.append(buffer+start, pos-start);
         } // if
         // ...only Request Message Header
         else {
            if (buffer[pos++]!='\n') {
               continue;
            } // if
            line.append(buffer+start, pos-start-1);
            unsigned len = line.length();
            while (len-->0) {
               if (line[len]=='\r') {
                  line.pop_back();
               } // if
            } // while
         } // else
         start = pos;
         return true;
      } // while
      line.append(buffer+start, read-start);
      if (!Read(buffer, sizeof buffer, read)) {
        return false;
      } // if
      if (read==0) {
         return false;
      } // if
      pos = 0;
      start = 0;
   } // for
   return false; // Impossible!
} // Worker::Parse()

bool Worker::Process() {
   switch (state) {
   case RequestLine :
      state = RequestDone; // Assume failure
      request = Request::Parse(line);
      if (request==nullptr) {
         return false;
      } // if
      switch (request->method) {
      case Request::Unknown :
      case Request::PUT     :
      case Request::DELETE  :
      case Request::TRACE   :
      case Request::OPTIONS :
      case Request::CONNECT :
      case Request::PATCH   :
      default :
         return false;
      case Request::GET  :
      case Request::HEAD :
      case Request::POST :
         break;
      } // switch
      state = RequestHeader; // Can continue!
      break;
   case RequestHeader : {
      if (!line.empty()) {
         request->Append(line);
         break;
      } // if
      state = RequestDone;

      auto h = request->headers.find(ContentLength);
      if (h==request->headers.end()) {
         break;
      } // if

      const WWW::Set &set = h->second;
      if (set.empty()) {
         break;
      } // if

      // Only look at first entry
      contentLength = atoi(set.begin()->c_str());
      if (contentLength==0) {
         break;
      } // if

      state = RequestBody;
      break;
   } // RequestHeader
   case RequestBody :
      state = RequestDone;
      break;
   case RequestDone :
      break;
   } // switch
   return true;
} // Worker::Process()

bool Worker::Reply() {
   if (request==nullptr) {
      Write(Response(HTTP10, Response::InsufficientStorage));
      return false;
   } // if
   switch (request->method) {
   case Request::Unknown :
      Write(Response(HTTP10, Response::BadRequest));
      return false;
   case Request::GET :
      return GET(false);
   case Request::HEAD    :
      return GET(true);
   case Request::POST    :
      return POST();
   case Request::PUT     :
   case Request::DELETE  :
   case Request::TRACE   :
   case Request::OPTIONS :
   case Request::CONNECT :
   case Request::PATCH   :
   default :
      break;
   } // switch
   Write(Response(HTTP10, Response::NotImplemented));
   return false;
} // Worker::Reply()

void *Worker::Run() {
   while (Parse()) {
      if (!Process()) {
         break;
      } // if
      if (state==RequestDone) {
         if (!Reply()) {
            break;
         } // if
         state = RequestLine;
         delete request;
         request = nullptr;
      } // if
      line.clear();
   } // while
   std::cout << "Shutting down" << std::endl;
   Close();
   delete this;
   return nullptr;
} // Worker::Run()

Worker::~Worker() {
   delete request;
} // Worker::~Worker()
