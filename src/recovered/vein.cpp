// vein.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vein.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VEIN_GROWTH_LIFECYCLE
#define RA2_VEIN_STAGE_ACCESSORS
#define RA2_VEIN_STAGE_GRAPHIC_ASSIGN
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_CDTIMER_CONVERSION_BODY
#define RA2_STAGE_SETTERS
#define RA2_CELL_AS_COORD
#define RA2_VEIN_MAP_PREDICATE
// Header-static dynamic initializers of the unit at 0x0070F590 and its
// list globals (VeinholeMonsterClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/veinhole.hpp"


DynamicVectorClass<VeinholeMonsterClass *> VeinholeMonsterClass::VeinholeMonsters;


// Dynamic initializer (0x0070F5B0, _$E5) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F5E0, _$E8) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F600, _$E11) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F620, _$E14) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F640, _$E17) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F660, _$E20) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F680, _$E23) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F6C0, _$E26) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F6F0, _$E29) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F720, _$E32) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F750, _$E35) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F760, _$E38) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// Dynamic initializer (0x0070F780, _$E45) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// atexit destructor (0x0070F7C0, _$E42) emitted by HeaderStatics_empty_rect_init_0070f590.cpp; no source of its own.


// VeinholeMonsterClass::VeinholeMonsterClass(NoInitClass const &) (0x0070F820)
// The constructor Load rebuilds objects with: registers in VeinholeMonsters and builds the
// vein node block, the heap and the per-cell table. Body 389 bytes (the
// inventory stops at 172 inside the first operator new call).
#include "shared/cd_timer.hpp"

VeinholeMonsterClass::VeinholeMonsterClass(NoInitClass const &noinit) : ObjectClass(noinit), Control(noinit), LogicTimer(noinit)
{
    VeinholeMonsters.Add(this);
    GrowthNodes = new UnknownVeinNode[Rules_Vein_Node_Count()];
    GrowthQueue = new UnknownVeinHeap(Rules_Vein_Node_Count());
    GrowthQueue->Clear();
    if (unknown_veinhole_buffer_ad07a0 == 0) {
        unknown_veinhole_buffer_ad07a0 = new unsigned char[unknown_cells_428ac0()];
    }
    GrowthState = new unsigned char[unknown_cells_428ac0()];
}


// VeinholeMonsterClass::VeinholeMonsterClass(Cell const *cell) (0x0070F9B0)
// A veinhole monster at the cell. Only where one may spawn (Can_Monster_Go_Here)
// does it join VeinholeMonsters, sink and retile its 3x3 cells (unless 0x00A40774 is
// set), start its stage timer, build the node block, heap and tables, move
// onto the map at the cell's center and register in the techno index.
#include "shared/map_cell_view.hpp"
#include "shared/techno_index.hpp"
#include "shared/smudge.hpp"
#include "shared/tileset_index.hpp"
#include "shared/terrain_type.hpp"

// The tile offsets (within the veinhole tile set; 0: none) of the 3x3 cells
// under a new monster (0x007F9508).
extern unsigned char const unknown_veinhole_tiles_7f9508[3][3];

VeinholeMonsterClass::VeinholeMonsterClass(::Cell const *cell) :
    GrowthCount(0),
    GrowthQueue(0),
    GrowthNodes(0),
    GrowthTimer(Rules->VeinholeGrowthRate),
    GrowthState(0),
    CurrentState(0),
    DesiredState(0),
    CellID(*cell),
    ShapeFrame(0),
    IsDead(false),
    IsToPuffGas(false),
    VeinCount(0)
{
    AbstractClass::Create_ID();
    if (!Can_Monster_Go_Here(*cell)) {
        return;
    }
    VeinholeMonsters.Add(this);
    if (unknown_a40774 == 0) {
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                CellClass *spot = &Map[*cell + ::Cell(x, y)];
                spot->unknown_11c = unknown_veinhole_tiles_7f9508[y + 1][x + 1];
                if (spot->unknown_11c > 0) {
                    spot->IsoTileIndex = spot->unknown_11c + unknown_tileset_a6e018 - 1;
                } else {
                    spot->IsoTileIndex = 0;
                }
                spot->unknown_11a = 0;
                spot->unknown_11b--;
            }
        }
    }
    Control.Set_Rate(4);
    Control.unknown_14 = 0;
    Control.Value = -1;
    GrowthNodes = new UnknownVeinNode[Rules_Vein_Node_Count()];
    GrowthQueue = new UnknownVeinHeap(Rules_Vein_Node_Count());
    GrowthQueue->Clear();
    if (unknown_veinhole_buffer_ad07a0 == 0) {
        unknown_veinhole_buffer_ad07a0 = new unsigned char[unknown_cells_428ac0()];
    }
    GrowthState = new unsigned char[unknown_cells_428ac0()];
    CoordStruct coord = CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    ObjectClass::Unlimbo(&coord, 0);
    Strength = Rules->unknown_cb4->MaxStrength;
    unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
}


