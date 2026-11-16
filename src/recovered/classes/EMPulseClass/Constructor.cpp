// Default EMPulse object; original004B4CC0, existing full EMPulseClass layout.
#include "shared/empulse_class.hpp"
extern "C" Cell const CELL_NONE;
// @ref _CELL_NONE=008559f0
EMPulseClass::EMPulseClass() : Position(CELL_NONE), Radius(0), CreatedFrame(0), Duration(0) {}
