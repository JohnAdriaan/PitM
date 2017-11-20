//
// Worker.cc
//

#include <iostream>

#include "Worker.hh"

#include "../PitM.hh"

Worker::Worker(BSD::TCP &client, const BSD::Address &address) :
        TCP(client),
        Thread() {
   if (!TCP::Valid()) {
      delete this;
      return;
   } // if
   if (!Start()) {
      delete this;
      return;
   } // if
} // Worker::Worker(client, address)

void *Worker::Run() {
   unsigned read;
   char buffer[1024+1];
   while (Read(buffer, sizeof buffer-1, read)) {
      buffer[read] = '\0';
      std::cout << buffer;
      if (buffer[0]=='q' && buffer[1]=='u' && buffer[2]=='i' && buffer[3]=='t') {
         quit.Post();
      } // if
      if (read==0) {
         Close();
      } // if
   } // while
   delete this;
   return nullptr;
} // Worker::Run()