// 0070FDF0: remove the surrounding vein overlay, buffers and tracking entries.
#include "shared/overlay_type.hpp"
#include "shared/tactical.hpp"
#include "shared/object_index.hpp"
extern Rect unknown_rect_839630;

inline RectPoint2D Rect::Top_Left() const { return RectPoint2D(X, Y); }
inline Rect Rect::operator-(RectPoint2D const &point) const { return Rect(Top_Left() - point, Width, Height); }

VeinholeMonsterClass::~VeinholeMonsterClass()
{
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            ::Cell cell(CellID + ::Cell(x, y));
            int cell_id = unknown_cell_index_428a90(cell);
            if (cell_id >= 0 && cell_id < unknown_cells_428ac0()) {
                unknown_veinhole_buffer_ad07a0[cell_id] = false;
                GrowthState[cell_id] = false;
            }
            CellClass *map_cell = &Map[cell];
            if (map_cell->OverlayIndex == 0x7e || (map_cell->OverlayIndex != -1 && OverlayTypes[map_cell->OverlayIndex]->IsVeins)) {
                Map.Radar_Background(map_cell->MapCoords);
                RectPoint2D origin(unknown_rect_839630.Top_Left());
                unknown_tactical_83996c->Register_Dirty_Area(Union(map_cell->Overlay_Render_Rect(), map_cell->Overlay_Shadow_Render_Rect()) - origin, false);
                map_cell->OverlayIndex = -1;
                map_cell->OverlayData = 0;
            }
        }
    }
    unknown_76 = true;
    unknown_74 = false;
    if (Strength > 0) Unknown_006ECB90(this, true);
    Clear_Growth();
    VeinholeMonsters.Delete(this);
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
}


// VeinholeMonsterClass *VeinholeMonsterClass::Get_Monster_At(Cell const &cell) (0x00710160)
// The last monster in VeinholeMonsters whose cell is cell, or null.
#include "shared/house.hpp"

VeinholeMonsterClass *__fastcall VeinholeMonsterClass::Get_Monster_At(::Cell const &cell)
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (VeinholeMonsters[index]->CellID == cell) {
            return VeinholeMonsters[index];
        }
    }
    return 0;
}


// VeinholeMonsterClass *VeinholeMonsterClass::Get_Vein_Owner_At(Cell const &cell) (0x007101B0)
// The last monster in VeinholeMonsters whose per-cell table (+0xB0) has a byte set at the cell, or null.

VeinholeMonsterClass *__fastcall VeinholeMonsterClass::Get_Vein_Owner_At(::Cell const &cell)
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (VeinholeMonsters[index]->GrowthState[unknown_cell_index_428a90(cell)] != 0) {
            return VeinholeMonsters[index];
        }
    }
    return 0;
}


// void VeinholeMonsterClass::Update_All() (0x007101F0)
// Runs every monster's AI, last first, then deletes the dead ones.

void __cdecl VeinholeMonsterClass::Update_All()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->AI();
    }
    Remove_Dead();
}


