// CarryoverClass::CarryoverClass(TechnoClass *techno) (0x00468CB0)
// Records the object's kind, type, cell, strength and house.
#include "shared/carryover.hpp"
// The unit builds buildings (Create): the full BuildingClass before techno.hpp's stub.
#include "shared/building_class.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
// @ref ??_7CarryoverClass@@6B@=0079d83c

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
