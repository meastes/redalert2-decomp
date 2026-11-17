/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/vein.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/vein.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/vein.cpp where both functions declare the same local types (names only).

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


/// <summary>
/// Creates a veinhole monster in preparation for loading a saved game.
/// The monster's own state arrives from the save game stream, but the growth records
/// are allocated fresh here since the pointers that were saved are meaningless now.
/// </summary>
// (0x0070F820)
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
    if (GlobalGrowthState == 0) {
        GlobalGrowthState = new unsigned char[Map_Cell_Count()];
    }
    GrowthState = new unsigned char[Map_Cell_Count()];
}


/// <summary>
/// Creates a veinhole monster at the cell specified.
/// The ground around the monster is sunk into a pit and ramped down to meet the terrain
/// beyond, and the records it needs to tend its own veins are allocated. A monster that
/// cannot be accommodated at the location is left in limbo and never joins the game.
/// </summary>
/// <param name="cell">The cell to place the center of the monster at.</param>
// (0x0070F9B0)
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
    if (ScenarioInit == 0) {
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                CellClass *spot = &Map[*cell + ::Cell(x, y)];
                spot->Ramp = unknown_veinhole_tiles_7f9508[y + 1][x + 1];
                if (spot->Ramp > 0) {
                    spot->IsoTileIndex = spot->Ramp + IsometricTileTypeClass_RampStart - 1;
                } else {
                    spot->IsoTileIndex = 0;
                }
                spot->SubTile = 0;
                spot->Height--;
            }
        }
    }
    Control.Set_Rate(4);
    Control.unknown_14 = 0;
    Control.Value = -1;
    GrowthNodes = new UnknownVeinNode[Rules_Vein_Node_Count()];
    GrowthQueue = new UnknownVeinHeap(Rules_Vein_Node_Count());
    GrowthQueue->Clear();
    if (GlobalGrowthState == 0) {
        GlobalGrowthState = new unsigned char[Map_Cell_Count()];
    }
    GrowthState = new unsigned char[Map_Cell_Count()];
    CoordStruct coord = CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    ObjectClass::Unlimbo(&coord, 0);
    Strength = Rules->VeinholeTypeClass->MaxStrength;
    TargetTracker.Add_Index(Fetch_ID(), this);
}


// 0070FDF0: remove the surrounding vein overlay, buffers and tracking entries.
#include "shared/overlay_type.hpp"
#include "shared/tactical.hpp"
#include "shared/object_index.hpp"
extern Rect TacticalRect;

inline RectPoint2D Rect::Top_Left() const { return RectPoint2D(X, Y); }
inline Rect Rect::operator-(RectPoint2D const &point) const { return Rect(Top_Left() - point, Width, Height); }

/// <summary>
/// Destroys this veinhole monster and scrubs its veins off the map.
/// The cells around the veinhole are stripped of their vein overlay and flagged for
/// redraw, and the monster is detached from everything that was tracking it.
/// </summary>
VeinholeMonsterClass::~VeinholeMonsterClass()
{
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            ::Cell cell(CellID + ::Cell(x, y));
            int cell_id = Map_Cell_Index(cell);
            if (cell_id >= 0 && cell_id < Map_Cell_Count()) {
                GlobalGrowthState[cell_id] = false;
                GrowthState[cell_id] = false;
            }
            CellClass *map_cell = &Map[cell];
            if (map_cell->OverlayIndex == 0x7e || (map_cell->OverlayIndex != -1 && OverlayTypes[map_cell->OverlayIndex]->IsVeins)) {
                Map.Radar_Background(map_cell->MapCoords);
                RectPoint2D origin(TacticalRect.Top_Left());
                TacticalMap->Register_Dirty_Area(Union(map_cell->Overlay_Render_Rect(), map_cell->Overlay_Shadow_Render_Rect()) - origin, false);
                map_cell->OverlayIndex = -1;
                map_cell->OverlayData = 0;
            }
        }
    }
    IsInLimbo = true;
    IsDown = false;
    if (Strength > 0) Detach_This_From_All(this, true);
    Clear_Growth();
    VeinholeMonsters.Delete(this);
    TargetTracker.Remove_Index(Fetch_ID());
}


