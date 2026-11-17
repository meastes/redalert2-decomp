// Dynamic initializer of the global CDList (0x004D4920, _$E): constructs it
// (0x004D2940) and registers its atexit destructor (0x004D4940, the following
// _$E draft, emitted by this definition; it calls ~GetCDClass 0x004D2A10).
#include "shared/get_cd.hpp"

GetCDClass CDList;
