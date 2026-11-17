// ??0CaptureManagerClass@@QAE@PAVTechnoClass@@@Z (0x00467DB0)
// Manager for owner; listed in the unit list.
#include "shared/capture_manager.hpp"
#include "shared/techno.hpp"

CaptureManagerClass::CaptureManagerClass(TechnoClass *owner)
{
    Unit = 0;
    unknown_28 = 0;
    Owner = owner;
    unknown_CaptureManagerClass_list_008505a0.Add(this);
}
