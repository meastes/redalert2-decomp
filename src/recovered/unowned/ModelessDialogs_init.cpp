// msgloop.cpp's two lists (Westwood wwlib msgloop.cpp): the modeless dialogs (0x00A71900) and the accelerators
// (0x00A71928). Their dynamic initializers are 0x005B5DE0 / 0x005B5E60 and their atexit destructors
// 0x005B5E20 / 0x005B5EA0 (comment-only drafts follow). This first source of the unit sets the vector
// implementation switches the unit's members use.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/msgloop.hpp"
#include "shared/crt_string.hpp"

DynamicVectorClass<HWND> _ModelessDialogs;
DynamicVectorClass<AcceleratorTracker> _Accelerators;
