// void AnimClass::Delete_Me() (0x00423070)
// Stops the animation's sounds: Attach_To(0), resets the first
// audio controller and, unless unknown_178 is set, plays the type's
// StopSound at the center through the second controller; then the
// ObjectClass slot.
#include "shared/anim.hpp"
// @ref ?Delete_Me@ObjectClass@@UAEXXZ=005d4e90
// @ref ?Unknown_00406070@UnknownAudioController@@QAEXXZ=00406070
// @ref ?Voc_Play_At@@YIPAUAudioEvent@@HABUCoordStruct@@PAUAudioEventHandle@@@Z=00713e60

void AnimClass::Delete_Me()
{
    Attach_To(0);
    unknown_180.Unknown_00406070();
    if (!unknown_178 && Class->StopSound != -1) {
        Voc_Play_At(Class->StopSound, Center_Coord(), &unknown_194);
    }
    ObjectClass::Delete_Me();
}