/// <summary>
/// Finds the veinhole monster that sits at the cell specified.
/// </summary>
/// <returns>Returns with a pointer to the monster found, or NULL if there is none there.</returns>
// (0x00710160)
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


/// <summary>
/// Finds the veinhole monster that owns the veins at the cell specified.
/// </summary>
/// <returns>Returns with a pointer to the monster that grew the veins there, or NULL if
/// no monster lays claim to that cell.</returns>
// (0x007101B0)
// The last monster in VeinholeMonsters whose per-cell table (+0xB0) has a byte set at the cell, or null.

VeinholeMonsterClass *__fastcall VeinholeMonsterClass::Get_Vein_Owner_At(::Cell const &cell)
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (VeinholeMonsters[index]->GrowthState[Map_Cell_Index(cell)] != 0) {
            return VeinholeMonsters[index];
        }
    }
    return 0;
}


/// <summary>
/// Handles the per frame logic for every veinhole monster.
/// This routine is called by the main game logic loop. Monsters that have finished
/// dying are disposed of afterward.
/// </summary>
// (0x007101F0)
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
    int sample=Scen->Random()(0,0x7ffffffe);
    return (high-low)*((double)sample*vein_random_unit_79c280)+low;
}
/// <summary>
/// Handles the per frame logic for this veinhole monster.
/// The monster dozes until something wanders close by, then rouses itself and starts
/// belching gas clouds at the intruder until it is left alone again. This routine also
/// drives the growth and the withering of the monster's patch of veins.
/// </summary>
/// <remarks>Only call this routine once per veinhole monster per game logic loop.</remarks>
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
                if((double)(abs(Scen->Random()())%10000)*vein_idle_scale_7a5fe8<vein_idle_chance_7ae718) {
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
                ParticleClass *particle=GasSystem->Spawn_Held_Particle(ParticleTypes[ParticleTypeClass::Index_Or_Make(vein_gas_7f96cc)],position);
                particle->GasDrift.Z=12;
                double angle=Vein_Random_Double(0.0,1.0)*vein_full_angle_79c9b8;
                particle->GasDrift.X=(int)(Fast_Cos(angle)*vein_speed_79d558);
                particle->GasDrift.Y=(int)(Fast_Sin(angle)*vein_speed_79d558);
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


/// <summary>
/// Draws every veinhole monster on the map.
/// </summary>
// (0x00710820)
// Calls Draw_It on every monster, last first.

void __cdecl VeinholeMonsterClass::Draw_All()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->Draw_It();
    }
}


/// <summary>
/// Fetches the veinhole monster artwork for the theater specified.
/// This routine is called whenever the theater changes so that the monster is drawn
/// with artwork that suits the terrain around it.
/// </summary>
/// <param name="theater">The theater to load the monster shape for.</param>
// (0x00710840)
// Loads VEINHOLE.<theater suffix> into the shape pointer.
#include "shared/anim_type.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"

void __fastcall VeinholeMonsterClass::Init(int theater)
{
    char buffer[32];
    sprintf(buffer, "VEINHOLE.%s", Theaters[theater].Suffix);
    VeinholeMonsterClass_MonsterShape = MixFileClass::Retrieve(buffer);
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void VeinholeMonsterClass::Draw_It() (0x00710880)
/// <summary>
/// Draws this veinhole monster onto the tactical map.
/// The monster is drawn with whichever mouth frame its logic has settled on, and it
/// registers itself as a selectable object so that the player can click on it. A
/// monster hidden under fog keeps its mouth shut.
/// </summary>
// (0x0074D490)
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
        void const *shape = VeinholeMonsterClass_MonsterShape;
        int frame = ShapeFrame;
        CoordStruct coord = Cell_Coord(CellID);
        if (Map[coord].unknown_140 & 0x400000) {
            frame = 0;
        }
        Point2D drawpoint;
        TacticalMap->Coord_To_Pixel(coord, drawpoint);
        int height = 15 * Map[CellID].Height;
        drawpoint.Y += -58 - height;
        TacticalMap->Add_To_Selectables(this, Point2D(drawpoint.X, TacticalRect.Y + drawpoint.Y + 58));
        ConvertClass *drawer = ColorSchemes[PlayerPtr->ColorSchemeIndex]->Converter;
        CC_Draw_Shape(LogicalSurface, drawer, (ShapeFileStruct const *)shape, frame, drawpoint, TacticalRect, 0x2e00, 0, -17 - height, 0,
                      Map[coord].TileBrightness, 0, 0, Point2D(0, 0));
    }
}


