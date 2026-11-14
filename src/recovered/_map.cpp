// _map.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_map.json carries the addresses and the rows that
// stay in their own files.


#define RA2_MAP_GLOBAL
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/mouse_class.hpp"


// The unit defines Map after its header statics (Map_0040d060.cpp): MouseClass's user-declared
// destructor and the strips' StageClass (shared/mouse_class.hpp, shared/sidebar.hpp).
// Header-static dynamic initializers of the unit at 0x0040CE70: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0040CE70-0x0040D051); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0040CE90, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CEC0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CEE0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CF00, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CF20, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CF40, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CF60, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CFA0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040CFD0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040D000, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040D030, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// Dynamic initializer (0x0040D040, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0040ce70.cpp; no source of its own.


// MouseClass Map (0x008324E0): the game's screen object, defined in the unit at 0x0040CE70 after
// its header statics (ra2-decomp's _map.cpp `MouseClass Map;`, the YR layout twin). The
// definition emits the dynamic initializer (0x0040D060: the out-of-line MouseClass constructor
// and the atexit registration) and the atexit destructor (0x0040D080: ~MouseClass inline, with
// the sidebar strips' member destructors, up to the out-of-line ~RadarClass); MouseClass's
// vtable brings the deleting destructor (0x0040D160).

MouseClass Map;


// Atexit destructor of Map (0x0040D080, _$E): emitted by the definition in
// Map_0040d060.cpp; no source of its own.


class Leaf_40d100 { public: unsigned int get(unsigned int unused_argument); };
unsigned int Leaf_40d100::get(unsigned int unused_argument) { unsigned int unused_this_address = (unsigned int)this; unsigned int return_value; unsigned int unused_value; return_value = 1; return return_value; }


class Leaf_40d110 { public: unsigned int get(unsigned int unused_argument); };
unsigned int Leaf_40d110::get(unsigned int unused_argument) { unsigned int unused_this_address = (unsigned int)this; unsigned int return_value; unsigned int unused_value; return_value = 1; return return_value; }


// 0x0040d130: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 5 sites (data).
// Ghidra prototype: void FUN_0040d130(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0040d130 {

void  FUN_0040d130(void)
{
    return;
}

}  // namespace seed_0040d130


// ScrollClass::Init_IO (0x0040D140)
// Inline virtual (forwards to the base class) emitted with the ScrollClass vtable.
// No source of its own: emitted by this unit (population-2484).
