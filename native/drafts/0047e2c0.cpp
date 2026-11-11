// NON-EXACT DRAFT 0x47E2C0 Explosion_Damage (Unknown_0047E2C0) (4611 B)
// entry 0x47E2C0; exact-match score 97.92 (lane best), this text 97.92 before the native adaptation; plan "draft, verify"
// origin lane: Scout765 (build/class-work/Scout765/w/keep/ed6_9792.cpp, 97.92; Scout767 w/ed7.cpp drops the overlay macro); native adaptation Scout1108
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1108 (native/drafts.json, native M11). Not original bytes.
// int __fastcall Explosion_Damage(CoordStruct const &coord, int strength, TechnoClass *source, WarheadTypeClass *warhead, bool chain) -- 0x0047E2C0
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/rules.hpp"
#include "shared/warhead_type.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
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

// NATIVE adaptation (Scout1108): the lane scored this draft with a private
// overlay (Scout767 hg/shared under RA2_UNIT_EXPLOSION_DAMAGE): the vector's
// inline Reset, Vector3::Normalize (TibSun), and MapClass's low-bridge members
// 0x0055DCA0/0x0055EDB0 on map_view.hpp's Map. They are spelled here as
// file-local functions with the overlay's statements; the two MapClass members
// are reached through link/behaviour_m11_explosion.cpp (the full MapClass).
// CellClass 0x00472540 is the real class's Recalc_Attributes.
template <class T> inline void Vector_Reset(DynamicVectorClass<T> &vector)
{
    int size = vector.Length();
    vector.Clear();
    vector.Resize(size);
}
inline Vector3 Vector3_Normalize(Vector3 const &v)
{
    double length = unknown_sqrt_4ba410(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    if (length != 0.0) {
        return Vector3(v.X / length, v.Y / length, v.Z / length);
    }
    return v;
}
bool Native_Map_Damage_Low_Bridge_0055dca0(Cell const &cell);
bool Native_Map_Damage_Low_Bridge_0055edb0(Cell const &cell);

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

// @ref ??_7?$DynamicVectorClass@PAUDamageGroup@@@@6B@=0079dd88
// @ref ??_7?$VectorClass@PAUDamageGroup@@@@6B@=0079dd5c
// @ref ??0?$VectorClass@PAUDamageGroup@@@@QAE@IPBQAUDamageGroup@@@Z=00480050
// @ref __real@4@4002a000000000000000=0079c4c8
// @ref __real@8@3ffefd70a3d70a3d7000=0079dd78
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

        object = isbridge ? cptr->AltOccupierPtr : cptr->OccupierPtr;
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
                if (object->Kind_Of() == 2 && object->unknown_slot21()) {
                    distance /= 2;
                }
                if (object->Strength > 0 && object->unknown_74 && !object->unknown_76 && distance <= range) {
                    int damage = strength;
                    object->unknown_slot86(&damage, distance, warhead, source, false, 0);
                    hit = true;
                }
            }
        }
    }

    for (int k = 0; k < groups.Count(); k++) {
        delete groups[k];
    }
    Vector_Reset(groups);   // NATIVE: groups.Reset()

    if (blocked) {
        return 2;
    }

    double force = strength * 0.01 < 4.0 ? strength * 0.01 : 4.0;
    if (warhead->Rocker && force > 0.3) {
        for (int x = cell.X - 3; x <= cell.X + 3; x++) {
            for (int y = cell.Y - 3; y <= cell.Y + 3; y++) {
                object = isbridge ? Map[Cell(x, y)].AltOccupierPtr : Map[Cell(x, y)].OccupierPtr;
                for (; object; object = object->unknown_30) {
                    TechnoClass *techno = object->As_Techno();
                    if (techno) {
                        if ((short)x == cell.X && (short)y == cell.Y && source) {
                            CoordStruct tcoord = techno->Coord;
                            CoordStruct scoord = source->Coord;
                            Vector3 dir((float)(scoord.X - tcoord.X), (float)(scoord.Y - tcoord.Y), (float)(scoord.Z - tcoord.Z));
                            dir = Vector3_Normalize(dir) * 10.0f;   // NATIVE: dir.Normalize()
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
                if (Native_Map_Damage_Low_Bridge_0055dca0(cell)) {   // NATIVE: Map.Unknown_0055DCA0(cell)
                    Unknown_006D7C80((AbstractClass *)cptr);
                }
            }
        }
        if (cptr->OverlayIndex >= 0xCD && cptr->OverlayIndex <= 0xE6) {
            if (warhead == Rules->IonCannonWarhead || Random_Pick(1, Rules->unknown_13a8) < strength) {
                if (Native_Map_Damage_Low_Bridge_0055edb0(cell)) {   // NATIVE: Map.Unknown_0055EDB0(cell)
                    Unknown_006D7C80((AbstractClass *)cptr);
                }
            }
        }
    }

    if (cptr->OverlayIndex != -1 && OverlayTypes[cptr->OverlayIndex]->Explodes) {
        cptr->Register_For_Redraw();
        cptr->OverlayIndex = -1;
        cptr->Recalc_Attributes(-1);   // NATIVE: Unknown_00472540
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
