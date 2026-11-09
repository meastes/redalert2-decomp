// void ToggleSellCommandClass::Execute(int key) const -- 0x0051C530
// Toggles the sell mode (0x0049C7C0, -1).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void ToggleSellCommandClass::Execute(int key) const
{
    Map.Unknown_0049C7C0(-1);
}
