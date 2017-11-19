//
// FD.hh
//

#ifndef FD_hh
#define FD_hh

#include "../List.tt"

class _FD_ : private _General_ {
}; // _FD_

class Pool;

class FD : private _FD_ {

public: // Methods

   inline bool Valid() const;

   void Close();

protected: // Typedefs

   typedef int Type;

protected: // Methods

   FD(Type fd=Invalid);

   FD(FD &fd);

   virtual ~FD();

protected: // Variables

   Type fd;

private: // Enums

   enum { Invalid = (Type)-1 };

private: // Pool hooks

   friend Pool;

   friend List<FD>;

   void Add(Pool &pool);

   hook void Readable();

   hook void Writable();

   List<FD>::Node node;

}; // FD

inline bool FD::Valid() const {
   return fd!=Invalid;
} // FD::Valid()

#endif // FD_hh