// Veinhole AI710250: table-inclusive bound1480+8 established before source.
#include "shared/particle_system.hpp"
#include "shared/id_source.hpp"
#include "shared/fast_math.hpp"
extern int vein_frames_7f9514[4][13];
extern int vein_transition_7f95e4[4],vein_loop_7f95f4[4];
extern int vein_step_7f9604[4],vein_stage_7f9614[4];
extern char vein_gas_7f96cc[];
extern double const vein_random_unit_79c280,vein_full_angle_79c9b8,vein_speed_79d558;
extern double const vein_idle_scale_7a5fe8,vein_idle_chance_7ae718,vein_attack_chance_7ae720;
inline double Vein_Random_Double(double low,double high)
{
    int sample=unknown_id_source_a3d290->Random()(0,0x7ffffffe);
    return (high-low)*((double)sample*vein_random_unit_79c280)+low;
}
void VeinholeMonsterClass::AI()
{
    // Same evidenced 0x18 layout, using the shared StageClass operation hierarchy.
    ((StageClass *)&Control)->Graphic_Logic();
    switch(CurrentState) {
    case 0:
        if(LogicTimer!=0) {
            if(DesiredState!=3) DesiredState=2;
        } else if(DesiredState!=3) {
            if(Map.Is_Something_Nearby(CellID,2)) DesiredState=1;
            else if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[CurrentState]) {
                if((double)(abs(unknown_id_source_a3d290->Random()())%10000)*vein_idle_scale_7a5fe8<vein_idle_chance_7ae718) {
                    ((StageClass *)&Control)->Set_Stage(vein_loop_7f95f4[CurrentState]-1);
                    ((StageClass *)&Control)->Set_Step(-1);
                }
            }
        }
        break;
    case 1:
        if(LogicTimer!=0) {
            if(DesiredState!=3) DesiredState=2;
            ((StageClass *)&Control)->Set_Step(1);
            if(((StageClass *)&Control)->Fetch_Stage()<0) ((StageClass *)&Control)->Set_Stage(0);
        } else if(!Map.Is_Something_Nearby(CellID,2) && DesiredState==1) {
            DesiredState=0;
            ((StageClass *)&Control)->Set_Step(1);
            if(((StageClass *)&Control)->Fetch_Stage()<0) ((StageClass *)&Control)->Set_Stage(0);
        }
        break;
    case 2:
        if(((StageClass *)&Control)->Fetch_Stage()>=0 && vein_frames_7f9514[2][((StageClass *)&Control)->Fetch_Stage()]==11) {
            if(!IsToPuffGas) {
                CoordStruct base(CellID);
                CoordStruct position=CellID.As_Coord(Map.Get_Height_GL(base)+400);
                ParticleClass *particle=unknown_gas_cloud_system_a40d20->Spawn_Held_Particle(ParticleTypes[ParticleTypeClass::Index_Or_Make(vein_gas_7f96cc)],position);
                particle->unknown_b0.Z=12;
                double angle=Vein_Random_Double(0.0,1.0)*vein_full_angle_79c9b8;
                particle->unknown_b0.X=(int)(Fast_Cos(angle)*vein_speed_79d558);
                particle->unknown_b0.Y=(int)(Fast_Sin(angle)*vein_speed_79d558);
                IsToPuffGas=true;
            }
        } else IsToPuffGas=false;
        if(LogicTimer!=0 || IsToPuffGas) {
            if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[CurrentState]) {
                if(Vein_Random_Double(0.0,1.0)<vein_attack_chance_7ae720) {
                    ((StageClass *)&Control)->Set_Stage(vein_loop_7f95f4[CurrentState]-1);
                    ((StageClass *)&Control)->Set_Step(-1);
                }
            }
        } else {
            if(DesiredState!=3) DesiredState=0;
            ((StageClass *)&Control)->Set_Step(-1);
        }
        break;
    case 3:
        if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[3] && !IsDead) Destroy_Monster();
        break;
    }
    if(GrowthTimer==0) {
        if(!IsDead) {
            Grow();
            GrowthTimer=Rules->VeinholeGrowthRate;
        } else {
            Shrink();
            GrowthTimer=Rules->VeinholeShrinkRate;
        }
    }
    if(!IsDead) {
        if(((StageClass *)&Control)->Fetch_Stage()==vein_transition_7f95e4[CurrentState]) {
            if(DesiredState!=CurrentState) {
                CurrentState=DesiredState;
                ((StageClass *)&Control)->Set_Step(vein_step_7f9604[CurrentState]);
                ((StageClass *)&Control)->Set_Stage(vein_stage_7f9614[CurrentState]);
            } else ((StageClass *)&Control)->Set_Step(0);
        } else if(((StageClass *)&Control)->Fetch_Stage()!=-1) ShapeFrame=vein_frames_7f9514[CurrentState][((StageClass *)&Control)->Fetch_Stage()]-1;
        else ((StageClass *)&Control)->Set_Step(0);
    }
}


// void VeinholeMonsterClass::Draw_All() (0x00710820)
// Calls Draw_It on every monster, last first.

void __cdecl VeinholeMonsterClass::Draw_All()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->Draw_It();
    }
}


// void VeinholeMonsterClass::Init(int theater) (0x00710840)
// Loads VEINHOLE.<theater suffix> into the shape pointer.
#include "shared/anim_type.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"

