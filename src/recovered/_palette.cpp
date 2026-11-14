// _palette.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/_palette.json carries the addresses and the rows that
// stay in their own files.


#define RA2_PALETTE_CONSTRUCTORS
#include "shared/convert.hpp"
#include "shared/pkey.hpp"
#include "shared/random2.hpp"
#include "shared/rect.hpp"


// Global palettes, the mixfile key, a random generator and screen rectangles defined in the unit
// at 0x0040CE70 after the Map global and its lists (.CRT$XCU 0x007C9170-0x007C91A4). The first
// nine initializers run PaletteClass's constructors (the inline default one zeroes 256 RGB
// entries; black 0x00837510 and white 0x00838410 fill through 0x00602FD0).

PaletteClass unknown_palette_838710;
PaletteClass unknown_palette_837e10;
PaletteClass unknown_palette_838a10;
PaletteClass unknown_palette_837510(RGBClass(0, 0, 0));
PaletteClass unknown_palette_838410(RGBClass(255, 255, 255));
PaletteClass unknown_palette_838110;
PaletteClass unknown_palette_837b10;
PaletteClass unknown_palette_838d10;
PaletteClass unknown_palette_837810;
// The key the mixfiles are opened with (0x00839010).
PKey FastKey;
// A random number generator seeded with 0 (0x00839218).
Random2Class unknown_random_839218(0);
// An empty rectangle (0x00839610).
Rect unknown_rect_839610(0, 0, 0, 0);
// The tactical area: X, Y, width, height (0x00839630).
Rect unknown_rect_839630(0, 0, 640, 400);
// A screen-sized clipping window (0x00839640).
Rect unknown_window_839640(0, 0, 640, 400);


// Dynamic initializer of unknown_palette_837e10 (0x0040D790, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_838a10 (0x0040D7B0, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_837510 (0x0040D7D0, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_838410 (0x0040D7F0, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_838110 (0x0040D810, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_837b10 (0x0040D830, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_838d10 (0x0040D850, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_palette_837810 (0x0040D870, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of FastKey (0x0040D890, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.


// Dynamic initializer of unknown_random_839218 (0x0040D8C0, _$E): emitted by the definition in
// palette_globals_init_0040d770.cpp; no source of its own.
