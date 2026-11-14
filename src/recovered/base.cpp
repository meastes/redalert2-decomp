// base.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/base.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
#define BASE_DECLARE_IS_NODE
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define RA2_BASE_NODE_MEMBER_OPERATORS
#include "shared/house_nodes.hpp"
#define VECTOR_IMPLEMENT_ALL
#include "shared/base.hpp"
#include "shared/techno.hpp"
#include "shared/building_type_class.hpp"
#include "shared/building_type_list.hpp"
#include "shared/overlay_type.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/ccini_class.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/swizzle.hpp"
#include "shared/wwcrc_engine.hpp"


// Header-static dynamic initializers of the unit at 0x0042BCF0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0042BCF0-0x0042BED6); the other
// drafts of this plan are emitted by these includes.
// Unit-wide (BaseClass unit 0x0042BCF0): base.hpp's guarded Is_Node declaration.


// Dynamic initializer (0x0042BD20, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BD40, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BD60, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BD80, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BDA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BDC0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BE00, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BE30, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BE60, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BE90, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BEA0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// Dynamic initializer (0x0042BEC0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0042bcf0.cpp; no source of its own.


// BaseNodeClass::operator== (0x0042BEE0; TibSun base.cpp): the type and the cell.

bool BaseNodeClass::operator==(BaseNodeClass const &node)
{
    return Type == node.Type && Cell == node.Cell;
}


// BaseNodeClass::operator!= (0x0042BF10; TibSun base.cpp): the inlined operator== negated.

bool BaseNodeClass::operator!=(BaseNodeClass const &node)
{
    return !(*this == node);
}


// BaseNodeClass::operator> (0x0042BF60; TibSun base.cpp): always true.

bool BaseNodeClass::operator>(BaseNodeClass const &)
{
    return true;
}


// BaseClass::BaseClass() (0x0042BF70)
// Empty node and cell lists, everything else zero.

BaseClass::BaseClass()
    : PercentBuilt(0), unknown_50(0, 0), unknown_54(0, 0, 0, 0), unknown_64(0, 0, 0, 0), House(0)
{
}


// bool BaseClass::Is_Built(int index) const (0x0042C000)
// A node counts as built when its building stands there; a wall node also
// when its cell already holds the wall's overlay or any building (TS BASE.CPP
// Is_Built).

bool BaseClass::Is_Built(int index) const
{
    if (Get_Building(index) != 0) {
        return true;
    }
    if (Nodes[index].Type >= 0) {
        BuildingTypeClass *type = BuildingTypes[Nodes[index].Type];
        if (type->Wall) {
            CellClass &cell = Map[Nodes[index].Cell];
            if (cell.OverlayIndex == type->ToOverlay->ArrayIndex) {
                return true;
            }
            if (cell.Cell_Building() != 0) {
                return true;
            }
        }
    }
    return false;
}


// bool BaseClass::unknown_42c080(int index) const (0x0042C080)
// The node's +0x08 flag.

bool BaseClass::unknown_42c080(int index) const
{
    return Nodes[index].unknown_08;
}


// BuildingClass *BaseClass::Get_Building(int index) const (0x0042C0A0)
// The house's building standing on a node (TS BASE.CPP Get_Building): its
// type, or, for an upgrade node, a building carrying more upgrades of that
// type than the earlier nodes at the same cell account for.

BuildingClass *BaseClass::Get_Building(int index) const
{
    Cell cell = Nodes[index].Cell;
    if ((cell.X != 0 || cell.Y != 0) && Nodes[index].Type >= 0) {
        BuildingTypeClass *type = BuildingTypes[Nodes[index].Type];
        BuildingClass *building = (BuildingClass *)Map[cell].Cell_Building();
        if (building != 0 && building->unknown_slot104() == Nodes[index].Cell && building->House == House) {
            if (building->Type == type) {
                return building;
            }
            if (type->PowersUpBuilding[0]) {
                int count = 0;
                for (int upgrade = 0; upgrade < 3; upgrade++) {
                    if (building->unknown_4c0[upgrade] == type) {
                        count++;
                    }
                }
                for (int previous = 0; previous < index; previous++) {
                    if (Nodes[previous].Type >= 0 && BuildingTypes[Nodes[previous].Type] == type) {
                        CoordStruct coord = building->Coord;
                        if (Nodes[previous].Cell == Coord_Cell(coord)) {
                            count--;
                        }
                    }
                }
                return count > 0 ? building : 0;
            }
        }
    }
    return 0;
}