void __fastcall VeinholeMonsterClass::Init(int theater)
{
    char name[32];
    sprintf(name, "VEINHOLE.%s", Theaters[theater].Suffix);
    unknown_veinhole_shape_ad079c = MixFileClass::Retrieve(name);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void VeinholeMonsterClass::Draw_It() (0x00710880)
// from ra2-decomp 0x0074D490 (VeinholeMonsterClass::Draw_It, src/vein.cpp)
// Draws an unplaced monster: its shape frame (0 while the cell has flag
// 0x400000) at the cell centre, raised by the cell level, in the player's
// colours, and records the screen position with the tactical view.
#include "shared/palette.hpp"
#include "shared/color_scheme.hpp"
#include "shared/terrain.hpp"
#include "shared/text_print.hpp"

void VeinholeMonsterClass::Draw_It()
{
    if (!IsDead) {
        void const *shape = unknown_veinhole_shape_ad079c;
        int frame = ShapeFrame;
        CoordStruct coord = Cell_Coord(CellID);
        if (Map[coord].unknown_140 & 0x400000) {
            frame = 0;
        }
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        int height = 15 * Map[CellID].unknown_11b;
        pixel.Y += -58 - height;
        unknown_tactical_83996c->Add_To_Selectables(this, Point2D(pixel.X, unknown_rect_839630.Y + pixel.Y + 58));
        ConvertClass *drawer = ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter;
        CC_Draw_Shape(unknown_surface_83995c, drawer, (ShapeFileStruct const *)shape, frame, pixel, unknown_rect_839630, 0x2e00, 0, -17 - height, 0,
                      Map[coord].unknown_10c, 0, 0, Point2D(0, 0));
    }
}


// int VeinholeMonsterClass::Take_Damage(...) (0x007109C0, slot 86: take damage)
// ObjectClass's result decides the state (+0xB8): 4 -> 3 and the object is
// ended through 0x006ECB90, 5 -> unchanged, otherwise 2 and a 120 frame timer.

int VeinholeMonsterClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int arg6)
{
    int result = ObjectClass::Take_Damage(damage, distance, warhead, source, forced, arg6);
    switch (result) {
    case 0:
        break;
    case 4:
        DesiredState = 3;
        Unknown_006ECB90(this, false);
        break;
    case 5:
        return 5;
    default:
        DesiredState = 2;
        LogicTimer = CDTimerClass<FrameTimerClass>(120);
        break;
    }
    return result;
}


// bool VeinholeMonsterClass::Can_Monster_Go_Here(Cell const &cell) (0x00710A60)
// True while the global at 0x00A40774 is positive; otherwise only when no
// monster is within 2 cells and the 3x3 block around the cell has no blocked
// cell, no occupier list and the same level as the cell (+0x11B).
#include "shared/crt_memory.hpp"

extern int unknown_a40774;

bool __fastcall VeinholeMonsterClass::Can_Monster_Go_Here(::Cell const &cell)
{
    if (unknown_a40774 > 0) {
        return true;
    }
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (abs(cell.X - VeinholeMonsters[index]->CellID.X) < 3 && abs(cell.Y - VeinholeMonsters[index]->CellID.Y) < 3) {
            return false;
        }
    }
    int level = Map[cell].unknown_11b;
    if (level < 1) {
        return false;
    }
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            ::Cell neighbor = ::Cell(cell.X + x, cell.Y + y);
            CellClass *near_cell = &Map[neighbor];
            if (near_cell->unknown_11c || near_cell->unknown_ec != 0 || near_cell->unknown_11b != level) {
                return false;
            }
        }
    }
    return true;
}


// void VeinholeMonsterClass::Reset() (0x00710B50)
// Deletes every monster and empties VeinholeMonsters.

void __cdecl VeinholeMonsterClass::Reset()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        delete VeinholeMonsters[index];
    }
    VeinholeMonsters.Clear();
}


// void VeinholeMonsterClass::Grow() (0x00710BB0)
// Grows the vein network while the monster has room for nodes and the
// scenario allows it (+0x222B): takes one to five nodes off the heap; each
// node's cell is grown (0x0047AD00) and claimed, and its four straight
// neighbours on the map, within one level and accepted by 0x0047A670, not
// yet claimed, become new nodes keyed by the frame / 50 plus a random 1..50.
#include "shared/random.hpp"
#include "shared/adjacent_cell.hpp"

void VeinholeMonsterClass::Grow()
{
    if (GrowthQueue == 0) {
        return;
    }
    if (GrowthCount > Rules_Vein_Node_Count() - 40) {
        return;
    }
    if (VeinCount > Rules_Vein_Node_Count() - 100) {
        return;
    }
    if (!unknown_id_source_a3d290->unknown_222b) {
        return;
    }
    int count = 0;
    Random2Class &random = unknown_id_source_a3d290->Random();
    int steps = abs(random()) % 5 + 1;
    UnknownVeinNode *node = GrowthQueue->Remove_Min();
    while (count < steps && node != 0) {
        CellClass *cell = &Map[node->Position];
        if (cell->OverlayData < 0x30) {
            cell->Place_Veins();
            VeinCount++;
        }
        if (cell->OverlayIndex == 0x7e) {
            int index = unknown_cell_index_428a90(node->Position);
            if (index >= 0 && index < unknown_cells_428ac0()) {
                GrowthState[index] = 1;
            }
        }
        if (cell->OverlayData >= 0x30) {
            for (int facing = 0; facing < 8; facing += 2) {
                ::Cell spot = node->Position + AdjacentCell[facing & 7];
                if (!Map.Unknown_0055A660(spot, true)) {
                    continue;
                }
                CellClass *adjacent = &Map[spot];
                if (abs(adjacent->unknown_11b - cell->unknown_11b) >= 2) {
                    continue;
                }
                int index = unknown_cell_index_428a90(spot);
                if (index < 0 || index >= unknown_cells_428ac0()) {
                    continue;
                }
                if (adjacent->Can_Place_Veins() && unknown_veinhole_buffer_ad07a0[index] == 0 && GrowthCount < Rules_Vein_Node_Count()) {
                    GrowthNodes[GrowthCount].Position = spot;
                    GrowthNodes[GrowthCount].Key = Frame / 50 + abs(unknown_id_source_a3d290->Random()() % 50) + 1;
                    unknown_veinhole_buffer_ad07a0[index] = 1;
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                }
                GrowthState[index] = 1;
            }
        }
        count++;
        if (count >= steps) {
            break;
        }
        node = GrowthQueue->Remove_Min();
    }
}


