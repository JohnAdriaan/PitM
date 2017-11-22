//
// Debug.cc
//

#ifndef NDEBUG

#include <malloc.h>
#include <iostream>

#include "General.hh"

class List : private _General_ {
public: // Classes
   class Block; friend Block;
private:
   List();
   void Link(Block &block);
   void Unlink(const Block &block);
   ~List();
private: // Variables
   Block *head;
   Block *tail;
   unsigned total;
   unsigned count;
private: // Static variables
   static List list;
}; // List

class List::Block : private _General_ {
friend List;
public: // Methods
   inline void Link();
   inline void Unlink();
private: // Variables
   Block *prev;
   Block *next;
}; // List::Block

List List::list;

List::List() :
      head(nullptr),
      tail(nullptr),
      total(0),
      count(0) {
   std::cerr << "Checking allocations" << std::endl;
} // List::List()

void List::Link(Block &block) {
   if (head==nullptr) {
      head = &block;
   } // if
   if (tail!=nullptr) {
      tail->next = &block;
   } // if
   block.prev = tail;
   block.next = nullptr;
   ++total;
   ++count;
} // List::Link(Block)

void List::Unlink(const Block &block) {
   if (head==&block) {
      head = block.next;
   } // if
   if (tail==&block) {
      tail = block.prev;
   } // if
   --count;
} // List::Unlink(Block)

List::~List() {
   std::cerr << "There were " << total
             << " blocks allocated in total." << std::endl;
   std::cerr << "There were " << count
             << " blocks that weren't deleted." << std::endl;
} // List::~List()

inline void List::Block::Link() {
   List::list.Link(*this);
} // Block::Link()

inline void List::Block::Unlink() {
   if (prev!=nullptr) {
      prev->next = next;
   } // if
   if (next!=nullptr) {
      next->prev = prev;
   } // if
   List::list.Unlink(*this);
} // Block::Unlink()

void *operator new(Size size) {
   size += sizeof(List::Block);
   List::Block *block = (List::Block *)malloc(size);
   if (block==nullptr) {
      return nullptr;
   } // if
   block->Link();
   return block+1;
} // operator new(size)

void operator delete(void *ptr) {
   if (ptr==nullptr) {
      return;
   } // if
   List::Block *block = ((List::Block *)ptr)-1;
   block->Unlink();
   free(block);
} // operator delete(ptr)

#endif // !NDEBUG
