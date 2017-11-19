//
// Pool.cc
//

#include <poll.h>

#include "Pool.hh"

Pool::Pool() :
      List<FD>(&FD::node) {
} // Pool::Pool()

bool Pool::Poll() {
   unsigned count = Count(); // Get an instantaneous count. Note may change!

   pollfd *fds = new pollfd[count];
   if (fds==nullptr) {
      return false;
   } // if

   FD *fd = Head();
   for (unsigned num=0;
        num<count;
        ++num) {
      fds[num].fd = fd!=nullptr ? fd->fd : FD::Invalid;
      fds[num].events = POLLIN|POLLOUT;
      fd = Next(fd);
   } // for
   if (::poll(fds, count, -1)==-1) { // Infinite timeout
      delete [] fds;
      return false;
   } // if

   // Call all Readables before any Writables
   Call(fds, count, POLLIN,  &FD::Readable);
   Call(fds, count, POLLOUT, &FD::Writable);

   delete [] fds;
   return true;
} // Pool::Poll()

void Pool::Call(pollfd *fds, unsigned count, unsigned flag, Fn FD::*fn) {
   for (FD *fd = Head();
        fd!=nullptr;
        fd = Next(fd)) {
      if (fd->fd==FD::Invalid) {
         continue;
      } // if
      for (unsigned num=0;
           num<count;
           ++num) {
         if (fds[num].fd==FD::Invalid) {
            continue;
         } // if
         if (fds[num].fd!=fd->fd) {
            continue;
         } // if
         // Found!
         if ((fds[num].revents & flag)==flag) {
            (fd->*fn)();
         } // if
         break;
      } // for
   } // for
} // Pool::Call(fds, count, flag, fn)
