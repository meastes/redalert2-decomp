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

// The C4 arming the same draft does on a building (TibSun Per_Cell_Process's sabotage block:
// IsGoingToBlow, the CountDown timer and WhomToRepay): the real members, which its view does not
// name (Scout1231; the draft wrote them at their VC6 offsets, +0x5AF being the top byte of the
// arm64 animation pointer unknown_454[7], which slot 79 then dereferenced: user crash 2026-11-10).
bool Native_Building_Is_Going_To_Blow_00500AA0(BuildingClass *building)
{
    return building->unknown_5af;
}
void Native_Building_Set_Going_To_Blow_00500AA0(BuildingClass *building)
{
    building->unknown_5af = true;
}
void Native_Building_Arm_C4_00500AA0(BuildingClass *building, int countdown, AbstractClass *who)
{
    building->unknown_420 = countdown;
    building->unknown_438 = who;
}
