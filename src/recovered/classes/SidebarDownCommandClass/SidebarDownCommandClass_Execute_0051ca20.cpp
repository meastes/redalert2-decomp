// void SidebarDownCommandClass::Execute(int key) const -- 0x0051CA20
// Scrolls the current column down by a row (SidebarClass::Scroll).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void SidebarDownCommandClass::Execute(int key) const
{
    Map.Scroll(false, -1);
}
