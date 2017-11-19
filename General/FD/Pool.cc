//
// Pool.cc
//

#include "Pool.hh"

Pool::Pool() :
      List<FD>(&FD::node) {
} // Pool::Pool()

bool Pool::Poll() {
   return false;
} // Pool::Poll()
