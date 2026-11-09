// void LSidebarDownCommandClass::Execute(int key) const -- 0x0051CAB0
// Scrolls the left column down by a row (SidebarClass::Scroll).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void LSidebarDownCommandClass::Execute(int key) const
{
    Map.Scroll(false, 0);
}
