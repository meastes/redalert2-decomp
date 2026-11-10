// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void CellClass::Recalc_Attributes(int cell_height) (0x00472540): Recalc_Attributes.
// Adopted from ra2-decomp CellClass::Recalc_Attributes (YR 0x0047D2B0).
// Recomputes the cell's land type (+0xEC), ramp (+0x11C) and elevation (+0x11D)
// from its overlay and tile, makes a tunnel's tube, attaches the tile's
// animation, marks the cells a cliff shadows, applies the rules' cliff back
// impassability and stores the passability and height in the map's zone tables.
// Needs RA2_CELL_RECALC_ATTRIBUTES and RA2_COORD_FROM_CELL_INLINE (first row 0x00470490).
#include "shared/cell.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/tileset_index.hpp"
#include "shared/overlay.hpp"
#include "shared/overlay_type.hpp"
#include "shared/rules.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/tactical.hpp"
#include "shared/tube.hpp"
#include "shared/unknown_geometry.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?Get_Tile_Pixel_Dimensions@IsometricTileTypeClass@@QAE_NHAAH0@Z=0052b690
// @ref ?Land_Type@IsometricTileTypeClass@@QAEHH@Z=00529300
// @ref ?unknown_tileset_a52ea4@@3HA=00a52ea4
// @ref ?unknown_tileset_a52f04@@3HA=00a52f04
// @ref ?unknown_tileset_a6cb78@@3HA=00a6cb78
// @ref ?unknown_tileset_a6cf50@@3HA=00a6cf50
// @ref ?unknown_tube_facings_7d2b08@@3PAHA=007d2b08
// The tube facing of each tunnel tile (0x007D2B08, a function-local static of the original).
extern int unknown_tube_facings_7d2b08[];