// void VeinholeMonsterClass::Shrink() (0x00710FF0)
// Spreads the veins: takes one to four nodes (random) with the lowest keys
// off the heap and grows the vein at each node's cell (0x00712300).
// Body: 646 bytes (ret at 0x00711275); the inventory splits it at 0x00711076.

void VeinholeMonsterClass::Shrink()
{
    if (GrowthQueue == 0) {
        return;
    }
    int count = 0;
    int steps = abs(unknown_id_source_a3d290->Random()()) % 4 + 1;
    UnknownVeinNode *node = GrowthQueue->Remove_Min();
    while (count < steps && node != 0) {
        Reduce_Veins_At(&Map[Map[node->Position].MapCoords]);
        count++;
        if (count >= steps) {
            break;
        }
        node = GrowthQueue->Remove_Min();
    }
}


// static void VeinholeMonsterClass::Init_Vein_Growth_System(bool rebuild) (0x00711280)
// With rebuild, frees every monster's buffers and the shared table and
// allocates them again (nodes, heap and per-cell table); then clears the
// shared table and the cells' vein bit (0x8000), lets every monster mark its
// cells again (placed ones 0x00711AA0, others 0x007115B0) and removes vein
// overlays (0x7E) no monster covers.

void __fastcall VeinholeMonsterClass::Init_Vein_Growth_System(bool rebuild)
{
    int cells = unknown_cells_428ac0();
    int index;
    if (rebuild) {
        Deinit_Vein_Growth_System();
        for (index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
            VeinholeMonsters[index]->GrowthNodes = new UnknownVeinNode[Rules_Vein_Node_Count()];
            VeinholeMonsters[index]->GrowthQueue = new UnknownVeinHeap(Rules_Vein_Node_Count());
            VeinholeMonsters[index]->GrowthState = new unsigned char[cells];
            memset(VeinholeMonsters[index]->GrowthState, 0, cells);
            VeinholeMonsters[index]->GrowthQueue->Clear();
        }
    }
    if (unknown_veinhole_buffer_ad07a0 == 0) {
        unknown_veinhole_buffer_ad07a0 = new unsigned char[cells];
    }
    Map.Reset_Iterator();
    for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        cell->unknown_140 &= ~0x8000;
    }
    memset(unknown_veinhole_buffer_ad07a0, 0, cells);
    for (index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (VeinholeMonsters[index]->IsDead) {
            VeinholeMonsters[index]->Build_Shrinking_Queue();
        } else {
            VeinholeMonsters[index]->Build_Growth_Queue();
        }
    }
    Map.Reset_Iterator();
    for (cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
        if (cell->OverlayIndex == 0x7e && unknown_veinhole_buffer_ad07a0[unknown_cell_index_428a90(cell->MapCoords)] == 0) {
            cell->OverlayIndex = -1;
        }
        cell->unknown_140 &= ~0x8000;
    }
}


// VeinholeMonsterClass::Deinit_Vein_Growth_System (0x007114F0): every monster's Clear_Growth, last first, then the
// shared table (Clear_Global_Data).

void __cdecl VeinholeMonsterClass::Deinit_Vein_Growth_System()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->Clear_Growth();
    }
    Clear_Global_Data();
}


