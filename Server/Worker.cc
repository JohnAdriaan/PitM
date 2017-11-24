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

static const String html =
      "<!DOCTYPE html>\n"
      "<html lang=en-AU>\n"
      "<head>\n"
      "<meta charset=UTF-8 />\n"
      "<meta name=viewport content=\"width=device-width, initial-scale=1.0\" />\n"
      "<link rel=\"shortcut icon\" href=/favicon.ico />\n"
      "<link rel=stylesheet type=text/css href=style.css />\n"
      "<title>Pi in the Middle (PitM)</title>\n"
      "</head>\n";

static const String badge =
      "<body>\n"
      "<h1 class=left>\n"
      "<a href=\"/\"><img src=favicon.ico alt=\"John Burger\" height=64 width=64 /></a>\n"
      "PitM"
      "</h1>";

static const String fullHeading =
      badge +
      "<h1 class=right>\n"
      "Pi in the Middle<br />\n"
      "<small><small><small><small>"
      "Ver: " + PitM::Version() + ", by John Burger"
      "</small></small></small></small>\n"
      "</h1>\n";

static const String tail =
      "</body>\n"
      "</html>";

Worker::Worker(BSD::TCP &client, const BSD::Address &address) :
        TCP(client),
        Thread(),
        request(),
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
   std::cout << "GET " << request.Path() << std::endl;
   if (request.Path()=="/") {
      return SendHomePage(head);
   } // if
   if (request.Path()=="/favicon.ico") {
      return SendObj(head,favicon, &faviconSize);
   } // if
   if (request.Path()=="/style.css") {
      return SendStyleSheet(head);
   } // if
   if (request.Path()=="/config") {
      return SendConfigPage(head);
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Worker::GET(head)

bool Worker::SendHomePage(bool head) {
   static const String body =
      html +
      fullHeading +
      "<h3><a href=\"/status\">Status</a></h3>\n"
      "<h3><a href=\"/stats\">Statistics</a></h3>\n"
      "<h3><a href=\"/config\">Configuration</a></h3>\n"
      "<h3><a href=\"/logs\">Logs</a></h3>\n"
      "<form action=\"/quit\" method=POST"
      " onsubmit=\"return confirm('Are you sure you want to quit?')\">\n"
      "<input type=submit value=\"Quit\" style=\"color:red\" />\n"
      "</form>\n" +
      tail;

   Response response(HTTP11, Response::OK, body.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Worker::SendHomePage(head)

bool Worker::SendStyleSheet(bool head) {
   static const String body =
      "h1.left  { display: inline-block; position: relative; width: 96px }\n"
      "h1.right { display: inline-block; position: relative }\n"
      "fieldset { display: inline-block }\n";
   Response response(HTTP11, Response::OK, body.length());
   response.Add(CacheControl, MaxAge, 60*60); // An hour ought to do!
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Worker::SendStyleSheet(head)

static String Selection(const BSD::Interfaces &interfaces,
                        const String &label,
                        String current) {
   String selection;
   selection.reserve(256);
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
   BSD::Interfaces up     = BSD::Interface::List(BSD::IPv4, BSD::Up);
   BSD::Interfaces upDown = BSD::Interface::List(BSD::IPv4, BSD::UpDown);

   const String body =
      html +
      badge +
      "<h1 class=right>Configuration</h1>\n"
      "<form method=POST>\n" + // action="/config" is assumed
      Selection(upDown, "Left ", Config::config.Left())  +
      Selection(upDown, "Right", Config::config.Right()) +
      "<br /><fieldset>\n"
      "<legend>Web</legend>\n" +
      Selection(up, "Server", Config::config.Server()) +
      " Port: <input style=\"width:5em\" type=number min=1 max=65535 name=Port value=" +
        std::to_string(Config::config.Port()) + " />\n"
      "</fieldset>\n"
      "<p /><input type=submit>\n"
      "</form>\n" +
      tail;

   Response response(HTTP11, Response::OK, body.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Worker::SendConfigPage(head,badLeft,badRight,badServer,badPort)

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
   response.Add(CacheControl, MaxAge, 60*60); // An hour ought to do!
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !FD::Write(obj,length)) {
      return false;
   } // if
   return true;
} // Worker::SendObj(head,obj,size)

bool Worker::POST() {
   std::cout << "POST " << request.Path() << std::endl;
   std::cout << request.Body() << std::endl;
   if (request.Path()=="/config") {
      return Config();
   } // if
   if (request.Path()=="/quit") {
      return Quit();
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Worker::POST()

bool Worker::Config() {
   String left = request.Get("Left=", "None");
   String right = request.Get("Right=", "None");
   String server = request.Get("Server=", "None");
   String port = request.Get("Port=");

   Config::config.Set(left, right, server, port);
   Response response(HTTP11, Response::NoContent);
   return Write(response);
} // Worker::Config()

bool Worker::Quit() {
   static const String body =
      html +
      fullHeading +
      "<p>Thank you for using PitM!</p>\n" +
      tail;
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
               if (line[len]!='\r') {
                  break;
               } // if
               line.pop_back();
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
      request = Request::Parse(line);
      state = RequestHeader;
      break;
   case RequestHeader : {
      if (!line.empty()) {
         request.Append(line);
         break;
      } // if
      state = RequestDone;

      const WWW::MapSet &headers = request.Headers();
      auto h = headers.find(ContentLength);
      if (h==headers.end()) {
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
      request.Body(line);
      state = RequestDone;
      break;
   case RequestDone :
      break;
   } // switch
   return true;
} // Worker::Process()

bool Worker::Reply() {
   switch (request.Method()) {
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
      Write(Response(HTTP10, Response::NotImplemented));
      return false;
   default :
      break;
   } // switch
   WWW::Set set = { "GET", "HEAD", "POST" };
   Response response(HTTP10, Response::MethodNotAllowed);
   response.Add(Allow, set);
   Write(response);
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
      } // if
      line.clear();
   } // while
   std::cout << "Shutting down" << std::endl;
   Close();
   delete this;
   return nullptr;
} // Worker::Run()
