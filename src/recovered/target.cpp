// target.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/target.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/object_index.hpp"
#include "shared/building.hpp"
#include "shared/unit.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/aircraft.hpp"
#include "shared/infantry.hpp"
#include "shared/terrain.hpp"
#include "shared/bullet.hpp"
#include "shared/anim.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/trigger.hpp"
#include "shared/trigger_type.hpp"
#include "shared/tag.hpp"
#include "shared/tag_type.hpp"
#include "shared/house.hpp"
#include "shared/techno_type.hpp"
#include "shared/target.hpp"


// Header-static dynamic initializers of the unit at 0x006B5250:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006B5250-0x006B5436); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006B5280, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B52A0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B52C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B52E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5300, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5320, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5360, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5390, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B53C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B53F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5400, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer (0x006B5420, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006b5250.cpp; no source of its own.


// Dynamic initializer of the object index (0x006B5440, _$E): zeroes
// unknown_object_index_abff58 (0x00ABFF58) and registers its atexit destructor
// (0x006B5470, the following _$E draft, emitted by this definition).


IndexClass<long, AbstractClass *> unknown_object_index_abff58;


// atexit destructor of unknown_object_index_abff58 (0x006B5470, _$E): emitted by
// the definition in unknown_object_index_abff58_init_006b5440.cpp; no source of its own.


// TargetClass (Target.cpp, 0x006B54A0-0x006B6654; layout: shared/target.hpp):
// a reference to a game object by kind and ID. Kind 11 is a cell (ID = X + Y *
// 1000); kind 0x34 an object in the object index (shared/object_index.hpp).

// TargetClass::TargetClass(AbstractClass *object) (0x006B54A0): a cell by its
// position, any other object by its ID; null gives no target.

TargetClass::TargetClass(AbstractClass *object)
{
    if (object != 0) {
        int kind = object->Kind_Of();
        if (kind == 11) {
            Kind = kind;
            Cell cell = ((CellClass *)object)->MapCoords;
            ID = cell.X + cell.Y * 1000;
        } else {
            Kind = 0x34;
            ID = object->Fetch_ID();
        }
    } else {
        Kind = 0;
        ID = 0;
    }
}


// TargetClass::TargetClass(Cell const &cell) (0x006B5510): the cell; the zero
// cell (shared/unknown_geometry.hpp, this unit's copy) gives no target.

TargetClass::TargetClass(Cell const &cell)
{
    if (cell == unknown_geometry_k) {
        Kind = 0;
    } else {
        Kind = 11;
        ID = cell.X + cell.Y * 1000;
    }
}


// TargetClass::TargetClass(CoordStruct const &coord) (0x006B5560): the cell
// holding the coordinate.

TargetClass::TargetClass(CoordStruct const &coord)
{
    Kind = 11;
    ID = coord.X / 256 + coord.Y / 256 * 1000;
}


// AbstractTypeClass *TargetClass::As_AbstractType() const (0x006B55A0): the indexed object if it is a AbstractTypeClass, or null.

AbstractTypeClass *TargetClass::As_AbstractType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AbstractTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TagClass *TargetClass::As_Tag() const (0x006B5670): the indexed object if it is a TagClass, or null.

TagClass *TargetClass::As_Tag() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TagClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TagTypeClass *TargetClass::As_TagType() const (0x006B5740): the indexed object if it is a TagTypeClass, or null.

TagTypeClass *TargetClass::As_TagType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TagTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// AbstractClass *TargetClass::As_Abstract() const (0x006B5810): the indexed
// object or the cell, or null.

AbstractClass *TargetClass::As_Abstract() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AbstractClass *>(unknown_object_index_abff58[ID]);
    }
    if (Kind == 11) {
        return &Map[Cell(ID % 1000, ID / 1000)];
    }
    return 0;
}


// TechnoClass *TargetClass::As_Techno() const (0x006B5910): the indexed object
// as a techno (AbstractClass::As_Techno), or null.

