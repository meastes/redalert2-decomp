// void RSidebarDownCommandClass::Execute(int key) const -- 0x0051CB40
// Scrolls the right column down by a row (SidebarClass::Scroll).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void RSidebarDownCommandClass::Execute(int key) const
{
    Map.Scroll(false, 1);
}
