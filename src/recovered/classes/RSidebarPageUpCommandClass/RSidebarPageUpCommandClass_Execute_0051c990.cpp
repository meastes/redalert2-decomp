// void RSidebarPageUpCommandClass::Execute(int key) const -- 0x0051C990
// Scrolls the right column up by a page (SidebarClass::Scroll_Page).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void RSidebarPageUpCommandClass::Execute(int key) const
{
    Map.Scroll_Page(true, 1);
}