TechnoClass *TargetClass::As_Techno() const
{
    if (Kind == 0x34) {
        return unknown_object_index_abff58[ID]->As_Techno();
    }
    return 0;
}


// ObjectClass *TargetClass::As_Object() const (0x006B59E0): the indexed object if it is a ObjectClass, or null.

ObjectClass *TargetClass::As_Object() const
{
    if (Kind == 0x34) {
        return dynamic_cast<ObjectClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// FootClass *TargetClass::As_Foot() const (0x006B5AB0): the indexed object if it is a FootClass, or null.

FootClass *TargetClass::As_Foot() const
{
    if (Kind == 0x34) {
        return dynamic_cast<FootClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TriggerClass *TargetClass::As_Trigger() const (0x006B5B80): the indexed object if it is a TriggerClass, or null.

TriggerClass *TargetClass::As_Trigger() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TriggerClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// HouseClass *TargetClass::As_House() const (0x006B5C50): the indexed object if it is a HouseClass, or null.

HouseClass *TargetClass::As_House() const
{
    if (Kind == 0x34) {
        return dynamic_cast<HouseClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TechnoTypeClass *TargetClass::As_TechnoType() const (0x006B5D20): the indexed object if it is a TechnoTypeClass, or null.

TechnoTypeClass *TargetClass::As_TechnoType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TechnoTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TriggerTypeClass *TargetClass::As_TriggerType() const (0x006B5DF0): the indexed object if it is a TriggerTypeClass, or null.

TriggerTypeClass *TargetClass::As_TriggerType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TriggerTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TeamTypeClass *TargetClass::As_TeamType() const (0x006B5EC0): the indexed object if it is a TeamTypeClass, or null.

TeamTypeClass *TargetClass::As_TeamType() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TeamTypeClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TerrainClass *TargetClass::As_Terrain() const (0x006B5F90): the indexed object if it is a TerrainClass, or null.

TerrainClass *TargetClass::As_Terrain() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TerrainClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// BulletClass *TargetClass::As_Bullet() const (0x006B6060): the indexed object if it is a BulletClass, or null.

BulletClass *TargetClass::As_Bullet() const
{
    if (Kind == 0x34) {
        return dynamic_cast<BulletClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// AnimClass *TargetClass::As_Anim() const (0x006B6130): the indexed object if it is a AnimClass, or null.

AnimClass *TargetClass::As_Anim() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AnimClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// TeamClass *TargetClass::As_Team() const (0x006B6200): the indexed object if it is a TeamClass, or null.

TeamClass *TargetClass::As_Team() const
{
    if (Kind == 0x34) {
        return dynamic_cast<TeamClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// InfantryClass *TargetClass::As_Infantry() const (0x006B62D0): the indexed object if it is a InfantryClass, or null.

InfantryClass *TargetClass::As_Infantry() const
{
    if (Kind == 0x34) {
        return dynamic_cast<InfantryClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// UnitClass *TargetClass::As_Unit() const (0x006B63A0): the indexed object if it is a UnitClass, or null.

UnitClass *TargetClass::As_Unit() const
{
    if (Kind == 0x34) {
        return dynamic_cast<UnitClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// BuildingClass *TargetClass::As_Building() const (0x006B6470): the indexed object if it is a BuildingClass, or null.

BuildingClass *TargetClass::As_Building() const
{
    if (Kind == 0x34) {
        return dynamic_cast<BuildingClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// AircraftClass *TargetClass::As_Aircraft() const (0x006B6540): the indexed object if it is a AircraftClass, or null.

AircraftClass *TargetClass::As_Aircraft() const
{
    if (Kind == 0x34) {
        return dynamic_cast<AircraftClass *>(unknown_object_index_abff58[ID]);
    }
    return 0;
}


// CellClass *TargetClass::As_Cell() const (0x006B6610): the cell, or null.

CellClass *TargetClass::As_Cell() const
{
    if (Kind == 11) {
        return &Map[Cell(ID % 1000, ID / 1000)];
    }
    return 0;
}
