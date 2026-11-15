// Westwood facing.cpp in one source (twins: mischa85/RedAlert2 src/facing.cpp, TibSun facing.cpp); the
// Parameter names from mischa85/RedAlert2 392e904 src/facing.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/facing.cpp where both functions declare the same local types (names only).
// accepted bodies under the template's names, the directions as UnknownFacingDir until DirType is converted.
#include "shared/facing.hpp"

FacingClass::FacingClass() : DesiredFacing(0), StartFacing(0), ROT(0) {}

FacingClass::FacingClass(int rate) : DesiredFacing(0), StartFacing(0), ROT(0) { Set_ROT(rate); }

extern "C" int __cdecl abs(int value);
bool FacingClass::Set_Desired(UnknownFacingDir const &facing)
{
    if (DesiredFacing != (short)facing.Raw) {
    *(UnknownFacingDir *)&StartFacing = Current();
    *(UnknownFacingDir *)&DesiredFacing = facing;
    if (ROT > 0) {
        short delta = (short)(DesiredFacing - StartFacing);
        RotationTimer = CDTimerClass<FrameTimerClass>(abs(delta) / ROT);
    }
    return true;
    }
    return false;
}

bool FacingClass::Set(UnknownFacingDir const &facing)
{
    if(Current().Raw!=facing.Raw) {
        *(UnknownFacingDir *)&DesiredFacing=facing;
        *(UnknownFacingDir *)&StartFacing=facing;
        RotationTimer=CDTimerClass<FrameTimerClass>(0);
        return true;
    }
    RotationTimer=CDTimerClass<FrameTimerClass>(0);
    return false;
}


UnknownFacingDir FacingClass::Current() const
{
    if (Is_Rotating()) {
        short diff = (short)(DesiredFacing - StartFacing);
        short rot = (short)(abs(diff) / ROT);
        UnknownFacingDir dir = *(UnknownFacingDir const *)&DesiredFacing;
        if (rot > 0)
            dir.Raw = (unsigned short)(dir.Raw - (diff / rot) * (short)RotationTimer.Value());
        return dir;
    }
    return *(UnknownFacingDir const *)&DesiredFacing;
}

UnknownFacingDir FacingClass::Desired() const { return *(UnknownFacingDir const *)&DesiredFacing; }

bool FacingClass::Is_Rotating() const { return ROT > 0 && RotationTimer.Value() != 0; }

char FacingClass::Is_Rotating_CW() const { if (Is_Rotating()) return (short)(DesiredFacing - StartFacing) > 0; return 0; }

char FacingClass::Is_Rotating_CCW() const { if (Is_Rotating()) return (short)(DesiredFacing - StartFacing) < 0; return 0; }

UnknownFacingDir FacingClass::Difference() const
{
    UnknownFacingDir current = Current();
    UnknownFacingDir desired = *(UnknownFacingDir const *)&DesiredFacing;
    return UnknownFacingDir((unsigned short)(desired.Raw - current.Raw));
}

UnknownFacingDir FacingClass::Difference(UnknownFacingDir const &dir) const
{
    return UnknownFacingDir((unsigned short)(dir.Raw - Current().Raw));
}

void FacingClass::Set_ROT(int rate) { if (rate >= 127) rate = 127; ROT = (unsigned short)((unsigned char)rate << 8); }
