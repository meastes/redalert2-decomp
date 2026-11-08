// Coord geometry model for the selection helpers (TibSun Coord over the map coordinate;
// hypothesis: here a CoordStruct with a forwarding (x, y, z) constructor).
#ifndef RA2_SHARED_SELECTION_COORD_HPP
#define RA2_SHARED_SELECTION_COORD_HPP
#include "shared/coord.hpp"
#ifdef RA2_COORD_TPOINT3D
class Coord : public TPoint3D<int> {
    typedef TPoint3D<int> BASECLASS;
public:
    Coord(int x, int y, int z) : BASECLASS(x, y, z) {}
    Coord(BASECLASS const &point) : BASECLASS(point) {}

};
#else
class Coord : public CoordStruct {
public:
    Coord(int x, int y, int z) : CoordStruct(x, y, z) {}
    Coord(CoordStruct const &coord) : CoordStruct(coord) {}
};
#endif
#endif
