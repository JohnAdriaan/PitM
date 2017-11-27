//
// File.hh
//

#ifndef File_hh
#define File_hh

#include "../FD/FD.hh"

class File : public FD {

public: // Enums

   enum Creates {
      Exists,     // Open only if exists
      Create,     // Create if doesn't exist
      Exclusive,  // Only create if doesn't exist
      Truncate    // Start with empty file
   }; // Creates

   enum Accesses {
      ReadOnly,
      WriteOnly,
      ReadWrite
   }; // Accesses

public: // Static methods

   static String Expand(const String &name);

public: // Methods

   explicit File(const char *path,
                 Accesses access=ReadOnly,
                 Creates create=Exists);

   explicit File(File &rhs) = default;

   ::Size Size() const;

}; // File

#endif // File_hh
