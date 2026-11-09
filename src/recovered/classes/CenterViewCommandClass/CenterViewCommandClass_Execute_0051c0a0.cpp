// void CenterViewCommandClass::Execute(int key) const -- 0x0051C0A0
// Centres the view on the selected objects (0x0049E3B0) and redraws.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
#include "shared/unknown_globals.hpp"

void CenterViewCommandClass::Execute(int key) const
{
    if (unknown_objects_a40c60.Count() > 0) {
        Map.Unknown_0049E3B0();
        Map.Flag_To_Redraw(true);
    }
}
