//
// FD.cc
//

#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>

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
   // There are times when despite a poll(), an operation will still block.
   // Mark the FD as non-blocking to guarantee it'll never block
   ::fcntl(fd, F_SETFL, O_NONBLOCK);
   node.Add(pool);
} // FD::Add(Pool)

bool FD::SendFile(FD &source, unsigned size) {
   if (!Valid()) {
      return false;
   } // if
   if (!source.Valid()) {
      return false;
   } // if
   for (;;) {
      ssize_t written = ::sendfile(fd, source.fd, nullptr, size); // From current
      if (written<0) { // Error?
         return false;
      } // if
      if (written==0) {  // End of file?
         break;
      } // if
      size -= written;
      if (size==0) {     // All she wrote?
         break;
      } // if
   } // for
   return true;
} // FD::SendFile(FD, size)

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
