//
// Worker.cc
//

#include <iostream> // TODO

#include "../General/File/File.hh"

#include "../General/WWW/HTTP/Response.hh"

#include "Worker.hh"

#include "../PitM.hh"

extern const char favicon[];

extern char faviconSize; // Get the ADDRESS of this!!!

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
   using namespace WWW::HTTP;
   std::cout << "GET " << request->path << std::endl;
   if (request->path=="/") {
      return SendHomePage(head);
   } // if
   if (request->path=="/favicon.ico") {
      return SendObj(head,favicon, &faviconSize);
   } // if
   Write(Response(HTTP10, Response::NotFound));
   return false;
} // Worker::GET(head)

bool Worker::SendHomePage(bool head) {
   String body = "<html><body><h1>PitM</h1></body></html>";

   using namespace WWW::HTTP;
   Response response(HTTP11, Response::OK, body.length());
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !Write(body)) {
      return false;
   } // if
   return true;
} // Worker::SendHomePage(head)

bool Worker::SendFile(bool head,const char *path) {
   File file(path);
   if (!file.Valid()) {
      return false;
   } // if
   Size length = file.Size();

   using namespace WWW::HTTP;
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

   using namespace WWW::HTTP;
   Response response(HTTP11, Response::OK, length);
   if (!Write(response)) {
      return false;
   } // if
   if (!head && !FD::Write(obj,length)) {
      return false;
   } // if
   return true;
} // Worker::SendObj(head,obj,size)

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
   using namespace WWW::HTTP;
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
