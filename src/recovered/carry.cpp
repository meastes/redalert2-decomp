// carry.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/carry.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x00468AC0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00468AC0-0x00468CA6); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x00468AF0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468B10, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468B30, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468B50, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468B70, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468B90, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468BD0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468C00, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468C30, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468C60, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468C70, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.


// Dynamic initializer (0x00468C90, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00468ac0.cpp; no source of its own.




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
// infantry is placed with ScenarioInit cleared. Always false.

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
        bool old = ScenarioInit != 0;
        techno->Strength = Strength;
        if (RTTI == 15) {
            ScenarioInit = 0;
        }
        CoordStruct coord(Position.X * 256 + 128, Position.Y * 256 + 128, 0);
        techno->Unlimbo(&coord, 0);
        if (RTTI == 15) {
            ScenarioInit = old;
        }
    }
    return false;
}


// void *CarryoverClass::`scalar deleting destructor'(unsigned int flags) (0x00468E80)
// Compiler-generated with the vtable (LinkClass's destructor). No source of its own.