// bool BaseClass::Is_Node(BuildingClass *building) (0x0042C210)
// Whether a building of this house stands on one of the base's nodes (TibSun Is_Node;
// Get_Node expands inline; the if/else form renumbers labels and moves a load in Read_INI 0x42C460).

bool BaseClass::Is_Node(BuildingClass *building)
{
    if (Get_Node(building) != 0) {
        return true;
    }
    return false;
}


// BaseNodeClass *BaseClass::Get_Node(BuildingClass const *building) (0x0042C2B0)
// The node a building of this house was placed for: same type and cell.

BaseNodeClass *BaseClass::Get_Node(BuildingClass *building)
{
    if (building->House == House) {
        for (int index = 0; index < Nodes.Count(); index++) {
            if (building->Type->ArrayIndex == Nodes[index].Type && building->unknown_slot104() == Nodes[index].Cell) {
                return &Nodes[index];
            }
        }
    }
    return 0;
}


// BaseNodeClass *BaseClass::Next_Buildable(int type) (0x0042C3A0)
// The next node to build (of a type, -1 any), or null.

BaseNodeClass *BaseClass::Next_Buildable(int type)
{
    int index = Next_Buildable_Index(type);
    if (index != -1) {
        return &Nodes[index];
    }
    return 0;
}


// int BaseClass::Next_Buildable_Index(int type) (0x0042C3D0)
// The first node not yet built (of a type, -1 any) or -1. Outside the
// campaign a flagged node is taken only when the house may build it now
// (0x004F64C0), and its cell is reset for a new placement.

int BaseClass::Next_Buildable_Index(int type)
{
    for (int index = 0; index < Nodes.Count(); index++) {
        if (type == -1) {
            if (!Is_Built(index)) {
                if (unknown_game_mode_a3d298 == 0 || !Nodes[index].unknown_08) {
                    return index;
                }
                if (House->unknown_func_4f64c0(Nodes[index])) {
                    Nodes[index].Cell = unknown_geometry_k;
                    return index;
                }
            }
        } else if (Nodes[index].Type == type && !Is_Built(index)) {
            return index;
        }
    }
    return -1;
}


// void BaseClass::Read_INI(CCINIClass &ini, char const *section) (0x0042C460)
// PercentBuilt and NodeCount, then each node "%03d" = type,x,y; the type is a
// BuildingTypes name, or a number when it starts with '-' (TS BASE.CPP Read_INI).
// The node's other two fields are left unset (BaseNodeClass(type, cell)).

void BaseClass::Read_INI(CCINIClass &ini, char const *section)
{
    PercentBuilt = ini.Get_Int(section, "PercentBuilt", PercentBuilt);
    int count = ini.Get_Int(section, "NodeCount", 0);
    for (int index = 0; index < count; index++) {
        char key[10];
        char buffer[128];
        sprintf(key, "%03d", index);
        ini.Get_String(section, key, 0, buffer, sizeof(buffer));
        int type;
        if (buffer[0] == '-') {
            type = atoi(strtok(buffer, ","));
        } else {
            type = BuildingTypeClass::From_Name(strtok(buffer, ","));
        }
        Cell cell;
        cell.X = (short)atoi(strtok(0, ","));
        cell.Y = (short)atoi(strtok(0, ","));
        Nodes.Add(BaseNodeClass(type, cell));
    }
}


// void BaseClass::Write_INI(CCINIClass &ini, char const *section) (0x0042C5D0)
// PercentBuilt, NodeCount and each node "%03d" = type ID (or number),x,y.

