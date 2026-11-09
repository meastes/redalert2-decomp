// void ToggleRepairCommandClass::Execute(int key) const -- 0x0051C4B0
// Toggles the repair mode (0x0049CA20, -1).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void ToggleRepairCommandClass::Execute(int key) const
{
    Map.Unknown_0049CA20(-1);
}
