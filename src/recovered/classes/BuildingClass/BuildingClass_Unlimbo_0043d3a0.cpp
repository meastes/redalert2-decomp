// BuildingClass slot 53 Unlimbo 0x0043D3A0 (TibSun building.cpp Unlimbo with the RA2 additions).
// Shape from the TibSun reconstruction (OpenTS building.cpp BuildingClass::Unlimbo; SPDX-License-Identifier:
// GPL-3.0-or-later; Copyright 2025 Electronic Arts Inc.; Copyright 2026 OpenTS contributors; contains material
// derived from Electronic Arts source code, EA's GPLv3 Section 7 additional terms apply) and the YR twin in
// gibbo101/ra2-decomp (src/building.cpp, GAMEMD 0x00440580; REFRESH_EOL is Cell(32767, 32767) in its sun.h):
// coord.As_Cell(), the REFRESH_EOL loop test and `sysloc + PositionCoord, &Map[PositionCoord]`.
// Draft by Scout753 (ul0), finished by Scout880.
#include "shared/building.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/house.hpp"
#include "shared/cell.hpp"
#include "shared/rules.hpp"
#include "shared/overlay_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/light_source.hpp"
#include "shared/building_light.hpp"
#include "shared/base.hpp"
#include "shared/abstract.hpp"
extern "C" int __cdecl stricmp(char const *, char const *);
extern "C" char *__cdecl strncpy(char *, char const *, unsigned);
extern bool unknown_flag_8339e8;
void __fastcall Building_Refresh_Fence_Cell_0044e440(Cell const &origin);
void __fastcall Building_Refresh_House_Buildings_0044fa40(HouseClass *house);
// The map's object lookup at a cell (0x0049A5B0; TibSun MapClass::Cell_Object).
class UnknownCellObjectMap {
public:
    ObjectClass *Cell_Object(Cell const &cell, Point2D const &offset);
};
// @ref ?Cell_Object@UnknownCellObjectMap@@QAEPAVObjectClass@@ABVCell@@ABUPoint2D@@@Z=0049a5b0
// @ref ?unknown_flag_8339e8@@3_NA=008339e8
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref _stricmp=00781f20
// @ref _strncpy=007823d0
// @ref ?unknown_4444b0@BuildingClass@@QAE_N_N@Z=004444b0
// @ref _unknown_geometry_l=0084ed30
bool BuildingClass::unknown_slot53(CoordStruct const *coord, int dir)
{
    bool ok = unknown_slot101(&Map[*coord], -1, -1, 0, true) == 0;

    if (ok) {
        if (Type == Rules->unknown_78c) {
            CellClass *cellptr = &Map[*coord];
            HouseClass *owner = 0;
            if (cellptr->unknown_50 >= 0) {
                owner = Houses[cellptr->unknown_50];
            }
            if (owner == House) {
                if (cellptr->OverlayIndex == 2 || cellptr->OverlayIndex == 0) {
                    House->Sell_Wall(cellptr->MapCoords, true);
                }
            }
        } else if (Type->ToOverlay != 0) {
            CellClass *cellptr = &Map[*coord];
            HouseClass *owner = 0;
            if (cellptr->unknown_50 >= 0) {
                owner = Houses[cellptr->unknown_50];
            }
            if (owner == House) {
                if (cellptr->OverlayIndex == Type->ToOverlay->ArrayIndex) {
                    House->Sell_Wall(cellptr->MapCoords, true);
                }
            }
        } else if (Type->Gate) {
            Cell cell = coord->As_Cell();
            Cell const *occupy = Type->unknown_slot36(true);
            while (*occupy != Cell(32767, 32767)) {
                CellClass *cellptr = &Map[cell + *occupy];
                HouseClass *owner = 0;
                if (cellptr->unknown_50 >= 0) {
                    owner = Houses[cellptr->unknown_50];
                }
                if (owner == House) {
                    if (cellptr->OverlayIndex == 2 || cellptr->OverlayIndex == 0 || cellptr->OverlayIndex == 0x1A) {
                        House->Sell_Wall(cellptr->MapCoords, true);
                    }
                }
                BuildingClass *building = static_cast<BuildingClass *>(cellptr->Cell_Building());
                if (building != 0 && building->Type->LaserFence && building->Owner() == Owner()) {
                    Building_Refresh_Fence_Cell_0044e440(cellptr->Fetch_CellID());
                }
                occupy++;
            }
        }
    }

    if (Type->Wall) {
        if (unknown_slot101(&Map[*coord], -1, -1, 0, true) == 0) {
            ObjectClass *o = Type->ToOverlay->Create_One_Of(House);
            if (o && o->unknown_slot53(coord, 0)) {
                Map[*coord].unknown_50 = House->ArrayIndex;
                unknown_slot149(3);
                Map.Sight_From(*coord, Type->Sight, House, false, false, false, true, false);
                Map.Sight_From(*coord, Type->Sight, House, false, false, false, true, true);
                unknown_slot60();
                return true;
            }
        }
        return false;
    }

    if (Type->PowersUpBuilding[0] != '\0') {
        ObjectClass *object = ((UnknownCellObjectMap &)Map).Cell_Object(coord->As_Cell(), Point2D(0, 0));
        if (object != 0 && object->Kind_Of() == 6) {
            BuildingClass *building = (BuildingClass *)object;
            BuildingTypeClass *btype = Type;
            if (House == building->House && stricmp(btype->PowersUpBuilding, building->Type->ID) == 0) {
                bool ok = false;
                if (btype->PowersUpToLevel != -1) {
                    if (btype->PowersUpToLevel <= 0 || btype->PowersUpToLevel > 3) {
                        return false;
                    }
                } else {
                    if (building->unknown_5c6 < building->Type->Upgrades) {
                        ok = true;
                    }
                }
                if (!ok && building->unknown_5c6) {
                    return false;
                }

                House->unknown_5669 = true;
                House->unknown_5668 = true;
                int levels = Type->PowersUpToLevel;
                if (levels == -1) {
                    levels = 1;
                }
                char *anim = building->Type->Anims[building->unknown_5c6].Anim.Buffer;
                if (stricmp(anim, Type->unknown_1c8) != 0) {
                    strncpy(anim, Type->unknown_1c8, sizeof(building->Type->Anims[0].Anim.Buffer));
                }
                while (levels) {
                    building->Advance_Upgrade_Animation_0044c5d0();
                    levels--;
                }
                building->unknown_4c0[building->unknown_5c6 - 1] = Type;
                Building_Refresh_House_Buildings_0044fa40(House);
                House->Enable_Available_Super_Weapons_004f4db0();
                House->unknown_144 = true;
                if (Type->IsThreatRatingNode) {
                    House->unknown_func_4f3160();
                }
                unknown_slot60();
                return true;
            }
        }

        return false;
    }

    if (Type->LaserFence) {
        if ((Dir_To_8(UnknownFacingDir((unsigned short)(dir << 8))) & 3) == 2) {
            unknown_4ec = 8;
        } else {
            unknown_4ec = 12;
        }
    }

    if (ok && Type->LaserFencePost) {
        unknown_4ec = 0;
        BuildingClass *building = static_cast<BuildingClass *>(Map[coord->As_Cell()].Cell_Building());
        if (building != 0 && building->Type->LaserFence && Owner() == building->Owner()) {
            building->unknown_slot60();
        }
    }

    if (TechnoClass::unknown_slot53(coord, dir)) {

        if (Type->CloakGenerator) {
            House->unknown_55e8 = true;
        }

        if (!unknown_7d) {
            return true;
        }

        if (unknown_game_mode_a3d298 != 0 && !House->Unknown_004F52F0() && !House->Type->MultiplayPassive) {
            unknown_59b = true;
        }

        if (unknown_id_source_a3d290->unknown_000 & 0x1000) {
            if (unknown_452be0()) {
                Create_Fogged_Copy_00452c70(0, 0, false);
            }
        }

        if (!unknown_slot31()) {
            Reserve_Base_Area_004510d0(false);
        }

        int width = Type->Width();
        int height = Type->Foundation_Height(false);
        Cell cell = coord->As_Cell();

        for (int y = 0; y < height + 2; y++) {
            for (int x = 0; x < width + 2; x++) {
                CellClass *cptr = &Map[Cell(x, y) + (cell - Cell(1, 1))];
                cptr->unknown_122++;
            }
        }

        if (Type->FirestormWall) {
            Update_Firestorm_Wall_00450950();
            CellClass *cellptr = &Map[*coord];
            for (int facing = 0; facing < 8; facing += 2) {
                BuildingClass *building = static_cast<BuildingClass *>(cellptr->Adjacent_Cell(facing).Cell_Building());
                if (building != 0 && building->Type->FirestormWall) {
                    building->Update_Firestorm_Wall_00450950();
                }
            }
        }

        House->unknown_func_4e8640();
        House->unknown_func_4eac70(this);
        House->unknown_144 = true;
        unknown_43c = 0;

        if ((!unknown_373 && (Map[*coord].unknown_12c & 0x10)) || unknown_game_mode_a3d298 != 0) {
            unknown_slot96(unknown_player_a35db4);
        } else if (Type->LightIntensity != 0) {
            if (unknown_4e8 == 0) {
                unknown_4e8 = new LightSourceClass(Center_Coord(), Type->LightVisibility, Type->LightIntensity, Type->LightRedTint, Type->LightGreenTint, Type->LightBlueTint);
            }
            unknown_4e8->Activate(false);
        }

        if (Type->LaserFencePost && !unknown_a40774) {
            if (unknown_7d && !unknown_76) {
                unknown_44dfc0(0);
                unknown_44dfc0(2);
                unknown_44dfc0(4);
                unknown_44dfc0(6);
                unknown_44e5a0(1);
            }
        }

        if (!House->Unknown_004F52F0()) {
            unknown_slot96(House);
        }

        if (unknown_372) {
            unknown_flag_8339e8 = true;
            Map.Flag_To_Redraw(0);
        }

        if (Type->NaturalParticleSystem != 0 && unknown_25c[2] == 0) {
            CoordStruct sysloc = Type->NaturalParticleLocation;
            unknown_25c[2] = new ParticleSystemClass(Type->NaturalParticleSystem, sysloc + Position_Coord_004f3f40(), &Map[Position_Coord_004f3f40()], 0, unknown_geometry_l);
        }

        if (Type == Rules->unknown_78c) {
            Cell cell = Map[*coord].Fetch_CellID();
            Map[cell + Cell(1, 0)].Wall_Update(false);
            Map[cell + Cell(-1, 0)].Wall_Update(false);
            Map[cell + Cell(0, 1)].Wall_Update(false);
            Map[cell + Cell(0, -1)].Wall_Update(false);
        }

        if (Type == Rules->unknown_77c || Type == Rules->unknown_784) {
            Cell cell = Map[*coord].Fetch_CellID();
            Map[cell + Cell(-1, 0)].Wall_Update(false);
            Map[cell + Cell(3, 0)].Wall_Update(false);
        }

        if (Type == Rules->unknown_780 || Type == Rules->unknown_788) {
            Cell cell = Map[*coord].Fetch_CellID();
            Map[cell + Cell(0, -1)].Wall_Update(false);
            Map[cell + Cell(0, 3)].Wall_Update(false);
        }

        if (Type->HasSpotlight) {
            unknown_4d4 = new BuildingLightClass(this);
        }

        unknown_32b = false;
        if (Type->Factory != 0) {
            unknown_4444b0(false);
        }

        for (int i = 0; i < Rules->unknown_7c8.Count(); i++) {
            if (Rules->unknown_7c8[i] == Type) {
                House->unknown_50.Add(this);
                break;
            }
        }
        if (Type->UnitRepair) {
            House->unknown_80.Add(this);
        }
        if (Type->Cloning) {
            House->unknown_98.Add(this);
        }
        if (Type->PsychicDetectionRadius > 0) {
            House->unknown_b0.Add(this);
        }
        House->Buildings.Add(this);

        if (!House->Unknown_004F52F0()) {
            House->unknown_55f0.unknown_42cad0(this);
        }

        if (Type->Factory != 0) {
            House->unknown_func_4f3170(Type->Factory, Type->Naval, 0);
        }
        return true;
    }
    return false;
}
