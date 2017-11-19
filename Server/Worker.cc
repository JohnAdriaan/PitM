//
// Worker.cc
//

#include "Worker.hh"

Worker::Worker(BSD::TCP &client, const BSD::Address &address) :
        TCP(client) {
} // Worker::Worker(client, address)
