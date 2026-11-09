// void TypeSelectCommandClass::Execute(int key) const -- 0x0051BB50
// On release (0x800) redraws the stopwatch (0x006F7670), else starts it (0x006F7650).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
void __fastcall Unknown_006F7670();
void Unknown_006F7650();

void TypeSelectCommandClass::Execute(int key) const
{
    if (key & 0x800) {
        Unknown_006F7670();
        return;
    }
    Unknown_006F7650();
}
