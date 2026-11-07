// void SuperClass::Place(Cell const &cell, bool player) (0x0069D850)
// Fires the super weapon at cell (TS SuperClass::Place; RA2 types 0 nuke,
// 1 iron curtain, 2 lightning storm, 3 chronosphere, 4 chrono warp,
// 5/6 paradrops; source case order follows the code layout). The chrono warp's
// piggyback swap has TibSun's and the YR twin's comip shape (TibSun building.cpp
// Mission_Unload, ra2-decomp reinf.cpp: `piggy = IPiggybackPtr(teleport);
// if (piggy != NULL) piggy->Begin_Piggyback(...)`, operator-> checked).
#include "shared/house.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/rules.hpp"
#include "shared/foot.hpp"
#include "shared/locomotion.hpp"
#include "shared/vox_class.hpp"
#include "shared/vox_speech.hpp"
#include "shared/building_type_list.hpp"
#include "shared/building_type_class.hpp"
#include "shared/teleport_locomotion.hpp"
#include "shared/infantry_type.hpp"
#include "shared/bullet.hpp"
#include "shared/weapon_type.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/radar_event.hpp"
#include "shared/sound_play.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/aircraft_type.hpp"
// @ref ??_C@_0BL@LECF@EVA_NuclearMissileLaunched?$AA@=007f65f4
// @ref ??_C@_0BK@KIGD@EVA_ChronosphereActivated?$AA@=007f662c
// @ref ??_C@_0BJ@ECIB@EVA_IronCurtainActivated?$AA@=007f6610
// @ref ??_C@_07IIFO@PDPLANE?$AA@=007ee570
// @ref ??_C@_07KHHH@PSIWARN?$AA@=007cfa20
// @ref ??_C@_0M@BCEA@NukePayload?$AA@=007d1240
// @ref ??_C@_0BK@OPHL@EVA_LightningStormCreated?$AA@=007ddbc8
// @ref ?Unknown_00438B80@@YIXPAVHouseClass@@PAVCellClass@@PAVBulletClass@@PBD@Z=00438b80
// @ref ?Unknown_0063A690@@YIXPAVHouseClass@@HHHPAVCellClass@@HHH@Z=0063a690
// @ref ??HCoordStruct@@QBE?AU0@ABU0@@Z=0069ecf0
// @ref ??0CoordStruct@@QAE@HHH@Z=00434820
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref ??H?$TPoint3D@H@@QBE?BV0@ABV0@@Z=0069ecf0
// @ref ?_AddRef@?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QBEXXZ=0069ed20
// @ref ?_AddRef@?$UnknownComPtr@UILocomotion@@$1?unknown_uuid_ILocomotion_7ce600@@3UGUID@@B@@QBEXXZ=00453ea0
// @ref ?Speech_Cancel@@YIXH@Z=00715e80
// @ref ?Find_Index@VoxClass@@SIHPBD@Z=00716640

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
// The cell returned for coordinates outside the map (map.hpp's declaration; map.hpp cannot follow map_view.hpp).
extern CellClass unknown_dummy_cell_a6f9f8;                       // 0x00A6F9F8
// The psychic-warning radar note for a launched bullet (0x00438B80; name and types hypotheses).
void __fastcall Unknown_00438B80(HouseClass *house, CellClass *cell, BulletClass *bullet, char const *name);
// Drops a paradrop of count infantry of a type from a plane (0x0063A690; hypothesis).
void __fastcall Unknown_0063A690(HouseClass *house, int plane, int arg3, int arg4, CellClass *cell, int arg6, int type, int count);

