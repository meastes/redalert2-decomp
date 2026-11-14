// carry.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/carry.json carries the addresses and the rows that
// stay in their own files.


#include "shared/carryover.hpp"
#include "shared/building_class.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/unit.hpp"
#include "shared/infantry.hpp"
#include "shared/building_type_list.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/cell.hpp"


// CarryoverClass::CarryoverClass(TechnoClass *techno) (0x00468CB0)
// Records the object's kind, type, cell, strength and house.
// The unit builds buildings (Create): the full BuildingClass before techno.hpp's stub.

CarryoverClass::CarryoverClass(TechnoClass *techno) :
    RTTI(0),
    Position(0, 0),
    Strength(0),
    House(-1)
{
    if (techno) {
        RTTI = techno->Kind_Of();
        switch (RTTI) {
            case 1:
            case 6:
                Type = techno->Techno_Type_Class()->Fetch_Heap_ID();
                break;
            case 15:
                Type = techno->Techno_Type_Class()->Fetch_Heap_ID();
                break;
        }
        House = techno->House->Type->House;
        Strength = techno->Strength;
        Position = techno->unknown_slot104();
    }
}


// bool CarryoverClass::Create() const (0x00468D50)
// Recreates the object for its house at its cell with its strength;
// infantry is placed with unknown_a40774 cleared. Always false.

bool CarryoverClass::Create() const
{
    HouseClass *house = HouseClass::As_Pointer(House);
    TechnoClass *techno = 0;
    switch (RTTI) {
        case 1:
            techno = new UnitClass(UnitTypes[Type], house);
            break;
        case 6:
            techno = new BuildingClass(BuildingTypes[Type], house);
            break;
        case 15:
            techno = new InfantryClass(InfantryTypes[Type], house);
            break;
    }
    if (techno) {
        bool old = unknown_a40774 != 0;
        techno->Strength = Strength;
        if (RTTI == 15) {
            unknown_a40774 = 0;
        }
        CoordStruct coord(Position.X * 256 + 128, Position.Y * 256 + 128, 0);
        techno->Unlimbo(&coord, 0);
        if (RTTI == 15) {
            unknown_a40774 = old;
        }
    }
    return false;
}


// void *CarryoverClass::`scalar deleting destructor'(unsigned int flags) (0x00468E80)
// Compiler-generated with the vtable (LinkClass's destructor). No source of its own.
