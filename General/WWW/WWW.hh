//
// WWW.hh
//

#ifndef WWW_hh
#define WWW_hh

#include <set>
#include <map>

#include "../Socket/BSD.hh"

namespace WWW {

   class _WWW_;

   typedef std::set<String, NoCase> Set;

   typedef std::map<String, String, NoCase> Map;

   typedef std::map<String, Set, NoCase> MapSet;

   namespace HTTP {
   } // HTTP

   namespace HTML {
   } // HTML

} // W#WW

class WWW::_WWW_ : private _General_ {
}; // _WWW_

#endif // WWW_hh
