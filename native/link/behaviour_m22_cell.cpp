// Behaviour-written bodies for native M22 (Scout1177): cell leaves the trap
// census and the coverage ranking reached. NOT EXACT: each follows its pass-6a
// decompile against cell_class.hpp's members.
#define RA2_CELL_MAP_VIEW          // unknown_47c900
#include "shared/techno.hpp"
#include "shared/cell_class.hpp"

namespace {
// CellClass +0xAC read as 24 shorts (the per-house disguise sensor counts; the
// default layout declares it int[12]).
short *Disguise_Sensors(CellClass *cell)
{
    return reinterpret_cast<short *>(&cell->unknown_ac[0]);
}
}

// UnknownCellClass is techno.hpp's view of a CellClass.
// 0x0047C360: one more sensor of the house covers the cell (+0x7C counts).
// Reached by FootClass slot 285 (a sensor unit's move; Soviet campaign 1).
void UnknownCellClass::Unknown_0047C360(int house)
{
    reinterpret_cast<CellClass *>(this)->unknown_7c[house]++;
}

// 0x0047C370: one fewer (FootClass slot 286).
void UnknownCellClass::Unknown_0047C370(int house)
{
    reinterpret_cast<CellClass *>(this)->unknown_7c[house]--;
}

// 0x0047C380 / 0x0047C390: the same for the disguise sensors (+0xAC; BuildingClass slots 290, 291).
void UnknownCellClass::Unknown_0047C380(int house)
{
    Disguise_Sensors(reinterpret_cast<CellClass *>(this))[house]++;
}

void UnknownCellClass::Unknown_0047C390(int house)
{
    Disguise_Sensors(reinterpret_cast<CellClass *>(this))[house]--;
}

// 0x0047C300: whether a disguise sensor of the house covers the cell.
bool UnknownCellClass::unknown_47c300(int house)
{
    return Disguise_Sensors(reinterpret_cast<CellClass *>(this))[house] > 0;
}

bool CellClass::unknown_47c300(int house) const
{
    return Disguise_Sensors(const_cast<CellClass *>(this))[house] > 0;
}

// 0x0047C900 (pass-6a decompile): maps the cell (IsMapped, IsVisible: bits 3
// and 4 of +0x12C) and, while the shroud counter (+0x130) is positive, flags it
// to be shrouded (IsToShroud, bit 5 of +0x140).
void CellClass::unknown_47c900()
{
    unknown_12c |= 0x18;
    if (unknown_130 > 0) {
        unknown_140 |= 0x20;
    }
}

// 0x0047A120 (pass-6a decompile): always true (UnitClass slot 27).
bool CellClass::unknown_47a120(TechnoClass *techno) const
{
    (void)techno;
    return true;
}
