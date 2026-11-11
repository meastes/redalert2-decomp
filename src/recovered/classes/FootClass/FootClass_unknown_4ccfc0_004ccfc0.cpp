// FootClass *FootClass::unknown_4ccfc0() (0x004CCFC0)
// The first passenger leaves this transport: a Gunner type left empty runs
// slot 281, and the type's LeaveTransportSound plays at the transport.
#include "shared/foot.hpp"
// @ref ?Detach_Object@CargoClass@@QAEPAVObjectClass@@XZ=004689f0
// @ref ?Voc_Static_Add@@YIPAUAudioEvent@@HABUCoordStruct@@I@Z=00714290

FootClass *FootClass::unknown_4ccfc0()
{
    FootClass *passenger = (FootClass *)Cargo.Detach_Object();
    if (Techno_Type_Class()->Gunner && Cargo.Count == 0) {
        unknown_slot281((int)passenger);
    }
    if (passenger != 0 && Techno_Type_Class()->LeaveTransportSound != -1) {
        Voc_Static_Add(Techno_Type_Class()->LeaveTransportSound, CoordStruct(Coord), 0);
    }
    return passenger;
}
