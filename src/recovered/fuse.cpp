// fuse.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/fuse.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/bullet.hpp"
#include "shared/coord_distance.hpp"


// Header-static dynamic initializers of the unit at 0x004CEC20:  the
// shared/unknown_geometry.hpp constants (0x004CEC20-0x004CEDE1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004CEC50, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CEC70, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CEC90, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CECB0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CECD0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CECF0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CED30, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CED60, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CED90, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CEDC0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


// Dynamic initializer (0x004CEDD0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004cec20.cpp; no source of its own.


class Leaf_4cedf0 { public: unsigned int get(); };
unsigned int Leaf_4cedf0::get() { unsigned int initialization_value = (unsigned int)this; unsigned int fuse_address; unsigned int arming_start_frame; fuse_address = initialization_value; initialization_value = *(unsigned int*)(0xa40d2c); *(unsigned int*)(fuse_address) = initialization_value; initialization_value = 0; *(unsigned int*)(fuse_address + 8) = initialization_value; arming_start_frame = *(unsigned int*)(0xa40d2c); *(unsigned int*)(fuse_address + 0xc) = arming_start_frame; *(unsigned int*)(fuse_address + 0x14) = initialization_value; *(unsigned int*)(fuse_address + 0x18) = initialization_value; *(unsigned int*)(fuse_address + 0x1c) = initialization_value; *(unsigned int*)(fuse_address + 0x20) = initialization_value; *(unsigned int*)(fuse_address + 0x24) = initialization_value; return fuse_address; }


// void UnknownBulletData::Initialize_004cee20(CoordStruct const &from, CoordStruct const &to, int arm, int max) (0x004CEE20)
// Starts both timers (the first runs at least as long as the second), keeps the
// target coordinate and its distance from the start.

void UnknownBulletData::Initialize_004cee20(CoordStruct const &from, CoordStruct const &to, int arm, int max)
{
    if (max <= arm) {
        max = arm;
    }
    unknown_00 = max;
    unknown_0c = arm;
    unknown_18 = to;
    unknown_24 = Coord_Distance(from, to);
}


// int UnknownBulletData::Proximity_004ceee0(CoordStruct const &coord) (0x004CEEE0)

int UnknownBulletData::Proximity_004ceee0(CoordStruct const &coord)
{
    if (unknown_0c.Value() != 0) {
        return 0;
    }
    CoordStruct d = coord - unknown_18;
    int distance = (int)unknown_sqrt_4ba410((double)d.X * d.X + (double)d.Y * d.Y + (double)d.Z * d.Z) / 2;
    if (distance < 0x20) {
        return 1;
    }
    if (distance < 0x100 && distance > unknown_24) {
        return 2;
    }
    unknown_24 = distance;
    return 0;
}
