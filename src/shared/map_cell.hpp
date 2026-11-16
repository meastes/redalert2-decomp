// CellClass as the Map units see it: the real class (shared/cell_class.hpp)
// with the Map units' declarations of the members they call (RA2_CELL_MAP_VIEW:
// the manglings their accepted rows bind). Units that include the real
// shared/cell.hpp first see the class without them.
#ifndef RA2_SHARED_MAP_CELL_HPP
#define RA2_SHARED_MAP_CELL_HPP

#include "shared/coord.hpp"
#include "shared/rect.hpp"

#define RA2_CELL_MAP_VIEW
#include "shared/cell_class.hpp"

#endif
