// void HouseClass::unknown_func_4e7150() (0x004E7150)
// Deletes the house's technos, the triggers of its house type and the
// house itself.
#include "shared/techno.hpp"
#include "shared/trigger.hpp"

void HouseClass::unknown_func_4e7150()
{
    for (int index = 0; index < Technos.Count(); index++) {
        if (Technos[index]->House == this) {
            delete Technos[index];
            index--;
        }
    }
    for (int i = 0; i < Triggers.Count(); i++) {
        if ((AbstractTypeClass *)Triggers[i]->Class->unknown_a4 == Type) {
            delete Triggers[i];
            i--;
        }
    }
    delete this;
}
