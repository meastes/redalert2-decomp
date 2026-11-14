// tiberium.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tiberium.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/tiberium.hpp"
#include "shared/ccini_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/abstract.hpp"
#include "shared/veinhole.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/dir_type.hpp"
#include "shared/incdec.hpp"
#include "shared/crt_memory.hpp"
#include "shared/special.hpp"


// Header-static dynamic initializers of the unit at 0x006E8730 and the
// unit's TiberiumClass list; the other drafts of this plan are emitted by this file.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif

// The unit's list of TiberiumClass objects (0x00AC0B30; name is a hypothesis).
DynamicVectorClass<TiberiumClass *> unknown_TiberiumClass_list_00ac0b30;


// Dynamic initializer (0x006E8760, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8780, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E87A0, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E87C0, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E87E0, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8800, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8840, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8870, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E88A0, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E88D0, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E88E0, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8900, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8920, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// Dynamic initializer (0x006E8960, _$E) emitted by HeaderStatics_diagonal_init_006e8730.cpp; no source of its own.


// TiberiumClass::TiberiumClass(char const *ini_name) (0x006E89A0)

TiberiumClass::TiberiumClass(char const *ini_name)
    : AbstractTypeClass(ini_name),
      ArrayIndex(-1),
      Spread(0),
      unknown_a0(0.1),
      Growth(0),
      unknown_b0(0.1),
      Value(0),
      Power(0),
      unknown_c0(0),
      Image(0),
      unknown_e4(0),
      NumFrames(0),
      NumImages(0),
      SpreadCount(0),
      SpreadQueue(0),
      SpreadMap(0),
      SpreadData(0),
      GrowthCount(0),
      GrowthQueue(0),
      GrowthMap(0),
      GrowthData(0)
{
    ArrayIndex = unknown_TiberiumClass_list_00ac0b30.Count();
    unknown_TiberiumClass_list_00ac0b30.Add(this);
    Debris.Clear();
    unknown_objects_ac0cb0.Add(this);
}


// TiberiumClass::~TiberiumClass() (0x006E8B60)

TiberiumClass::~TiberiumClass()
{
    Detach_This_From_All(this, true);
    unknown_objects_ac0cb0.Delete(this);
    unknown_TiberiumClass_list_00ac0b30.Delete(this);
    delete SpreadData;
    SpreadData = 0;
    delete SpreadMap;
    SpreadMap = 0;
    delete SpreadQueue;
    SpreadQueue = 0;
    if (GrowthQueue) {
        GrowthQueue->Clear();
        delete GrowthQueue;
        GrowthQueue = 0;
    }
    if (GrowthData) {
        delete GrowthData;
        GrowthData = 0;
    }
    if (GrowthMap) {
        delete GrowthMap;
        GrowthMap = 0;
    }
    GrowthCount = 0;
}


// bool TiberiumClass::Read_INI(CCINIClass &ini) (0x006E8D30, slot 25)
// Spread and growth rates, value, power, colour and debris animations, then
// the overlay image set chosen by "Image=" (-1 keeps the current one).
// Assumptions: overlay indices 27/127/147/102 are the first images of the
// four ore/gem/vein families; field names stay hypotheses.

bool TiberiumClass::Read_INI(CCINIClass &ini)
{
    if (AbstractTypeClass::Read_INI(ini)) {
        Spread = ini.Get_Int(ID, "Spread", Spread);
        unknown_a0 = ini.Get_Double(ID, "SpreadPercentage", unknown_a0);
        Growth = ini.Get_Int(ID, "Growth", Growth);
        unknown_b0 = ini.Get_Double(ID, "GrowthPercentage", unknown_b0);
        Value = ini.Get_Int(ID, "Value", Value);
        Power = ini.Get_Int(ID, "Power", Power);
        unknown_c0 = ini.Get_ColorScheme(ID, "Color", unknown_c0);
        Debris = TypeListReader<AnimTypeClass>::Get(ini, ID, "Debris", Debris);
        switch (ini.Get_Int(ID, "Image", -1)) {
            case -1:
                break;

            case 2:
                Image = OverlayTypes[27];
                unknown_e4 = 12;
                NumFrames = 12;
                break;

            case 3:
                Image = OverlayTypes[127];
                unknown_e4 = 12;
                NumFrames = 12;
                NumImages = 8;
                break;

            case 4:
                Image = OverlayTypes[147];
                unknown_e4 = 12;
                NumFrames = 12;
                NumImages = 8;
                break;

            default:
                Image = OverlayTypes[102];
                unknown_e4 = 12;
                NumFrames = 12;
                NumImages = 8;
                break;
        }
        return true;
    }
    return false;
}


