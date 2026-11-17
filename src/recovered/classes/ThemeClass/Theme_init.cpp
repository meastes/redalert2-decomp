// Dynamic initializer of the global Theme (0x004D4660, _$E): constructs it
// (0x006E7C60) and registers its atexit destructor (0x004D4680, the following
// _$E draft, emitted by this definition: the implicit destructor, i.e. the
// Themes vector's, expanded in place).
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/theme.hpp"

ThemeClass Theme;
