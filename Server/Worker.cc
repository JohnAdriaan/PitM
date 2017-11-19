//
// Worker.cc
//

#include "Worker.hh"

Worker::Worker(Pool &pool, BSD::TCP &client, const BSD::Address &address) :
        TCP(client) {
   if (!Valid()) {
      delete this;
      return;
   } // if
   pool.Add(*this);
} // Worker::Worker(pool, client, address)

void Worker::Readable() {
} // Worker::Readable()

void Worker::Writable() {
} // Worker::Writable()

Worker::~Worker() {
} // Worker::~Worker()
