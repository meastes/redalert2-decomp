// void OptionsCommandClass::Execute(int key) const -- 0x0051CD80
// Opens the options menu (Queue_Options 0x00623540).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
#include "shared/queue.hpp"

void OptionsCommandClass::Execute(int key) const
{
    Queue_Options();
}
