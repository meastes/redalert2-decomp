// bool TechnoClass::unknown_slot155() (0x006C12D0, slot 155)
// True for a current mission other than 0, 6 and 16 unless the type sets +0xAA0.
#include "shared/techno.hpp"

bool TechnoClass::unknown_slot155()
{
    if (Get_Mission() && Get_Mission() != 6 && Get_Mission() != 16 && !Techno_Type_Class()->IsTrain) {
        return true;
    }
    return false;
}
