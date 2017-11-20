//
// Worker.cc
//

#include "Worker.hh"

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
   delete this;
   return nullptr;
} // Worker::Run()
