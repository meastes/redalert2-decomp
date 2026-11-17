// TechnoTypeClass::~TechnoTypeClass() (0x006DA950)
// Frees the owned cameo shapes, leaves the object list (0x00AC0CB0) and
// TechnoTypes; the member lists and the object type part follow.
#include "shared/techno_type.hpp"
#include "shared/object.hpp"

// Frees the cameo shapes the type owns (inline).
static inline void Free_Cameo(TechnoTypeClass *type)
{
    if (type->CameoAllocated && type->CameoData) {
        delete type->CameoData;
        type->CameoData = 0;
        type->CameoAllocated = false;
    }
}
static inline void Free_Alt_Cameo(TechnoTypeClass *type)
{
    if (type->AltCameoAllocated && type->AltCameoData) {
        delete type->AltCameoData;
        type->AltCameoData = 0;
        type->AltCameoAllocated = false;
    }
}

TechnoTypeClass::~TechnoTypeClass()
{
    Free_Cameo(this);
    Free_Alt_Cameo(this);
    AbstractTypePtrTracker.Delete(this);
    TechnoTypes.Delete(this);
}