// comip's operator=: the new reference through _AddRef, which stays out of line
// (0x0069ED20 for IPiggyback, 0x00453EA0 for ILocomotion; as BuildingClass 0x00449290).
template <> void UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::_AddRef() const;
template <> inline UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> &
UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::operator=(UnknownComPtr const &other)
{
    if (Pointer != other.Pointer) {
        IPiggyback *old = Pointer;
        Pointer = other.Pointer;
        _AddRef();
        if (old != 0) {
            old->Release();
        }
    }
    return *this;
}
template <> void UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::_AddRef() const;
template <> inline UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> &
UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::operator=(UnknownComPtr const &other)
{
    if (Pointer != other.Pointer) {
        ILocomotion *old = Pointer;
        Pointer = other.Pointer;
        _AddRef();
        if (old != 0) {
            old->Release();
        }
    }
    return *this;
}

void SuperClass::Place(Cell const &cell, bool player)
{
    switch (Type->Type) {
    case 3:
        if (unknown_0x57) {
            unknown_0x4A = cell;
            CoordStruct coord = Map[cell].Center_Coord();
            if (Map[cell].unknown_140 & 0x100) {
                coord.Z += unknown_geometry_j;
            }
            unknown_place_anim_69cb30(coord + CoordStruct(0, 0, 5));
            if (player) {
                unknown_pending_super_833694 = 4;
                return;
            }
            if (Anim != 0) {
                Anim->unknown_17d = true;
                Anim->unknown_180.Unknown_00405D50();
                return;
            }
        }
        break;

    case 4: {
        Unknown_0063B5B0(13, unknown_0x4A);
        Unknown_0063B5B0(13, cell);
        CoordStruct dest = Map[cell].Center_Coord();
        CoordStruct from = Map[unknown_0x4A].Center_Coord();
        if (Map[cell].unknown_140 & 0x100) {
            dest.Z += unknown_geometry_j;
        }
        if (Map[unknown_0x4A].unknown_140 & 0x100) {
            from.Z += unknown_geometry_j;
        }
        unknown_69ca70();
        new AnimClass(Rules->ChronoBlastDest, dest + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
        new AnimClass(Rules->ChronoBlast, from + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
        for (int index = 0; index < 9; index++) {
            Cell const *offset = &unknown_cells_abd7a0[index];
            ObjectClass *object;
            if (Map[*offset + unknown_0x4A].unknown_140 & 0x100) {
                object = Map[unknown_0x4A + *offset].AltOccupierPtr;
            } else {
                object = Map[*offset + unknown_0x4A].OccupierPtr;
            }
            while (object != 0) {
                FootClass *foot = (FootClass *)object;
                if (object->Is_Foot() && !object->unknown_slot21()) {
                    TechnoTypeClass *ttype = object->unknown_slot32();
                    if (ttype->Organic && !object->unknown_slot32()->Teleporter && !object->unknown_slot21()) {
                        int damage = object->unknown_slot32()->MaxStrength;
                        object->unknown_slot86(&damage, 0, Rules->unknown_c74, 0, true, 0);
                    } else if (!foot->unknown_slot203() && !foot->unknown_210 && !object->unknown_slot111() && !object->unknown_slot112()) {
                        UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggyback(foot->Locomotor);
                        UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> teleport(CLSID_TeleportLocomotionClass);
                        UnitClass *unit = object->Kind_Of() == 1 ? (UnitClass *)object : 0;
                        CoordStruct coord = Map[*offset + cell].Center_Coord();
                        if (Map[coord].unknown_140 & 0x100) {
                            coord.Z += unknown_geometry_j;
                        }
                        if (unit != 0) {
                            foot->Locomotor.Checked()->Mark_All_Occupation_Bits(0);
                            foot->Locomotor.Checked()->Force_Track(-1, CoordStruct(coord));
                            object->unknown_slot59(coord);
                            foot->unknown_5a0 = true;
                        }
                        teleport.Checked()->Link_To_Object(object);
                        piggyback = UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>(teleport);
                        if (piggyback != 0) {
                            piggyback.Checked()->Begin_Piggyback(foot->Locomotor);
                            foot->Locomotor = teleport;
                        }
                        CoordStruct delta = object->Center_Coord() - Map[unknown_0x4A].Center_Coord();
                        if (unit == 0) {
                            coord = delta + Map[cell].Center_Coord();
                            if (Map[coord].unknown_140 & 0x100) {
                                coord.Z += unknown_geometry_j;
                            }
                        }
                        foot->unknown_210 = true;
                        foot->unknown_21c = coord;
                        foot->unknown_slot260(&Map[*offset + cell], true);
                    }
                }
                object = object->unknown_30;
            }
        }
        if (!unknown_int_a3d548) {
            Speech_Play_Name("EVA_ChronosphereActivated", -1, -1);
        }
        if (player) {
            unknown_pending_super_833694 = -1;
            Speech_Cancel(VoxClass::Find_Index("EVA_ChronosphereReady"));
            return;
        }
        break;
    }

    case 2:
        if (unknown_0x57) {
            Unknown_0051F930(Rules->LightningStormDuration, Rules->unknown_13e8, cell, (int)House);
            if (!unknown_int_a3d548) {
                Speech_Play_Name("EVA_LightningStormCreated", -1, -1);
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_LightningStormReady"));
                return;
            }
        }
        break;

    case 1:
        if (unknown_0x57) {
            CoordStruct coord = Map[cell].Center_Coord();
            new AnimClass(Rules->IronCurtainInvokeAnim, coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
            if (!unknown_int_a3d548) {
                Speech_Play_Name("EVA_IronCurtainActivated", -1, -1);
            }
            Unknown_0063B5B0(13, cell);
            for (int index = 0; index < 9; index++) {
                Cell const *offset = &unknown_cells_abd7a0[index];
                for (ObjectClass *object = Map[cell + *offset].OccupierPtr; object != 0; object = object->unknown_30) {
                    object->unknown_slot82(Rules->IronCurtainDuration);
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_IronCurtainReady"));
                return;
            }
        }
        break;

    case 5:
        if (unknown_0x57) {
            int plane = AircraftTypeClass::From_Name("PDPLANE");
            CellClass *cellptr = &Map[cell];
            if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
                if (cellptr->Is_Tile_Water()) {
                    Cell spot = Map.Unknown_005502E0(cell, 0, -1, 0, 0, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
                    if (spot != unknown_cell_abd768) {
                        CellClass *nearptr = &Map[spot];
                        if (nearptr != 0 && nearptr != &unknown_dummy_cell_a6f9f8 && !nearptr->Is_Tile_Water()) {
                            cellptr = nearptr;
                        }
                    }
                }
                if (House->unknown_130 == 0) {
                    if (Rules->AllyParaDropInf.Count() == Rules->AllyParaDropNum.Count()) {
                        for (int i = 0; i < Rules->AllyParaDropInf.Count(); i++) {
                            int type = Rules->AllyParaDropInf[i]->ArrayIndex;
                            int count = Rules->AllyParaDropNum[i];
                            if (plane != -1 && type != -1) {
                                Unknown_0063A690(House, plane, 1, 0x1a, cellptr, 0, type, count);
                            }
                        }
                    }
                } else {
                    for (int i = 0; i < Rules->SovParaDropInf.Count(); i++) {
                        int type = Rules->SovParaDropInf[i]->ArrayIndex;
                        int count = Rules->SovParaDropNum[i];
                        if (plane != -1 && type != -1) {
                            Unknown_0063A690(House, plane, 1, 0x1a, cellptr, 0, type, count);
                        }
                    }
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_ReinforcementsReady"));
                return;
            }
        }
        break;

    case 6:
        if (unknown_0x57) {
            int plane = AircraftTypeClass::From_Name("PDPLANE");
            CellClass *cellptr = &Map[cell];
            if (cellptr != 0 && cellptr != &unknown_dummy_cell_a6f9f8) {
                if (cellptr->Is_Tile_Water()) {
                    Cell spot = Map.Unknown_005502E0(cell, 0, -1, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
                    if (spot != unknown_cell_abd768) {
                        CellClass *nearptr = &Map[spot];
                        if (nearptr != 0 && nearptr != &unknown_dummy_cell_a6f9f8 && !nearptr->Is_Tile_Water()) {
                            cellptr = nearptr;
                        }
                    }
                }
                if (Rules->AmerParaDropInf.Count() == Rules->AmerParaDropNum.Count()) {
                    for (int i = 0; i < Rules->AmerParaDropInf.Count(); i++) {
                        int type = Rules->AmerParaDropInf[i]->ArrayIndex;
                        int count = Rules->AmerParaDropNum[i];
                        if (plane != -1 && type != -1) {
                            Unknown_0063A690(House, plane, 1, 0x1a, cellptr, 0, type, count);
                        }
                    }
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_ReinforcementsReady"));
            }
        }
        break;

    case 0:
        if (unknown_0x57) {
            if (Is_One_Time()) {
                Cell spot = cell;
                CoordStruct coord = Cell_Coord(spot);
                coord.Z = Map.Unknown_0055A280(coord);
                if (Map[spot].unknown_140 & 0x100) {
                    coord.Z += unknown_geometry_j;
                }
                WeaponTypeClass *payload = unknown_WeaponTypeClass_list_00839b90[WeaponTypeClass::Index_From_Name("NukePayload")];
                CellClass *target = &Map[coord];
                CoordStruct sound = coord;
                CoordStruct launch(sound.X, sound.Y, sound.Z + 20000);
                BulletTypeClass *btype = payload->Projectile;
                BulletClass *bullet = Create_Bullet(btype, target, 0, payload->unknown_a4, payload->Warhead, payload->unknown_a8, false);
                bullet->Set_Weapon_Type(payload);
                Unknown_00438B80(House, target, bullet, "PSIWARN");
                if (bullet != 0) {
                    double y = unknown_sin_4ba480(1.5707483884323126);
                    y *= unknown_cos_4ba4d0(1.5707483884323126);
                    y *= -100.0;
                    BulletVelocityXY planar(unknown_cos_4ba4d0(1.5707483884323126) * unknown_cos_4ba4d0(1.5707483884323126) * -100.0, y);
                    BulletVelocity velocity;
                    velocity.XY() = planar;
                    velocity.Z = unknown_sin_4ba480(1.5707483884323126) * -100.0;
                    bullet->unknown_slot118(launch, velocity);
                    if (!unknown_int_a3d548) {
                        Speech_Play_Name("EVA_NuclearMissileLaunched", -1, -1);
                    }
                    Voc_Static_Add(Rules->unknown_16c, sound, 0);
                }
                if (player) {
                    unknown_pending_super_833694 = -1;
                    Speech_Cancel(VoxClass::Find_Index("EVA_NuclearMissileReady"));
                }
                House->unknown_144 = true;
                return;
            }
            for (int index = 0; index < BuildingTypes.Count(); index++) {
                BuildingTypeClass *btype = BuildingTypes[index];
                if (btype->NukeSilo && (btype->SuperWeapon == Type->ArrayIndex || btype->SuperWeapon2 == Type->ArrayIndex)) {
                    BuildingClass *site = House->Find_Building(index, -1);
                    if (site != 0) {
                        Cell spot = cell;
                        CoordStruct coord = Cell_Coord(spot);
                        coord.Z = Map.Unknown_0055A280(coord);
                        CoordStruct sound = coord;
                        site->unknown_slot116(0x16, false);
                        site->unknown_slot117();
                        House->NukeDest = cell;
                        *(int *)((char *)site + 0x4CC) = Type->Type;   // TODO: BuildingClass +0x4CC (building_class.hpp unknown_4cc; the BuildingClass in scope differs)
                        Voc_Static_Add(Rules->unknown_16c, sound, 0);
                        if (!unknown_int_a3d548) {
                            Speech_Play_Name("EVA_NuclearMissileLaunched", -1, -1);
                        }
                    }
                    break;
                }
            }
            if (player) {
                unknown_pending_super_833694 = -1;
                Speech_Cancel(VoxClass::Find_Index("EVA_NuclearMissileReady"));
            }
            House->unknown_144 = true;
        }
        break;
    }
}
