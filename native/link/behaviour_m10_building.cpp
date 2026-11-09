// Behaviour-written rows the skirmish harness reaches once units take move
// and deploy orders (Scout1103, native M10). NOT EXACT; never original code or bytes.
#include "shared/building_class.hpp"

// Native call bridge for the Per_Cell_Process draft (native/drafts/00500aa0.cpp):
// its BuildingClass view (map_techno.hpp) does not declare BuildingClass
// 0x00452390 (an Agent infiltrates for house); this calls the real member.
void Native_Building_Infiltrate_00452390(BuildingClass *building, HouseClass *house)
{
    building->Unknown_00452390(house);
}
