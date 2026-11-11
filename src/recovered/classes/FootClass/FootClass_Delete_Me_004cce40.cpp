// void FootClass::Delete_Me() (0x004CCE40)
// The unit is removed: the owned object at +0x230 is told (0x004681E0),
// the unit leaves its team, then ObjectClass's slot 60.
#include "shared/foot.hpp"
#include "shared/capture_manager.hpp"
// @ref ?Release_4681e0@CaptureManagerClass@@QAEXXZ=004681e0
// @ref ?Unknown_006B8FB0@TeamClass@@QAE_NPAVFootClass@@H_N@Z=006b8fb0
// @ref ?Delete_Me@ObjectClass@@UAEXXZ=005d4e90

void FootClass::Delete_Me()
{
    if (unknown_230 != 0) {
        ((CaptureManagerClass *)unknown_230)->Release_4681e0();
    }
    if (Team != 0) {
        Team->Unknown_006B8FB0(this, -1, 0);
    }
    TechnoClass::Delete_Me();
}
