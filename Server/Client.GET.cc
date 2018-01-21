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
   Write(HTTP::Response(HTTP10, Response::NotFound));
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
   status += ToCommas(Monitor::NumTotal());
   status += " Logged: ";
   status += ToCommas(Monitor::NumLogged());

   String page;
   page += html;
   page += Heading(status, true);
   page += body;
   page += tail;

   HTTP::Response response(HTTP11, Response::OK, page.length());
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
      ".banner .left  { display: inline-block; position: relative; width: 96px }\n"
      ".banner .right { display: inline-block; position: relative }\n"
      "h1       { margin-bottom: 0px }\n"
      "h1 a     { color: black }\n"
      "h2       { font-size:16pt; padding:4px; border-style:solid; border-width:1px }\n"
      "fieldset { display: inline-block }\n"
      "legend   { color: #404040 }\n"
      "label    { color: #404040 }\n";
   HTTP::Response response(HTTP11, Response::OK, body.length());
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
   for (const auto &p : Query::Service::Ports()) {
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
   for (const auto &s : Query::Service::Services()) {
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

   using namespace Query;
   Interfaces interfaces = Interface::List(Interface::NoProtocol,
                                           Interface::Up);

   std::list<String> protocols = { "Ethernet", "PPPoE", "PPPoA" };
   String body;
   body.reserve(2048);
   body += header;
   body += Fill();
   body += "<form method=POST>\n"; // action="/config" is assumed

   body += "<fieldset>\n"
           "<legend>Server</legend>\n";
   body += "<label for=Server>Port:</label>\n";
   body += " <input type=number min=1 max=";
   body += ToString(Config::MaxPort);
   body += " id=Server name=Server style='width:5em' value=";
   body += ToString(config.server);
   body += " />\n"
           "</fieldset>\n"
           "<p></p>\n";

   body += "<fieldset>\n"
           "<legend>Monitor</legend>\n";
   body += Selection(interfaces, "Left", config.left, true);
   body += Selection(interfaces, "Right", config.right, true);
   body += Selection(protocols, "Protocol", config.protocol, false);
   body += "<label for=ICMP>ICMP:</label>\n";
   body += " <input type=checkbox id=ICMP name=ICMP value=Y"; // If checked - not present if not
   if (config.icmp) {
      body += " checked";
   } // if
   body += " /> <label for=ICMP>(ping etc.)</label>\n"
           "<p></p>\n";
   body += "<fieldset>\n"
           "<legend>Ports</legend>\n";

   // First, show existing ports as drop-downs
   unsigned selection = 1;
   for (const auto &p : config.ports) {
      body += ::Ports(selection++, p);
      body += ' ';
   } // for
   // Then show a new one, with simply "Select..."
   body += ::Ports(selection);
   body += "</fieldset>\n"
           "</fieldset>\n";

   body += "<p></p>\n"
           "<input type=submit value=Reset formaction='/config/reset' />\n" // Not a Reset button!
           "<input type=submit />\n"
           "</form>\n";
   body += tail;

   HTTP::Response response(HTTP11, Response::OK, body.length());
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

   HTTP::Response response(HTTP11, Response::OK, length);

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

   HTTP::Response response(HTTP11, Response::OK, length);
   response.Add(CacheControl, MaxAge, 60*60); // An hour ought to do!
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !FD::Write(obj,length)) {
      return false;
   } // if
   return true;
} // Client::SendObj(head,obj,size)
