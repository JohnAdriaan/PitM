//
// FD.hh
//

#ifndef FD_hh
#define FD_hh

#include "../General.hh"

class _FD_ : private _General_ {
}; // _FD_

class FD : private _FD_ {

public: // Constants

   static const unsigned MaxSendFile = 0x7FFF0000;

public: // Methods

   inline bool Valid() const;

   // Returns false on error.
   // Returns true, but read==0, for end of file
   bool Read(void *buffer, unsigned size, unsigned &read);

   // Doesn't return until all written - or error
   bool Write(const String &string);

   // Doesn't return until all written - or error
   bool Write(const void *buffer, unsigned size);

   bool Write(const void *buffer, unsigned size, unsigned &wrote);

   bool SendFile(FD &source, unsigned length=MaxSendFile);

   void Close();

protected: // Typedefs

   typedef int Type;

protected: // Methods

   explicit FD(Type fd=Invalid);

   explicit FD(FD &fd);

   ~FD();

protected: // Variables

   Type fd;

private: // Enums

   enum { Invalid = (Type)-1 };

}; // FD

inline bool FD::Valid() const {
   return fd!=Invalid;
} // FD::Valid()

#endif // FD_hh
