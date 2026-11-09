// bool TypeSelectCommandClass::unknown_slot7(int key) const -- 0x0051BB40
// True unless the key carries the release bit (0x800).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

bool TypeSelectCommandClass::unknown_slot7(int key) const
{
    return ((unsigned)~key >> 11) & 1;
}
