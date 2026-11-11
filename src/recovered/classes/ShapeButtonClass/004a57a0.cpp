// 0x004A57A0: ShapeButtonClass's scalar deleting destructor, the copy the linker kept from the
// DropList unit: runs ShapeButtonClass::~ShapeButtonClass, then operator delete when flags bit 0
// is set. Stand-in owner class until the emitting unit produces it naturally.
#include "shared/gadget.hpp"

class Leaf_4a57a0 { public: void* get(unsigned int flags); };
void* Leaf_4a57a0::get(unsigned int flags) { ((ShapeButtonClass *)this)->ShapeButtonClass::~ShapeButtonClass(); if (flags & 1) operator delete(this); return this; }
