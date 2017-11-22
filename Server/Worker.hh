//
// Worker.hh
//

#ifndef Worker_hh
#define Worker_hh

#include "../General/Thread/Thread.hh"

#include "../General/Socket/TCP.hh"

#include "../General/WWW/HTTP/Request.hh"

class Server;

class Worker : public BSD::TCP,
               private MT::Thread {

friend Server;

private: // Methods

   Worker(BSD::TCP &client, const BSD::Address &address);

   // Parse the next line
   bool Parse();

   // Process the next line
   bool Process();

   // Reply to Request
   bool Reply();

   virtual ~Worker();

private: // Thread overrides

   overrides void *Run();

private: // Enums

   enum States {
      RequestLine,
      RequestHeader,
      RequestBody,
      RequestDone
   }; // States

private: // Variables

   WWW::HTTP::Request *request;

   States state;

   String line;

   char buffer[2048];

   unsigned read;

   unsigned pos;

   unsigned start;

   unsigned contentLength;

}; // TCP

#endif // Worker_hh
