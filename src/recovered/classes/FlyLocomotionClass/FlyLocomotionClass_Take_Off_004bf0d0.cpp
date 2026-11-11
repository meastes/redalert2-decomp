// @ref ?Set@FacingClass@@QAE_NABUUnknownFacingDir@@@Z=004b8b10
// @ref ?Unknown_004131E0@UnknownTracker839EA0@@QAEXPAVFootClass@@@Z=004131e0
// @ref ?Voc_Play_At@@YIPAUAudioEvent@@HABUCoordStruct@@PAUAudioEventHandle@@@Z=00713e60
// @ref ?Desired@FacingClass@@QBE?AUUnknownFacingDir@@XZ=004b8c80
// @ref ?unknown_tracker_839ea0@@3UUnknownTracker839EA0@@A=00839ea0
// @ref _unknown_geometry_k=00865dd8
#include "shared/fly_locomotion.hpp"
#include "shared/techno_type.hpp"
#include "shared/sound_play.hpp"
void FlyLocomotionClass::Take_Off_004bf0d0()
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112()) {
        unknown_51=false;
        unknown_50=true;
        if (LinkedTo->unknown_slot178()==unknown_geometry_k)
            unknown_tracker_839ea0.Unknown_004131E0(LinkedTo);
        unknown_38=LinkedTo->Techno_Type_Class()->Flight_Level();
        if (LinkedTo->Get_Height_AGL()==0) {
            LinkedTo->unknown_2e0.Set(LinkedTo->unknown_2f8.Desired());
        }
        Voc_Play_At(LinkedTo->Techno_Type_Class()->AuxSound1,Object_Coord(LinkedTo),0);
    }
}
