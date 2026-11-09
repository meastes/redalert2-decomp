// void SidebarUpCommandClass::Execute(int key) const -- 0x0051C6C0
// Scrolls the current column up by a row (SidebarClass::Scroll).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void SidebarUpCommandClass::Execute(int key) const
{
    Map.Scroll(true, -1);
}
