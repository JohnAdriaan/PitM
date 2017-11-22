//
// WWW.hh
//

#ifndef WWW_hh
#define WWW_hh

#include <map>

#include "../Socket/BSD.hh"

namespace WWW {

   class _WWW_;

   typedef std::map<String, String> Map;

   namespace HTTP {
   } // HTTP

   namespace HTML {
   } // HTML

} // W#WW

class WWW::_WWW_ : private _General_ {
}; // _WWW_

#endif // WWW_hh