// TiberiumClass::Read_Types (0x006E8FF0)
// Reads the [Tiberiums] list of the rules INI: each entry's value is a type ID,
// found or made at the position the entry's key number names, then read.

bool __fastcall TiberiumClass::Read_Types(CCINIClass &ini)
{
    int count = ini.Entry_Count("Tiberiums");
    for (int index = 0; index < count; index++) {
        char const *key = ini.Get_Entry("Tiberiums", index);
        char id[24];
        if (ini.Get_String("Tiberiums", key, "", id, sizeof(id)) > 0) {
            int number = atoi(key);
            TiberiumClass *type;
            if (number < unknown_TiberiumClass_list_00ac0b30.Count()) {
                type = unknown_TiberiumClass_list_00ac0b30[number];
            } else {
                type = new TiberiumClass(id);
            }
            type->Read_INI(ini);
        }
    }
    return true;
}


// TiberiumClass::Compute_CRC (0x006E90A0, slot 13): the AbstractTypeClass part, then the spread and growth rates,
// value, power, colour scheme and the image fields (TibSun tiberium.cpp: SpreadDelay, GrowthDelay, CreditValue,
// Power, Color, FrameCount, Variety).

void TiberiumClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)Spread);
    crc((long)Growth);
    crc((long)Value);
    crc((long)Power);
    crc((long)unknown_c0);
    crc((long)unknown_e4);
    crc((long)NumFrames);
}


// HRESULT TiberiumClass::Load(IStream *stream) (0x006E9160)
// Drops the spread and growth queues and the debris list, loads the base type, restores the object
// in place and the debris list, fixes up the pointers and leaves the queues to be rebuilt (TibSun
// tiberium.cpp).

HRESULT __stdcall TiberiumClass::Load(IStream *stream)
{
    Clear_Spread();
    Clear_Growth();
    Debris.Clear();

    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TiberiumClass(NoInitClass());
        Swizzle_Pointer(Image);
        Debris.Load(stream);
        for (int index = 0; index < Debris.Count(); index++) {
            Swizzle_Pointer(Debris[index]);
        }
        GrowthQueue = 0;
        GrowthMap = 0;
        GrowthData = 0;
        SpreadQueue = 0;
        SpreadMap = 0;
        SpreadData = 0;
        result = S_OK;
    }
    return result;
}


// 0x006e9380: function not yet identified
//
// Owner: TiberiumClass, virtual slot 7 (vtable +0x1c) (from RTTI/vtables).
// Body: 47 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: int GetSizeMax_006e9380(int param_1,int *param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006e9380 {

extern unsigned int __stdcall AbstractClass_GetSizeMax_00410180(int *, int *);

int __stdcall GetSizeMax_006e9380(int self, int *size)
{
    int result;

    result = (int)(AbstractClass_GetSizeMax_00410180((int *)(self), (int *)(size)));
    if (result >= 0) {
        *size = *size + *(int *)(self + 0xd4) * 4 + 4;
        result = (int)(0);
    }
    return result;
}

}  // namespace seed_006e9380


// TiberiumClass::Save (0x006E93B0)

HRESULT __stdcall TiberiumClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    Debris.Save(stream);
    return S_OK;
}


// TiberiumClass::Detach (0x006E9420)