// Veinhole growth traversal, independently bounded at007115B0.
#include "shared/map_view.hpp"
void VeinholeMonsterClass::Build_Growth_Queue()
{
    DynamicVectorClass<::Cell> pending;
    pending.Set_Growth_Step(5000);
    GrowthCount=0;
    VeinCount=0;
    GrowthQueue->Clear();
    for (int index=unknown_cells_428ac0()-1; index>=0; --index) GrowthState[index]=0;
    int row_or_facing;
    for (row_or_facing=-2; row_or_facing<=2; ++row_or_facing) {
        for (int column=-2; column<=2; ++column) {
            CellClass *cell=&Map[::Cell(column,row_or_facing)+CellID];
            if (cell->OverlayIndex==0x7e) {
                int index=unknown_cell_index_428a90(cell->MapCoords);
                pending.Add(::Cell(column,row_or_facing)+CellID);
                if (index>=0 && index<unknown_cells_428ac0()) {
                    unknown_veinhole_buffer_ad07a0[index]=1;
                    GrowthState[index]=1;
                    cell->unknown_140|=0x8000;
                    if (cell->OverlayData>=0x30 && cell->OverlayData<=0x33) {
                        ++VeinCount;
                    } else if (GrowthCount<Rules_Vein_Node_Count()) {
                        GrowthNodes[GrowthCount].Position=cell->MapCoords;
                        GrowthNodes[GrowthCount].Key=0;
                        GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                    }
                }
            }
        }
    }
    while (pending.Count()>0) {
        int last=pending.Count()-1;
        CellClass *cell=&Map[pending[last]];
        pending.Delete(last);
        for (row_or_facing=0; row_or_facing<8; ++row_or_facing) {
            CellClass *adjacent=&cell->Adjacent_Cell(row_or_facing);
            int index=unknown_cell_index_428a90(adjacent->MapCoords);
            if (adjacent->OverlayIndex==0x7e && !(adjacent->unknown_140&0x8000)) {
                if ((adjacent->OverlayData>=0x30 && adjacent->OverlayData<0x35) || !adjacent->Can_Place_Veins()) {
                    pending.Add(adjacent->MapCoords);
                    ++VeinCount;
                } else if (GrowthCount<Rules_Vein_Node_Count()) {
                    GrowthNodes[GrowthCount].Position=adjacent->MapCoords;
                    GrowthNodes[GrowthCount].Key=0;
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                    if (adjacent->OverlayData>=0x30) pending.Add(adjacent->MapCoords);
                }
                adjacent->unknown_140|=0x8000;
                if (index>=0 && index<unknown_cells_428ac0()) {
                    unknown_veinhole_buffer_ad07a0[index]=1;
                    GrowthState[index]=1;
                }
                ++VeinCount;
            }
        }
    }
}


#include "shared/anim.hpp"

void VeinholeMonsterClass::Build_Shrinking_Queue()
{
    GrowthCount = 0;
    GrowthQueue->Clear();
    VeinCount = 0;

    Map.Reset_Iterator();
    CellClass *iter = Map.Iterate();
    while (iter != 0) {
        if (iter->OverlayIndex == 0x7e && iter->OverlayData >= 0x30) {
            int index = unknown_cell_index_428a90(iter->MapCoords);
            if (index >= 0 && index < unknown_cells_428ac0() && GrowthState[index]) {
                if (GrowthCount < Rules_Vein_Node_Count()) {
                    GrowthNodes[GrowthCount].Position = iter->MapCoords;
                    Point2D diff = Point2D(iter->MapCoords.X, iter->MapCoords.Y) - Point2D(CellID.X, CellID.Y);
                    GrowthNodes[GrowthCount].Key = 1000 - Cell_Distance(diff.X, diff.Y);
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                    VeinCount++;
                }
            }
        }
        iter = Map.Iterate();
    }
}


// VeinholeMonsterClass::Clear_Global_Data (0x00711C70): frees the shared per-cell table.

void __cdecl VeinholeMonsterClass::Clear_Global_Data()
{
    if (unknown_veinhole_buffer_ad07a0 != 0) {
        delete[] unknown_veinhole_buffer_ad07a0;
        unknown_veinhole_buffer_ad07a0 = 0;
    }
}


// VeinholeMonsterClass::Clear_Growth (0x00711C90; Clear_Growth as TiberiumClass names it): drops this monster's growth queue and tables.

void VeinholeMonsterClass::Clear_Growth()
{
    if (GrowthQueue != 0) {
        GrowthQueue->Clear();
        delete GrowthQueue;
        GrowthQueue = 0;
    }
    if (GrowthNodes != 0) {
        delete[] GrowthNodes;
        GrowthNodes = 0;
    }
    if (GrowthState != 0) {
        delete[] GrowthState;
        GrowthState = 0;
    }
    GrowthCount = 0;
}


void VeinholeMonsterClass::Destroy_Monster()
{
    CellClass &cptr = Map[CellID];
    cptr.OverlayIndex = 0x7e;
    cptr.OverlayData = 0;
    cptr.Recalc_Attributes(-1);
    for (int dir = 0; dir < 8; dir++) {
        CellClass &adj = cptr.Adjacent_Cell(dir);
        if (dir % 2 != 0) {
            adj.OverlayIndex = -1;
            adj.OverlayData = 0;
        } else {
            adj.OverlayIndex = 0x7e;
            adj.OverlayData = 0;
        }
    }
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            CellClass &cptr2 = Map[::Cell(x, y) + CellID];
            cptr2.Place_Veins();
            cptr2.Recalc_Attributes(-1);
        }
    }
    Build_Shrinking_Queue();
    Map.Update_Cell_Zone(CellID);
    Map.Unknown_00566340(CellID);
    IsDead = true;
}


