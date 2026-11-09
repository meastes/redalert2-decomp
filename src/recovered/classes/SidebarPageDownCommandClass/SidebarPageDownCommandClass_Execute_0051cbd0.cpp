// void SidebarPageDownCommandClass::Execute(int key) const -- 0x0051CBD0
// Scrolls the current column down by a page (SidebarClass::Scroll_Page).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void SidebarPageDownCommandClass::Execute(int key) const
{
    Map.Scroll_Page(false, -1);
}