void TiberiumClass::Detach(AbstractClass *target, bool all)
{
    for (int index = Debris.Count() - 1; index >= 0; index--) {
        if (Debris[index] == (AnimTypeClass *)target) {
            Debris.Delete(index);
        }
    }
    if (Image == (OverlayTypeClass *)target) {
        Image = 0;
    }
}


// void TiberiumClass::Spread_AI() (0x006E9490)
// While the scenario lets tiberium spread, each type whose spread timer has run out spreads once and
// restarts the timer at its Spread rate.

void TiberiumClass::Spread_AI()
{
    if (unknown_id_source_a3d290->unknown_222a) {
        for (int index = 0; index < unknown_TiberiumClass_list_00ac0b30.Count(); index++) {
            if (unknown_TiberiumClass_list_00ac0b30[index]->SpreadTimer == 0) {
                unknown_TiberiumClass_list_00ac0b30[index]->Process_Spread();
                unknown_TiberiumClass_list_00ac0b30[index]->SpreadTimer =
                    CDTimerClass<FrameTimerClass>(unknown_TiberiumClass_list_00ac0b30[index]->Spread);
            }
        }
    }
}


// void TiberiumClass::Init_Spread() (0x006E9520)
// Rebuilds the spread queue of every tiberium type for the current map (Init_Spread_Queue's body, written
// out: the original expands that member here and keeps its unreferenced copy after this function).

void TiberiumClass::Init_Spread()
{
    for (int index = 0; index < unknown_TiberiumClass_list_00ac0b30.Count(); index++) {
        TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[index];
        if (tiberium->SpreadQueue != 0) {
            tiberium->SpreadQueue->Clear();
            delete tiberium->SpreadQueue;
            tiberium->SpreadQueue = 0;
        }
        if (tiberium->SpreadData != 0) {
            delete[] tiberium->SpreadData;
            tiberium->SpreadData = 0;
        }
        if (tiberium->SpreadMap != 0) {
            delete[] tiberium->SpreadMap;
            tiberium->SpreadMap = 0;
        }
        tiberium->SpreadCount = 0;
        tiberium->SpreadData = new UnknownVeinNode[unknown_cells_428ac0()];
        tiberium->SpreadMap = new bool[unknown_cells_428ac0()];
        tiberium->SpreadQueue = new UnknownVeinHeap(unknown_cells_428ac0());
        tiberium->Rebuild_Spread();
    }
}


// 0x006e9670: function not yet identified
//
// Body: 172 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: void Deinit_Tiberium_Spread_System_006e9670(void)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_006e9670 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int Tiberiums_Vector_00ac0b34;
extern "C" int Tiberiums_ActiveCount_00ac0b40;

void  Deinit_Tiberium_Spread_System_006e9670(void)
{
    int tiberium;
    int *queue;
    int slot;
    int index;

    index = (int)(0);
    if (0 < Tiberiums_ActiveCount_00ac0b40) {
        do {
            tiberium = (int)(*(int *)(Tiberiums_Vector_00ac0b34 + index * 4));
            queue = (int *)(*(int **)(tiberium + 0xf4));
            if (queue != (int *)0x0) {
                slot = (int)(0);
                if (*queue >= 0) {
                    do {
                        slot = (int)(slot + 1);
                        *(unsigned int *)(queue[2] + -4 + slot * 4) = 0;
                    } while (slot <= *queue);
                }
                *queue = 0;
                slot = (int)(*(int *)(tiberium + 0xf4));
                if (slot != 0) {
                    operator_delete_00781d3b((unsigned int)(*(unsigned int *)(slot + 8)));
                    operator_delete_00781d3b((unsigned int)(slot));
                }
                *(unsigned int *)(tiberium + 0xf4) = 0;
            }
            if (*(int *)(tiberium + 0xfc) != 0) {
                operator_delete_00781d3b((unsigned int)(*(int *)(tiberium + 0xfc)));
                *(unsigned int *)(tiberium + 0xfc) = 0;
            }
            if (*(int *)(tiberium + 0xf8) != 0) {
                operator_delete_00781d3b((unsigned int)(*(int *)(tiberium + 0xf8)));
                *(unsigned int *)(tiberium + 0xf8) = 0;
            }
            *(unsigned int *)(tiberium + 0xf0) = 0;
            index = (int)(index + 1);
        } while (index < Tiberiums_ActiveCount_00ac0b40);
    }
    return;
}

}  // namespace seed_006e9670


