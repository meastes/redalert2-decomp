// bool OverlayTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x005DC870)
// Places an overlay of this type at the cell (no house).
#include "shared/overlay_type.hpp"
#include "shared/overlay.hpp"
// @ref ??2@YAPAXI@Z=00782017
// @ref ??0OverlayClass@@QAE@PAVOverlayTypeClass@@ABVCell@@H@Z=005da7d0

bool OverlayTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    if (new OverlayClass((OverlayTypeClass *)this, cell, -1) != 0) {
        return true;
    }
    return false;
}
