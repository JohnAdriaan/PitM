//
// General.hh
//

#ifndef General_hh
#define General_hh

#include <string>

// The opposite of explicit
#define implicit

// This is used to mark virtual base functions as pure.
// This is easier to isolate in a global search than what C++ specifies.
#define pure = 0

#define hook      virtual
#define overrides virtual

typedef unsigned char  byte;
typedef unsigned short word;

typedef std::string    String;

class _General_ {
}; // _General_

class _Template_ : private _General_ {
}; // _Template_

#endif // General_hh
