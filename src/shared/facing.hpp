// Direction values and FacingClass: turning state shared by technos and locomotors.
#ifndef RA2_SHARED_FACING_HPP
#define RA2_SHARED_FACING_HPP
#include "shared/coord.hpp"
#include "shared/cd_timer.hpp"
#if defined(RA2_FACING_DIR256) || defined(RA2_SHIP_SOM)
#include "shared/dir_type.hpp"
#endif
// A 16-bit direction in a 4-byte value (hypothesis: Westwood's DirStruct;
// cf. UnknownDirStruct in shared/bullet.hpp).
enum Dir32 {}; // The reference's wrapped 32-direction index type.
struct UnknownFacingDir {
    unsigned Round_To_32() const;
    Dir32 As_Dir32() const;
    double As_Radian32() const;
    void From_Radian(double angle); // original inline angle conversion
    double As_Radian() const; // original signed16 angle observation
    UnknownFacingDir() {}
    UnknownFacingDir(unsigned short raw) : Raw(raw) {}
#ifdef RA2_SHIP_SOM
    UnknownFacingDir(Dir256 dir) { From_Dir256(dir); }
    void From_Dir256(Dir256 dir) { Raw=0; Raw|=(dir<<8); }
    void From_Facing(int facing) { Raw=0; Raw |= facing << 13; }
    UnknownFacingDir(FacingType facing) { From_Facing(facing); }
    UnknownFacingDir operator-(UnknownFacingDir const &other) const { return UnknownFacingDir((unsigned short)((short)Raw-(short)other.Raw)); }
#endif
#ifdef RA2_FACING_DIR256
    UnknownFacingDir(Dir256 dir) { From_Dir256(dir); }
    void From_Dir256(Dir256 dir) { Raw=0; Raw|=(dir<<8); }
#endif
// The native Parasite AI observes a signed low-word angle, then rotates left by a quarter turn.
#ifdef RA2_FACING_LEFT90
    UnknownFacingDir(int facing) : Facing((short)facing) {}
    UnknownFacingDir Left_90() const { return UnknownFacingDir((int)Facing - 0x3fff); }
#endif
#ifdef RA2_FACING_DIFFERENCE
    // TibSun DirType::operator-: the signed difference of two directions.
    UnknownFacingDir operator-(UnknownFacingDir const &other) const { return UnknownFacingDir(Facing - other.Facing); }
#endif
    UnknownFacingDir(CoordStruct const &from, CoordStruct const &to);   // 0x00423EF0 (the direction from one coordinate to another)
    // TibSun DirType::Direction: the same code as the constructor above (the
    // AnimClass unit calls it on a default-constructed direction and reads the
    // direction back through the returned reference).
    UnknownFacingDir &Direction(CoordStruct const &from, CoordStruct const &to);   // 0x00423EF0
    // Private source hypothesis for original signed-angle magnitude comparison.
    bool Magnitude_At_Least_004bfb30(UnknownFacingDir const &other) const;
#ifdef RA2_FACING_LEFT90
    union { unsigned short Raw; short Facing; };
#else
    unsigned short Raw;
#endif
    unsigned short Padding;
};
#ifdef RA2_FACING_DIR256
inline bool operator==(UnknownFacingDir const &left,UnknownFacingDir const &right) { return left.Raw==right.Raw; }
inline bool operator!=(UnknownFacingDir const &left,UnknownFacingDir const &right) { return left.Raw!=right.Raw; }
#endif

#ifdef RA2_SHIP_SOM
inline bool operator<(UnknownFacingDir const &left, UnknownFacingDir const &right) { return abs((short)left.Raw) < abs((short)right.Raw); }
#endif
// One of eight directions (the top bits of a direction, rounded; read as
// 32 bits).
inline int Dir_To_8(UnknownFacingDir const &dir)
{
    return (((*(unsigned const *)&dir >> 12) + 1) >> 1) & 7;
}

// The direction of an 8-way facing, and a direction as one of 256 (read as
// 32 bits).
inline UnknownFacingDir Facing_Dir(int facing)
{
    return UnknownFacingDir((unsigned short)(facing << 13));
}
inline int Dir_To_256(UnknownFacingDir const &dir)
{
    return (((*(unsigned const *)&dir >> 7) + 1) >> 1) & 0xFF;
}
// A direction as one of 32 (BuildingClass slot 183; read as 32 bits).
inline int Dir_To_32(UnknownFacingDir const &dir)
{
    return (((*(unsigned const *)&dir >> 10) + 1) >> 1) & 0x1F;
}


// Westwood's FacingClass (TibSun/mischa85 facing.h): a direction that turns toward a desired one at a
// rate. The module is src/recovered/facing.cpp. Known differences from the template header
// (manifests/original_target/modules.json header_gaps): the directions are UnknownFacingDir, not
// DirType (the face module is not converted yet), and the members are 16-bit values with their
// padding words; Is_Rotating_CW/CCW return char and Difference is const.
class FacingClass {
public:
    FacingClass();
    FacingClass(int rate);
    FacingClass(NoInitClass const &noinit) : RotationTimer(noinit) {}
    UnknownFacingDir Current() const;
    UnknownFacingDir Desired() const;
    bool Set_Desired(UnknownFacingDir const &facing);
    void Set_ROT(int rate);
    bool Set(UnknownFacingDir const &facing);
    bool Is_Rotating() const;
    char Is_Rotating_CW() const;
    char Is_Rotating_CCW() const;
    UnknownFacingDir Difference() const;
    UnknownFacingDir Difference(UnknownFacingDir const &facing) const;

    short DesiredFacing;                // +0x00 the direction being turned toward
    short unknown_02;
    short StartFacing;                  // +0x04 the direction the turn started from
    short unknown_06;
    CDTimerClass<FrameTimerClass> RotationTimer;    // +0x08 frames left in the turn
    short ROT;                          // +0x14 the turn rate (Dir256 steps in the high byte)
};

#endif
