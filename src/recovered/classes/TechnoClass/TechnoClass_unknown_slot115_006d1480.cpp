// ConvertClass *TechnoClass::unknown_slot115() (0x006D1480, slot 115)
// The colour scheme converter (ColorScheme::Converter of a ColorSchemes entry) to draw with: from
// the type's own palette list when it has one, else the owner's scheme, or
// slot 51's house's when slot 245 rejects the player.
#include "shared/techno.hpp"
#include "shared/anim.hpp"
#include "shared/color_scheme.hpp"
// @ref ?ColorSchemes@@3V?$DynamicVectorClass@PAVColorScheme@@@@A=00ab6ef8
// @ref ?unknown_player_a35db4@@3PAVHouseClass@@A=00a35db4

ConvertClass *TechnoClass::unknown_slot115()
{
    TechnoTypeClass *type = Techno_Type_Class();
    int scheme = House->ColorSchemeIndex;
    DynamicVectorClass<ColorScheme *> *palette = (DynamicVectorClass<ColorScheme *> *)type->Palette;
    if (palette != 0) {
        return (*palette)[scheme]->Converter;
    }
    if (!unknown_slot245(unknown_player_a35db4) && unknown_slot51(true) != 0) {
        return ColorSchemes[unknown_slot51(true)->ColorSchemeIndex]->Converter;
    }
    return ColorSchemes[scheme]->Converter;
}
