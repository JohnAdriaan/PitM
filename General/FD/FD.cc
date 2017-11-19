//
// FD.cc
//

#include <unistd.h>

#include "Pool.hh"

FD::FD(int fd) :
    fd(fd),
    node(*this) {
} // FD::FD(fd)

FD::FD(FD &fd) :
    fd(fd.fd),
    node(*this) {
   fd.fd = Invalid;
} // FD::FD(FD)

void FD::Add(Pool &pool) {
   node.Add(pool);
} // FD::Add(Pool)

void FD::Readable() {
   Close();
} // FD::Readable()

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
