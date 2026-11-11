// Westwood facing.cpp in one source (twins: mischa85/RedAlert2 src/facing.cpp, TibSun facing.cpp); the
// accepted bodies under the template's names, the directions as UnknownFacingDir until DirType is converted.
#include "shared/facing.hpp"

FacingClass::FacingClass() : DesiredFacing(0), StartFacing(0), ROT(0) {}

FacingClass::FacingClass(int rate) : DesiredFacing(0), StartFacing(0), ROT(0) { Set_ROT(rate); }

extern "C" int __cdecl abs(int value);
bool FacingClass::Set_Desired(UnknownFacingDir const &dir)
{
    if (DesiredFacing != (short)dir.Raw) {
    *(UnknownFacingDir *)&StartFacing = Current();
    *(UnknownFacingDir *)&DesiredFacing = dir;
    if (ROT > 0) {
        short delta = (short)(DesiredFacing - StartFacing);
        RotationTimer = CDTimerClass<FrameTimerClass>(abs(delta) / ROT);
    }
    return true;
    }
    return false;
}

bool FacingClass::Set(UnknownFacingDir const &dir)
{
    if(Current().Raw!=dir.Raw) {
        *(UnknownFacingDir *)&DesiredFacing=dir;
        *(UnknownFacingDir *)&StartFacing=dir;
        RotationTimer=CDTimerClass<FrameTimerClass>(0);
        return true;
    }
    RotationTimer=CDTimerClass<FrameTimerClass>(0);
    return false;
}


UnknownFacingDir FacingClass::Current() const
{
    if (Is_Rotating()) {
        short delta = (short)(DesiredFacing - StartFacing);
        short ticks = (short)(abs(delta) / ROT);
        UnknownFacingDir result = *(UnknownFacingDir const *)&DesiredFacing;
        if (ticks > 0)
            result.Raw = (unsigned short)(result.Raw - (delta / ticks) * (short)RotationTimer.Value());
        return result;
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
