//
// Worker.hh
//

#ifndef Worker_hh
#define Worker_hh

#include <Socket/TCP.hh>

#include <Thread/Thread.hh>

#include <WWW/HTTP/Request.hh>

#include "Server.hh"

class PitM::Server::Worker : public BSD::TCP,
                             private MT::Thread {

friend Server;

private: // Methods

   Worker(BSD::TCP &client, const BSD::Address &address);

   virtual ~Worker() = default;

private: // GET Responses

   // Reply to GET or HEAD Request
   bool GET(bool head);

   // Create and send Home Page
   bool SendHomePage(bool head);

   // Create and send Style Sheet
   bool SendStyleSheet(bool head);

   // Create and send Config Page
   bool SendConfigPage(bool head);

   // Send file using Linux's ::sendfile()
   bool SendFile(bool head,const char *path);

   // Send a linked-in binary 'file'
   bool SendObj(bool head,const void *obj, const void *size);

private: // POST Responses

   // Accept submitted data
   bool POST();

   // Configure
   bool Config();

      // Copy config from POST.
      // Return false if the Ports have changed (thus no Config::Set())
      bool POSTConfig();

      // Re-display current config
      bool Refresh();

   // Close the application
   bool Quit();

private: // Run() methods

   // Parse the next line
   bool Parse();

   // Process the next line
   bool Process();

   // Reply to Process()ed Request
   bool Reply();

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

   PitM::Config config;

   WWW::HTTP::Request request;

   States state;

   String line;

   char buffer[2048];

   unsigned read;

   unsigned pos;

   unsigned start;

   unsigned contentLength;

}; // Worker

#endif // Worker_hh
