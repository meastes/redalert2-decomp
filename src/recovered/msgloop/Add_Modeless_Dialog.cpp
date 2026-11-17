// void __fastcall Add_Modeless_Dialog(HWND dialog) -- 0x005B6000 (Westwood msgloop.cpp)
// Tracks a modeless dialog for the message pump.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/msgloop.hpp"
#include "shared/crt_string.hpp"

void __fastcall Add_Modeless_Dialog(HWND dialog)
{
    _ModelessDialogs.Add(dialog);
}
