// NON-EXACT DRAFT 0x564290 MapClass::Unknown_00564290 (Subzone_Span)
// entry 0x564290; exact-match score 42.47 (misses_index best); plan "draft, verify"
// origin lane: Scout660 (build/class-work/Scout664/w/keep/b_4247.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Low exact score; the shape follows TibSun map.cpp Subzone_Span.
// MapClass::Unknown_00564290: TibSun Subzone_Span, grows one subzone from a seed cell (0x00564290).
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_subzone.hpp"
#include "shared/crt_memory.hpp"
// @ref ?Unknown_00564290@MapClass@@QAEHPAUCellSubzoneStruct@@HIABVRect@@ABVCell@@@Z=00564290

inline int Span_Pack32(int zone1, int zone2)
{
    return (zone1 << 16) | zone2;
}

inline int Span_Pack8(int zone1, int zone2)
{
    return ((zone1 & 15) << 4) | (zone2 & 15);
}

int MapClass::Unknown_00564290(CellSubzoneStruct *seed, int level, unsigned int id, Rect const &bounds, Cell const &cell)
{
    int zone = seed->ZoneID;
    CellSubzoneStruct *begin = seed;
    CellSubzoneStruct *end = seed;
    HashObject<unsigned int, SubzoneConnectionStruct> entry;
    int x = cell.X;
    int y = cell.Y;
    int xmin = bounds.X;
    int ymin = bounds.Y;
    int last_adjacent = -1;
    int begin_x = x;
    int xmax = bounds.Width + bounds.X - 1;
    int end_x = x;
    int ymax = bounds.Height + bounds.Y - 1;
    HashTableClass<unsigned int, SubzoneConnectionStruct> *table = unknown_7c[level];
    int prev_height = seed->Height;

    while (true) {
        if (x < xmin) {
            break;
        }
        if (abs(begin->Height - prev_height) >= 2) {
            break;
        }
        begin->SubzoneID[level] = (short)id;
        prev_height = begin->Height;
        int prev_zone = begin[-1].ZoneID;
        begin--;
        x--;
        if (prev_zone != zone) {
            break;
        }
    }

    begin_x = x;
    int adjacent = begin->SubzoneID[level];
    if (adjacent > 0 && abs(begin->Height - prev_height) < 2 && adjacent != last_adjacent) {
        Cell probe;
        probe = Cell(x, y);
        if (Unknown_0055A660(probe, true)) {
            probe = Cell(x + 1, y);
            if (Unknown_0055A660(probe, true)) {
                int packed_pair = Span_Pack32(adjacent, (unsigned short)id);
                entry.Value.unknown_04 = false;
                entry.Key = entry.Value.unknown_00 = packed_pair;
                table->Add_Object(Span_Pack8(adjacent, (unsigned short)id), entry);
                last_adjacent = adjacent;
            }
        }
    }

    prev_height = seed->Height;
    while (end->ZoneID == zone) {
        if (end_x > xmax) {
            break;
        }
        if (abs(end->Height - prev_height) >= 2) {
            break;
        }
        end->SubzoneID[level] = (short)id;
        prev_height = end->Height;
        end++;
        end_x++;
    }

    adjacent = end->SubzoneID[level];
    if (adjacent > 0 && abs(end->Height - prev_height) < 2 && adjacent != last_adjacent) {
        Cell probe;
        probe.X = (short)end_x;
        probe.Y = (short)y;
        if (Unknown_0055A660(probe, true)) {
            probe = Cell(end_x - 1, y);
            if (Unknown_0055A660(probe, true)) {
                int packed_pair = Span_Pack32(adjacent, (unsigned short)id);
                entry.Value.unknown_04 = false;
                entry.Key = entry.Value.unknown_00 = packed_pair;
                table->Add_Object(Span_Pack8(adjacent, (unsigned short)id), entry);
                last_adjacent = adjacent;
            }
        }
    }

    int skip = end - seed - 1;
    int stride = MapSize.Width + MapSize.Height + 1;

    CellSubzoneStruct *above = &begin[-stride];
    CellSubzoneStruct *below = &begin[stride];
    x = begin_x;

    Cell span_cell;
    span_cell.X = 0;
    span_cell.Y = 0;

    while (x <= end_x) {
        int shadow_subzone = above->SubzoneID[level];
        CellSubzoneStruct *span_ptr;
        if (x == begin_x) {
            Cell left;
            left.Y = (short)y;
            left.X = (short)(x + 1);
            span_cell = left;
            span_ptr = &above[stride + 1];
        } else if (x < end_x) {
            Cell middle;
            middle.X = (short)x;
            middle.Y = (short)y;
            span_cell = middle;
            span_ptr = &above[stride];
        } else {
            Cell right;
            right.X = (short)(x - 1);
            right.Y = (short)y;
            span_cell = right;
            span_ptr = &above[stride - 1];
        }

        if (shadow_subzone != 0 || y <= ymin || x < xmin || x > xmax) {
            if (shadow_subzone != (unsigned short)id && shadow_subzone != last_adjacent) {
                if (shadow_subzone != 0) {
                    if (abs(span_ptr->Height - above->Height) < 2 && Unknown_0055A660(span_cell, true)) {
                        Cell shadow_cell;
                        shadow_cell.Y = (short)(y - 1);
                        shadow_cell.X = (short)x;
                        if (Unknown_0055A660(shadow_cell, true)) {
                            int packed_pair = Span_Pack32(shadow_subzone, (unsigned short)id);
                            if (x < xmin) {
                                entry.Value.unknown_04 = true;
                            } else {
                                entry.Value.unknown_04 = false;
                                if (x > xmax) {
                                    entry.Value.unknown_04 = true;
                                }
                            }
                            entry.Key = entry.Value.unknown_00 = packed_pair;
                            table->Add_Object(Span_Pack8(shadow_subzone, (unsigned short)id), entry);
                            last_adjacent = shadow_subzone;
                        }
                    }
                }
            }
            above++;
            x++;
        } else if (above->ZoneID == zone) {
            if (abs(span_ptr->Height - above->Height) < 2) {
                Cell shadow_cell;
                shadow_cell.X = (short)x;
                shadow_cell.Y = (short)(y - 1);
                Unknown_00564290(above, level, id, bounds, shadow_cell);
                continue;
            }
            above++;
            x++;
        } else {
            above++;
            x++;
        }
    }

    x = begin_x;
    while (x <= end_x) {
        int shadow_subzone = below->SubzoneID[level];
        CellSubzoneStruct *span_ptr;
        if (x == begin_x) {
            Cell left;
            left.X = (short)(x + 1);
            left.Y = (short)y;
            span_cell = left;
            span_ptr = &below[-stride + 1];
        } else if (x < end_x) {
            Cell middle;
            middle.X = (short)x;
            middle.Y = (short)y;
            span_cell = middle;
            span_ptr = &below[-stride];
        } else {
            Cell right;
            span_ptr = &below[-stride - 1];
            right.X = (short)(x - 1);
            right.Y = (short)y;
            span_cell = right;
        }

        if (shadow_subzone != 0 || y >= ymax || x < xmin || x > xmax) {
            if (shadow_subzone != (unsigned short)id && shadow_subzone != last_adjacent) {
                if (shadow_subzone != 0) {
                    if (abs(below->Height - span_ptr->Height) < 2 && Unknown_0055A660(span_cell, true)) {
                        Cell shadow_cell;
                        shadow_cell.X = (short)x;
                        shadow_cell.Y = (short)(y + 1);
                        if (Unknown_0055A660(shadow_cell, true)) {
                            int packed_pair = Span_Pack32(shadow_subzone, (unsigned short)id);
                            if (x < xmin) {
                                entry.Value.unknown_04 = true;
                            } else {
                                entry.Value.unknown_04 = false;
                                if (x > xmax) {
                                    entry.Value.unknown_04 = true;
                                }
                            }
                            entry.Key = entry.Value.unknown_00 = packed_pair;
                            table->Add_Object(Span_Pack8(shadow_subzone, (unsigned short)id), entry);
                            last_adjacent = shadow_subzone;
                        }
                    }
                }
            }
            below++;
            x++;
        } else if (below->ZoneID == zone && abs(below->Height - span_ptr->Height) < 2) {
            Unknown_00564290(below, level, id, bounds, Cell(x, y + 1));
        } else {
            below++;
            x++;
        }
    }

    return skip;
}