// void VeinholeMonsterClass::Remove_Dead() (0x00711E20)
// Deletes every placed monster (IsDead) whose node heap is empty, last
// first (expanded into Update_All 0x007101F0).

void __cdecl VeinholeMonsterClass::Remove_Dead()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        UnknownVeinHeap *heap = VeinholeMonsters[index]->GrowthQueue;
        if (VeinholeMonsters[index]->IsDead && heap->Count == 0) {
            delete VeinholeMonsters[index];
        }
    }
}


bool __fastcall VeinholeMonsterClass::Load_All(IStream *stream)
{
    Reset();

    int cell_count = unknown_cells_428ac0();

    int monster_count;
    if (FAILED(stream->Read(&monster_count, sizeof(monster_count), 0))) {
        return false;
    }

    unknown_veinhole_buffer_ad07a0 = new unsigned char[cell_count];
    if (FAILED(stream->Read(unknown_veinhole_buffer_ad07a0, cell_count, 0))) {
        return false;
    }

    for (int i = 0; i < monster_count; i++) {
        VeinholeMonsterClass *monster = (VeinholeMonsterClass *)new char[sizeof(VeinholeMonsterClass)];

        LONG_PTR id;
        if (FAILED(stream->Read(&id, sizeof(id), 0))) {
            return false;
        }

        Swizzler.Here_I_Am(id, monster);
        if (FAILED(stream->Read(monster, sizeof(VeinholeMonsterClass), 0))) {
            return false;
        }

        if (monster != 0) {
            new (monster) VeinholeMonsterClass(NoInitClass());
        }

        unsigned char *state = monster->GrowthState;
        UnknownVeinHeap *queue = monster->GrowthQueue;
        UnknownVeinNode *nodes = monster->GrowthNodes;

        if (FAILED(stream->Read(state, cell_count, 0))) {
            return false;
        }
        monster->GrowthState = state;

        if (FAILED(stream->Read(nodes, sizeof(UnknownVeinNode) * Rules->unknown_d4, 0))) {
            return false;
        }
        monster->GrowthNodes = nodes;

        if (!queue->Load(stream, nodes)) {
            return false;
        }
        monster->GrowthQueue = queue;

        unknown_object_index_abff58.Add_Index(monster->Fetch_ID(), monster);
    }

    return true;
}


bool __fastcall VeinholeMonsterClass::Save_All(IStream *stream)
{
    int monster_count = VeinholeMonsters.Count();
    if (FAILED(stream->Write(&monster_count, sizeof(monster_count), 0))) {
        return false;
    }

    int cell_count = unknown_cells_428ac0();
    if (FAILED(stream->Write(unknown_veinhole_buffer_ad07a0, cell_count, 0))) {
        return false;
    }

    for (int i = 0; i < monster_count; i++) {
        LONG_PTR id = (LONG_PTR)VeinholeMonsters[i];
        if (FAILED(stream->Write(&id, sizeof(id), 0))) {
            return false;
        }

        if (FAILED(stream->Write(VeinholeMonsters[i], sizeof(VeinholeMonsterClass), 0))) {
            return false;
        }

        if (FAILED(stream->Write(VeinholeMonsters[i]->GrowthState, cell_count, 0))) {
            return false;
        }

        if (FAILED(stream->Write(VeinholeMonsters[i]->GrowthNodes, sizeof(UnknownVeinNode) * Rules->unknown_d4, 0))) {
            return false;
        }

        VeinholeMonsters[i]->GrowthQueue->Save(stream, VeinholeMonsters[i]->GrowthNodes);
    }

    return true;
}


// HRESULT __stdcall VeinholeMonsterClass::Save(IStream *stream, BOOL clear_dirty) (0x007122D0)

HRESULT __stdcall VeinholeMonsterClass::Save(IStream *stream, BOOL clear_dirty)
{
    return AbstractClass::Save(stream, clear_dirty);
}


// ObjectTypeClass *VeinholeMonsterClass::Class_Of() const (0x007122F0, slot 33)
// The type named by the rules (+0xCB4).

ObjectTypeClass *VeinholeMonsterClass::Class_Of() const
{
    return Rules->unknown_cb4;
}