// void TiberiumClass::Process_Spread() (0x006E9720)
// Spreads ore from up to 5-25 queued cells (a random share of the queue's SpreadPercentage); a cell
// with more than one open neighbour is queued again at score 0 (TibSun Spread_AI).

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

void TiberiumClass::Process_Spread()
{
    if (SpreadQueue != 0 && SpreadQueue->Count != 0 && unknown_a0 > 0.00001) {
        int count = MIN(25, MAX(5, (int)(SpreadQueue->Count * unknown_a0)));
        count = abs(unknown_id_source_a3d290->Random()()) % count + 1;
        if (SpreadQueue->Active_Count() > unknown_cells_428ac0() - 20) {
            Rebuild_Spread();
        }
        int index = 0;
        UnknownVeinNode *node = SpreadQueue->Remove_Min();
        while (index < count && node != 0) {
            CellClass *cellptr = &Map[node->Position];
            int possible = 0;
            for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
                CellClass &adjacent = cellptr->Adjacent_Cell(facing);
                if (adjacent.Can_Tiberium_Germinate(0)) {
                    possible++;
                }
            }
            if (possible != 0) {
                cellptr->Spread_Tiberium(false);
                index++;
                if (possible > 1) {
                    SpreadData[SpreadCount].Position = cellptr->MapCoords;
                    SpreadData[SpreadCount].Key = 0.0f;
                    SpreadQueue->Insert(&SpreadData[SpreadCount++]);
                    SpreadMap[unknown_cell_index_428a90(cellptr->MapCoords)] = true;
                }
            } else {
                SpreadMap[unknown_cell_index_428a90(cellptr->MapCoords)] = false;
            }
            if (index < count) {
                node = SpreadQueue->Remove_Min();
            }
        }
    }
}


// void TiberiumClass::Init_Spread_Queue() (0x006E9A50)
// Drops this type's spread queue and tables, allocates them for the current map (one entry and one
// flag per cell) and refills them. Init_Spread expands it (this copy is never called).

void TiberiumClass::Init_Spread_Queue()
{
    if (SpreadQueue != 0) {
        SpreadQueue->Clear();
        delete SpreadQueue;
        SpreadQueue = 0;
    }
    if (SpreadData != 0) {
        delete[] SpreadData;
        SpreadData = 0;
    }
    if (SpreadMap != 0) {
        delete[] SpreadMap;
        SpreadMap = 0;
    }
    SpreadCount = 0;
    SpreadData = new UnknownVeinNode[unknown_cells_428ac0()];
    SpreadMap = new bool[unknown_cells_428ac0()];
    SpreadQueue = new UnknownVeinHeap(unknown_cells_428ac0());
    Rebuild_Spread();
}


// void TiberiumClass::Rebuild_Spread() (0x006E9B90)
// Queues every cell of the map that holds this tiberium and can spread, at score 0.

void TiberiumClass::Rebuild_Spread()
{
    SpreadCount = 0;
    SpreadQueue->Clear();
    for (int index = unknown_cells_428ac0() - 1; index >= 0; index--) {
        SpreadMap[index] = false;
    }
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        if (cell->Tiberium_Type_Here() == ArrayIndex && cell->Can_Tiberium_Spread()) {
            SpreadData[SpreadCount].Position = cell->MapCoords;
            SpreadData[SpreadCount].Key = 0.0f;
            SpreadQueue->Insert(&SpreadData[SpreadCount++]);
            SpreadMap[unknown_cell_index_428a90(cell->MapCoords)] = true;
        }
    }
}


