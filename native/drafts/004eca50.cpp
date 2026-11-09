// NON-EXACT DRAFT 0x4ECA50 HouseClass::Random_Cell_In_Zone (RA's Random_Cell_In_Zone)
// entry 0x4ECA50; exact-match score 54.60 (misses_index best, Scout1046 rescore of Scout705 rz3); plan "draft, verify".
// Origin: build/class-work/Scout1046/w/rz0.cpp. Recovered source text by our lanes/workers; never
// exact-match credit. Tracked by Scout1111 (native/drafts.json). Not original bytes. Reached at
// frame ~4,000 of the M12 '#0' run (HouseClass 0x004EB270 from UnitClass slot 93: the computer
// player's units). Native adaptation (Scout1111): two lines marked NATIVE.
// Cell HouseClass::Random_Cell_In_Zone(int zone) (0x004ECA50)
// RA's Random_Cell_In_Zone: a random cell of a base zone. Core zone 0 is
// scattered around the base centre; zones 1 to 4 pick a random direction in
// their quadrant (north-east, south-east, south-west, north-west) and a
// distance of one to two radii (+0x53C8, between 0x300 and 0x800 leptons)
// from the base centre. A cell off the map is replaced by the nearest valid
// one; a zone without a coordinate falls back to the core zone.
#include "shared/techno.hpp"
#include "shared/bullet.hpp"
#include "shared/facing.hpp"
#include "shared/dir_type.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/building_light.hpp"
extern CoordStruct const unknown_coord_a40f48;   // NATIVE: the none coordinate 0x00A40F48 (declared as in its other users)
Cell Native_Map_Clip_To_Map_00568d60(Cell const &cell);   // NATIVE: link/behaviour_m12_map.cpp (map_view.hpp's MapClass lacks it)
// @ref ?Fast_Sin@@YAMN@Z=004ba480
// @ref ?Fast_Cos@@YAMN@Z=004ba4d0
// @ref ?unknown_facing_radians_79b620@@3NB=0079b620
// @ref ?unknown_coord_a40f48@@3UCoordStruct@@A=00a40f48
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Unknown_0055A660@MapClass@@QAE_NABVCell@@_N@Z=0055a660
// @ref ??0UnknownVector3@@QAE@HHH@Z=0041b630
// @ref ?Coord_Scatter@@YI?AUCoordStruct@@ABU1@H_N@Z=00491750

#define LESS_MAX(a, b) (((a) < (b)) ? (b) : (a))
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
static inline DirType Rotate_Back_One_Facing(DirType const &dir)
{
    int fraction = dir.Facing & 0x1FFF;
    DirType sector(dir);
    DirType back(sector.Snap_To_8().Raw - 1);
    return DirType(back.Snap_To_8().Facing ^ fraction);
}
inline UnknownVector3 Move_Coord(CoordStruct const &start, DirType const &dir, int distance)
{
    double radians = (dir.Facing - 0x3FFF) * unknown_facing_radians_79b620;
    int z = start.Z;
    int y = (int)(start.Y - (double)Fast_Sin(radians) * distance);
    int x = (int)(start.X + (double)Fast_Cos(radians) * distance);
    UnknownVector3 result(x, y, z);
    return result;
}

Cell HouseClass::Random_Cell_In_Zone(int zone)
{
    int radius = LESS_MAX(MIN(unknown_53c8, 0x800), 0x300);
    int maxdist = radius * 2;
    UnknownVector3 coord;
    switch (zone) {
    case 0:
        coord = (UnknownVector3 const &)Coord_Scatter(BaseCenter, Random_Pick(0, radius), true);
        break;
    case 1: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw &= ~0xC000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    case 2: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw &= ~0x8000;
        dir.Raw |= 0x4000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    case 3: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw &= ~0x4000;
        dir.Raw |= 0x8000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    case 4: {
        int distance = Random_Pick(radius, maxdist);
        DirType dir(unknown_id_source_a3d290->Random()());
        dir.Raw |= 0xC000;
        coord = Move_Coord(BaseCenter, Rotate_Back_One_Facing(dir), distance);
        break;
    }
    default:
        (CoordStruct &)coord = unknown_coord_a40f48;
        break;
    }
    Cell cell;
    if ((CoordStruct const &)coord == unknown_coord_a40f48) {
        if (zone == 0) {
            cell = Coord_Cell(BaseCenter);
        } else {
            cell = Random_Cell_In_Zone(0);
        }
    } else {
        cell = Cell(coord.X / 256, coord.Y / 256);
    }
    if (!Map.Unknown_0055A660(cell, true)) {
        cell = Native_Map_Clip_To_Map_00568d60(cell);   // NATIVE: MapClass::Clip_To_Map 0x00568D60
    }
    return cell;
}
