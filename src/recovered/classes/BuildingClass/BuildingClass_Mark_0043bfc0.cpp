// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool BuildingClass::unknown_slot71(int mark) 0x0043BFC0 (slot 71, Mark), from ra2-decomp BuildingClass::Mark
// (YR 0x0043F180, src/building.cpp): lifts the building's cell content, or puts it down: a building that turns
// into isometric tiles lays them over its clear foundation cells (clearing smudges, revealing for the player)
// and deletes itself; a wall becomes its overlay; anything else occupies its cell, snaps its coordinate,
// places its animations and clears the smudges under its foundation. MARK_CHANGE refreshes the animations.
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
#include "shared/smudge_type.hpp"
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
#include "shared/overlay.hpp"
#include "shared/isometric_tile_type.hpp"
bool BuildingClass::unknown_slot71(int mark)
{
    if (TechnoClass::unknown_slot71(mark)) {
        Cell cell = unknown_slot104();

        switch (mark) {
            case 0:
                Map.Unknown_0054AF70(cell, this);
                break;

            case 1:
            case 3:

                if (Type->unknown_bf0 != 0) {
                    if (unknown_slot101(&Map[cell], -1, -1, 0, true) == 0) {
                        int w = Type->Width();
                        int h = Type->Foundation_Height(false);
                        bool any_created = false;
                        int x;
                        int y;

                        for (y = 0; y < h; y++) {
                            for (x = 0; x < w; x++) {
                                Cell newcell = cell + Cell(x, y);
                                if (Map[newcell].unknown_4718b0(Type->SpeedType, (int)Type, (int)House)) {
                                    if (Map[newcell].IsoTileIndex != Type->unknown_bf0->ArrayIndex && Map[newcell].unknown_ec != 1) {
                                        CellClass *cptr = &Map[newcell];
                                        if (cptr->SmudgeIndex != -1) {
                                            SmudgeTypeClass *sptr = SmudgeTypes[cptr->SmudgeIndex];
                                            Cell position = cptr->MapCoords;
                                            int width = sptr->Width;
                                            int xx = cptr->SmudgeData % width;
                                            int yy = cptr->SmudgeData / width;
                                            position -= Cell(xx, yy);
                                            for (int smy = 0; smy < sptr->Height; smy++) {
                                                for (int smx = 0; smx < sptr->Width; smx++) {
                                                    Map[position + Cell(smx, smy)].SmudgeIndex = -1;
                                                    Map[position + Cell(smx, smy)].unknown_47a4e0();
                                                }
                                            }
                                        }
                                        ObjectClass *iptr = Type->unknown_bf0->unknown_slot35(House);
                                        iptr->unknown_slot53(&CoordStruct(newcell), 0);
                                        if (House == unknown_player_a35db4) {
                                            Map.Sight_From(CoordStruct(newcell), 1, unknown_player_a35db4, false, false, false, true, false);
                                            Map.Sight_From(CoordStruct(newcell), 1, unknown_player_a35db4, false, false, false, true, true);
                                        }
                                        any_created = true;
                                    }
                                }
                            }
                        }

                        for (y = 0; y < h + 2; y++) {
                            for (x = 0; x < w + 2; x++) {
                                Cell snewcell = cell + Cell(x - 1, y - 1);
                                Map[snewcell].unknown_47c080();
                            }
                        }

                        if (any_created) {
                            unknown_slot152(3);
                            unknown_slot60();
                            return true;
                        } else {
                            TechnoClass::unknown_slot71(0);
                            return false;
                        }
                    } else {
                        TechnoClass::unknown_slot71(0);
                        return false;
                    }
                } else if (Type->Wall) {
                    new OverlayClass(Type->ToOverlay, cell, House->Type->House);
                    unknown_slot152(3);
                    unknown_slot60();

                } else {
                    if (mark != 3) {
                        if (unknown_slot101(&Map[cell], -1, -1, 0, true) != 0) {
                            return false;
                        }
                    }
                    Map.Unknown_0054AB70(cell, this);
                    unknown_slot103(Type->unknown_slot27(CoordStruct(cell)));
                    unknown_44c9a0();

                    int w = Type->Width();
                    int h = Type->Foundation_Height(false);
                    int x;
                    int y;
                    for (y = 0; y < h; y++) {
                        for (x = 0; x < w; x++) {
                            Cell newcell = cell + Cell(x, y);
                            CellClass *cptr = &Map[newcell];
                            if (cptr->SmudgeIndex != -1) {
                                SmudgeTypeClass *sptr = SmudgeTypes[cptr->SmudgeIndex];
                                Cell position = cptr->MapCoords;
                                int width = sptr->Width;
                                int xx = cptr->SmudgeData % width;
                                int yy = cptr->SmudgeData / width;
                                position -= Cell(xx, yy);
                                for (int smy = 0; smy < sptr->Height; smy++) {
                                    for (int smx = 0; smx < sptr->Width; smx++) {
                                        Map[position + Cell(smx, smy)].SmudgeIndex = -1;
                                        Map[position + Cell(smx, smy)].unknown_47a4e0();
                                    }
                                }
                            }
                        }
                    }

                    for (y = 0; y < h + 2; y++) {
                        for (x = 0; x < w + 2; x++) {
                            Cell snewcell = cell + Cell(x - 1, y - 1);
                            Map[snewcell].unknown_47c080();
                        }
                    }
                }
                break;

            case 2:
                unknown_44d190();
                break;

            default:
                break;
        }
        return true;
    }
    return false;
}