void BaseClass::Write_INI(CCINIClass &ini, char const *section)
{
    ini.Put_Int(section, "PercentBuilt", PercentBuilt, 0);
    ini.Put_Int(section, "NodeCount", Nodes.Count(), 0);
    for (int index = 0; index < Nodes.Count(); index++) {
        char key[12];
        char buffer[128];
        sprintf(key, "%03d", index);
        if (Nodes[index].Type >= 0) {
            sprintf(buffer, "%s,%d,%d", (char const *)BuildingTypes[Nodes[index].Type]->ID, Nodes[index].Cell.X, Nodes[index].Cell.Y);
        } else {
            sprintf(buffer, "%d,%d,%d", Nodes[index].Type, Nodes[index].Cell.X, Nodes[index].Cell.Y);
        }
        ini.Put_String(section, key, buffer);
    }
}


// HRESULT __stdcall BaseClass::Load(IStream *stream) (0x0042C6A0)
// Restores the plan in place (the NoInit constructor), then each list from
// its count and elements, and swizzles the house. Every path answers 0.

HRESULT __stdcall BaseClass::Load(IStream *stream)
{
    HRESULT result = stream->Read(this, sizeof(*this), 0);
    if (FAILED(result)) {
        return 0;
    }
    new (this) BaseClass(NoInitClass());
    int count = 0;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return 0;
    }
    for (int index = 0; index < count; index++) {
        BaseNodeClass node;
        result = stream->Read(&node, sizeof(node), 0);
        if (FAILED(result)) {
            return 0;
        }
        Nodes.Add(node);
    }
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return 0;
    }
    for (index = 0; index < count; index++) {
        Cell cell;
        result = stream->Read(&cell, sizeof(cell), 0);
        if (FAILED(result)) {
            return 0;
        }
        unknown_20.Add(cell);
    }
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return 0;
    }
    for (index = 0; index < count; index++) {
        Cell cell;
        result = stream->Read(&cell, sizeof(cell), 0);
        if (FAILED(result)) {
            return 0;
        }
        unknown_38.Add(cell);
    }
    Swizzler.Swizzle((void **)&House);
    return 0;
}


// HRESULT BaseClass::Save(IStream *stream) (0x0042C8E0)
// The object's bytes, then each list as a count and its elements. A failed
// write answers 0 (the original does not pass the error on).

HRESULT __stdcall BaseClass::Save(IStream *stream)
{
    HRESULT result = stream->Write(this, sizeof(*this), 0);
    if (FAILED(result)) {
        return 0;
    }
    int count = Nodes.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return 0;
    }
    for (int index = 0; index < count; index++) {
        result = stream->Write(&Nodes[index], sizeof(BaseNodeClass), 0);
        if (FAILED(result)) {
            return 0;
        }
    }
    count = unknown_20.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return 0;
    }
    for (index = 0; index < count; index++) {
        result = stream->Write(&unknown_20[index], sizeof(Cell), 0);
        if (FAILED(result)) {
            return 0;
        }
    }
    count = unknown_38.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return 0;
    }
    for (index = 0; index < count; index++) {
        result = stream->Write(&unknown_38[index], sizeof(Cell), 0);
        if (FAILED(result)) {
            return 0;
        }
    }
    return result;
}


// void BaseClass::Compute_CRC(WWCRCEngine &crc) const (0x0042C9F0)
// The node count, then each node's type and cell.

void BaseClass::Compute_CRC(WWCRCEngine &crc) const
{
    crc((long)Nodes.Count());
    for (int index = 0; index < Nodes.Count(); index++) {
        crc((long)Nodes[index].Type);
        crc(Nodes[index].Cell.X);
        crc(Nodes[index].Cell.Y);
    }
}


// BaseClass::BaseClass(NoInitClass const &) (0x0042CA50)
// The Load restore: rebuilds the three lists empty and keeps the other fields.

BaseClass::BaseClass(NoInitClass const &noinit)
{
}


// void BaseClass::unknown_42cad0(BuildingClass *building) (0x0042CAD0)
// Marks the node of a building (+0x08 set, +0x0C cleared).

void BaseClass::unknown_42cad0(BuildingClass *building)
{
    BaseNodeClass *node = Get_Node(building);
    if (node != 0) {
        node->unknown_08 = true;
        node->unknown_0c = 0;
    }
}


// int BaseClass::unknown_42cb70(BaseNodeClass *node) (0x0042CB70)
// Counts on the node's +0x0C (0 without a node).

