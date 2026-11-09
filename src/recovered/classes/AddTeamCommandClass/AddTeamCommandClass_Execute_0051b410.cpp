// void AddTeamCommandClass::Execute(int key) const -- 0x0051B410
// Adds the selection to team TeamNumber (0x006F61F0).
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/object.hpp"
#include "shared/tab.hpp"
void __fastcall Unknown_006F61F0(int group);

void AddTeamCommandClass::Execute(int key) const
{
    Unknown_006F61F0(TeamNumber);
}
