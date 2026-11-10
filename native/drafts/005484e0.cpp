// NON-EXACT DRAFT 0x5484E0 MapClass::Set_Map_Dimensions (Set_Map_Dimensions)
// entry 0x5484E0; exact-match score 94.60 (Scout1013 best; loose 98.86); plan "draft, verify"
// origin lane: Scout1013 (build/class-work/Scout1013/keep/smd_9460.cpp; from Scout1002 w3/smd0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): the draft was scored with Scout1013's private overlay
// (RA2_MAP_SET_DIMENSIONS stand-in members in map_techno.hpp, RA2_CELL_ADD_IN_PLACE
// Cell::operator+= in coord.hpp). Natively the Map unit's stand-in ObjectClass/AnimClass
// do not lay out like the real classes (pointer widths), so the members the overlay
// added are read through native helpers compiled against the real headers
// (native/link/behaviour_map_dimensions.cpp), and the tube cells use Cell + Cell
// (same value as the base operator+= 0x0056AA20). Each changed line is marked NATIVE.
// void MapClass::Set_Map_Dimensions (0x005484E0): TibSun Set_Map_Dimensions.
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_techno.hpp"
#include "shared/object_lists.hpp"
#include "shared/tube.hpp"
#include "shared/id_source.hpp"
#include "shared/always.hpp"
// NATIVE: shared/main_loop.hpp left out (its min() repeats always.hpp's without the overlay's guard; nothing used)
#include "shared/map_region.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/map_view.hpp"
// @ref ??4AbstractClass@@QAEAAV0@ABV0@@Z=0056a9f0
// @ref ?unknown_dummy_cell_a6f9f8@@3VCellClass@@A=00a6f9f8
// @ref ?unknown_cell_a6f238@@3VCell@@A=00a6f238
// @ref ?unknown_objects_a40368@@3V?$DynamicVectorClass@PAVObjectClass@@@@A=00a40368
// @ref ?Anims@@3V?$DynamicVectorClass@PAVAnimClass@@@@A=00a40960
// @ref ?unknown_TubeClass_list_008661c8@@3V?$DynamicVectorClass@PAVTubeClass@@@@A=008661c8
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?unknown_a40774@@3HA=00a40774
// @ref ?unknown_map_min_a70a1c@@3HA=00a70a1c
// @ref ?unknown_map_max_a70a20@@3HA=00a70a20
// @ref ?unknown_int_8325d0@@3HA=008325d0
// @ref ?unknown_int_8325d4@@3HA=008325d4
// @ref ?Set_CellID@CellClass@@QAEXABVCell@@@Z=0047a450
// @ref ??Y?$TPoint2D@F@@QAEABV0@ABV0@@Z=0056aa20
// @ref ??H?$TPoint2D@F@@QBE?BV0@ABV0@@Z=0042add0
// @ref ?unknown_833968@@3HA=00833968
// @ref ??2@YAPAXI@Z=00782017
// @ref ??3@YAXPAX@Z=00781d3b

// NATIVE: real-layout accessors (native/link/behaviour_map_dimensions.cpp).
CoordStruct ra2native_object_coord_5484e0(ObjectClass const *object);
Cell ra2native_object_cell_5484e0(ObjectClass const *object);
bool ra2native_anim_attached_5484e0(ObjectClass const *anim);
bool ra2native_is_gas_system_5484e0(ObjectClass const *object);

extern int unknown_833968;
extern int unknown_int_8325d0;   // NATIVE: declared here (shared/main_loop.hpp, left out above)
extern int unknown_int_8325d4;   // NATIVE

