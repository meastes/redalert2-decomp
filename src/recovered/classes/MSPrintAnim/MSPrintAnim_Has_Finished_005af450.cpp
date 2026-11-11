// bool MSPrintAnim::Has_Finished() (0x005AF450, slot 7): finished (as slot 4 sees it) and unknown_48 set
#include "shared/ms_anim.hpp"

bool MSPrintAnim::Has_Finished()
{
    return unknown_48 && Position > Text.Length() && unknown_28 > 2;
}
