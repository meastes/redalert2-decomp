// coord.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/coord.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_PLUS_POINT2D
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/coord.hpp"
#include "shared/rect.hpp"
#include "shared/abstract.hpp"
#include "shared/random.hpp"
#include "shared/techno.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/map_view.hpp"


// Header-static dynamic initializers of the unit at 0x00491430:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00491430-0x00491616); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00491460, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x00491480, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x004914A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x004914C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x004914E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x00491500, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x00491540, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x00491570, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x004915A0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x004915D0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x004915E0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


// Dynamic initializer (0x00491600, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00491430.cpp; no source of its own.


extern Cell const AdjacentCell[8];
Cell const AdjacentCell[8] = {
 Cell(0,-1),Cell(1,-1),Cell(1,0),Cell(1,1),
 Cell(0,1),Cell(-1,1),Cell(-1,0),Cell(-1,-1)
};


extern Point2D const unknown_adjacent_offsets_00851ac0[8];
Point2D const unknown_adjacent_offsets_00851ac0[8] = {
 Point2D(0,-256),Point2D(256,-256),Point2D(256,0),Point2D(256,256),
 Point2D(0,256),Point2D(-256,256),Point2D(-256,0),Point2D(-256,-256)
};


extern double const unknown_facing_radians_79b620;
// Actual three-component position, preserving the out-of-line vector constructor.
inline UnknownVector3 Compute_Scatter_Vector(CoordStruct const &coord,double radians,int distance)
{
    int x,y,z;
    z = coord.Z;
    y = (int)(coord.Y - (double)Fast_Sin(radians) * distance);
    x = (int)(coord.X + (double)Fast_Cos(radians) * distance);
    UnknownVector3 result(x,y,z);
    return result;
}
CoordStruct __fastcall Coord_Scatter(CoordStruct const &coord, int distance, bool lock)
{
    {
    UnknownFacingDir direction(0);
    unsigned direction_word = (*(unsigned *)&direction & 0xFFFF00FF) | ((unknown_id_source_a3d290->Random()() & 255) << 8);
    double radians = ((short)direction_word - 0x3FFF) * unknown_facing_radians_79b620;
    UnknownVector3 offset=Compute_Scatter_Vector(coord,radians,distance);
    CoordStruct result(offset.X,offset.Y,offset.Z);
    if ((unsigned)(result.X / 256) >= 512 || (unsigned)(result.Y / 256) >= 512) result = coord;
    if (lock) {
        result = result - CoordStruct(result.X & 255,result.Y & 255,0);
        result = result + CoordStruct(128,128,0);
    }
    return result;
}

}


// CoordStruct __fastcall Adjacent_Coord_With_Height(CoordStruct const &coord, int facing) -- 0x00491880
// TibSun coord.cpp: the coordinate one cell toward a facing (the AdjacentPoint offsets at
// 0x00851AC0), its height moved by the ground-height difference between the two places.

CoordStruct __fastcall Adjacent_Coord_With_Height(CoordStruct const &coord, int facing)
{
    CoordStruct newcoord = (coord + unknown_adjacent_offsets_00851ac0[facing]);
    newcoord.Z += Map.Get_Height_GL(newcoord) - Map.Get_Height_GL(coord);
    return newcoord;
}
