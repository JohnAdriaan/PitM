//
// General.hh
//

#ifndef General_hh
#define General_hh

#include <strings.h>

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

typedef size_t Pos;
typedef size_t Size;

typedef std::string String;

inline unsigned ToNumber(const String &string) {
   return ::atoi(string.c_str());
} // ToNumber(String)

inline String ToString(int number) {
   return std::to_string(number);
} // ToString(number)

inline String ToString(unsigned number) {
   return std::to_string(number);
} // ToString(number)

inline String &operator +=(String &lhs, int number) {
   return lhs += ToString(number);
} // operator +=(String, number)

inline String &operator +=(String &lhs, unsigned number) {
   return lhs += ToString(number);
} // operator +=(String, number)

// For case-insensitive comparisons in the STL
struct NoCase {

   inline bool operator()(const String &lhs, const String &rhs) const noexcept;

}; // NoCase

inline bool NoCase::operator()(const String &lhs, const String &rhs) const noexcept {
   return ::strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
} // operator()(lhs, rhs)

class _General_ {
}; // _General_

class _Template_ : private _General_ {
}; // _Template_

#endif // General_hh
