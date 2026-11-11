// BuildingType cost adjustments reconstructed from the exact target.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/building_type.hpp"
#include "shared/unit.hpp"
#include "shared/unit_type.hpp"
#include "shared/rules.hpp"
#include "shared/aircraft_type.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// Genuine mathematical clamp; no original-address binding.

// Maximum_Cost is defined with Raw_Cost earlier in this whole class unit.
template<class T> inline T Maximum_Cost(T first,T second);

int BuildingTypeClass::Cost_Of(HouseClass *house) const
{
    int value=TechnoTypeClass::Cost_Of(house);
    TechnoTypeClass *first_type=Rules->PadAircraft[0];
    if(this==first_type->Dock[0] && !Rules->unknown_142c) {
        int average=(int)((unsigned int)Rules->PadAircraft[0]->Cost_Of(house)+(unsigned int)Rules->PadAircraft[1]->Cost_Of(house))/2;
        value=(int)((unsigned int)value+(unsigned int)average);
    }
    if(FreeUnit) {
        value=(int)((unsigned int)value+(unsigned int)FreeUnit->Cost_Of(house));
        return Maximum_Cost(value,0);
    }
    return value;
}