void CellClass::Recalc_Attributes(int cell_height)
{
    if (this == &unknown_dummy_cell_a6f9f8) {
        return;
    }

    UnknownMapElem4 *zone = &Map.unknown_64[Map.Get_Cell_Zone_Index(MapCoords)];
    char *subzone = Map.unknown_6c + Map.Get_Cell_Zone_Index(MapCoords) * 10;

    int overlay = OverlayIndex;

    if (overlay != -1) {
        OverlayTypeClass *otype = OverlayTypes[overlay];
        unknown_ec = otype->Land;
        if (unknown_ec == 4 || unknown_ec == 9 || otype->NoUseTileLandType) {
            if (IsoTileIndex != 0xFFFF && IsoTileIndex < IsoTileTypes.Count()) {
                IsometricTileTypeClass *itype = IsoTileTypes[IsoTileIndex];
                unknown_11c = itype->unknown_func_52b6f0((unsigned char)unknown_11a);
            }
            if (unknown_11c != 0 && otype->Tiberium) {
                OverlayIndex = -1;
                OverlayData = 0;
            }
            if (Rules->CliffBackImpassability != 0) {
                if (unknown_11b + 4 <= Map[MapCoords + Cell(+0, -1)].unknown_11b
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +0)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+2, +2)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, -1)].unknown_11b)) {
                    if (Rules->CliffBackImpassability == 2) {
                        unknown_ec = 3;
                    }
                }
            }
            Fixup_LAT();
            unknown_478f40();
            zone->unknown_01 = unknown_11b;
            subzone[8] = unknown_11b;
            zone->unknown_00 = unknown_4c;
            return;
        }
    }

    if (IsoTileIndex >= IsoTileTypes.Count()) {
        IsoTileIndex = 0xFFFF;
    }

    if (IsoTileIndex != 0xFFFF) {
        IsometricTileTypeClass *itype = IsoTileTypes[IsoTileIndex];
        bool valid = itype->Is_Tile_Index_Valid((unsigned char)unknown_11a, false);

        if (unknown_11c != 0 && !valid) {
            valid = itype->Is_Tile_Index_Valid((unsigned char)unknown_11a, true);
        }

        if (!valid) {
            IsoTileIndex = 0xFFFF;
            unknown_11a = 0;
            unknown_ec = 0;
            unknown_11c = 0;
            if (Rules->CliffBackImpassability != 0) {
                if (unknown_11b + 4 <= Map[MapCoords + Cell(+0, -1)].unknown_11b
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +0)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+2, +2)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, -1)].unknown_11b)) {
                    if (Rules->CliffBackImpassability == 2 && unknown_ec == 0) {
                        unknown_ec = 3;
                    }
                }
            }
            unknown_478f40();
            zone->unknown_01 = unknown_11b;
            subzone[8] = unknown_11b;
            zone->unknown_00 = unknown_4c;
            return;
        }

        unknown_11c = itype->unknown_func_52b6f0((unsigned char)unknown_11a);
        Fixup_LAT();
        if (OverlayIndex != -1) {
            if (OverlayClass::To_Tiberium(OverlayIndex) != -1) {
                if (unknown_11c > 4) {
                    unknown_ec = itype->Land_Type((unsigned char)unknown_11a);
                    OverlayIndex = -1;
                    OverlayData = 0;
                } else if (!OverlayTypes[OverlayIndex]->Land) {
                    unknown_ec = 5;
                }
            } else if (!OverlayTypes[OverlayIndex]->NoUseTileLandType) {
                unknown_ec = itype->Land_Type((unsigned char)unknown_11a);
            }
        } else {
            unknown_ec = itype->Land_Type((unsigned char)unknown_11a);
        }

        if (unknown_ec == 10) {
            if (unknown_116 < 0 || unknown_116 >= unknown_TubeClass_list_008661c8.Count()) {
                int tunnel_dir = -1;

                if (IsoTileIndex >= unknown_tileset_a52ea4 && IsoTileIndex <= unknown_tileset_a52ea4 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a52ea4;
                } else if (IsoTileIndex >= unknown_tileset_a6cf50 && IsoTileIndex <= unknown_tileset_a6cf50 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a6cf50;
                } else if (IsoTileIndex >= unknown_tileset_a52f04 && IsoTileIndex <= unknown_tileset_a52f04 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a52f04;
                } else if (IsoTileIndex >= unknown_tileset_a6cb78 && IsoTileIndex <= unknown_tileset_a6cb78 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a6cb78;
                }

                if (tunnel_dir != -1) {
                    new TubeClass(MapCoords, unknown_tube_facings_7d2b08[tunnel_dir]);
                }
            }
        }

        if (cell_height != -1) {
            unknown_11b = cell_height;
        }

        int width;
        int height;
        itype->Get_Tile_Pixel_Dimensions((unsigned char)unknown_11a, width, height);
        unknown_11d = (height - 30) / 15;
        if (!IsAnimAttached && itype->unknown_298 != -1 && itype->AttachesTo == (unsigned char)unknown_11a) {
            CoordStruct anim_coord = CoordStruct(MapCoords, unknown_11b * unknown_geometry_g) + unknown_tactical_83996c->Unknown_006A2BF0(itype->Offset);
            AnimClass *anim = new AnimClass(AnimTypes[itype->unknown_298], anim_coord, 0, -1, 0x1600, 0);
            anim->ZAdjust = itype->ZAdjust;
            anim->unknown_176 = true;
            anim->unknown_177 = true;
            IsAnimAttached = true;
        }

        if (itype->IsShadowCaster) {
            Cell const *list = itype->Shadow_Caster_List();
            if (list) {
                Cell pos;
                while (*list != Cell(0x7FFF, 0x7FFF) && *list != unknown_geometry_k) {
                    pos = *list + MapCoords;
                    Map[pos].IsOvershadowed = true;
                    list++;
                }
            }
        }
    } else {
        if (overlay == -1 || OverlayTypes[overlay]->NoUseTileLandType) {
            unknown_ec = 0;
        } else {
            unknown_ec = OverlayTypes[overlay]->Land;
        }
        unknown_11c = 0;
    }

    if (Rules->CliffBackImpassability != 0) {
        if (unknown_11b + 4 <= Map[MapCoords + Cell(+0, -1)].unknown_11b
        || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +0)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(+2, +2)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, +1)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +1)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, -1)].unknown_11b)) {
            if (Rules->CliffBackImpassability == 2) {
                if (unknown_ec == 0 || unknown_ec == 2 || unknown_ec == 6 || unknown_ec == 8) {
                    unknown_ec = 3;
                }
            }
        }
    }

    unknown_478f40();
    zone->unknown_01 = unknown_11b;
    zone->unknown_00 = unknown_4c;
    subzone[8] = unknown_11b;
}