void VeinholeMonsterClass::Reduce_Veins_At(CellClass *cellptr)
{
    if (cellptr->OverlayIndex == 0x7e && cellptr->OverlayData >= 0x30) {

        bool facingbools[4];
        int facing;
        for (facing = 0; facing < 8; facing += 2) {
            CellClass *adjacent = &cellptr->Adjacent_Cell(facing);
            facingbools[facing / 2] = (adjacent->OverlayIndex == 0x7e && (adjacent->OverlayData < 0x30 || adjacent->OverlayData > 0x30 + 3));
        }

        cellptr->Redraw_Veins();
        VeinCount--;

        for (facing = 0; facing < 8; facing += 2) {
            CellClass *adjacent = &cellptr->Adjacent_Cell(facing);
            if (adjacent->OverlayIndex == -1) {
                int cindex = unknown_cell_index_428a90(adjacent->MapCoords);
                if (cindex >= 0 && cindex < unknown_cells_428ac0()) {
                    GrowthState[cindex] = false;
                    unknown_veinhole_buffer_ad07a0[cindex] = false;
                }

                if (facingbools[facing / 2] == true && !IsDead) {
                    GrowthQueue->Remove_Matching(UnknownVeinNode(adjacent->MapCoords));
                }
            }
        }

        int overlay = cellptr->OverlayIndex;
        ::Cell &cell = cellptr->MapCoords;
        int cindex = unknown_cell_index_428a90(cell);
        if (cindex >= 0 && cindex < unknown_cells_428ac0()) {
            if (overlay == -1) {
                GrowthState[cindex] = false;
                unknown_veinhole_buffer_ad07a0[cindex] = false;
            } else if (!IsDead) {
                if (GrowthCount < Rules->unknown_d4) {
                    GrowthNodes[GrowthCount].Position = cell;
                    GrowthNodes[GrowthCount].Key = float(Frame / 50 + abs(unknown_id_source_a3d290->Random()() % 50) + 1);
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                    GrowthState[cindex] = true;
                    unknown_veinhole_buffer_ad07a0[cindex] = true;
                }
            }
        }
    }
}


// HRESULT __stdcall VeinholeMonsterClass::GetClassID(GUID *clsid) (0x007126C0)

HRESULT __stdcall VeinholeMonsterClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_VeinholeMonsterClass;
    return S_OK;
}


// int VeinholeMonsterClass::Kind_Of() const (0x00712700, slot 11)

int VeinholeMonsterClass::Kind_Of() const
{
    return 0x3F;
}


// int VeinholeMonsterClass::In_Which_Layer() const (0x00712720, slot 29)

int VeinholeMonsterClass::In_Which_Layer() const
{
    return -1;
}


// void *VeinholeMonsterClass::`scalar deleting destructor'(unsigned int) (0x00712730)
// Compiler-emitted with the unit's vtable; no source of its own.


extern "C" unsigned char vtable_712a40[]; extern "C" void __cdecl release_buffer_712a40(void*); extern "C" void __cdecl release_object_712a40(void*);
class Leaf_712a40 { public: void* get(unsigned int flags); };
void* Leaf_712a40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_712a40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_712a40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_712a40(this); return this; }


extern "C" unsigned char vtable_712a90[]; extern "C" void __cdecl release_buffer_712a90(void*); extern "C" void __cdecl release_object_712a90(void*);
class Leaf_712a90 { public: void* get(unsigned int flags); };
void* Leaf_712a90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_712a90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_712a90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_712a90(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<VeinholeMonsterClass *>::Clear (0x00712750; DynamicVectorClass<VeinholeMonsterClass *> vtable 0x007ae734 slot 3).
// DynamicVectorClass<VeinholeMonsterClass *>::ID_pointer (0x00712780; DynamicVectorClass<VeinholeMonsterClass *> vtable 0x007ae734 slot 5).
// VectorClass<VeinholeMonsterClass *>::Fetch (0x007127A0; VectorClass<VeinholeMonsterClass *> vtable 0x007ae754 slot 6).
// VectorClass<VeinholeMonsterClass *>::operator equal (0x007127B0; VectorClass<VeinholeMonsterClass *> vtable 0x007ae754 slot 1).
// VectorClass<VeinholeMonsterClass *>::Resize (0x007127F0; VectorClass<VeinholeMonsterClass *> vtable 0x007ae754 slot 2).
// VectorClass<VeinholeMonsterClass *>::Clear (0x007128A0; VectorClass<VeinholeMonsterClass *> vtable 0x007ae754 slot 3).
// VectorClass<VeinholeMonsterClass *>::ID_object (0x007128D0; VectorClass<VeinholeMonsterClass *> vtable 0x007ae754 slot 4).
// VectorClass<VeinholeMonsterClass *>::ID_pointer (0x00712910; VectorClass<VeinholeMonsterClass *> vtable 0x007ae754 slot 5).
// DynamicVectorClass<VeinholeMonsterClass *>::Resize (0x00712930; DynamicVectorClass<VeinholeMonsterClass *> vtable 0x007ae734 slot 2).
// DynamicVectorClass<VeinholeMonsterClass *>::ID_object (0x007129E0; DynamicVectorClass<VeinholeMonsterClass *> vtable 0x007ae734 slot 4).
// bool DynamicVectorClass<VeinholeMonsterClass *>::Delete(int index) (0x00712AE0).
// Instantiated from shared/vector_class.hpp; called at 0x007100BF for VeinholeMonsterClass::VeinholeMonsters (0x00AD0748).
