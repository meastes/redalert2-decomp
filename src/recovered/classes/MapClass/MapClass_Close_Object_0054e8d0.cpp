// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// ObjectClass *MapClass::Close_Object(CoordStruct const &coord) (0x0054E8D0), from ra2-decomp
// MapClass::Close_Object (YR 0x0056C210, src/map.cpp) on Map388's draft: the clickable object
// closest to coord among the occupiers of its cell and the eight around it, skipping a techno
// cloaked (+0x1B8 2) unless the player owns it (+0x372); a building counts by its cell centre and
// only within 0xB5 leptons. Null unless the closest lies within 0xB5. The twin's trailing
// `Cell unused = cell + _offsets[0];` is kept (twin-shaped; its inline Cell sum is the pending
// inline site the original's budget shows).
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_techno.hpp"
// @ref _atexit=0078298a
// @ref _?$S63@?1??Close_Object@MapClass@@QAEPAVObjectClass@@ABUCoordStruct@@@Z@4EA=00a6f9f0
// @ref _?_offsets@?1??Close_Object@MapClass@@QAEPAVObjectClass@@ABUCoordStruct@@@Z@4PAVCell@@A=00a6f1ec
// @ref _$E64=0054ebc0
// @ref ??GCoordStruct@@QBE?AU0@ABU0@@Z=004348f0
// @ref ??0CoordStruct@@QAE@HHH@Z=0041b630
// @ref ?Length@CoordStruct@@QBEHXZ=0041b780

// The distance between two coordinates (TibSun Distance): the length (0x0041B780) of their
// difference.
inline int Distance(CoordStruct const &coord1, CoordStruct const &coord2)
{
    return (coord1 - coord2).Length();
}

ObjectClass *MapClass::Close_Object(CoordStruct const &coord)
{
    TechnoClass *object = 0;
    int distance = 0;
    Cell cell = Cell(coord);

    static Cell _offsets[] = {Cell(0, 0), Cell(-1, 0), Cell(1, 0), Cell(0, -1), Cell(0, 1), Cell(-1, 1), Cell(1, 1), Cell(1, -1), Cell(-1, -1)};
    for (int index = 0; index < 9; index++) {
        Cell newcell = Cell(cell + _offsets[index]);
        if (In_Radar(newcell)) {
            TechnoClass *o = (TechnoClass *)(*this)[newcell].Cell_Occupier();
            while (o != 0) {
                if (!o->AbstractFlags.IsTechno || o->unknown_372 || o->unknown_1b8 != 2) {
                    int d = -1;
                    if (o->Kind_Of() == 6) {
                        d = Distance(coord, CoordStruct(newcell.X * 256 + 128, newcell.Y * 256 + 128, 0));
                        if (d > 0xB5) d = -1;
                    } else {
                        d = Distance(coord, o->Center_Coord());
                    }
                    if (d >= 0 && (!object || d < distance)) {
                        distance = d;
                        object = o;
                    }
                }
                o = (TechnoClass *)o->unknown_30;
            }
        }
    }

    if (object && distance > 0xB5) {
        object = 0;
    }
    Cell unused = cell + _offsets[0];
    return (ObjectClass *)object;
}
