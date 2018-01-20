//
// Client.cc
//

#include <Socket/Service.hh>
#include <WWW/HTTP/Response.hh>

#include "Client.hh"

using namespace WWW;

using namespace WWW::HTTP;

using namespace PitM;

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
   config.server   = Query::Service::Find(request.Get("Server="));
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
