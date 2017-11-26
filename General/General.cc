//
// General.cc
//

#include <locale>
#include <iomanip>
#include <strings.h>

#include "General.hh"

bool NoCase::operator()(const String &lhs, const String &rhs) const noexcept {
   return ::strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
} // operator()(lhs, rhs)

String ToCommas(int number) {
   static std::locale locale("");
   std::stringstream s;
   s.imbue(locale);
   s << std::fixed << number;
   return s.str();
} // ToCommas(number)

String ToCommas(unsigned number) {
   static std::locale locale("");
   std::stringstream s;
   s.imbue(locale);
   s << std::fixed << number;
   return s.str();
} // ToCommas(number)
