//
// Pool.hh
//

#ifndef Pool_hh
#define Pool_hh

#include "FD.hh"

class Pool : private List<FD> {

friend FD;

public: // Methods

   Pool();

   bool Poll();

}; // Pool

#endif // Pool_hh
