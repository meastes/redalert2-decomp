#pragma once

#include "coord.h"

struct Facing_Offset : Cell { // RA2: the element type of the static neighbour tables of Where_To_Place_Building; its destructor makes the executable register each table with atexit
		Facing_Offset(Cell const & cell) : Cell(cell) {}
		~Facing_Offset() {}
};
