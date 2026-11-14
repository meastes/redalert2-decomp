// combat.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/combat.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define RA2_VECTOR3_NORMALIZE
#define RA2_VECTOR_RESET_INLINE
#define RA2_MAP_DAMAGE_LOW_BRIDGE
#define RA2_CELL_OCCUPIER_BRIDGE
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/warhead_type.hpp"
#include "shared/rules.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/anim.hpp"
#include "shared/infantry.hpp"
#include "shared/foot.hpp"
#include "shared/techno.hpp"
#include "shared/tactical.hpp"
#include "shared/tileset_index.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/particle_system.hpp"
#include "shared/coord_distance.hpp"
#include "shared/overlay_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/vector3.hpp"
#include "shared/anim_type.hpp"
#include "shared/abstract.hpp"
#include "shared/type_list.hpp"
#include "shared/spot_light.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/queue.hpp"
#include "shared/combat.hpp"
#include "shared/weapon_type.hpp"
#include "shared/facing.hpp"
#include "shared/fast_math.hpp"


class Leaf_47e2b0 { public: void get(); };
void Leaf_47e2b0::get() {  }


// Unit-wide forms for Explosion_Damage 0x0047E2C0 (the DamageGroup vector, Vector3::Normalize, the low-bridge damage members, CellClass::Cell_Occupier(bool)).
// Header-static dynamic initializers of the unit at 0x0047DFD0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0047DFD0-0x0047E1B1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0047DFF0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E020, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E040, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E060, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E080, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E0A0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E0C0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E100, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E130, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E160, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E190, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// Dynamic initializer (0x0047E1A0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0047dfd0.cpp; no source of its own.


// int __fastcall Modify_Damage(int damage, WarheadTypeClass *warhead, int armor, int distance) -- 0x0047E1C0
// TibSun combat.cpp Modify_Damage in RA2 form: no damage when inert; healing (negative damage) only
// within 8 leptons; otherwise the warhead's PercentAtMax fall-off over CellSpread, never below zero,
// scaled by Verses[armor] and capped at Rules MaxDamage.

int __fastcall Unknown_0047E1C0(int damage, WarheadTypeClass *warhead, int armour, int distance)
{
    if (!damage || unknown_id_source_a3d290->Special().Inert || warhead == 0) {
        return 0;
    }
    if (damage < 0) {
        if (distance < 8) {
            return damage;
        }
        return 0;
    }
    float fdamage = (float)damage;
    float atmax = fdamage * warhead->PercentAtMax;
    int spread = (int)(warhead->CellSpread * 256.0f);
    if (atmax != fdamage && spread != 0) {
        damage = (int)((fdamage - atmax) * (spread - distance) / spread + atmax);
    }
    damage = damage > 0 ? damage : 0;
    damage = (int)(damage * warhead->Verses[armour]);
    damage = damage < Rules->MaxDamage ? damage : Rules->MaxDamage;
    return damage;
}


// Explosion_Damage (0x0047E2C0, __fastcall; TibSun combat.cpp Explosion_Damage): collateral damage around a coordinate.
// Returns 1 when nothing was hit, 0 when something was, 2 when a close shield blocked it.

