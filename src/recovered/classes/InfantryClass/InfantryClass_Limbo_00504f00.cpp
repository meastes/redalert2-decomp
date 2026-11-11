// bool InfantryClass::Limbo() (0x00504F00, slot 52)
// Stops the locomotor's animation, resets the infantry state (+0x5D0 2,
// +0x5C3, Sequence 0) and continues with FootClass's slot 52.
#include "shared/infantry.hpp"
// @ref ?_com_issue_error@@YGXJ@Z=00795940
// @ref ?Limbo@FootClass@@UAE_NXZ=004c9bd0

bool InfantryClass::Limbo()
{
    Locomotor.Checked()->Stop_Movement_Animation();
    unknown_5d0 = 2;
    unknown_5c3 = false;
    Sequence = 0;
    return FootClass::Limbo();
}
