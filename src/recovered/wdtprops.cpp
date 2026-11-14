// wdtprops.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtprops.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/world_domination_tour.hpp"
#include "shared/string_table.hpp"


// Header-static dynamic initializers of the unit at 0x0072CF90: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0072CF90-0x0072D171); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0072CFB0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072CFE0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D000, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D020, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D040, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D060, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D080, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D0C0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D0F0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D120, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D150, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// Dynamic initializer (0x0072D160, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0072cf90.cpp; no source of its own.


// rc_ptr<CampaignProperties> __fastcall WorldDominationTour::CampaignProperties::Create(rc_ptr<WDTState> const &wdt_state) (0x0072D180)

namespace WorldDominationTour {

rc_ptr<CampaignProperties> __fastcall CampaignProperties::Create(rc_ptr<WDTState> const &wdt_state)
{
    return rc_ptr<CampaignProperties>(new CampaignProperties(wdt_state));
}

}


// WorldDominationTour::CampaignProperties::~CampaignProperties() (0x0072D1C0)

namespace WorldDominationTour {

CampaignProperties::~CampaignProperties()
{
}

}


// WorldDominationTour::CampaignProperties::CampaignProperties(rc_ptr<WDTState> const &wdt_state) (0x0072D1F0)
// The short and long descriptions are the WDTState labels, or the (quoted) WDT:Default... labels without a state.
namespace WorldDominationTour {
CampaignProperties::CampaignProperties(rc_ptr<WDTState> const &wdt_state) :
    TheWDTState(wdt_state),
    ShortDescription(wdt_state.Get() ? StringTable::Load_String(wdt_state->ShortLabel, 0, "D:\\Ra2\\WDTProps.cpp", 20) : StringTable::Load_String("\"WDT:DefaultShortDescription\"", 0, "D:\\Ra2\\WDTProps.cpp", 20)),
    LongDescription(wdt_state.Get() ? StringTable::Load_String(wdt_state->LongLabel, 0, "D:\\Ra2\\WDTProps.cpp", 21) : StringTable::Load_String("\"WDT:DefaultLongDescription\"", 0, "D:\\Ra2\\WDTProps.cpp", 21))
{
}
}


// int WorldDominationTour::CampaignProperties::unknown_18() const (0x0072D290)

namespace WorldDominationTour {

int CampaignProperties::unknown_18() const
{
    if (TheWDTState.Get()) {
        return TheWDTState->unknown_18;
    }
    return 0;
}

}


// int WorldDominationTour::CampaignProperties::unknown_72d2a0() const (0x0072D2A0): 1.

namespace WorldDominationTour {

int CampaignProperties::unknown_72d2a0() const
{
    return 1;
}

}


// int WorldDominationTour::CampaignProperties::unknown_14() const (0x0072D2B0)

namespace WorldDominationTour {

int CampaignProperties::unknown_14() const
{
    if (TheWDTState.Get()) {
        return TheWDTState->unknown_14;
    }
    return 1;
}

}


// int WorldDominationTour::CampaignProperties::Last_Conflict() const (0x0072D2D0)

namespace WorldDominationTour {

int CampaignProperties::Last_Conflict() const
{
    if (TheWDTState.Get()) {
        return TheWDTState->ConflictCount - 1;
    }
    return 0;
}

}


// Wstring WorldDominationTour::CampaignProperties::unknown_10() const (0x0072D2E0)

namespace WorldDominationTour {

Wstring CampaignProperties::unknown_10() const
{
    return Wstring(TheWDTState.Get() ? TheWDTState->unknown_10 : "");
}

}


// WorldDominationTour::CampaignProperties scalar deleting destructor
// (0x0072D320): compiler generated with the unit's vtable; no source of its
// own.
