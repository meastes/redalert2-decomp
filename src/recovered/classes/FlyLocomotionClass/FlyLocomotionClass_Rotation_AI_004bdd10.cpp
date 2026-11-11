// @ref ??AMapClass@@QAEAAVCellClass@@ABUCoordStruct@@@Z=00548000
// @ref ?Current@FacingClass@@QBE?AUUnknownFacingDir@@XZ=004b8be0
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Set@FacingClass@@QAE_NABUUnknownFacingDir@@@Z=004b8b10
// @ref ?Trigger_Veins@CellClass@@QAEXXZ=0047bb30
#include "shared/fly_locomotion.hpp"
#include "shared/cell.hpp"
extern "C" int __cdecl abs(int);
inline UnknownFacingDir Fly_Add_Rotation_004bdd10(UnknownFacingDir const &direction, int rotation)
{
    return UnknownFacingDir((unsigned short)(direction.Raw+((unsigned int)rotation<<8)));
}
void FlyLocomotionClass::Rotation_AI()
{
    if (unknown_53 && LinkedTo->Strength>0) {
        if (!Is_Powered()) {
            UnknownFacingDir current=LinkedTo->unknown_2e0.Current();
            UnknownFacingDir direction=Fly_Add_Rotation_004bdd10(current,unknown_54);
            LinkedTo->unknown_2e0.Set(direction);
            int step=abs(unknown_54)<1?abs(unknown_54):1;
            if (unknown_54<0) unknown_54+=step;
            else unknown_54-=step;
        } else {
            unknown_53=false;
            Map[Object_Coord(LinkedTo)].Trigger_Veins();
        }
    }
}
