// bool CombatantSelectCommandClass::unknown_slot5(int key) const -- 0x0051BA80
// True with the shift bit (0x100).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

bool CombatantSelectCommandClass::unknown_slot5(int key) const
{
    return ((unsigned)key >> 8) & 1;
}
