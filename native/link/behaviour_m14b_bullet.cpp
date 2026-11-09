// Behaviour rows (Scout1121, native M14b; NOT EXACT, native/behaviour_rewrites.json).
#include "shared/bullet.hpp"

// 0x004CEDF0 UnknownBulletData::UnknownBulletData (pass-6a decompile 004cedf0):
// both frame timers started now with no delay, the coordinate and +0x24 zero.
UnknownBulletData::UnknownBulletData()
{
    unknown_18.X = 0;
    unknown_18.Y = 0;
    unknown_18.Z = 0;
    unknown_24 = 0;
}

// shared/bullet.hpp's Unknown_004BBAF0 is the tracked draft's Get_Obstacle_004bbaf0
// (native/drafts/004bbaf0.cpp, 0x004BBAF0): two names of one address; BulletClass
// 0x00461C10 tests the obstacle cell as a bool.
CellClass *__fastcall Get_Obstacle_004bbaf0(CellClass *source, CellClass *target, CellClass *last,
    CoordStruct coord, BulletTypeClass const *type, HouseClass const *house);
int __fastcall Unknown_004BBAF0(CellClass *source, CellClass *target, CellClass *last, CoordStruct coord,
    BulletTypeClass *type, HouseClass *house)
{
    return Get_Obstacle_004bbaf0(source, target, last, coord, type, house) != 0;
}
