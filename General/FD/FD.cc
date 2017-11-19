//
// FD.cc
//

#include <fcntl.h>
#include <unistd.h>

#include "Pool.hh"

FD::FD(Type fd) :
    fd(fd),
    node(*this) {
} // FD::FD(fd)

FD::FD(FD &fd) :
    fd(fd.fd),
    node(*this) {
   fd.fd = Invalid;
} // FD::FD(FD)

void FD::Add(Pool &pool) {
   if (!Valid()) {
      return;
   } // if
   ::fcntl(fd, F_SETFL, O_NONBLOCK);
   node.Add(pool);
} // FD::Add(Pool)

// If this ends up being called, something's wrong!
void FD::Readable() {
   Close();
} // FD::Readable()

// If this ends up being called, something's wrong!
void FD::Writable() {
   Close();
} // FD::Writable()

void FD::Close() {
   if (!Valid()) {
      return;
   } // if
   ::close(fd);
} // FD::Close()

FD::~FD() {
   Close();
} // FD::~FD()