// 0x006e9d00: function not yet identified
//
// Body: 135 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Clear_Spread_006e9d00(int param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006e9d00 {

extern void  operator_delete_00781d3b(unsigned int);

void __fastcall Clear_Spread_006e9d00(int self)
{
    int *queue;
    int slot;

    queue = (int *)(*(int **)(self + 0xf4));
    if (queue != (int *)0x0) {
        slot = (int)(0);
        if (*queue >= 0) {
            do {
                slot = (int)(slot + 1);
                *(unsigned int *)(queue[2] + -4 + slot * 4) = 0;
            } while (slot <= *queue);
        }
        *queue = 0;
        slot = (int)(*(int *)(self + 0xf4));
        if (slot != 0) {
            operator_delete_00781d3b((unsigned int)(*(unsigned int *)(slot + 8)));
            operator_delete_00781d3b((unsigned int)(slot));
        }
        *(unsigned int *)(self + 0xf4) = 0;
    }
    if (*(int *)(self + 0xfc) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0xfc)));
        *(unsigned int *)(self + 0xfc) = 0;
    }
    if (*(int *)(self + 0xf8) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0xf8)));
        *(unsigned int *)(self + 0xf8) = 0;
    }
    *(unsigned int *)(self + 0xf0) = 0;
    return;
}

}  // namespace seed_006e9d00


// void __fastcall TiberiumClass::Clear_Spread_Cell(Cell const &cell) (0x006E9D90)
// Marks the cell as no longer queued for spread in every tiberium type.

void __fastcall TiberiumClass::Clear_Spread_Cell(Cell const &cell)
{
    int index = unknown_cell_index_428a90(cell);
    for (int type = 0; type < unknown_TiberiumClass_list_00ac0b30.Count(); type++) {
        unknown_TiberiumClass_list_00ac0b30[type]->SpreadMap[index] = false;
    }
}


// void TiberiumClass::Add_Spread_Cell(Cell const &cell) (0x006E9DD0)
// Queues a cell that can spread and is not queued yet, at the current frame plus a random 0-49
// (rebuilding the queue first when its table is nearly full).

void TiberiumClass::Add_Spread_Cell(Cell const &cell)
{
    int index = unknown_cell_index_428a90(cell);
    if (Map[cell].Can_Tiberium_Spread() && !SpreadMap[index]) {
        if (SpreadCount >= unknown_cells_428ac0() - 20) {
            Rebuild_Spread();
        }
        SpreadData[SpreadCount].Position = cell;
        SpreadData[SpreadCount].Key = abs(unknown_id_source_a3d290->Random()()) % 50 + Frame;
        SpreadQueue->Insert(&SpreadData[SpreadCount++]);
        SpreadMap[unknown_cell_index_428a90(cell)] = true;
    }
}


// void TiberiumClass::Growth_AI() (0x006E9F20)
// While the scenario lets tiberium spread, each type whose growth timer has run out grows once and
// restarts the timer at its Growth rate (0.3 of it with the TiberiumGrows option).

void TiberiumClass::Growth_AI()
{
    if (unknown_id_source_a3d290->unknown_222a) {
        for (int index = 0; index < unknown_TiberiumClass_list_00ac0b30.Count(); index++) {
            if (unknown_TiberiumClass_list_00ac0b30[index]->GrowthTimer == 0) {
                unknown_TiberiumClass_list_00ac0b30[index]->Process_Growth();
                double factor = unknown_id_source_a3d290->Special().TiberiumGrows ? 0.3 : 1.0;
                unknown_TiberiumClass_list_00ac0b30[index]->GrowthTimer =
                    CDTimerClass<FrameTimerClass>(unknown_TiberiumClass_list_00ac0b30[index]->Growth * factor);
            }
        }
    }
}


// void TiberiumClass::Init_Growth() (0x006E9FE0)
// Rebuilds the growth queue of every tiberium type for the current map (Init_Growth_Queue's body, written
// out: the original expands that member here and keeps its unreferenced copy after this function).

