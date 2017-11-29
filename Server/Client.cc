//
// Client.cc
//

#include <File/File.hh>
#include <Socket/Service.hh>
#include <Socket/Interface.hh>
#include <WWW/HTTP/Response.hh>

#include "../Monitor/Monitor.hh"

#include "Client.hh"

extern const byte icon[];

extern char iconSize; // Get the ADDRESS of this!!!

using namespace WWW;

using namespace WWW::HTTP;

using namespace PitM;

static const String html =
      "<!DOCTYPE html>\n"
      "<html lang=en-AU>\n"
      "<head>\n"
      "<meta charset=UTF-8 />\n"
      "<meta name=viewport content=\"width=device-width, initial-scale=1.0\" />\n"
      "<link rel='shortcut icon' href=/favicon.ico />\n"
      "<link rel=stylesheet type=text/css href=style.css />\n"
      "<title>PitM: Pi in the Middle</title>\n"
      "</head>\n";

static const String tail =
      "</body>\n"
      "</html>";

static String Heading(String header, bool border=false) {
   static const String preface =
         "<body>\n"
         "<h1 class=left>\n"
         "<a href='/'>\n"
         "<img src=/icon.ico alt='John Burger' height=64 width=64 />\n"
         "PitM</a></h1>\n"
         "<h1 class=right>Pi in the Middle\n"
         "<div style='font-size:10pt'>"
         "&nbsp;Ver: " + PitM::Version() + ", by John Burger<br /><br /></div>\n"
         "<div style='font-size:16pt;padding:4px;border-style:solid;border-width:1px";
   static const String postface =
         "</div>\n"
         "</h1>\n";

   String heading = preface;
   if (!border) {
      heading += ";border-color:transparent";
   } // if
   heading += "'>\n";
   heading += header;
   heading += postface;
   return heading;
} // Heading(header, border)

Server::Client::Client(BSD::TCP &client, const BSD::Address &/*address*/) :
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
} // Client::Client(client, address)

