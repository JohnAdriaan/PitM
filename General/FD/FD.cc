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

bool FD::Read(void *buffer, unsigned size, unsigned &read) {
   if (!Valid()) {
      return false;
   } // if
   read = ::read(fd, buffer, size);
   return read!=(unsigned)-1;
} // FD::Read(buffer, size, read)

// Doesn't return until all written - or error
bool FD::Write(void *buffer, unsigned size) {
   if (!Valid()) {
      return false;
   } // if
   unsigned wrote;
   while (size>0) {
      if (!Write(buffer, size, wrote)) {
         return false;
      } // if
      (byte *&)buffer += wrote; // Avoid arithmetic warning
      size -= wrote;
   } // while
   return true;
} // FD::Write(buffer, size)

bool FD::Write(void *buffer, unsigned size, unsigned &wrote) {
   if (!Valid()) {
      return false;
   } // if
   wrote = ::write(fd, buffer, size);
   return wrote!=(unsigned)-1;
} // FD::Write(buffer, size, wrote)

bool FD::SendFile(FD &source, unsigned size) {
   if (!Valid()) {
      return false;
   } // if
   if (!source.Valid()) {
      return false;
   } // if
   for (;;) {
      ssize_t wrote = ::sendfile(fd, source.fd, nullptr, size); // From current
      if (wrote<0) { // Error?
         return false;
      } // if
      if (wrote==0) {  // End of file?
         break;
      } // if
      size -= wrote;
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
   Type old = fd;
   if (!Valid()) {
      return;
   } // if
   fd = Invalid;
   ::close(old);
} // FD::Close()

FD::~FD() {
   Close();
} // FD::~FD()
