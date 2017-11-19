//
// TCP.cc
//

#include "TCP.hh"

using namespace BSD;

TCP::TCP(TCP &tcp) :
     Socket(tcp) {
} // TCP::TCP(TCP)