/// <summary>
/// Applies damage to this veinhole monster.
/// A monster that survives being hurt rouses itself and starts belching gas at whatever
/// disturbed it. One that does not begins its death throes and is detached from
/// everything that was tracking it.
/// </summary>
/// <param name="damage">The damage to inflict; adjusted to the amount actually taken.</param>
/// <param name="forced">Should the damage be applied regardless of the usual immunities?</param>
/// <returns>Returns with the result of the damage, such as RESULT_DESTROYED.</returns>
// (0x007109C0, slot 86: take damage)
// ObjectClass's result decides the state (+0xB8): 4 -> 3 and the object is
// ended through 0x006ECB90, 5 -> unchanged, otherwise 2 and a 120 frame timer.

int VeinholeMonsterClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int no_crew)
{
    int result = ObjectClass::Take_Damage(damage, distance, warhead, source, forced, no_crew);
    switch (result) {
    case 0:
        break;
    case 4:
        DesiredState = 3;
        Detach_This_From_All(this, false);
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


/// <summary>
/// Can a veinhole monster be placed at the cell specified?
/// A monster demands a flat patch of clear ground to itself and must keep its distance
/// from any other veinhole. The check is waived while the scenario is still being set
/// up, since the map author is trusted to have placed them sensibly.
/// </summary>
/// <param name="cell">The cell to consider as the center of the monster.</param>
/// <returns>bool; Can the monster go here?</returns>
// (0x00710A60)
// True while the global at 0x00A40774 is positive; otherwise only when no
// monster is within 2 cells and the 3x3 block around the cell has no blocked
// cell, no occupier list and the same level as the cell (+0x11B).
#include "shared/crt_memory.hpp"

extern int ScenarioInit;

bool __fastcall VeinholeMonsterClass::Can_Monster_Go_Here(::Cell const &cell)
{
    if (ScenarioInit > 0) {
        return true;
    }
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (abs(cell.X - VeinholeMonsters[index]->CellID.X) < 3 && abs(cell.Y - VeinholeMonsters[index]->CellID.Y) < 3) {
            return false;
        }
    }
    int level = Map[cell].Height;
    if (level < 1) {
        return false;
    }
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            ::Cell neighbor = ::Cell(cell.X + x, cell.Y + y);
            CellClass *near_cell = &Map[neighbor];
            if (near_cell->Ramp || near_cell->Land != 0 || near_cell->Height != level) {
                return false;
            }
        }
    }
    return true;
}


/// <summary>
/// Destroys every veinhole monster in the game.
/// This routine is used when the scenario is being torn down, or when a saved game is
/// about to be loaded over the top of it.
/// </summary>
// (0x00710B50)
// Deletes every monster and empties VeinholeMonsters.

void __cdecl VeinholeMonsterClass::Reset()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        delete VeinholeMonsters[index];
    }
    VeinholeMonsters.Clear();
}


