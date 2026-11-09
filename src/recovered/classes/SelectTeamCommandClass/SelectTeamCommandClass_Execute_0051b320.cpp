// void SelectTeamCommandClass::Execute(int key) const -- 0x0051B320
// Selects team TeamNumber (0x006F5F00).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"

void SelectTeamCommandClass::Execute(int key) const
{
    Unknown_006F5F00(TeamNumber);
}
