// Dynamic initializer of a global RandomStraw (0x004D4540, _$E): constructs
// CryptRandom (0x00A40778) and registers its atexit
// destructor (0x004D4560, the following _$E draft, emitted by this
// definition). Owning unit: the global-object block around 0x004D4500
// (likely the game's globals unit; not yet identified).
#include "shared/random_straw.hpp"

RandomStraw CryptRandom;