bool Server::Client::GET(bool head) {
   if (request.Path()=="/") {
      return SendHomePage(head);
   } // if
   if (request.Path()=="/icon.ico") {
      return SendObj(head, icon, &iconSize);
   } // if
   if (request.Path()=="/favicon.ico") {
      return SendObj(head, icon, &iconSize);
   } // if
   if (request.Path()=="/style.css") {
      return SendStyleSheet(head);
   } // if
   if (request.Path()=="/config") {
      return SendConfigPage(head);
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Client::GET(head)

bool Server::Client::SendHomePage(bool head) {
   static const String body =
      "<h3><a href='/config'>Configuration</a></h3>\n"
      "<h3><a href='/stats'>Statistics</a></h3>\n"
      "<h3><a href='/logs'>Logs</a></h3>\n"
      "<form action='/quit' method=POST"
      " onsubmit='return confirm(\"Are you sure you want to quit?\")'>\n"
      "<input type=submit value='Quit' style='color:red' />\n"
      "</form>\n";

   String status;
   status += "Packets: ";
   status += ToCommas(Monitor::Total());
   status += " Logged: ";
   status += ToCommas(Monitor::Logged());

   String page;
   page += html;
   page += Heading(status, true);
   page += body;
   page += tail;

   Response response(HTTP11, Response::OK, page.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(page)) {
      return false;
   } // if
   return true;
} // Client::SendHomePage(head)

bool Server::Client::SendStyleSheet(bool head) {
   static const String body =
      "*        { font-family: sans-serif }\n"
      "noscript { color: red }\n"
      "h1 a     { color: black }\n"
      "h1.left  { display: inline-block; position: relative; width: 96px }\n"
      "h1.right { display: inline-block; position: relative }\n"
      "fieldset { display: inline-block }\n"
      "legend   { color: #404040 }\n"
      "label    { color: #404040 }\n";
   Response response(HTTP11, Response::OK, body.length());
   response.Add(CacheControl, MaxAge, 60*60); // An hour ought to do!
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Client::SendStyleSheet(head)

template <typename Element>
String Selection(const std::list<Element> &list,
                 const String &label,
                 const String &current,
                 bool hasNone) {
   bool selected = false;

   String selection;
   selection.reserve(256);
   selection += "<label for=" + label + ">" + label + ":</label>\n";
   selection += " <select id=" + label + " name=" + label + ">\n";
   if (hasNone) {
      selection += "  <option value=''";
      if (current.empty()) {
         selection += " selected";
         selected = true;
      } // if
      selection += ">None</option>\n";
   } // if
   for (const auto &i : list) {
      selection += "  <option value='";
      selection += i;
      selection += '\'';
      if (current==(String)i) {
         selection += " selected";
         selected = true;
      } // if
      selection += '>';
      selection += i;
      selection += "</option>\n";
   } // for
   if (!selected && !current.empty()) {
      selection += "  <option value='";
      selection += current;
      selection += "' selected disabled>";
      selection += current;
      selection += "</option>\n";
   } // if
   selection += " </select>\n";
   return selection;
} // Selection(list, label, current, hasNone)

static String Fill() {
   static const String divider = " - ";
   String prefix;
   String fill;
   fill.reserve(2048);
   fill += "<script>\n";
   fill += "var ports = [";
   prefix = "'";
   for (const auto &p : BSD::Service::Ports()) {
      unsigned port = p.first;
      if (port>Config::MaxPort) {
         continue;
      } // if
      fill += prefix;
      fill += port;
      String sep = divider;
      for (const auto &s : p.second) {
         fill += sep;
         fill += s.first;
         sep = ",";
      } // for
      prefix = "','";
   } // for
   fill += "'];\n";
   fill += "var names = [";
   prefix = "'";
   for (const auto &s : BSD::Service::Services()) {
      unsigned port = s.second.Port();
      if (port>Config::MaxPort) {
         continue;
      } // if
      fill += prefix;
      fill += s.first;
      fill += divider;
      fill += port;
      prefix = "','";
   } // for
   fill += "'];\n";
   fill += "function Fill(id,c) {\n"  // id,current
           " var s = document.getElementById(id);\n"
           " var n = c=='' ? 'Select\u2026' : 'REMOVE';\n" // HEllip
           " var o = new Option(n,'',true,c=='');\n"
           " s.appendChild(o);\n"
           " var g = document.createElement('OPTGROUP');\n"
           " g.label = 'Numeric';\n"
           " for (var p=0;p<ports.length;++p) {\n"
           "  var t = ports[p];\n"
           "  var v = t.substring(0,t.indexOf('"+divider+"'));\n"
           "  var o = new Option(t,v,false,v==c);\n"
           "  g.appendChild(o);\n"
           " } // for\n"
           " s.appendChild(g);\n"
           " g = document.createElement('OPTGROUP');\n"
           " g.label = 'Name';\n"
           " for (var n=0;n<names.length;++n) {\n"
           "  var t = names[n];\n"
           "  var v = t.substring(0,t.indexOf('"+divider+"'));\n"
           "  var o = new Option(t,v,false,v==c);\n"
           "  g.appendChild(o);\n"
           " } // for\n"
           " s.appendChild(g);\n"
           "} // Fill(id,c)\n";
   fill += "</script>\n";
   return fill;
} // Fill()

// If current is empty, then show "Select..."
static String Ports(unsigned selection,const String &current=String()) {
   String id = 'P' + ToString(selection);
   String select;
   select.reserve(1024);

   select += "<select id=";
   select += id;
   select += " name=";
   select += id;
   select += " onchange='this.form.submit();'>\n";
   select += " <script>Fill('"+id+"','"+current+"');</script>\n";
   select += "</select>\n";
   return select;
} // Ports(current)

bool Server::Client::SendConfigPage(bool head) {
   static const String header =
      html +
      Heading("<noscript>This page requires JavaScript.</noscript>\n"
              "<script>document.write('Configuration');</script>");

   BSD::Interfaces up     = BSD::Interface::List(BSD::IPv4, BSD::Up);
   BSD::Interfaces upDown = BSD::Interface::List(BSD::NoProtocol, BSD::Down);

   std::list<String> protocols = { "Ethernet", "PPPoE", "PPPoA" };
   String body;
   body.reserve(2048);
   body += header;
   body += Fill();
   body += "<form method=POST>\n" // action="/config" is assumed
           "<fieldset>\n"
           "<legend>Control</legend>\n";
   body += Selection(up, "Server", config.server, true);
   body += "<label for=Port>Port:</label>\n";
   body += " <input type=number min=1 max=65535 id=Port name=Port style='width:5em' value=";
   body += ToString(config.port);
   body += " />\n"
           "</fieldset><p />\n"
           "<fieldset>\n"
           "<legend>Monitor</legend>\n";
   body += Selection(upDown, "Left", config.left, true);
   body += Selection(upDown, "Right", config.right, true);
   body += Selection(protocols, "Protocol", config.protocol, false);
   body += "<label for=ICMP>ICMP:</label>\n";
   body += " <input type=checkbox id=ICMP name=ICMP value=Y"; // If checked - not present if not
   if (config.icmp) {
      body += " checked";
   } // if
   body += " /> <label for=ICMP>(ping etc.)</label><p />\n";
   body += "<fieldset>\n"
           "<legend>Ports</legend>\n";
   unsigned selection = 1;
   for (const auto &p : config.ports) {
      body += Ports(selection++, p);
      body += ' ';
   } // for
   body += Ports(selection);
   body += "</fieldset>\n"
           "</fieldset>\n"
           "<p />\n"
           "<input type=submit value=Reset formaction='/config/reset' />\n" // Not a Reset button!
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
} // Client::SendConfigPage(head)

bool Server::Client::SendFile(bool head,const char *path) {
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
} // Client::SendFile(head,path)

bool Server::Client::SendObj(bool head, const void *obj, const void *size) {
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
} // Client::SendObj(head,obj,size)

bool Server::Client::POST() {
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
} // Client::POST()

bool Server::Client::Config() {
   if (!POSTConfig()) {
      return Refresh();
   } // if
   Config::master.Set(config);
   Response response(HTTP11, Response::NoContent);
   return Write(response);
} // Client::Config()

bool Server::Client::POSTConfig() {
   config.server   = request.Get("Server=");
   config.port     = BSD::Service::Find(request.Get("Port="));
   config.left     = request.Get("Left=");
   config.right    = request.Get("Right=");
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

   if (config.ports==ports) {
      return true;
   } // if
   config.ports = ports;
   return false;
} // Client::POSTConfig()

bool Server::Client::Refresh() {
   Response response(HTTP11, Response::SeeOther);
   response.Add(HTTP::Location, "/config");
   return Write(response);
} // Client::Refresh()

bool Server::Client::Quit() {
   static const String body =
      html +
      Heading("Finished") +
      "<p>Thank you for using PitM!</p>\n" +
      tail;

   Response response(HTTP11, Response::OK, body);
   response.Add(HTTP::Connection, HTTP::Close);
   Write(response);
   PitM::Quit();
   return false; // Might as well close; shutting down anyway!
} // Client::Quit()

bool Server::Client::Parse() {
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
} // Client::Parse()

bool Server::Client::Process() {
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
} // Client::Process()

bool Server::Client::Reply() {
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
} // Client::Reply()

void *Server::Client::Run() {
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
   Close();
   delete this;
   return nullptr;
} // Client::Run()
