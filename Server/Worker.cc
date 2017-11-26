//
// Worker.cc
//

#include <iostream> // TODO

#include "../General/File/File.hh"

#include "../General/Socket/Service.hh"
#include "../General/Socket/Interface.hh"

#include "../General/WWW/HTTP/Response.hh"

#include "../Packet/Packet.hh"

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
      "<title>PitM: Pi in the Middle</title>\n"
      "</head>\n";

static const String heading =
      "<body>\n"
      "<h1 class=left>\n"
      "<a style=\"color:black\" href=\"/\">\n"
      "<img src=favicon.ico alt=\"John Burger\" height=64 width=64 />\n"
      "PitM</a></h1>\n"
      "<h1 class=right>\n"
      "Pi in the Middle\n"
      "<div style=\"font-size:10pt\">"
      "&nbsp;Ver: " + PitM::Version() + ", by John Burger<br /><br /></div>\n"
      "<div id=heading style=\"font-size:16pt;padding:4px;border-style:solid;border-width:1px;border-color:transparent\">&nbsp;</div>\n"
      "</h1>\n";

static const String tail =
      "</body>\n"
      "</html>";

Worker::Worker(BSD::TCP &client, const BSD::Address &address) :
        TCP(client),
        Thread(),
        config(Config::master),
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
      return SendObj(head, favicon, &faviconSize);
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
      heading +
      "<h3><a href=\"/config\">Configuration</a></h3>\n"
      "<h3><a href=\"/stats\">Statistics</a></h3>\n"
      "<h3><a href=\"/logs\">Logs</a></h3>\n"
      "<form action=\"/quit\" method=POST"
      " onsubmit=\"return confirm('Are you sure you want to quit?')\">\n"
      "<input type=submit value=\"Quit\" style=\"color:red\" />\n"
      "</form>\n";

   String status = body;
   status += "<script>\n"
             " var h = document.getElementById(\"heading\");\n"
             " h.innerHTML = \""
             "Packets: ";
   status += ToCommas(Packet::Total());
   status += " Logged: ";
   status += ToCommas(Packet::Logged());
   status += "\";\n"
             " h.style.borderColor = \"Black\";\n"
             "</script>\n";
   status += tail;

   Response response(HTTP11, Response::OK, status.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(status)) {
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

template <typename Element>
String Selection(const std::list<Element> &list,
                 const String &label,
                 const String &current,
                 bool hasNone) {
   String selection;
   selection.reserve(256);
   selection += "<label for=" + label + ">" + label + ": </label>\n";
   selection += "<select name=" + label + " id=" + label + ">\n";
   if (hasNone) {
      selection += "  <option value=\"None\"";
      if (current.empty()) {
         selection += " selected";
      } // if
      selection += ">None</option>\n";
   } // if
   for (const auto &i : list) {
      selection += "  <option value=\"";
      selection += i;
      selection += '"';
      if (current==(String)i) {
         selection += " selected";
      } // if
      selection += '>';
      selection += i;
      selection += "</option>\n";
   } // for
   selection += "</select>\n";
   return selection;
} // Selection(list, label, current, hasNone)

// If current is empty, then show "Select..."
static String Ports(unsigned selection,const String &current=String()) {
   String select;
   select.reserve(1024);

   select += "<select name=P";
   select += ToString(selection);
   select += " onchange=\"this.form.submit()\">\n";

   select += "<option value=\"\"";
   if (current.empty()) {
      select += " selected>Select&hellip;"; // ellipsis
   } // if
   else {
      select += ">REMOVE";
   } // else
   select += "</option>\n";

   for (const auto &p : BSD::Service::Ports()) {
      String port = ToString(p.first);
      select += " <option value=";
      select += port;
      if (current==port) {
         select += " selected";
      } // if
      select += '>';
      select += port;
      for (const auto &s : p.second) {
         if (!s.second.Alias()) {
            select += " - ";
            select += s.first;
            break;
         } // if
      } // for
      select += "</option>\n";
   } // for
   for (const auto &s : BSD::Service::Services()) {
      select += " <option value=\"";
      select += s.first;
      select += "\"";
      if (current==s.first) {
         select += " selected";
      } // if
      select += '>';
      select += s.first;
      select += " - ";
      select += ToString(s.second.Port());
      select += "</option>\n";
   } // for

   select += "</select>\n";
   return select;
} // Ports(current)

bool Worker::SendConfigPage(bool head) {
   static const String header =
      html +
      heading +
      "<script>\n"
      " document.getElementById(\"heading\").innerHTML = \"Configuration\";"
      "</script>\n";

   BSD::Interfaces up     = BSD::Interface::List(BSD::IPv4, BSD::Up);
   BSD::Interfaces upDown = BSD::Interface::List(BSD::NoProtocol, BSD::Down);

   std::list<String> protocols = { "Ethernet", "PPPoE", "PPPoA" };
   String body;
   body.reserve(2048);
   body += header;
   body += "<form method=POST>\n" // action="/config" is assumed
           "<fieldset>\n"
           "<legend>Control</legend>\n";
   body += Selection(up, "Server", config.server, true);
   body +=" Port: <input style=\"width:5em\" type=number min=1 max=65535 name=Port value=";
   body += ToString(config.port);
   body += " />\n"
           "</fieldset><p />\n"
           "<fieldset>\n"
           "<legend>Monitor</legend>\n";
   body += Selection(upDown, "Left ", config.left, true);
   body += Selection(upDown, "Right", config.right, true);
   body += Selection(protocols, "Protocol", config.protocol, false);
   body += " ICMP: <input type=checkbox name=ICMP value=Y"; // If checked - not present if not
   if (config.icmp) {
      body += " checked";
   } // if
   body += " /> (ping etc.)<p />\n"
           "<fieldset>\n"
           "<legend>Ports</legend>\n";
   unsigned selection = 1;
   for (const auto &p : config.ports) {
      body += Ports(selection, p);
      body += ' ';
      ++selection;
   } // for
   body += Ports(selection);
   body += "</fieldset>\n"
           "</fieldset>\n"
           "<p />\n"
           "<input type=submit value=Reset formaction=\"/config/reset\" />\n" // Not a Reset button!
           "<input type=submit />\n"
           "</form>\n";
   body += tail;

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
   if (request.Path()=="/config/reset") {
      config = Config::master;
      return Refresh();
   } // if
   if (request.Path()=="/quit") {
      return Quit();
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Worker::POST()

bool Worker::Config() {
   if (!POSTConfig()) {
      return Refresh();
   } // if
   Config::master.Set(config);
   Response response(HTTP11, Response::NoContent);
   return Write(response);
} // Worker::Config()

bool Worker::POSTConfig() {
   config.server   = request.Get("Server=", "None");
   config.port     = BSD::Service::Find(request.Get("Port="));
   config.left     = request.Get("Left=",   "None");
   config.right    = request.Get("Right=",  "None");
   config.protocol = request.Get("Protocol=");
   config.icmp     = request.Get("ICMP=")=="Y";

   Config::Ports ports;
   for (unsigned p=1;;++p) {
      String port = "P" + ToString(p) + '=';
      if (line.find(port)==String::npos) {
         break;
      } // if
      port = request.Get(port);
      if (!port.empty()) {
         ports.push_back(port);
      } // if
   } // for

   bool same = config.ports==ports;
   config.ports = ports;
   return same;
} // Worker::POSTConfig()

bool Worker::Refresh() {
   Response response(HTTP11, Response::SeeOther);
   response.Add(HTTP::Location, "/config");
   return Write(response);
} // Worker::Refresh()

bool Worker::Quit() {
   static const String body =
      html +
      heading +
      "<script>\n"
      " document.getElementById(\"heading\").innerHTML = \"Finished\";"
      "</script>\n"
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
      contentLength = ToNumber(*set.begin());
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
   std::cout << "Starting " << fd << std::endl;
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
   std::cout << "Stopping " << fd << std::endl;
   Close();
   delete this;
   return nullptr;
} // Worker::Run()
