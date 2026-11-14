// wdthistory.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdthistory.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x00728C60: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00728C60-0x00728E41); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x00728C80, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728CB0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728CD0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728CF0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728D10, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728D30, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728D50, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728D90, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728DC0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728DF0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728E20, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.


// Dynamic initializer (0x00728E30, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00728c60.cpp; no source of its own.




#include "shared/world_domination_tour.hpp"


// rc_ptr<History> __fastcall WorldDominationTour::History::Create(rc_ptr<WDTState> const &wdt_state) (0x00728E50)
// A new history of the wdt_state, returned counted.

namespace WorldDominationTour {

rc_ptr<History> __fastcall History::Create(rc_ptr<WDTState> const &wdt_state)
{
    return rc_ptr<History>(new History(wdt_state));
}

}


// WorldDominationTour::History::History(rc_ptr<WDTState> const &wdt_state) (0x00728EC0)

namespace WorldDominationTour {

History::History(rc_ptr<WDTState> const &wdt_state) :
    TheWDTState(wdt_state)
{
}

}


// rc_ptr<State> WorldDominationTour::History::Get_State(int index) const (0x00728F00)
// The state of entry index of the wdt_state (State::Create).

namespace WorldDominationTour {

rc_ptr<State> History::Get_State(int index) const
{
    return State::Create(TheWDTState, index);
}

}


// int WorldDominationTour::History::Is_Valid(unsigned index) const (0x00728F20)
// Whether index names one of the wdt_state's conflicts.

namespace WorldDominationTour {

int History::Is_Valid(unsigned index) const
{
    if (TheWDTState.Get() && index < TheWDTState->ConflictCount) {
        return true;
    }
    return false;
}

}


// WorldDominationTour::History::~History() (0x00728F40)

namespace WorldDominationTour {

History::~History()
{
}

}


// WorldDominationTour::History scalar deleting destructor (0x00728F60):
// compiler generated with the unit's vtable; no source of its own.