void TiberiumClass::Init_Growth()
{
    for (int index = 0; index < unknown_TiberiumClass_list_00ac0b30.Count(); index++) {
        TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[index];
        if (tiberium->GrowthQueue != 0) {
            tiberium->GrowthQueue->Clear();
            delete tiberium->GrowthQueue;
            tiberium->GrowthQueue = 0;
        }
        if (tiberium->GrowthData != 0) {
            delete[] tiberium->GrowthData;
            tiberium->GrowthData = 0;
        }
        if (tiberium->GrowthMap != 0) {
            delete[] tiberium->GrowthMap;
            tiberium->GrowthMap = 0;
        }
        tiberium->GrowthCount = 0;
        tiberium->GrowthData = new UnknownVeinNode[unknown_cells_428ac0()];
        tiberium->GrowthMap = new bool[unknown_cells_428ac0()];
        tiberium->GrowthQueue = new UnknownVeinHeap(unknown_cells_428ac0());
        tiberium->Rebuild_Growth();
    }
}


// 0x006ea130: function not yet identified
//
// Body: 172 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call, unconditional jump).
// Ghidra prototype: void Deinit_Tiberium_Growth_System_006ea130(void)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_006ea130 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" unsigned int Tiberiums_Vector_00ac0b34;
extern "C" int Tiberiums_ActiveCount_00ac0b40;

void  Deinit_Tiberium_Growth_System_006ea130(void)
{
    int tiberium;
    int *queue;
    int slot;
    int index;

    index = (int)(0);
    if (0 < Tiberiums_ActiveCount_00ac0b40) {
        do {
            tiberium = (int)(*(int *)(Tiberiums_Vector_00ac0b34 + index * 4));
            queue = (int *)(*(int **)(tiberium + 0x110));
            if (queue != (int *)0x0) {
                slot = (int)(0);
                if (*queue >= 0) {
                    do {
                        slot = (int)(slot + 1);
                        *(unsigned int *)(queue[2] + -4 + slot * 4) = 0;
                    } while (slot <= *queue);
                }
                *queue = 0;
                slot = (int)(*(int *)(tiberium + 0x110));
                if (slot != 0) {
                    operator_delete_00781d3b((unsigned int)(*(unsigned int *)(slot + 8)));
                    operator_delete_00781d3b((unsigned int)(slot));
                }
                *(unsigned int *)(tiberium + 0x110) = 0;
            }
            if (*(int *)(tiberium + 0x118) != 0) {
                operator_delete_00781d3b((unsigned int)(*(int *)(tiberium + 0x118)));
                *(unsigned int *)(tiberium + 0x118) = 0;
            }
            if (*(int *)(tiberium + 0x114) != 0) {
                operator_delete_00781d3b((unsigned int)(*(int *)(tiberium + 0x114)));
                *(unsigned int *)(tiberium + 0x114) = 0;
            }
            *(unsigned int *)(tiberium + 0x10c) = 0;
            index = (int)(index + 1);
        } while (index < Tiberiums_ActiveCount_00ac0b40);
    }
    return;
}

}  // namespace seed_006ea130


// void TiberiumClass::Process_Growth() (0x006EA1E0)
// Grows the ore of up to 5-50 queued cells (a random share of the queue's GrowthPercentage); a cell
// below the last stage is queued again 0-49 frames later and offered to the spread queue (TibSun
// Growth_AI).

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

