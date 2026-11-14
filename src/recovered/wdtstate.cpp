// wdtstate.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtstate.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/world_domination_tour.hpp"


// Header-static dynamic initializers of the unit at 0x00730510: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00730510-0x007306F1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00730530, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x00730560, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x00730580, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x007305A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x007305C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x007305E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x00730600, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x00730640, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x00730670, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x007306A0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x007306D0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// Dynamic initializer (0x007306E0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00730510.cpp; no source of its own.


// rc_ptr<State> __fastcall WorldDominationTour::State::Create(rc_ptr<WDTState> const &wdt_state, int index) (0x00730700)

namespace WorldDominationTour {

rc_ptr<State> __fastcall State::Create(rc_ptr<WDTState> const &wdt_state, int index)
{
    return rc_ptr<State>(new State(wdt_state, index));
}

}


// rc_ptr<State> __fastcall WorldDominationTour::State::Create() (0x00730790)

namespace WorldDominationTour {

rc_ptr<State> __fastcall State::Create()
{
    return rc_ptr<State>(new State());
}

}


// WorldDominationTour::State::State() (0x007307F0)

namespace WorldDominationTour {

State::State() :
    Owners(0)
{
}

}


// WorldDominationTour::State::State(rc_ptr<WDTState> const &wdt_state, unsigned index) (0x00730830)
// Takes the owner array of conflict index of the wdt_state (none when the
// index is out of range).

namespace WorldDominationTour {

State::State(rc_ptr<WDTState> const &wdt_state, unsigned index) :
    TheWDTState(wdt_state),
    Owners(0)
{
    // Through Get(): rc_ptr::operator-> (with its null test) would not fold
    // the second read (0x00730830 and its copy in Create 0x00730700).
    if (TheWDTState.Get() && index < TheWDTState.Get()->ConflictCount) {
        Owners = (unsigned char *)TheWDTState.Get()->unknown_44[index];
    }
}

}


// int WorldDominationTour::State::Owner_Of(unsigned territory) const (0x00730890)
// The owner the state records for a territory: 1-3 for the bytes 0-2, 0 for
// anything else, an index out of range or a state without data.

namespace WorldDominationTour {

int State::Owner_Of(unsigned territory) const
{
    if (TheWDTState.Get() != 0 && Owners != 0 && territory < TheWDTState->unknown_20) {
        switch (Owners[territory]) {
            case 0:
                return 1;
            case 1:
                return 2;
            case 2:
                return 3;
        }
    }
    return 0;
}

}


// WorldDominationTour::State::~State() (0x007308E0)

namespace WorldDominationTour {

State::~State()
{
}

}


// int WorldDominationTour::State::Count_Owned(int owner) const (0x00730900)
// How many territories the state gives owner (2 for the byte 1, 3 for the
// byte 2; other owners count nothing).

namespace WorldDominationTour {

int State::Count_Owned(int owner) const
{
    if (TheWDTState.Get() != 0 && Owners != 0) {
        int count = 0;
        unsigned char const *owners = Owners;
        unsigned i = TheWDTState->unknown_20;
        while (i--) {
            switch (*owners++) {
                case 1:
                    if (owner == 2) {
                        count++;
                    }
                    break;
                case 2:
                    if (owner == 3) {
                        count++;
                    }
                    break;
            }
        }
        return count;
    }
    return 0;
}

}


// WorldDominationTour::State scalar deleting destructor (0x00730950):
// compiler generated with the unit's vtable; no source of its own.
