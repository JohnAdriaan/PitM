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
   if (count==0) {
      return false;
   } // if

   count += 4; // Allow for a few extra arrivals
   pollfd *fds = new pollfd[count];
   if (fds==nullptr) {
      return false;
   } // if

   unsigned valid = 0;
   for (FD *fd = Head();
        fd!=nullptr && valid<count;
        fd = Next(fd)) {
      if (fd->fd==FD::Invalid) {
         continue;
      } // if
      fds[valid].fd = fd->fd;
      fds[valid].events = POLLIN|POLLOUT;
      ++valid;
   } // for

   bool polled = false;
   if (valid!=0) {
      if (::poll(fds, valid, -1)!=-1) { // Infinite timeout
         polled = true;
         // Call all Readables before any Writables
         Call(fds, valid, POLLIN,  &FD::Readable);
         Call(fds, valid, POLLOUT, &FD::Writable);
      } // if
   } // if

   delete [] fds;
   return polled;
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
            // Triggered!
            (fd->*fn)();
         } // if
         break;
      } // for
   } // for
} // Pool::Call(fds, count, flag, fn)