void TiberiumClass::Process_Growth()
{
    if (GrowthQueue != 0 && GrowthQueue->Active_Count() != 0 && unknown_b0 > 0.00001) {
        int count = MIN(50, MAX(5, (int)(GrowthQueue->Active_Count() * unknown_b0)));
        count = abs(unknown_id_source_a3d290->Random()()) % count + 1;
        if (GrowthQueue->Active_Count() > unknown_cells_428ac0() - 2 * count) {
            Rebuild_Growth();
        }
        int index = 0;
        UnknownVeinNode *node = GrowthQueue->Remove_Min();
        while (index < count && node != 0) {
            CellClass *cellptr = &Map[node->Position];
            if (cellptr->Tiberium_Type_Here() == ArrayIndex) {
                cellptr->Grow_Tiberium();
                if (cellptr->OverlayData < 11) {
                    GrowthData[GrowthCount].Position = node->Position;
                    GrowthData[GrowthCount].Key = float(Frame + abs(unknown_id_source_a3d290->Random()() % 50));
                    GrowthMap[unknown_cell_index_428a90(node->Position)] = true;
                    GrowthQueue->Insert(&GrowthData[GrowthCount++]);
                    Add_Spread_Cell(node->Position);
                } else {
                    GrowthMap[unknown_cell_index_428a90(node->Position)] = false;
                }
            }
            index++;
            if (index < count) {
                node = GrowthQueue->Remove_Min();
            }
        }
    }
}


// void TiberiumClass::Init_Growth_Queue() (0x006EA540)
// Drops this type's growth queue and tables, allocates them for the current map (one entry and one
// flag per cell) and refills them. Init_Growth expands it (this copy is never called).

void TiberiumClass::Init_Growth_Queue()
{
    if (GrowthQueue != 0) {
        GrowthQueue->Clear();
        delete GrowthQueue;
        GrowthQueue = 0;
    }
    if (GrowthData != 0) {
        delete[] GrowthData;
        GrowthData = 0;
    }
    if (GrowthMap != 0) {
        delete[] GrowthMap;
        GrowthMap = 0;
    }
    GrowthCount = 0;
    GrowthData = new UnknownVeinNode[unknown_cells_428ac0()];
    GrowthMap = new bool[unknown_cells_428ac0()];
    GrowthQueue = new UnknownVeinHeap(unknown_cells_428ac0());
    Rebuild_Growth();
}


// void TiberiumClass::Rebuild_Growth() (0x006EA680)
// Queues every cell of the map that holds this tiberium and can grow, at score 0.

void TiberiumClass::Rebuild_Growth()
{
    GrowthCount = 0;
    GrowthQueue->Clear();
    for (int index = unknown_cells_428ac0() - 1; index >= 0; index--) {
        GrowthMap[index] = false;
    }
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        if (cell->Tiberium_Type_Here() == ArrayIndex && cell->Can_Tiberium_Grow()) {
            GrowthData[GrowthCount].Position = cell->MapCoords;
            GrowthData[GrowthCount].Key = 0.0f;
            GrowthQueue->Insert(&GrowthData[GrowthCount++]);
            GrowthMap[unknown_cell_index_428a90(cell->MapCoords)] = true;
        }
    }
}


// 0x006ea7f0: function not yet identified
//
// Body: 135 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall Clear_Growth_006ea7f0(int param_1)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006ea7f0 {

extern void  operator_delete_00781d3b(unsigned int);

void __fastcall Clear_Growth_006ea7f0(int self)
{
    int *queue;
    int slot;

    queue = (int *)(*(int **)(self + 0x110));
    if (queue != (int *)0x0) {
        slot = (int)(0);
        if (*queue >= 0) {
            do {
                slot = (int)(slot + 1);
                *(unsigned int *)(queue[2] + -4 + slot * 4) = 0;
            } while (slot <= *queue);
        }
        *queue = 0;
        slot = (int)(*(int *)(self + 0x110));
        if (slot != 0) {
            operator_delete_00781d3b((unsigned int)(*(unsigned int *)(slot + 8)));
            operator_delete_00781d3b((unsigned int)(slot));
        }
        *(unsigned int *)(self + 0x110) = 0;
    }
    if (*(int *)(self + 0x118) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0x118)));
        *(unsigned int *)(self + 0x118) = 0;
    }
    if (*(int *)(self + 0x114) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0x114)));
        *(unsigned int *)(self + 0x114) = 0;
    }
    *(unsigned int *)(self + 0x10c) = 0;
    return;
}

}  // namespace seed_006ea7f0


