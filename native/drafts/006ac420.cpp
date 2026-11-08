// NON-EXACT DRAFT 0x6AC420 Tactical::Draw_Radial_Indicators
// entry 0x6AC420; exact-match score 78.52 (misses_index best); plan "draft, verify"
// origin lane: Scout861 (build/class-work/Scout861/w/ri1h.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Draws the radial indicators of the selected objects; while a building is being placed (the display's pending
// object), pulses its own radius toward black and marks the nearest building of the player's of the same type
// within twice that radius of the cursor cell (0x006AC420; TibSun Tactical::Draw_Radial_Indicators).
#include "shared/tactical.hpp"
#include "shared/object.hpp"
#include "shared/object_type.hpp"
#include "shared/building_class.hpp"
#include "shared/house.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/rgb_class.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_globals.hpp"
void __fastcall Draw_Radial_Indicator_00451BF0(unsigned int radius, unsigned char mode,
    CoordStruct center, RGBClass color, bool filled, bool radius_is_pixels);
extern bool unknown_placing_a40b32;
extern int unknown_radial_pulse_abfd78;
extern int unknown_radial_step_7f6974;
// @ref ?unknown_objects_a40c60@@3V?$DynamicVectorClass@PAVObjectClass@@@@A=00a40c60
// @ref ?unknown_placing_a40b32@@3_NA=00a40b32
// @ref ?unknown_radial_pulse_abfd78@@3HA=00abfd78
// @ref ?unknown_radial_step_7f6974@@3HA=007f6974
// @ref ?unknown_451930@BuildingClass@@QAEHXZ=00451930
// @ref ?Draw_Radial_Indicator_00451BF0@@YIXIEUCoordStruct@@VRGBClass@@_N2@Z=00451bf0
// @ref ?Adjust@RGBClass@@QAEXHABV1@@Z=0063cf20
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref ?unknown_475d60@CellClass@@QBE?AUCoordStruct@@XZ=00475d60
// @ref ?unknown_player_a35db4@@3PAVHouseClass@@A=00a35db4
// @ref _?$S92@?BA@??Draw_Radial_Indicators@Tactical@@QAEXXZ@4EA=00abe488
// @ref _$E93=006ac7a0
// @ref _?black@?BA@??Draw_Radial_Indicators@Tactical@@QAEXXZ@4VRGBClass@@A=00abe54c

void Tactical::Draw_Radial_Indicators()
{
    int count = unknown_objects_a40c60.Count();
    for (int i = 0; i < count; i++) {
        ObjectClass *object = unknown_objects_a40c60[i];
        if (object->Class_Of() && object->Class_Of()->unknown_208) {
            object->unknown_slot74(1);
        }
    }

    if (unknown_placing_a40b32) {
        BuildingClass *pending = (BuildingClass *)Map.unknown_11a0;
        if (pending != 0 && pending->Kind_Of() == 6) {
            if (pending->unknown_451930() > 0) {
                Cell cell = Map.unknown_1170;
                CoordStruct center = Map[cell].unknown_475d60();

                static RGBClass black(0, 0, 0);
                unknown_radial_pulse_abfd78 += unknown_radial_step_7f6974 * 10;
                if (unknown_radial_pulse_abfd78 >= 100) {
                    unknown_radial_step_7f6974 = -1;
                } else if (unknown_radial_pulse_abfd78 <= 0) {
                    unknown_radial_step_7f6974 = 1;
                }

                RGBClass color = pending->House->unknown_55e9;
                color.Adjust(unknown_radial_pulse_abfd78, black);
                int radius = pending->unknown_451930();
                Draw_Radial_Indicator_00451BF0(radius, 0, center, color, true, false);

                int nearest = 0x7FFFFFFF;
                int buildings = unknown_player_a35db4->Buildings.Count();
                int index;
                for (index = 0; index < buildings; index++) {
                    BuildingClass *building = (BuildingClass *)unknown_player_a35db4->Buildings[index];
                    if (building != 0 && building->unknown_7d && building != pending && building->Type == pending->Type) {
                        Cell zone = Map.unknown_1170;
                        Cell where = Cell(building->Center_Coord());
                        TPoint2D<short> delta = zone - where;
                        int distance = (short)Fast_Sqrt((double)delta.X * (double)delta.X + (double)delta.Y * (double)delta.Y);
                        if (distance <= radius * 2 && distance < nearest) {
                            nearest = distance;
                        }
                    }
                }
                for (index = 0; index < buildings; index++) {
                    BuildingClass *building = (BuildingClass *)unknown_player_a35db4->Buildings[index];
                    if (building != 0 && building->unknown_7d && building != pending && building->Type == pending->Type) {
                        Cell zone = Map.unknown_1170;
                        Cell where = Cell(building->Center_Coord());
                        TPoint2D<short> delta = zone - where;
                        int distance = (short)Fast_Sqrt((double)delta.X * (double)delta.X + (double)delta.Y * (double)delta.Y);
                        if (distance == nearest) {
                            building->unknown_slot74(0);
                        }
                    }
                }
            }
        }
    }
}
