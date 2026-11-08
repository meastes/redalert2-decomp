// NON-EXACT DRAFT 0x6C9DF0 FUN_006c9df0
// entry 0x6C9DF0; exact-match score 99.46 (misses_index best), last lane score 83.82; plan "draft as-is"
// origin lane: Scout1020 (build/class-work/Scout1020/keep/lz_c103_9946.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// LaserDrawClass *TechnoClass::unknown_6c9df0(AbstractClass *target, int weapon_index, WeaponTypeClass *weapon, CoordStruct const &source) (0x006C9DF0)
// Draws the weapon's laser from source (or the weapon's fire coordinate when
// source is the zero coordinate; a building then lowers it by the screen
// height of that coordinate above its slot 42 coordinate) to the target's
// coordinate: the weapon's inner/outer colours, or with IsHouseColor (+0x123)
// the owner's laser colour (+0x55EC) and its half as the outer colour.
// Body 560 bytes: ret 10h at 0x006CA01D (the inventory stopped at 368).
#include "shared/techno.hpp"
#include "shared/weapon_type.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/laser_draw.hpp"
// @ref _unknown_geometry_l=00ac0188
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?Coord_To_Pixel_Absolute@Tactical@@QAE?AUPoint2D@@ABUCoordStruct@@@Z=006a27a0
// @ref ??0LaserDrawClass@@QAE@UCoordStruct@@0H_NVRGBClass@@22H11MM@Z=00533110

LaserDrawClass *TechnoClass::unknown_6c9df0(AbstractClass *target, int weapon_index, WeaponTypeClass *weapon,
                                            CoordStruct const &source)
{
    RGBClass half;
    int duration = weapon->unknown_124;
    RGBClass const *spread = &weapon->unknown_102;
    RGBClass const *inner;
    RGBClass const *outer;
    if (weapon->unknown_123) {
        inner = &House->unknown_55ec;
        half.Red = inner->Red >> 1;
        half.Green = inner->Green >> 1;
        half.Blue = inner->Blue >> 1;
        outer = &half;
    } else {
        inner = &weapon->unknown_fc;
        outer = &weapon->unknown_ff;
    }
    CoordStruct from;
    CoordStruct to;
    int depth = 0;
    if (source != unknown_geometry_l) {
        from = source;
    } else {
        from = unknown_slot43(weapon_index);
        if (Kind_Of() == 6) {
            Point2D a = unknown_tactical_83996c->Coord_To_Pixel_Absolute(from);
            Point2D b = unknown_tactical_83996c->Coord_To_Pixel_Absolute(unknown_slot42());
            depth = a.Y - b.Y;
            if (depth > 0) {
                depth = 0;
            }
        } else {
            depth = 0;
        }
    }
    if (target != 0 && target->AbstractFlags.IsObject) {
        to = ((ObjectClass *)target)->unknown_slot40();
    } else {
        to = target->unknown_slot22();
    }
    LaserDrawClass *laser = new LaserDrawClass(from, to, depth, true, *inner, *outer, *spread, duration,
                                               false, true, 1.0f, 0.0f);
    if (laser != 0 && weapon->unknown_123) {
        laser->IsHouseColor = true;
    }
    return laser;
}
