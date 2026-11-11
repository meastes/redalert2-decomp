// NON-EXACT DRAFT 0x6A8560 Tactical::Draw_Tiles(Cell const &, Rect const &)
// entry 0x6A8560; exact-match score 63.04 (misses_index best); plan "draft, verify"
// origin lane: Scout876 (build/class-work/Scout876/w/z1.cpp; Scout872 r1 form)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1138 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1138): Has_Extra_Data reads the image's cell count and record through the
// header fields and ra2port::IsoTileRef (the draft's private helpers); changed lines are marked NATIVE.
// Its own unit (host_source null; Draw_Tiles 0x006A7BA0's unit sees a stand-in IsometricTileTypeClass), so
// 0x006A7BA0's Project_Tile_Left inline is repeated here (NATIVE). Until M17 the Cell overload
// was linked as an alias of the Rect overload (address_aliases.py keyed overloads by name only), so the
// cells a reveal queued (Tactical +0xE0/+0xE4) were drawn as empty areas: black terrain after reveals.
#define CELL_DECLARE_DRAW_IT
#include "shared/tactical.hpp"
#include "shared/cell.hpp"
#include "shared/display.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Draw_It@CellClass@@QAEXABUPoint2D@@ABVRect@@H@Z=00475680

static inline bool Has_Extra_Data(CellClass const *cellptr)
{
    int subtile = (unsigned char)cellptr->unknown_11a;
    int ittype = cellptr->IsoTileIndex;
    if (ittype == 0xFFFF || ittype == 0xFF) {
        return false;
    }
    IsometricTileTypeClass *type = IsoTileTypes[ittype];
    if (type == 0) {
        return false;
    }
    IsoTileImageStruct *set = (IsoTileImageStruct *)type->unknown_slot39();
    if (set == 0) {
        return false;
    }
    int last = set->unknown_00[1] * set->unknown_00[0] - 1;     // NATIVE: the image's cell count (header +0x00 x +0x04)
    if (subtile >= last) {
        subtile = last;
    }
    IsoTileCellStruct const *record = (IsoTileCellStruct const *)set->Tiles[subtile];   // NATIVE: ra2port::IsoTileRef
    if (record == 0 || !record->IsHasExtraData) {
        return false;
    }
    return true;
}

static inline void Project_Tile_Left(Tactical &view, CoordStruct const &coord, Point2D &pixel)   // NATIVE: 0x006A7BA0's inline
{
    view.Coord_To_Pixel(coord, pixel);
    pixel.X -= 60 / 2;
}

static inline CoordStruct Tile_Center(Cell const &cell)
{
    CoordStruct center;
    center.X = cell.X * 256 + 256 / 2;
    center.Y = cell.Y * 256 + 256 / 2;
    center.Z = 0;
    return center;
}

void Tactical::Draw_Tiles(Cell const &cell, Rect const &cliprect)
{
    CellClass *basecell = &Map[cell];

    CoordStruct coord = Coord_Snap(Tile_Center(cell));
    Point2D screen;
    Coord_To_Pixel(coord, screen);
    Point2D pixel;
    if (basecell->unknown_11b < 2) {
        pixel.X = screen.X - 30;
        pixel.Y = screen.Y;
        basecell->Draw_It(pixel, unknown_rect_839630, 0);
    }

    int x = cell.X;
    int y = cell.Y;
    for (int level = 0; level < 24; level += 2) {
        for (int col = 0; col < 3; col++) {
            int cx = x + ((col + 1) >> 1);
            int cy = y - (col >> 1) + 1;
            if (Map.In_Radar(Cell(cx, cy))) {
                CellClass *cellptr = &Map[Cell(cx, cy)];
                bool overlap = false;
                bool draw = false;
                int height = cellptr->unknown_11b;

                if (col != 1) {
                    if (height == level + 1) {
                        draw = true;
                        overlap = true;
                    }
                } else if (height > level && height < level + 4) {
                    draw = true;
                    overlap = true;
                }

                if (cellptr->unknown_11d > 0 && !draw) {
                    int diff = height - level;
                    if (col != 1) {
                        if (diff < 0 && cellptr->unknown_11d + diff >= 0) {
                            draw = true;
                        } else {
                            draw = overlap;
                        }
                    } else if (diff + 1 > 0) {
                        draw = overlap;
                    } else {
                        draw = true;
                        if (cellptr->unknown_11d + diff + 1 <= 0) {
                            draw = overlap;
                        }
                    }
                }

                if (height == level && !draw) {
                    if (!Has_Extra_Data(cellptr)) {
                        continue;
                    }
                } else if (!draw) {
                    continue;
                }

                Cell above(cx, cy);
                coord = Coord_Snap(Tile_Center(above));
                Point2D tilepixel;
                Project_Tile_Left(*this, coord, tilepixel);
                Map[above].Draw_It(tilepixel, unknown_rect_839630, 0);
            }
        }
        x++;
        y++;
    }
}