/// <summary>
/// Spreads this monster's vein patch by a few cells.
/// This routine is called by the monster's own logic whenever its growth timer expires.
/// Growth stops once the monster has reached the vein limit imposed by the rules, or if
/// vein growth has been switched off for the scenario.
/// </summary>
// (0x00710BB0)
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
    if (!Scen->IsVeinGrowth) {
        return;
    }
    int count = 0;
    Random2Class &random = Scen->Random();
    int steps = abs(random()) % 5 + 1;
    UnknownVeinNode *node = GrowthQueue->Remove_Min();
    while (count < steps && node != 0) {
        CellClass *cell = &Map[node->Position];
        if (cell->OverlayData < 0x30) {
            cell->Place_Veins();
            VeinCount++;
        }
        if (cell->OverlayIndex == 0x7e) {
            int index = Map_Cell_Index(node->Position);
            if (index >= 0 && index < Map_Cell_Count()) {
                GrowthState[index] = 1;
            }
        }
        if (cell->OverlayData >= 0x30) {
            for (int facing = 0; facing < 8; facing += 2) {
                ::Cell spot = node->Position + AdjacentCell[facing & 7];
                if (!Map.In_Local_Radar(spot, true)) {
                    continue;
                }
                CellClass *adjacent = &Map[spot];
                if (abs(adjacent->Height - cell->Height) >= 2) {
                    continue;
                }
                int index = Map_Cell_Index(spot);
                if (index < 0 || index >= Map_Cell_Count()) {
                    continue;
                }
                if (adjacent->Can_Place_Veins() && GlobalGrowthState[index] == 0 && GrowthCount < Rules_Vein_Node_Count()) {
                    GrowthNodes[GrowthCount].Position = spot;
                    GrowthNodes[GrowthCount].Key = Frame / 50 + abs(Scen->Random()() % 50) + 1;
                    GlobalGrowthState[index] = 1;
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


/// <summary>
/// Withers a few cells of this dying monster's vein patch.
/// This routine is called by the monster's own logic whenever its shrink timer expires.
/// </summary>
// (0x00710FF0)
// Spreads the veins: takes one to four nodes (random) with the lowest keys
// off the heap and grows the vein at each node's cell (0x00712300).
// Body: 646 bytes (ret at 0x00711275); the inventory splits it at 0x00711076.

void VeinholeMonsterClass::Shrink()
{
    if (GrowthQueue == 0) {
        return;
    }
    int count = 0;
    int steps = abs(Scen->Random()()) % 4 + 1;
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


/// <summary>
/// Prepares the vein growth system for play.
/// This routine is called once the map and its veinholes are in place. Every monster is
/// given the records it needs to tend its own patch, and any vein on the map that no
/// monster ends up claiming is quietly removed.
/// </summary>
/// <param name="clear">Should the existing growth records be thrown away and rebuilt?</param>
// (0x00711280)
// With rebuild, frees every monster's buffers and the shared table and
// allocates them again (nodes, heap and per-cell table); then clears the
// shared table and the cells' vein bit (0x8000), lets every monster mark its
// cells again (placed ones 0x00711AA0, others 0x007115B0) and removes vein
// overlays (0x7E) no monster covers.

void __fastcall VeinholeMonsterClass::Init_Vein_Growth_System(bool clear)
{
    int cells = Map_Cell_Count();
    int cell_count;
    if (clear) {
        Deinit_Vein_Growth_System();
        for (cell_count = VeinholeMonsters.Count() - 1; cell_count >= 0; cell_count--) {
            VeinholeMonsters[cell_count]->GrowthNodes = new UnknownVeinNode[Rules_Vein_Node_Count()];
            VeinholeMonsters[cell_count]->GrowthQueue = new UnknownVeinHeap(Rules_Vein_Node_Count());
            VeinholeMonsters[cell_count]->GrowthState = new unsigned char[cells];
            memset(VeinholeMonsters[cell_count]->GrowthState, 0, cells);
            VeinholeMonsters[cell_count]->GrowthQueue->Clear();
        }
    }
    if (GlobalGrowthState == 0) {
        GlobalGrowthState = new unsigned char[cells];
    }
    Map.Reset_Iterator();
    for (CellClass *iter = Map.Iterate(); iter != 0; iter = Map.Iterate()) {
        iter->unknown_140 &= ~0x8000;
    }
    memset(GlobalGrowthState, 0, cells);
    for (cell_count = VeinholeMonsters.Count() - 1; cell_count >= 0; cell_count--) {
        if (VeinholeMonsters[cell_count]->IsDead) {
            VeinholeMonsters[cell_count]->Build_Shrinking_Queue();
        } else {
            VeinholeMonsters[cell_count]->Build_Growth_Queue();
        }
    }
    Map.Reset_Iterator();
    for (iter = Map.Iterate(); iter != 0; iter = Map.Iterate()) {
        if (iter->OverlayIndex == 0x7e && GlobalGrowthState[Map_Cell_Index(iter->MapCoords)] == 0) {
            iter->OverlayIndex = -1;
        }
        iter->unknown_140 &= ~0x8000;
    }
}


/// <summary>
/// Frees the vein growth records of every veinhole monster.
/// </summary>
// (0x007114F0)
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
/// <summary>
/// Builds the queue of cells this monster can grow its veins from.
/// This routine spreads outward from the veinhole across the connected patch of veins,
/// claiming those cells for this monster and queueing the ones that still have room to
/// thicken or push further out.
/// </summary>
void VeinholeMonsterClass::Build_Growth_Queue()
{
    DynamicVectorClass<::Cell> pending;
    pending.Set_Growth_Step(5000);
    GrowthCount=0;
    VeinCount=0;
    GrowthQueue->Clear();
    for (int index=Map_Cell_Count()-1; index>=0; --index) GrowthState[index]=0;
    int row_or_facing;
    for (row_or_facing=-2; row_or_facing<=2; ++row_or_facing) {
        for (int column=-2; column<=2; ++column) {
            CellClass *cell=&Map[::Cell(column,row_or_facing)+CellID];
            if (cell->OverlayIndex==0x7e) {
                int index=Map_Cell_Index(cell->MapCoords);
                pending.Add(::Cell(column,row_or_facing)+CellID);
                if (index>=0 && index<Map_Cell_Count()) {
                    GlobalGrowthState[index]=1;
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
            int index=Map_Cell_Index(adjacent->MapCoords);
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
                if (index>=0 && index<Map_Cell_Count()) {
                    GlobalGrowthState[index]=1;
                    GrowthState[index]=1;
                }
                ++VeinCount;
            }
        }
    }
}


#include "shared/anim.hpp"

/// <summary>
/// Builds the queue of vein cells for this dying monster to wither away.
/// This routine is used once a veinhole has been destroyed. Only the mature vein cells
/// this monster is recorded as owning are gathered, and the ones farthest from the
/// veinhole are the first to go.
/// </summary>
void VeinholeMonsterClass::Build_Shrinking_Queue()
{
    GrowthCount = 0;
    GrowthQueue->Clear();
    VeinCount = 0;

    Map.Reset_Iterator();
    CellClass *iter = Map.Iterate();
    while (iter != 0) {
        if (iter->OverlayIndex == 0x7e && iter->OverlayData >= 0x30) {
            int index = Map_Cell_Index(iter->MapCoords);
            if (index >= 0 && index < Map_Cell_Count() && GrowthState[index]) {
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


/// <summary>
/// Frees the growth state that is shared between all the veinhole monsters.
/// </summary>
// (0x00711C70)

void __cdecl VeinholeMonsterClass::Clear_Global_Data()
{
    if (GlobalGrowthState != 0) {
        delete[] GlobalGrowthState;
        GlobalGrowthState = 0;
    }
}


/// <summary>
/// Frees the vein growth records held by this monster.
/// </summary>
// (0x00711C90; Clear_Growth as TiberiumClass names it)

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


/// <summary>
/// Kills this veinhole monster and leaves a patch of veins behind.
/// The cells the monster occupied are turned over to plain vein overlay, the map zones
/// are brought up to date, and a shrinking queue is built so that the leftover patch
/// will wither away over time.
/// </summary>
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
    Map.Update_Cell_Subzones(CellID);
    IsDead = true;
}


/// <summary>
/// Disposes of any veinhole monster that has finished dying.
/// A destroyed monster lingers on so that its patch of veins can wither away, so this
/// routine only deletes one once it has no shrinking work left to do.
/// </summary>
// (0x00711E20)
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


/// <summary>
/// Loads every veinhole monster from the save game stream.
/// This routine is called by the scenario load process. Any monsters presently in
/// existence are destroyed first, then each saved monster is recreated with its vein
/// growth records and handed to the swizzler and the target tracker.
/// </summary>
/// <returns>bool; Were all the monsters read successfully?</returns>
bool __fastcall VeinholeMonsterClass::Load_All(IStream *stream)
{
    Reset();

    int cell_count = Map_Cell_Count();

    int monster_count;
    if (FAILED(stream->Read(&monster_count, sizeof(monster_count), 0))) {
        return false;
    }

    GlobalGrowthState = new unsigned char[cell_count];
    if (FAILED(stream->Read(GlobalGrowthState, cell_count, 0))) {
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

        if (FAILED(stream->Read(nodes, sizeof(UnknownVeinNode) * Rules->MaxVeinholeGrowth, 0))) {
            return false;
        }
        monster->GrowthNodes = nodes;

        if (!queue->Load(stream, nodes)) {
            return false;
        }
        monster->GrowthQueue = queue;

        TargetTracker.Add_Index(monster->Fetch_ID(), monster);
    }

    return true;
}


/// <summary>
/// Saves every veinhole monster to the save game stream.
/// This routine is called by the scenario save process. Each monster is written along
/// with its vein growth records so that growth can pick up where it left off.
/// </summary>
/// <returns>bool; Were all the monsters written successfully?</returns>
bool __fastcall VeinholeMonsterClass::Save_All(IStream *stream)
{
    int monster_count = VeinholeMonsters.Count();
    if (FAILED(stream->Write(&monster_count, sizeof(monster_count), 0))) {
        return false;
    }

    int cell_count = Map_Cell_Count();
    if (FAILED(stream->Write(GlobalGrowthState, cell_count, 0))) {
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

        if (FAILED(stream->Write(VeinholeMonsters[i]->GrowthNodes, sizeof(UnknownVeinNode) * Rules->MaxVeinholeGrowth, 0))) {
            return false;
        }

        VeinholeMonsters[i]->GrowthQueue->Save(stream, VeinholeMonsters[i]->GrowthNodes);
    }

    return true;
}


/// <summary>
/// Saves this veinhole monster to the save game stream.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once it has been written?</param>
/// <returns>Returns with S_OK if the monster was written successfully.</returns>
// (0x007122D0)

HRESULT __stdcall VeinholeMonsterClass::Save(IStream *stream, BOOL cleardirty)
{
    return AbstractClass::Save(stream, cleardirty);
}


/// <summary>
/// Fetches the object type class for this veinhole monster.
/// </summary>
/// <returns>Returns with a pointer to the veinhole type as specified by the rules.</returns>
// (0x007122F0, slot 33)
// The type named by the rules (+0xCB4).

ObjectTypeClass *VeinholeMonsterClass::Class_Of() const
{
    return Rules->VeinholeTypeClass;
}


/// <summary>
/// Removes a stage of vein growth from the cell specified.
/// This routine is used by the shrinking logic of a dying veinhole. The neighboring
/// cells are re-examined afterward so that any cell left bare is dropped from the
/// growth records, while a cell that still carries veins is queued for further work.
/// </summary>
/// <param name="cellptr">Pointer to the cell to reduce the veins at.</param>
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
                int cindex = Map_Cell_Index(adjacent->MapCoords);
                if (cindex >= 0 && cindex < Map_Cell_Count()) {
                    GrowthState[cindex] = false;
                    GlobalGrowthState[cindex] = false;
                }

                if (facingbools[facing / 2] == true && !IsDead) {
                    GrowthQueue->Remove_Matching(UnknownVeinNode(adjacent->MapCoords));
                }
            }
        }

        int overlay = cellptr->OverlayIndex;
        ::Cell &cell = cellptr->MapCoords;
        int cindex = Map_Cell_Index(cell);
        if (cindex >= 0 && cindex < Map_Cell_Count()) {
            if (overlay == -1) {
                GrowthState[cindex] = false;
                GlobalGrowthState[cindex] = false;
            } else if (!IsDead) {
                if (GrowthCount < Rules->MaxVeinholeGrowth) {
                    GrowthNodes[GrowthCount].Position = cell;
                    GrowthNodes[GrowthCount].Key = float(Frame / 50 + abs(Scen->Random()() % 50) + 1);
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                    GrowthState[cindex] = true;
                    GlobalGrowthState[cindex] = true;
                }
            }
        }
    }
}


/// <summary>
/// Fetches the class identifier used by the save game system.
/// This routine is called by the persistence layer so that it knows which class to
/// recreate when the saved game is read back in.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x007126C0)

HRESULT __stdcall VeinholeMonsterClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_VeinholeMonsterClass;
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


// VectorClass<VeinholeMonsterClass *>::`scalar deleting destructor' (0x00712A40)
// Compiler-emitted in this unit; no source of its own.


// DynamicVectorClass<VeinholeMonsterClass *>::`scalar deleting destructor' (0x00712A90)
// Compiler-emitted in this unit; no source of its own.


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
