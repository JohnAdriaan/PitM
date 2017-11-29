//
// FD.cc
//

#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>

#include "FD.hh"

FD::FD(Type fd) :
    fd(fd) {
} // FD::FD(fd)

FD::FD(FD &fd) :
    fd(fd.fd) {
   fd.fd = Invalid;
} // FD::FD(FD)

bool FD::Read(void *buffer, unsigned size, unsigned &read) {
   if (!Valid()) {
      return false;
   } // if
   read = ::read(fd, buffer, size);
   return read!=(unsigned)-1;
} // FD::Read(buffer, size, read)

bool FD::Write(const String &string) {
   return Write(string.c_str(), string.length());
} // FD::Write(string)

bool FD::Write(const void *buffer, unsigned size) {
   if (!Valid()) {
      return false;
   } // if
   unsigned wrote;
   while (size>0) {
      if (!Write(buffer, size, wrote)) {
         return false;
      } // if
      (const byte *&)buffer += wrote; // Avoid arithmetic warning
      size -= wrote;
   } // while
   return true;
} // FD::Write(buffer, size)

bool FD::Write(const void *buffer, unsigned size, unsigned &wrote) {
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
