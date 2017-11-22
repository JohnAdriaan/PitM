//
// Worker.cc
//

#include <iostream>

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
         char c = buffer[pos++];
         switch (c) {
         case '\n' :
            line.append(buffer+start, pos-start-1);
            start = pos;
            unsigned len = line.length();
            while (len-->0) {
               if (line[len]=='\r') {
                  line.pop_back();
               } // if
            } // while
            return true;
         } // switch
      } // while
      line.append(buffer+start, read-start);
      if (!Read(buffer, sizeof buffer, read)) {
        return false;
      } // if
      pos = 0;
      start = 0;
   } // for
} // Worker::Parse()

void *Worker::Run() {
   while (Parse()) {
      switch (state) {
      case RequestLine :
         delete request;
         request = WWW::HTTP::Request::Parse(line);
         if (request==nullptr) {
            Close();
            break;
         } // if
         break;
      case RequestHeader :
         break;
      case RequestBody :
         break;
      } // switch
      line.clear();
   } // while
   Close();
   delete this;
   return nullptr;
} // Worker::Run()

Worker::~Worker() {
   delete request;
} // Worker::~Worker()
