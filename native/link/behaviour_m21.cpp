// Behaviour-written bodies for native M21 (Scout1170: super weapons by
// scripted play, RA2_NATIVE_TRAP_CONTINUE=1). NOT EXACT: each follows its
// pass-6a decompile against the shared headers' members.
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS   // Houses.Count() in the house token helpers
#include "shared/radsite.hpp"
#include "shared/cell_class.hpp"

// RadSiteCell is radsite.hpp's view of a CellClass (callers cast Map[cell]).
// 0x0047CE40: the cell's radiation site (+0xF8). Reached by the nuke's warhead.
RadSiteClass *RadSiteCell::Unknown_0047CE40()
{
    return (RadSiteClass *)reinterpret_cast<CellClass *>(this)->unknown_f8;
}

// 0x0047CE30: sets it.
void RadSiteCell::Unknown_0047CE30(RadSiteClass *site)
{
    reinterpret_cast<CellClass *>(this)->unknown_f8 = site;
}

// 0x0047CEA0: adds to the cell's radiation level (+0xF0, a double).
void RadSiteCell::Unknown_0047CEA0(double level)
{
    reinterpret_cast<CellClass *>(this)->unknown_f0 += level;
}

// 0x00422690 AnimClass (anim.hpp): stores +0x160 (the PSI warning's house; natively
// a house token, below). Reached by the nuke's launch (0x00438B80).
#include "shared/anim.hpp"
void AnimClass::unknown_func_422690(int value)
{
    unknown_160 = value;
}

// Native helpers (not original code; port/fixups.json): a 32-bit field that VC6
// fills with a house address holds the house's Houses index + 1 instead (0 for
// none, or a house not in the list), so the reader gets the same house back.
#include "shared/house.hpp"
int Native_House_Token(HouseClass const *house)
{
    for (int index = 0; house && index < Houses.Count(); index++) {
        if (Houses[index] == house) return index + 1;
    }
    return 0;
}
HouseClass *Native_House_From_Token(int token)
{
    return token > 0 && token <= Houses.Count() ? Houses[token - 1] : 0;
}

// 0x0051FC70 (pass-6a decompile): true while the dominator lighting state
// (0x00A5193C) is 2, the twin of 0x0051FC60's state 1. Reached by LogicClass::AI
// during the lightning storm.
#include "shared/lightning_storm.hpp"
bool Unknown_0051FC70()
{
    return unknown_dominator_state_a5193c == 2;
}

// 0x0069CFB0 (pass-6a decompile): stores the byte argument in SuperClass +0x57
// (ready). house_views.hpp's UnknownSuperView is a padded view of SuperClass, so
// the body writes the real member. Reached by the linked chrono warp
// (HouseClass 0x004E68D0 after the chronosphere).
#include "shared/house_views.hpp"
void UnknownSuperView::unknown_func_69cfb0(int arg1)
{
    reinterpret_cast<SuperClass *>(this)->unknown_0x57 = (unsigned char)arg1 != 0;
}

// Reached by Allied 1's win and the score screen (Scout1170, M21 task 6).
// 0x00408450 AudioSource::Finish (audio.hpp; pass-6a decompile): does nothing.
#include "shared/audio.hpp"
void AudioSource::Finish()
{
}

// 0x0041B3E0 TechnoClass slot 205 (techno.hpp; pass-6a decompile): does nothing.
#include "shared/techno.hpp"
void TechnoClass::unknown_slot205()
{
}

// 0x0069A640 (unknown_globals.hpp; pass-6a decompile): the score screen's empty
// constructor (returns this).
#include "shared/unknown_globals.hpp"
UnknownScore_0069A640::UnknownScore_0069A640()
{
}

// Rect(RectPoint2D const &, int, int): the out-of-line copy of rect.hpp's inline
// constructor (the body rect.hpp gives it under RA2_RECT_POINT_INLINE).
#include "shared/rect.hpp"
Rect::Rect(RectPoint2D const &point, int width, int height) : X(point.X), Y(point.Y), Width(width), Height(height)
{
}