// void TiberiumClass::Add_Growth_Cell(Cell const &cell) (0x006EA880)
// Queues a cell whose ore is below its last stage, at the current frame plus a random 0-49
// (rebuilding the queue first when its table is nearly full).

void TiberiumClass::Add_Growth_Cell(Cell const &cell)
{
    int index = unknown_cell_index_428a90(cell);
    if (Map[cell].OverlayData < 11) {
        if (GrowthCount > unknown_cells_428ac0() - 10) {
            Rebuild_Growth();
        }
        GrowthData[GrowthCount].Position = cell;
        GrowthData[GrowthCount].Key = abs(unknown_id_source_a3d290->Random()()) % 50 + Frame;
        GrowthQueue->Insert(&GrowthData[GrowthCount++]);
        GrowthMap[index] = true;
    }
}


class Leaf_6ea9b0 { public: void get(); };
void Leaf_6ea9b0::get() {  }


// int TiberiumClass::Size_Of() const (0x006EA9C0)
// The class size, 0x128 bytes.

int TiberiumClass::Size_Of() const
{
    return sizeof(TiberiumClass);
}


class Leaf_6ea9d0 { public: unsigned int get(); };
unsigned int Leaf_6ea9d0::get() { return 0x2e; }


class Leaf_6ea9e0 { public: unsigned int get(); };
unsigned int Leaf_6ea9e0::get() { return *(unsigned int*)((char*)this + 0x98); }


extern "C" void __cdecl release_6ea9f0(void*);
class Leaf_6ea9f0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_6ea9f0::get(unsigned int flags) { cleanup(); if (flags & 1) release_6ea9f0(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TiberiumClass *>::Clear (0x006EAA10; DynamicVectorClass<TiberiumClass *> vtable 0x007ad844 slot 3).
// DynamicVectorClass<TiberiumClass *>::ID_pointer (0x006EAA40; DynamicVectorClass<TiberiumClass *> vtable 0x007ad844 slot 5).
// VectorClass<TiberiumClass *>::Fetch (0x006EAA60; VectorClass<TiberiumClass *> vtable 0x007ad864 slot 6).
// VectorClass<TiberiumClass *>::operator equal (0x006EAA70; VectorClass<TiberiumClass *> vtable 0x007ad864 slot 1).
// VectorClass<TiberiumClass *>::Resize (0x006EAAB0; VectorClass<TiberiumClass *> vtable 0x007ad864 slot 2).
// VectorClass<TiberiumClass *>::Clear (0x006EAB60; VectorClass<TiberiumClass *> vtable 0x007ad864 slot 3).
// VectorClass<TiberiumClass *>::ID_object (0x006EAB90; VectorClass<TiberiumClass *> vtable 0x007ad864 slot 4).
// VectorClass<TiberiumClass *>::ID_pointer (0x006EABD0; VectorClass<TiberiumClass *> vtable 0x007ad864 slot 5).
// DynamicVectorClass<TiberiumClass *>::Resize (0x006EABF0; DynamicVectorClass<TiberiumClass *> vtable 0x007ad844 slot 2).
// DynamicVectorClass<TiberiumClass *>::ID_object (0x006EACA0; DynamicVectorClass<TiberiumClass *> vtable 0x007ad844 slot 4).


extern "C" unsigned char vtable_6eacd0[]; extern "C" void __cdecl release_buffer_6eacd0(void*); extern "C" void __cdecl release_object_6eacd0(void*);
class Leaf_6eacd0 { public: void* get(unsigned int flags); };
void* Leaf_6eacd0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6eacd0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6eacd0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6eacd0(this); return this; }


extern "C" unsigned char vtable_6ead20[]; extern "C" void __cdecl release_buffer_6ead20(void*); extern "C" void __cdecl release_object_6ead20(void*);
class Leaf_6ead20 { public: void* get(unsigned int flags); };
void* Leaf_6ead20::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6ead20; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6ead20((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6ead20(this); return this; }
