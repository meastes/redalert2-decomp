// int FootClass::In_Which_Layer() const (0x004CA130)
// The layer: the locomotor decides (ILocomotion::Layer).
#include "shared/foot.hpp"
// @ref ?_com_issue_error@@YGXJ@Z=00795940

int FootClass::In_Which_Layer() const
{
    return Locomotor.Checked()->Layer();
}
