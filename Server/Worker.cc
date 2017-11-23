//
// Worker.cc
//

#include <iostream>

#include "../General/WWW/HTTP/Response.hh"

#include "Worker.hh"

#include "../PitM.hh"

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

bool Worker::Parse() {
   for (;;) {
      while (pos<read) {
         if (state==RequestBody) {
            ++pos;
            if (--contentLength>0) {
               continue;
            } // if
            line.append(buffer+start, pos-start);
         } // if
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
      pos = 0;
      start = 0;
   } // for
   return false; // Impossible!
} // Worker::Parse()

bool Worker::Process() {
   switch (state) {
   case RequestLine :
      using namespace WWW::HTTP;
      state = RequestDone; // Assume failure
      request = Request::Parse(line);
      if (request==nullptr) {
         return false;
      } // if
      switch (request->method) {
      case Request::Unknown :
      case Request::HEAD    :
      case Request::POST    :
      case Request::PUT     :
      case Request::DELETE  :
      case Request::TRACE   :
      case Request::OPTIONS :
      case Request::CONNECT :
      case Request::PATCH   :
      default :
         return false;
      case Request::GET :
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

      const auto &h = request->headers.find(ContentLength);
      if (h==request->headers.end()) {
         break;
      } // if

      const WWW::Set &set = h->second;
      const auto &i = set.begin();
      if (i==set.end()) {
         break;
      } // if

      contentLength = atoi(i->c_str());
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
   using namespace WWW::HTTP;
   if (request==nullptr) {
      std::cout << line << std::endl;
      Write(Response(HTTP10, Response::InsufficientStorage));
      return false;
   } // if
   switch (request->method) {
   case Request::Unknown :
      std::cout << line << std::endl;
      Write(Response(HTTP10, Response::BadRequest));
      return false;
   case Request::HEAD    :
   case Request::POST    :
   case Request::PUT     :
   case Request::DELETE  :
   case Request::TRACE   :
   case Request::OPTIONS :
   case Request::CONNECT :
   case Request::PATCH   :
   default :
      std::cout << line << std::endl;
      Write(Response(HTTP10, Response::MethodNotAllowed));
      return false;
   case Request::GET :
      break;
   } // switch
   std::cout << "GET " << request->path << std::endl;
   for (const auto &h : request->headers) {
      std::cout << h.first << ":";
      char sep = ' ';
      for (const auto &i : h.second) {
         std::cout << sep << i;
         sep = ',';
      } // for
      std::cout << std::endl;
   } // for
   std::cout << std::endl << line << std::endl;
   Write(Response(HTTP10, Response::NotFound));
   return true;
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
   Close();
   delete this;
   return nullptr;
} // Worker::Run()

Worker::~Worker() {
   delete request;
} // Worker::~Worker()
