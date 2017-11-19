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

   inline void Add(FD &fd);

private: // Methods

   typedef void Fn();

   void Call(struct pollfd *fds, unsigned count, unsigned flag, Fn FD::*fn);

}; // Pool

inline void Pool::Add(FD &fd) {
   fd.Add(*this);
} // Pool:::Add(FD)

#endif // Pool_hh