inline UnknownVector3 Coord_Difference(CoordStruct const &a, CoordStruct const &b)
{
    return UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

inline int Coord_Distance_To(CoordStruct const &a, CoordStruct b)
{
    return Coord_Difference(a, b).Length();
}

inline int Coord_Length(UnknownVector3 const &d)
{
    double x = d.X;
    double y = d.Y;
    double z = d.Z;
    return (int)unknown_sqrt_4ba410(x * x + y * y + z * z);
}

inline bool Is_Techno_Object(ObjectClass *object)
{
    bool const &result = object->AbstractFlags.IsTechno;
    return result;
}
inline bool Is_Protected(ObjectClass *object)
{
    if (object) {
        if (Is_Techno_Object(object) && ((TechnoClass *)object)->unknown_slot203()) {
            return true;
        }
    }
    return false;
}

inline CoordStruct Center_Coord_Of(Cell const &c)
{
    return CoordStruct(c.X * 256 + 128, c.Y * 256 + 128, 0);
}

inline bool Is_Bridge_Tile(int ittype)
{
    return ittype == unknown_tileset_a6cb7c + 0 ||
        ittype == unknown_tileset_a6cb7c + 1 ||
        ittype == unknown_tileset_a6cb7c + 2 ||
        ittype == unknown_tileset_a6cb7c + 3 ||
        ittype == unknown_tileset_a52e70 + 0 ||
        ittype == unknown_tileset_a52e70 + 1 ||
        ittype == unknown_tileset_a52e70 + 2 ||
        ittype == unknown_tileset_a52e70 + 3;
}

inline bool Percent_Chance(int percent)
{
    return Random_Pick(0, 99) < percent;
}

struct DamageGroup {
    ObjectClass *Target;
    int Distance;
};

extern int const unknown_radius_count_7a5908[];
extern Cell const unknown_radius_offset_a6f248[];

int __fastcall Unknown_0047E2C0(CoordStruct const &coord, int strength, TechnoClass *source, WarheadTypeClass *warhead, bool chain)
{
    int distance;
    ObjectClass *object;
    DynamicVectorClass<DamageGroup *> groups;
    int i;

    if (!strength || unknown_id_source_a3d290->Special().Inert || warhead == 0) {
        return 1;
    }

    int range = (int)(warhead->CellSpread * 256.0f);
    Cell cell = Cell(coord);
    CellClass *cptr = &Map[cell];
    bool isbridge = false;
    bool hit = false;
    bool blocked = false;
    bool close = true;
    if (warhead->CellSpread > 0.5f) {
        close = false;
    }

    int cell_height = Map.Get_Height_GL(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
    if (cell_height < coord.Z) {
        unknown_tracker_839ea0.Unknown_004128A0(cptr, (int)warhead->CellSpread);
        TechnoClass *techno;
        while ((techno = unknown_tracker_839ea0.Fetch_Next_004134e0()) != 0) {
            if (techno->unknown_7d && techno->unknown_74 && techno->Strength > 0) {
                distance = Coord_Distance_To(coord, techno->Coord);
                if (distance <= range) {
                    if (close && distance < 0x55 && techno->unknown_slot203()) {
                        blocked = true;
                    }
                    DamageGroup *group = new DamageGroup;
                    group->Target = techno;
                    group->Distance = Coord_Distance_To(coord, techno->Coord);
                    groups.Add(group);
                }
            }
        }
    }

    if ((cptr->unknown_140 & 0x100) && coord.Z > Map.Get_Height_GL(coord) + unknown_geometry_j / 2) {
        isbridge = true;
    }

    int rings = (int)(warhead->CellSpread + 0.99);
    for (int m = 0; m < unknown_radius_count_7a5908[rings]; m++) {
        Cell tcell = cell + unknown_radius_offset_a6f248[m];
        cptr = &Map[tcell];
        if (cptr->OverlayIndex != -1) {
            OverlayTypeClass *overlay = OverlayTypes[cptr->OverlayIndex];
            if (overlay->ChainReaction && (!overlay->Tiberium || warhead->Tiberium) && chain) {
                cptr->unknown_475db0(strength / 10);
            }
            if (overlay->Wall) {
                if (warhead->WallAbsoluteDestroyer) {
                    cptr->Reduce_Wall(-1);
                } else if (warhead->Wall || (warhead->Wood && overlay->unknown_9c == 6)) {
                    cptr->Reduce_Wall(strength);
                }
            }
            if (cptr->OverlayIndex == -1) {
                Unknown_006D7C80((AbstractClass *)cptr);
            }
        }

        object = cptr->Cell_Occupier(isbridge);
        for (; object; object = object->unknown_30) {
            if (object != source && object->unknown_7d) {
                if (object->Kind_Of() == 1 && unknown_id_source_a3d290->Special().HarvesterImmune) {
                    if (Rules->unknown_9ec.Is_Present((UnitTypeClass *)object->Class_Of())) {
                        continue;
                    }
                }
                DamageGroup *group = new DamageGroup;
                group->Target = object;
                if (object->Kind_Of() == 6) {
                    if (m == 0) {
                        if (coord.Z - cptr->Center_Coord().Z > unknown_geometry_g * 2) {
                            group->Distance = Coord_Length(Coord_Difference(coord, cptr->Center_Coord())) - unknown_geometry_g * 2;
                        } else {
                            group->Distance = 0;
                        }
                    } else {
                        group->Distance = Coord_Length(Coord_Difference(coord, cptr->Center_Coord()));
                    }
                } else {
                    group->Distance = Coord_Length(Coord_Difference(coord, object->unknown_slot40()));
                }
                if (close && m == 0 && object) {
                    if (Is_Techno_Object(object) && ((TechnoClass *)object)->unknown_slot203() && group->Distance < 0x55) {
                        blocked = true;
                    }
                }
                groups.Add(group);
            }
        }
    }

    for (int j = 0; j < groups.Count(); j++) {
        object = groups[j]->Target;
        distance = groups[j]->Distance;
        if (object->unknown_7d && (object->Kind_Of() != 6 || !((BuildingClass *)object)->Type->InvisibleInGame)) {
            if (!blocked || Is_Protected(object)) {
                if (object->Kind_Of() == 2 && object->In_Air()) {
                    distance /= 2;
                }
                if (object->Strength > 0 && object->unknown_74 && !object->unknown_76 && distance <= range) {
                    int damage = strength;
                    object->Take_Damage(&damage, distance, warhead, source, false, 0);
                    hit = true;
                }
            }
        }
    }

    for (int k = 0; k < groups.Count(); k++) {
        delete groups[k];
    }
    groups.Reset();

    if (blocked) {
        return 2;
    }

    double force = strength * 0.01 < 4.0 ? strength * 0.01 : 4.0;
    if (warhead->Rocker && force > 0.3) {
        for (int x = cell.X - 3; x <= cell.X + 3; x++) {
            for (int y = cell.Y - 3; y <= cell.Y + 3; y++) {
                object = isbridge ? Map[Cell(x, y)].Cell_Occupier(true) : Map[Cell(x, y)].Cell_Occupier(false);
                for (; object; object = object->unknown_30) {
                    TechnoClass *techno = object->As_Techno();
                    if (techno) {
                        if ((short)x == cell.X && (short)y == cell.Y && source) {
                            CoordStruct tcoord = techno->Coord;
                            CoordStruct scoord = source->Coord;
                            Vector3 dir((float)(scoord.X - tcoord.X), (float)(scoord.Y - tcoord.Y), (float)(scoord.Z - tcoord.Z));
                            dir = dir.Normalize() * 10.0f;
                            tcoord += CoordStruct((int)dir.X, (int)dir.Y, (int)dir.Z);
                            techno->unknown_slot223(tcoord, (float)force);
                        } else {
                            techno->unknown_slot223(coord, (float)force);
                        }
                    }
                }
            }
        }
    }

    bool ion = warhead == Rules->IonCannonWarhead;
    cptr = &Map[cell];
    if (unknown_id_source_a3d290->Special().DestroyableBridges && warhead->Wall) {
        int ittype = cptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
        CellClass *bcptr = 0;
        if (cptr->unknown_140 & 0x100) {
            if (cptr->unknown_140 & 0x80) {
                bcptr = &Map[cptr->Fetch_CellID()];
            } else {
                bcptr = &Map[cptr->unknown_2c->Fetch_CellID()];
            }
        }

        if ((bcptr != 0 && (bcptr->OverlayIndex == 0x18 || bcptr->OverlayIndex == 0x19)) ||
            Is_Bridge_Tile(ittype)) {
            if (!(cptr->unknown_140 & 0x100) || (coord.Z <= unknown_geometry_j + unknown_geometry_g * (cptr->unknown_11b + 1) && coord.Z > unknown_geometry_j + unknown_geometry_g * (cptr->unknown_11b - 2))) {
                if (warhead->Wall && (warhead == Rules->IonCannonWarhead || Random_Pick(1, Rules->unknown_13a8) < strength)) {
                    bool damaged = Map.Unknown_00569090(&cell);
                    int tries = 3;
                    while (!damaged && ion && tries > 0) {
                        damaged = Map.Unknown_00569090(&cell);
                        tries--;
                    }
                    if (damaged) {
                        Unknown_006D7C80((AbstractClass *)cptr);
                    }
                    Point2D point;
                    unknown_tactical_83996c->Coord_To_Pixel(coord, point);
                    unknown_tactical_83996c->Register_Dirty_Area(Rect(point.X - 128, point.Y - 128, 256, 256), false);
                }
            }
        }

        int ittype2 = cptr->IsoTileIndex - unknown_tileset_a6cb68 + 1;
        if ((bcptr != 0 && (bcptr->OverlayIndex == 0xED || bcptr->OverlayIndex == 0xEE)) ||
            Is_Bridge_Tile(ittype2)) {
            if (!(cptr->unknown_140 & 0x100) || (coord.Z <= unknown_geometry_j + unknown_geometry_g * (cptr->unknown_11b + 1) && coord.Z > unknown_geometry_j + unknown_geometry_g * (cptr->unknown_11b - 2))) {
                if (warhead->Wall && (warhead == Rules->IonCannonWarhead || Random_Pick(1, Rules->unknown_13a8) < strength)) {
                    bool damaged = Map.Unknown_00569090(&cell);
                    int tries = 3;
                    while (!damaged && ion && tries > 0) {
                        damaged = Map.Unknown_00569090(&cell);
                        tries--;
                    }
                    if (damaged) {
                        Unknown_006D7C80((AbstractClass *)cptr);
                    }
                    Point2D point;
                    unknown_tactical_83996c->Coord_To_Pixel(coord, point);
                    unknown_tactical_83996c->Register_Dirty_Area(Rect(point.X - 96, point.Y - 96, 192, 192), false);
                }
            }
        }

        if (cptr->OverlayIndex >= 0x4A && cptr->OverlayIndex <= 0x63) {
            if (warhead == Rules->IonCannonWarhead || Random_Pick(1, Rules->unknown_13a8) < strength) {
                if (Map.Unknown_0055DCA0(cell)) {
                    Unknown_006D7C80((AbstractClass *)cptr);
                }
            }
        }
        if (cptr->OverlayIndex >= 0xCD && cptr->OverlayIndex <= 0xE6) {
            if (warhead == Rules->IonCannonWarhead || Random_Pick(1, Rules->unknown_13a8) < strength) {
                if (Map.Unknown_0055EDB0(cell)) {
                    Unknown_006D7C80((AbstractClass *)cptr);
                }
            }
        }
    }

    if (cptr->OverlayIndex != -1 && OverlayTypes[cptr->OverlayIndex]->Explodes) {
        cptr->Register_For_Redraw();
        cptr->OverlayIndex = -1;
        cptr->Recalc_Attributes(-1);
        Map.Update_Cell_Zone(cptr->MapCoords);
        Map.Unknown_00566340(cptr->MapCoords);
        Unknown_006D7C80((AbstractClass *)cptr);

        new AnimClass(Rules->BarrelExplode, coord, 0, 1, 0x600, 0);
        Unknown_0047E2C0(coord, Rules->AmmoCrateDamage, (TechnoClass *)0, Rules->unknown_c74, true);
        for (i = 0; i < Rules->BarrelDebris.Count(); i++) {
            if (Percent_Chance(15)) {
                new VoxelAnimClass(Rules->BarrelDebris[i], coord, 0);
                break;
            }
        }
        if (Percent_Chance(25)) {
            (new ParticleSystemClass(Rules->BarrelParticle, coord, 0, 0, unknown_geometry_l))->Spawn_Particle(coord, coord);
        }
    }

    if (warhead->Particle != 0) {
        ParticleSystemClass *psys;
        if (warhead->Particle->BehavesLike == 1) {
            psys = unknown_gas_cloud_system_a40d20;
        } else {
            psys = new ParticleSystemClass(warhead->Particle, coord, 0, 0, unknown_geometry_l);
        }
        psys->Spawn_Particle(coord, coord);
    }

    
    return !hit;
}


// AnimTypeClass *__fastcall Select_Damage_Anim(int damage, WarheadTypeClass *warhead, int land, CoordStruct *coords) -- 0x0047F4D0
// Purpose: picks the animation a warhead leaves where it hits. Conventional
// warheads over water (land 2) low enough in the cell use the splash list; the
// lightning warhead its own animation; otherwise the warhead's list, the
// stronger the damage the later the entry (one entry per 25 points; the
// EMP-style warheads draw one at random).

AnimTypeClass *__fastcall Select_Damage_Anim(int damage, WarheadTypeClass *warhead, int land, CoordStruct *coords)
{
    if (damage != 0 && warhead != 0) {
        if (land == 2 && warhead->Conventional && !(Map[*coords].unknown_140 & 0x100)
            && coords->Z < Map.Get_Height_GL(*coords) + unknown_geometry_g * 2) {
            if (Rules->SplashList.Count() == 0) {
                return 0;
            }
            int last = Rules->SplashList.Count() * 35 - 1;
            if (damage < last) {
                last = damage;
            }
            return Rules->SplashList[last / 35];
        }
        if (warhead == Rules->LightningWarhead) {
            return Rules->WeatherConBoltExplosion;
        }
        int count = warhead->AnimList.Count();
        if (count != 0) {
            if (warhead->EMEffect) {
                return warhead->AnimList[unknown_id_source_a3d290->Random()(0, count - 1)];
            }
            count = count * 25 - 1;
            if (damage >= count) {
                damage = count;
            }
            return warhead->AnimList[damage / 25];
        }
    }
    return 0;
}


// void __fastcall Combat_Lighting(CoordStruct coord, int damage, WarheadTypeClass *warhead, bool forced, int flags) -- 0x0047F600
// TibSun combat.cpp Combat_Lighting in RA2 form: when lighting detail allows it (or flags carry a light
// kind), a forced or Bright warhead makes a SpotLightClass, sized by CombatLightSize (capped at 1,
// times 63) or else by the damage clamped to 21..63, tagged with flags. The callers pass forced in a
// full word; only its low byte is tested.

void __fastcall Unknown_0047F600(CoordStruct coord, int damage, WarheadTypeClass *warhead, int forced, int flags)
{
    if ((unknown_a40b2c != 0 && unknown_a6eb1c >= Unknown_0053D840()) || (flags & 0xF) != 0) {
        if ((char)forced || warhead != 0 && warhead->Bright) {
            int size;
            float light = warhead->CombatLightSize;
            if (light > 0.0f) {
                if (light > 1.0f) {
                    light = 1.0f;
                }
                size = (int)(light * 63.0f);
            } else {
                damage *= 64;
                size = damage >> 8;
                if (size >= 63) {
                    size = 63;
                } else if (size <= 21) {
                    size = 21;
                }
            }
            (new SpotLightClass(coord, size))->unknown_14 |= flags;
        }
    }
}


// void __fastcall Wide_Area_Damage(CoordStruct const &coord, int radius, int rawdamage, TechnoClass *source, WarheadTypeClass *warhead) -- 0x0047F6E0
// TibSun combat.cpp Wide_Area_Damage without the terrain deformation; the cells keep the centre's height.
// The distance from the centre is TibSun's Distance(Coord(x + r, y + r), Coord(r, r)) on 3D coordinates
// (Coord_Distance): its 12-byte difference temporary gives the original's frame layout.

void __fastcall Wide_Area_Damage(CoordStruct const &coord, int radius, int rawdamage, TechnoClass *source, WarheadTypeClass *warhead)
{
    if (warhead == 0) {
        return;
    }

    int cell_radius = (radius + 255) / 256;
    Cell cell = Cell(coord);

    for (int x = -cell_radius; x <= cell_radius; x++) {
        for (int y = -cell_radius; y <= cell_radius; y++) {
            int xpos = cell.X + x;
            int ypos = cell.Y + y;

            if ((unsigned)xpos > 512) {
                continue;
            }
            if ((unsigned)ypos > 512) {
                continue;
            }
            Cell tcell = Cell(xpos, ypos);
            if (!Map.In_Radar(tcell)) {
                continue;
            }

            int dist_from_center = Coord_Distance(CoordStruct(x + cell_radius, y + cell_radius, 0), CoordStruct(cell_radius, cell_radius, 0));
            double damagemod = 1;
            if (dist_from_center > 0) {
                damagemod = 1.0 / ((double)cell_radius / dist_from_center);
            }
            CoordStruct tcoord(tcell.X * 256 + 128, tcell.Y * 256 + 128, coord.Z);
            Unknown_0047E2C0(tcoord, (int)(rawdamage * damagemod), source, warhead, true);
        }
    }
}


// bool __fastcall Calculate_Projectile_Pitch(bool high_arc, int speed, int distance, int height, double gravity, UnknownFacingDir *pitch) -- 0x0047F8B0
// TibSun combat.cpp Calculate_Projectile_Pitch: the angle from Calculate_Projectile_Angle as a
// direction; a flat shot is mirrored when one lepton further needs a smaller angle.

bool __fastcall Unknown_0047F8B0(bool high_arc, int speed, int distance, int height, double gravity, UnknownFacingDir *pitch)
{
    double angle;
    if (Calculate_Projectile_Angle(high_arc, speed, distance, height, gravity, angle)) {
        if (high_arc) {
            UnknownFacingDir dir;
            dir.From_Radian(angle);
            *pitch = dir;
            return true;
        }
        double test;
        Calculate_Projectile_Angle(false, speed, distance + 1, height, gravity, test);
        if (test < angle) {
            UnknownFacingDir dir;
            dir.From_Radian(-angle);
            *pitch = dir;
        } else {
            UnknownFacingDir dir;
            dir.From_Radian(angle);
            *pitch = dir;
        }
        return true;
    }
    return false;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool __fastcall Calculate_Projectile_Angle(bool high_arc, int speed, int distance, int height, double gravity, double &angle) -- 0x0047F9B0
// Adopted from ra2-decomp Calculate_Projectile_Angle (YR 0x0048A9D0).
// The launch angle (radians) reaching distance and height at speed under
// gravity, on the high or the low arc; a target above the shooter takes a
// fixed 45 degrees. False when no arc reaches it.

bool __fastcall Calculate_Projectile_Angle(bool high_arc, int speed, int distance, int height, double gravity, double &angle)
{
    if (height > 0) {
        angle = 0.7853822499999998;
        return true;
    }

    if ((double)distance == 0.0) {
        distance = 0;
    }

    double dx = (double)distance;
    double velocity = (double)speed;
    double dy = (double)height;
    double v = velocity;

    double vsq = v * v;
    double base = vsq - ((double)height * gravity);
    double g2 = gravity * gravity;
    double dx2 = (dx * dx);

    double value = (v * v * vsq)
                 - (2 * vsq * dy * gravity)
                 - (g2 * dx2);

    double denominator = (((dy * dy) / dx2) + 1.0) * 2;

    if (value < 0.0 || denominator == 0.0) {
        return false;
    }

    double numerator = !high_arc ? base + Fast_Sqrt(value)
                                 : base - Fast_Sqrt(value);

    value = numerator / denominator;

    if (value >= 0.0) {
        angle = Fast_Acos_004ba580(Fast_Sqrt(value) / v);    // ra2-decomp fastmath::acos: 0x004BA580 (pi/2 less the 0x004BA550 arc-sine table)
        return true;
    }
    return false;
}


// bool __fastcall Calculate_Projectile_Pitch_Flat(int speed, int distance, int height, double gravity, UnknownFacingDir *pitch) -- 0x0047FB00
// The flat solution only, normalised to [0, 2pi) before the conversion (no direct callers).

bool __fastcall Calculate_Projectile_Pitch_Flat(int speed, int distance, int height, double gravity, UnknownFacingDir *pitch)
{
    double angle;
    if (Calculate_Projectile_Angle(false, speed, distance, height, gravity, angle)) {
        if (angle < 0.0) {
            angle += 6.283185307179586;
        }
        UnknownFacingDir dir;
        dir.From_Radian(angle);
        *pitch = dir;
        return true;
    }
    return false;
}


// int __fastcall Calculate_Projectile_Speed(int range, double gravity) -- 0x0047FB70
// TibSun combat.cpp: sqrt(range * gravity * 1.2).

int __fastcall Unknown_0047FB70(int range, double speed)
{
    return (int)unknown_sqrt_4ba410((double)range * speed * 1.2);
}


// bool __fastcall Is_Projectile_Trajectory_Valid(int speed, int distance, int height, double gravity) -- 0x0047FBA0
// TibSun combat.cpp: whether either ballistic solution is non-negative.

bool __fastcall Is_Projectile_Trajectory_Valid(int speed, int distance, int height, double gravity)
{
    double v = (double)speed;
    double dx = (double)distance;
    if (dx == 0.0) dx = 0.001;
    double dy = (double)height;

    double y2 = dy * dy;
    double vsq = v * v;
    double g2 = gravity * gravity;
    double x2 = dx * dx;

    double discriminant = (v * v * vsq) - (2 * vsq * dy * gravity) - (g2 * x2);

    if (discriminant < 0.0) {
        return false;
    }

    double denominator = 2 * ((y2 / x2) + 1.0);
    if (denominator == 0.0) {
        return false;
    }

    double base = vsq - (dy * gravity);

    if (((base + unknown_sqrt_4ba410(discriminant)) / denominator) >= 0.0 ||
        ((base - unknown_sqrt_4ba410(discriminant)) / denominator) >= 0.0) {
        return true;
    }
    return false;
}


// double Get_Floater_Gravity() -- 0x0047FCD0
// TibSun combat.cpp: half the rules gravity.

double __cdecl Unknown_0047FCD0()
{
    return Rules->unknown_1320 * 0.5;
}


// DynamicVectorClass_DamageGroup_ptr_Clear_0047fcf0: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// DynamicVectorClass_DamageGroup_ptr_ID_pointer_0047fd20: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_Fetch_0047fd40: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_operator_equal_0047fd50: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_Resize_0047fd90: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_Clear_0047fe40: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_ID_object_0047fe70: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_ID_pointer_0047feb0: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// DynamicVectorClass_DamageGroup_ptr_Resize_0047fed0: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// DynamicVectorClass_DamageGroup_ptr_ID_object_0047ff80: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_scalar_deleting_0047ffb0: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// DynamicVectorClass_DamageGroup_ptr_scalar_deleting_00480000: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.


// VectorClass_DamageGroup_ptr_ctor_00480050: emitted by the DamageGroup vector instantiation in Explosion_Damage 0x0047E2C0.