int BaseClass::unknown_42cb70(BaseNodeClass *node)
{
    if (node == 0) {
        return 0;
    }
    return ++node->unknown_0c;
}


extern "C" unsigned char vtable_42d160[]; extern "C" void __cdecl release_buffer_42d160(void*); extern "C" void __cdecl release_object_42d160(void*);
class Leaf_42d160 { public: void* get(unsigned int flags); };
void* Leaf_42d160::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42d160; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42d160((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42d160(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<BaseNodeClass>::Clear (0x0042CB90; DynamicVectorClass<BaseNodeClass> vtable 0x0079c5c0 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<BaseNodeClass>::ID_pointer (0x0042CBC0; DynamicVectorClass<BaseNodeClass> vtable 0x0079c5c0 slot 5).
// BaseNodeClass is a size-only stand-in (16 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<BaseNodeClass>::Fetch (0x0042CBE0).
// DynamicVectorClass<Cell>::Clear (0x0042CC10; DynamicVectorClass<Cell> vtable 0x0079c5a0 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<Cell>::ID_pointer (0x0042CC40; DynamicVectorClass<Cell> vtable 0x0079c5a0 slot 5).
// Cell is a size-only stand-in (4 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<Cell>::Fetch (0x0042CC60; VectorClass<Cell> vtable 0x0079c5e0 slot 6).
// Cell is a size-only stand-in (4 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<BaseNodeClass>::operator== (0x0042CC80).
// DynamicVectorClass<BaseNodeClass>::Resize (0x0042CCE0).
// DynamicVectorClass<BaseNodeClass>::ID_object (0x0042CDD0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; BaseNodeClass comparison: shared/house_nodes.hpp.
// VectorClass<Cell>::operator== (0x0042CE10).
// VectorClass<Cell>::Resize (0x0042CEE0).
// VectorClass<Cell>::Clear (0x0042CFB0; VectorClass<Cell> vtable 0x0079c5e0 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<Cell>::ID_object (0x0042CFE0; VectorClass<Cell> vtable 0x0079c5e0 slot 4).
// Layout and behaviour: shared/vector_class.hpp; Cell's member-wise comparison: shared/coord.hpp.
// VectorClass<Cell>::ID_pointer (0x0042D030; VectorClass<Cell> vtable 0x0079c5e0 slot 5).
// Cell is a size-only stand-in (4 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<Cell>::Resize (0x0042D050).
// DynamicVectorClass<Cell>::ID_object (0x0042D120; DynamicVectorClass<Cell> vtable 0x0079c5a0 slot 4).
// Layout and behaviour: shared/vector_class.hpp; Cell's member-wise comparison: shared/coord.hpp.
// VectorClass<BaseNodeClass>::Resize (0x0042D1B0).
// VectorClass<BaseNodeClass>::Clear (0x0042D2A0; VectorClass<BaseNodeClass> vtable 0x0079c600 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<BaseNodeClass>::ID_object (0x0042D2D0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; BaseNodeClass comparison: shared/house_nodes.hpp.
// VectorClass<BaseNodeClass>::ID_pointer (0x0042D320; VectorClass<BaseNodeClass> vtable 0x0079c600 slot 5).
// BaseNodeClass is a size-only stand-in (16 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.


extern "C" unsigned char vtable_42d340[]; extern "C" void __cdecl release_buffer_42d340(void*); extern "C" void __cdecl release_object_42d340(void*);
class Leaf_42d340 { public: void* get(unsigned int flags); };
void* Leaf_42d340::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42d340; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42d340((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42d340(this); return this; }


extern "C" unsigned char vtable_42d390[]; extern "C" void __cdecl release_buffer_42d390(void*); extern "C" void __cdecl release_object_42d390(void*);
class Leaf_42d390 { public: void* get(unsigned int flags); };
void* Leaf_42d390::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42d390; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42d390((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42d390(this); return this; }


extern "C" unsigned char vtable_42d3e0[]; extern "C" void __cdecl release_buffer_42d3e0(void*); extern "C" void __cdecl release_object_42d3e0(void*);
class Leaf_42d3e0 { public: void* get(unsigned int flags); };
void* Leaf_42d3e0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_42d3e0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_42d3e0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_42d3e0(this); return this; }
