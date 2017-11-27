//
// File.cc
//

#include <fcntl.h>
#include <unistd.h>
#include <wordexp.h>

#include "File.hh"

String File::Expand(const String &name) {
   wordexp_t expand;
   ::wordexp(name.c_str(), &expand, 0);
   String result = expand.we_wordv[0];
   ::wordfree(&expand);
   return result;
} // File::Expand(name)

static mode_t Mode(File::Accesses access,
                   File::Creates create) {
   mode_t mode = 0;
   switch (access) {
   case File::ReadOnly :
      mode |= O_RDONLY;
      break;
   case File::WriteOnly :
      mode |= O_WRONLY;
      break;
   case File::ReadWrite :
      mode |= O_RDWR;
      break;
   } // switch
   switch (create) {
   case File::Exists :
//    mode |= 0;
      break;
   case File::Create :
      mode |= O_CREAT;
      break;
   case File::Exclusive :
      mode |= O_CREAT|O_EXCL;
      break;
   case File::Truncate :
      mode |= O_CREAT|O_TRUNC;
      break;
   } // switch
   return mode;
} // Mode(access, create)

File::File(const char *path, Accesses access, Creates create) :
      FD(::open(path,Mode(access,create))) {
} // File::File(path, access, create)

::Size File::Size() const {
   if (!Valid()) {
      return 0;
   } // if
   off_t old = ::lseek(fd, 0, SEEK_CUR);
   off_t pos = ::lseek(fd, 0, SEEK_END);
   ::lseek(fd, old, SEEK_SET);
   return pos;
} // File::Size()
