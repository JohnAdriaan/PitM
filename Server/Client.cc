//
// Client.cc
//

#include <WWW/HTTP/Response.hh>

#include "Client.hh"

using namespace WWW;

using namespace WWW::HTTP;

using namespace PitM;

const String Server::Client::html =
      "<!DOCTYPE html>\n"
      "<html lang=en-AU>\n"
      "<head>\n"
      "<meta charset=UTF-8 />\n"
      "<meta name=viewport content=\"width=device-width, initial-scale=1.0\" />\n"
      "<link rel='shortcut icon' href='/favicon.ico' />\n"
      "<link rel=stylesheet type='text/css' href='/style.css' />\n"
      "<title>PitM: Pi in the Middle</title>\n"
      "</head>\n";

const String Server::Client::tail =
      "</body>\n"
      "</html>";

String Server::Client::Heading(String header, bool border) {
   static const String preface =
         "<body>\n"
         "<h1 class=left>\n"
         "<a href='/'>\n"
         "<img src='/icon.ico' alt='John Burger' height=64 width=64 />\n"
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
} // Client::Heading(header, border)

Server::Client::Client(BSD::TCP &client, const BSD::Address &/*address*/) :
                Thread(),
                TCP(client),
                config(Config::master), // Copy existing Config
                request(),
                state(RequestLine),
                line(),
                buffer(),
                read(),
                pos(),
                start(),
                contentLength() {
   if (!TCP::Valid()) {
      delete this;
      return;
   } // if
   line.reserve(2048);
   if (!Thread::Start()) {
      delete this;
      return;
   } // if
} // Client::Client(client, address)

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
      request = HTTP::Request::Parse(line);
      state = RequestHeader;
      break;
   case RequestHeader : {
      if (!line.empty()) {
         request.Append(line);
         break;
      } // if
      state = RequestDone;

      const WWW::MapSet &headers = request.Headers();
      auto h = headers.find(HTTP::ContentLength);
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
      Write(HTTP::Response(HTTP10, Response::BadRequest));
      return false;
   case Request::GET :
      return GET(/*head=*/false);
   case Request::HEAD    :
      return GET(/*head=*/true);
   case Request::POST    :
      return POST();
   case Request::PUT     :
   case Request::DELETE  :
   case Request::TRACE   :
   case Request::OPTIONS :
   case Request::CONNECT :
   case Request::PATCH   :
      Write(HTTP::Response(HTTP10, Response::NotImplemented));
      return false;
   default :
      break;
   } // switch
   WWW::Set set = { "GET", "HEAD", "POST" };
   HTTP::Response response(HTTP10, Response::MethodNotAllowed);
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
