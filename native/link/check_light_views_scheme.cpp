// Native layout check (Scout1091): anim.hpp's UnknownAB6EF8Entry view reads
// ColorScheme::Converter (port/fixups.json). Compile-time only.
#include "shared/color_scheme.hpp"
#include "shared/anim.hpp"
#include <stddef.h>
#if defined(__LP64__)
static_assert(offsetof(UnknownAB6EF8Entry, unknown_30c) == offsetof(ColorScheme, Converter),
              "UnknownAB6EF8Entry::unknown_30c is ColorScheme::Converter");
static_assert(sizeof(((UnknownAB6EF8Entry *)0)->unknown_30c) == sizeof(ConvertClass *), "pointer width");
#endif