void MapClass::Set_Map_Dimensions(Rect const *rect, bool reset_cells, int cell_height, bool refresh_map)
{
    int i = 0;
    int w = MapSize.Width;
    CellClass *cells_base = 0;
    CellClass *cells = 0;
    int h = 2 * unknown_int_8325d4 + 8;
    CellClass *cptr;
    Cell cell(unknown_cell_a6f238);

    for (int j = 0; j < unknown_objects_a40368.Count(); j++) {
        unknown_objects_a40368[j]->Mark(0);
    }

    for (; i < Cells.Length(); i++) {
        if (!Unknown_0054AAB0(Cell(i % 512, i / 512))) {
            CellClass *c = Cells[i];
            if (c != 0) {
                c->unknown_11b = cell_height;
            }
        }
    }

    int cellcount;

    if (!reset_cells) {
        cellcount = 0;
        cells_base = (CellClass *) new char[h * w * sizeof(CellClass)];
        cells = cells_base;
        Reset_Iterator();
        cptr = Iterate();
        cell = cptr->Fetch_CellID();
        while (cptr != 0) {
            cells_base[cellcount] = *cptr;
            cells_base[cellcount].unknown_2c = 0;
            cells_base[cellcount].unknown_30 = 0;
            cptr = Iterate();
            cellcount++;
        }
    }

    MapSize = *rect;
    MapSize.X = 0;
    MapSize.Y = 0;

    if (MapSize.Width > 63 || MapSize.Height > 63) {
        unknown_833968 = 2;
    }

    unknown_map_min_a70a1c = unknown_int_8325d0;
    unknown_map_max_a70a20 = unknown_int_8325d0 + 2 * unknown_int_8325d4;

    int y = 0;
    unknown_120 = 1;
    unknown_124 = 1;
    int msize = rect->Height + rect->Width - 1;
    unknown_128 = msize;
    unknown_12c = msize;

    for (; y < 2 * unknown_12c + 2; y++) {
        for (int x = 0; x < unknown_128 + 2; x++) {
            Cell cell(x, y);
            if (Unknown_0054AAB0(cell)) {
                int idx = (512 * y) + x;
                CellClass *c = Cells[idx];
                if (c == 0) {
                    CellClass *cc = new CellClass;
                    cc->Set_CellID(cell);
                    Cells[idx] = cc;
                } else {
                    c = new(c) CellClass;
                    c->Set_CellID(cell);
                }
                Cells[idx]->unknown_11b = cell_height;
            }
        }
    }

    bool freshcalled = false;

    if (!reset_cells) {
        Reset_Iterator();
        Cell icell = Iterate()->Fetch_CellID();
        Cell cellpos = Cell(rect->Y + rect->X, rect->Y - rect->X);
        Cell offset = (icell - cell) - cellpos;

        for (i = 0; i < cellcount; i++) {
            CellClass *src = &cells[i];
            Cell scell = src->MapCoords;
            Cell shifted = scell + offset;

            CellClass *c = &(*this)[shifted];

            Cell dcell = c->Fetch_CellID();
            if (c != 0) {
                *c = *src;
                c->Set_CellID(dcell);
            }
        }

        Reset_Iterator();
        cptr = Iterate();
        while (cptr != 0) {

            Cell c = cptr->Fetch_CellID();

            if (c.X + c.Y < MapSize.Width - 2 * rect->Y + 1 ||
                c.Y - c.X > MapSize.Width + 2 * rect->X - 1 ||
                c.X + c.Y > MapSize.Width + 2 * (h + MapSize.Height - rect->Y - unknown_int_8325d4) ||
                c.X - c.Y > MapSize.Width + 2 * (w - rect->X - unknown_int_8325d0) - 1) {

                int cellindex = c.X + c.Y * 512;

                delete Cells[cellindex];
                Cells[cellindex] = new CellClass;
                Cells[cellindex]->Set_CellID(c);
                Cells[cellindex]->IsoTileIndex = 0xFFFF;
                Cells[cellindex]->unknown_11a = 0;
                Cells[cellindex]->unknown_11c = 0;
                Cells[cellindex]->OverlayIndex = -1;
                Cells[cellindex]->unknown_11b = cell_height;
            }
            cptr = Iterate();
        }

        for (i = 0; i < 101; i++) {
            if (unknown_id_source_a3d290->Unknown_00663240(i)) {
                unknown_id_source_a3d290->Set_Waypoint_Location(i, unknown_id_source_a3d290->Get_Waypoint_Location(i) + offset);
            }
        }

        for (i = 0; i < unknown_TubeClass_list_008661c8.Count(); i++) {
            TubeClass *tube = unknown_TubeClass_list_008661c8[i];
            tube->EnterCell = tube->EnterCell + offset;   // NATIVE: overlay Cell::operator+= (base 0x0056AA20)
            tube->ExitCell = tube->ExitCell + offset;     // NATIVE
        }

        CoordStruct coord = CoordStruct(offset, 0) - CoordStruct(128, 128, 0);

        for (i = 0; i < unknown_objects_a40368.Count(); i++) {
            ObjectClass *obj = unknown_objects_a40368[i];
            if (obj->Kind_Of() != 4) {
                obj->unknown_slot103(ra2native_object_coord_5484e0(obj) + coord);   // NATIVE: Coord +0x88
            }
        }

        for (i = 0; i < Anims.Count(); i++) {
            AnimClass *anim = Anims[i];
            if (!ra2native_anim_attached_5484e0(anim)) {   // NATIVE: AnimClass +0xB4
                anim->unknown_slot103(ra2native_object_coord_5484e0(anim) + coord);   // NATIVE
            }
        }

        unknown_a40774++;

        for (i = 0; i < unknown_objects_a40368.Count(); i++) {
            if (unknown_objects_a40368[i]->Kind_Of() != 0x18 || !ra2native_is_gas_system_5484e0(unknown_objects_a40368[i])) {   // NATIVE: Class +0x98, BehavesLike +0x284 == 1
                Cell cell = ra2native_object_cell_5484e0(unknown_objects_a40368[i]);   // NATIVE: slot 104
                Cell const *list = unknown_objects_a40368[i]->unknown_slot64(0);
                while (*list != Cell(0x7FFF, 0x7FFF)) {
                    Cell ncell = cell + *list;
                    if (!Unknown_0054AAB0(ncell)) {
                        unknown_objects_a40368[i]->unknown_slot52();
                        delete unknown_objects_a40368[i];
                        i--;
                        break;
                    }
                    list++;
                }
            }
        }

        unknown_a40774--;

        for (i = 0; i < Cells.Length(); i++) {
            CellClass *c1 = Cells[i];
            CellClass **c2 = &Cells[i];
            if (c1 != 0) {
                if (!Unknown_0054AAB0(Cell(i % 512, i / 512))) {
                    *c2 = 0;
                    delete c1;
                }
            }
        }

        Reset_Iterator();
        cptr = Iterate();
        while (cptr != 0) {
            if (cptr->IsBridgeDeck && cptr->unknown_2c == 0) {
                unsigned facing = !cptr->IsBridgeEastWest * 2;
                if (cptr->OverlayIndex == 0x18 || cptr->OverlayIndex == 0x19) {
                    cptr->Set_Under_Bridge(facing, true);
                } else {
                    cptr->Set_Under_Rail_Bridge(facing, true);
                }
            }
            cptr = Iterate();
        }

        if (refresh_map) {
            Unknown_005499C0();
            freshcalled = true;
        }

        for (i = 0; i < unknown_objects_a40368.Count(); i++) {
            unknown_objects_a40368[i]->Mark(1);
        }

        Map.Flag_To_Redraw(2);
    }

    if (!freshcalled && refresh_map) {
        Unknown_005499C0();
    }

    new(&unknown_dummy_cell_a6f9f8) CellClass;
    delete (void *)cells_base;
}
