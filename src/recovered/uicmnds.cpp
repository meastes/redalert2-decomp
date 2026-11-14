// uicmnds.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/uicmnds.json carries the addresses and the rows that
// stay in their own files.


#define RA2_UICMNDS_GROUP_SELECT
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/uicmnds.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/infantry.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/planning.hpp"
#include "shared/map_view.hpp"
#include "shared/cd_timer.hpp"
#include "shared/id_source.hpp"
#include "shared/string_table.hpp"
#include "shared/keyboard.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/object_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/radar_event_class.hpp"
#include "shared/tactical.hpp"
#include "shared/cell_class.hpp"
#include "shared/abstract_class.hpp"


// Header-static dynamic initializers of the unit at 0x006F55F0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x006F55F0-0x006F57D1); the other
// drafts of this plan are emitted by these includes.
// The group-select functions (0x006F5F00/0x006F60D0) need DisplayClass::Unknown_0049E3B0 from map_view.hpp.
// Unknown_006F85E0 (0x006F85E0) expands DynamicVectorClass<int> construction.


// Dynamic initializer (0x006F5610, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F5640, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F5660, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F5680, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F56A0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F56C0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F56E0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F5720, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F5750, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F5780, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F57B0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// Dynamic initializer (0x006F57C0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_006f55f0.cpp; no source of its own.


// void Unknown_006F57E0() (0x006F57E0)
// Deletes the list at 0x00AC1344 (Prog_End calls it; the list is the one Unknown_006F79C0 rebuilds).

extern UnknownPointerList *unknown_list_ac1344;

void Unknown_006F57E0()
{
    if (unknown_list_ac1344 != 0) {
        delete (DynamicVectorClass<TechnoClass *> *)unknown_list_ac1344;
        unknown_list_ac1344 = 0;
    }
}


// bool __fastcall Unknown_006F5800(int group) (0x006F5800)
// Whether no player-side, unselected techno of the map belongs to group
// (group - 1 is stored at +0x1AC).


bool __fastcall Unknown_006F5800(int group)
{
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76) {
            if (Is_Player_Side(techno) && !techno->unknown_77 && techno->unknown_1ac == group - 1) {
                return false;
            }
        }
    }
    return true;
}


// int __fastcall Unknown_006F5870(int group) (0x006F5870)
// Counts the player-side technos of the map that belong to group (group - 1
// is stored at +0x1AC).


int __fastcall Unknown_006F5870(int group)
{
    int count = Technos.Count();
    int members = 0;
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76) {
            if (Is_Player_Side(techno) && techno->unknown_1ac == group - 1) {
                members++;
            }
        }
    }
    return members;
}


// 0x006f58e0: function not yet identified
//
// Body: 58 bytes (frozen inventory bound).
// Referenced from 25 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_006f58e0(undefined4 param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006f58e0 {

struct Callee_005b4da0 { unsigned int Add_Message_005b4da0(int, int, int, int, int, int, int); };
extern "C" unsigned int DAT_00a35db4;
extern "C" unsigned int DAT_00a3dc70;

void __fastcall FUN_006f58e0(unsigned int message_address, int delay)
{
    unsigned int color;

    if (delay == -1) {
        delay = (int)(0xf0);
    }
    color = (unsigned int)(3);
    if (DAT_00a35db4 != 0) {
        color = (unsigned int)(*(unsigned int *)(DAT_00a35db4 + 0x15f40));
    }
    ((Callee_005b4da0*)(&DAT_00a3dc70))->Add_Message_005b4da0((int)(0), (int)(0), (int)(message_address), (int)(color), (int)(0x4046), (int)(delay), (int)(1));
    return;
}

}  // namespace seed_006f58e0


// 0x006f5920: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_006f5920(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006f5920 {

unsigned int  FUN_006f5920(void)
{
    return 0x4046;
}

}  // namespace seed_006f5920


// 0x006f5930: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_006f5930(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006f5930 {

unsigned int  FUN_006f5930(void)
{
    return 0xf0;
}

}  // namespace seed_006f5930


// void Unknown_006F5940() (0x006F5940; TabClass::AI and the deploy command 0x0051BF20): the deploy key. A selected
// player building with a positive 0x004532F0 is deployed alone (slot 199 with 9); otherwise every selected
// deployable techno (slot 39 or 38, slots 185 and 186) gets slot 199 with 9, except that when the selection mixes
// deploying and undeployed deployer infantry the deploying ones are left; one sound (Rules +0x64C) when any did.
// The count is read once after the building test; undeployed, the index and deploying are initialised in that
// order (bl, edi, then a stack byte).

void Unknown_006F5940()
{
    bool deployed = false;
    if (unknown_objects_a40c60.Count() <= 0) {
        return;
    }
    if (unknown_objects_a40c60[0] != 0) {
        BuildingClass *building = (BuildingClass *)unknown_objects_a40c60[0];
        if (building != 0 && building->Kind_Of() == 6 && building->unknown_4532f0() > 0
            && building->House->Is_Player_Control()) {
            ObjectClass *object = unknown_objects_a40c60[0];
            if (object != 0 && object->AbstractFlags.IsTechno && ((TechnoClass *)object)->unknown_slot199(9)) {
                Voc_Play(Rules->unknown_64c, 1.0f, 0x2000, 0);
            }
            return;
        }
    }

    int count = unknown_objects_a40c60.Count();
    bool undeployed = false;
    int index = 0;
    bool deploying = false;
    for (; index < count; index++) {
        ObjectClass *object = unknown_objects_a40c60[index];
        if (object != 0 && object->Kind_Of() == 15 && ((InfantryClass *)object)->unknown_5090a0()) {
            if (((InfantryClass *)object)->unknown_5090d0()) {
                deploying = true;
            } else {
                undeployed = true;
            }
            if (deploying && undeployed) {
                break;
            }
        }
    }

    for (index = 0; index < count; index++) {
        ObjectClass *object = unknown_objects_a40c60[index];
        if (object != 0 && (object->Can_Player_Move() || object->Can_Player_Fire()) && object->AbstractFlags.IsTechno
            && ((TechnoClass *)object)->unknown_slot185() && ((TechnoClass *)object)->unknown_slot186()) {
            if (object->Kind_Of() == 15 && ((InfantryClass *)object)->unknown_5090a0() && deploying && undeployed
                && ((InfantryClass *)object)->unknown_5090d0()) {
                continue;
            }
            if (((TechnoClass *)object)->unknown_slot199(9)) {
                deployed = true;
            }
        }
    }
    if (deployed) {
        Voc_Play(Rules->unknown_64c, 1.0f, 0x2000, 0);
    }
}


// void __fastcall Unknown_006F5B10() (0x006F5B10)
// The guard command (hypothesis): in planning mode it is refused; otherwise every listed selected
// object that passes slots 39 and 38 is asked slot 200 (0x0B, its slot 107 answer); when all agree the
// rules' GuardSound (+0x644) plays.
bool Planning_Mode_Active_614840();

void __fastcall Unknown_006F5B10()
{
    if (Planning_Mode_Active_614840()) {
        Planning_Refuse_No_Guard_Area();
        return;
    }
    if (unknown_objects_a40c60.Count() > 0) {
        char agreed = 1;
        int count = unknown_objects_a40c60.Count();
        for (int index = 0; index < count; index++) {
            ObjectClass *object = unknown_objects_a40c60[index];
            if (object != 0 && object->AbstractFlags.IsTechno && object->Can_Player_Move() && object->Can_Player_Fire()) {
                agreed = ((TechnoClass *)object)->unknown_slot200(0x0B, (AbstractClass *)object->Get_Target_Cell_Ptr(), 0, 0);
                if (!agreed) {
                    return;
                }
            }
        }
        if (agreed) {
            Voc_Play(Rules->unknown_644, 1.0f, 0x2000, 0);
        }
    }
}


// void Unknown_006F5BC0() (0x006F5BC0)
// Answers a group command (hypothesis): when every listed object that passes slot 39 or 38 answers slot 199 (6), the rules' +0x640 sound plays.


void Unknown_006F5BC0()
{
    if (unknown_objects_a40c60.Count() > 0) {
        char agreed = 1;
        int count = unknown_objects_a40c60.Count();
        for (int index = 0; index < count; index++) {
            ObjectClass *object = unknown_objects_a40c60[index];
            if (object != 0 && object->AbstractFlags.IsTechno && (object->Can_Player_Move() || object->Can_Player_Fire())) {
                agreed = (char)((TechnoClass *)object)->unknown_slot199(6);
                if (!agreed) {
                    break;
                }
            }
        }
        if (agreed) {
            Voc_Play(Rules->unknown_640, 1.0f, 0x2000, 0);
        }
    }
}


// void Unknown_006F5C50() (0x006F5C50)
// The cheer command: once the rules' MaximumCheerRate frames have passed (and not while
// 0x00A3D548 is set), queues a cheer event (0x2D) for the player.

void Unknown_006F5C50()
{
    if (Frame >= unknown_cheer_frame_ac134c && unknown_int_a3d548 == 0) {
        OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x2D));
        unknown_cheer_frame_ac134c = Rules->MaximumCheerRate + Frame;
    }
}


// void Unknown_006F5D10() (0x006F5D10)
// Answers a group command (hypothesis): when every listed object that is
// an object and passes slot 39 also answers slot 199 (7), the rules' +0x648
// sound plays; one refusal silences it.

void Unknown_006F5D10()
{
    if (unknown_objects_a40c60.Count() > 0) {
        char agreed = 1;
        int count = unknown_objects_a40c60.Count();
        for (int index = 0; index < count; index++) {
            ObjectClass *object = unknown_objects_a40c60[index];
            if (object != 0 && object->AbstractFlags.IsTechno && object->Can_Player_Move()) {
                agreed = (char)((TechnoClass *)object)->unknown_slot199(7);
                if (!agreed) {
                    break;
                }
            }
        }
        if (agreed) {
            Voc_Play(Rules->unknown_648, 1.0f, 0x2000, 0);
        }
    }
}


// void __fastcall Unknown_006F5D90(int group) (0x006F5D90)
// Assigns group (group - 1 at +0x1AC) to every selected player-side techno
// of the map, after clearing it from the others that held it.


void __fastcall Unknown_006F5D90(int group)
{
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76) {
            if (Is_Player_Side(techno)) {
                if (techno->unknown_1ac == group - 1) {
                    techno->unknown_1ac = -1;
                }
                if (techno->unknown_77) {
                    techno->unknown_1ac = group - 1;
                }
            }
        }
    }
}


// void __fastcall Unknown_006F5E10(int group) (0x006F5E10)
// Clears group (group - 1 at +0x1AC) from every player-side techno of the map.


void __fastcall Unknown_006F5E10(int group)
{
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76) {
            if (Is_Player_Side(techno) && techno->unknown_1ac == group - 1) {
                techno->unknown_1ac = -1;
            }
        }
    }
}


// bool __fastcall Unknown_006F5E80(int group) (0x006F5E80)
// Whether the player-side technos of the map that are in group (group - 1 at +0x1AC) are exactly
// the selected ones.

bool __fastcall Unknown_006F5E80(int group)
{
    for (int index = Technos.Count() - 1; index >= 0; index--) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76 && Is_Player_Side(techno)) {
            if (techno->unknown_1ac == group - 1 && !techno->unknown_77) {
                return false;
            }
            if (techno->unknown_1ac != group - 1 && techno->unknown_77) {
                return false;
            }
        }
    }
    return true;
}


// void __fastcall Unknown_006F5F00(int group) (0x006F5F00; TabClass::AI): the group-select key. A second press of
// the same group within 25 frames while the group is exactly the selection (0x006F5E80, expanded) re-selects it
// through 0x006F60D0; otherwise the selection is dropped and the group's player-side technos are selected and
// flashed (unknown_13c restarted at Rules->RadarCombatFlashTime), with the effect sound suppressed.
void __fastcall Unknown_006F60D0(int group);
// The frame and group of the last group selection (a second press within 25 frames centres the view).
extern int unknown_group_frame_7f8bcc;     // 0x007F8BCC
extern int unknown_group_7f8bd0;           // 0x007F8BD0

void __fastcall Unknown_006F5F00(int group)
{
    Map.Unknown_0049C980(0);
    Map.Unknown_0049C860(0, false);
    Map.Unknown_0049CA20(0);
    Map.Unknown_0049C7C0(0);
    Map.Unknown_0049CAC0(0);
    if (Frame - unknown_group_frame_7f8bcc < 25 && group == unknown_group_7f8bd0 && Unknown_006F5E80(group)) {
        Unknown_006F60D0(group);
        return;
    }
    unknown_group_frame_7f8bcc = Frame;
    unknown_group_7f8bd0 = group;
    unknown_selected_83996c->Unknown_006AAD40();
    unknown_sound_enabled_7d88d6 = true;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76 && techno->unknown_1ac == group - 1) {
            if (Is_Player_Side(techno) && !techno->unknown_77) {
                techno->unknown_slot80();
                techno->unknown_13c = CDTimerClass<FrameTimerClass>(Rules->RadarCombatFlashTime);
                unknown_sound_enabled_7d88d6 = false;
            }
        }
    }
    unknown_sound_enabled_7d88d6 = true;
    TechnoClass::unknown_6d7990();
}


// void __fastcall Unknown_006F60D0(int group) (0x006F60D0): re-selects the group: the selection is dropped unless
// its first object is a foot in the group, the group's player-side technos are selected (sound suppressed) and
// the screen is redrawn. The first object's foot flag goes through a bool local (it is stored and the element
// read again).

void __fastcall Unknown_006F60D0(int group)
{
    Map.Unknown_0049C980(0);
    Map.Unknown_0049C860(0, false);
    Map.Unknown_0049CA20(0);
    Map.Unknown_0049C7C0(0);
    Map.Unknown_0049CAC0(0);
    if (unknown_objects_a40c60.Count() != 0) {
        ObjectClass *object = unknown_objects_a40c60[0];
        bool is_foot = false;
        if (object != 0) {
            is_foot = object->AbstractFlags.IsFoot;
        }
        if (!is_foot || ((TechnoClass *)unknown_objects_a40c60[0])->unknown_1ac != group - 1) {
            unknown_selected_83996c->Unknown_006AAD40();
        }
    }
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76 && techno->unknown_1ac == group - 1) {
            if (Is_Player_Side(techno) && !techno->unknown_77) {
                techno->unknown_slot80();
                unknown_sound_enabled_7d88d6 = false;
            }
        }
    }
    Map.Unknown_0049E3B0();
    Map.Flag_To_Redraw(1);
    unknown_sound_enabled_7d88d6 = true;
}


// void __fastcall Unknown_006F61F0(int group) (0x006F61F0)
// Selects the group (hypothesis: group - 1 at +0x1AC): the screen's placement
// modes are switched off, then every unselected player-side techno of the
// group gets slot 80 (selection) with the effect sound suppressed.


void __fastcall Unknown_006F61F0(int group)
{
    Map.Unknown_0049C980(0);
    Map.Unknown_0049C860(0, false);
    Map.Unknown_0049CA20(0);
    Map.Unknown_0049C7C0(0);
    Map.Unknown_0049CAC0(0);
    unknown_sound_enabled_7d88d6 = true;
    for (int index = 0; index < Technos.Count(); index++) {
        TechnoClass *techno = Technos[index];
        if (techno != 0 && !techno->unknown_76 && techno->unknown_1ac == group - 1) {
            if (Is_Player_Side(techno) && !techno->unknown_77) {
                techno->unknown_slot80();
                unknown_sound_enabled_7d88d6 = false;
            }
        }
    }
    unknown_sound_enabled_7d88d6 = true;
}


// void __fastcall Unknown_006F62D0(unsigned long argument) (0x006F62D0)

void __fastcall Unknown_006F62D0(unsigned long argument)
{
    if (unknown_game_mode_a3d298 != 0 && !(unknown_id_source_a3d290->unknown_000 & 0x400) && Rules->unknown_111d) {
        OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 3, argument));
    }
}


// void Unknown_006F6380() (0x006F6380)
// Queues an event of type 3 naming the owner (slot 14) of the first selected object, when that owner is another
// house with flag +0x134, the player does not have +0x13D, and the multiplayer rules allow it.

void Unknown_006F6380()
{
    if (unknown_objects_a40c60.Count() != 0 && !unknown_player_a35db4->unknown_13d) {
        if (unknown_objects_a40c60[0]->Owning_House() != unknown_player_a35db4 && unknown_objects_a40c60[0]->Owning_House()->unknown_134) {
            unsigned long owner = unknown_objects_a40c60[0]->Owner();
            if (unknown_game_mode_a3d298 != 0 && !(unknown_id_source_a3d290->unknown_000 & 0x400) && Rules->unknown_111d) {
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 3, owner));
            }
        }
    }
}


// 0x006f64b0: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: void FUN_006f64b0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006f64b0 {

struct Callee_0049cac0 { void FUN_0049cac0(int unknown_argument); };
extern "C" unsigned int Map_008324e0;
extern "C" unsigned int DAT_00a3d548;

void  FUN_006f64b0(void)
{
    if (DAT_00a3d548 == 0) {
        ((Callee_0049cac0*)(&Map_008324e0))->FUN_0049cac0((int)(0xffffffff));
    }
    return;
}

}  // namespace seed_006f64b0


// bool Planning_Mode_Active_6f6510() (0x006F6510)
// Whether the waypoint planning mode is on: forwards to 0x00614840 (compiled as a tail jump). No callers
// in the image.

bool Planning_Mode_Active_614840();

bool Planning_Mode_Active_6f6510()
{
    return Planning_Mode_Active_614840();
}


// bool Unknown_006F6520() (0x006F6520)
// True when every listed object (selected objects) answers slot 275.

bool Unknown_006F6520()
{
    int count = unknown_objects_a40c60.Count();
    for (int index = 0; index < count; index++) {
        ObjectClass *object = unknown_objects_a40c60[index];
        bool flag;
        if (object != 0) {
            flag = object->AbstractFlags.IsTechno;
        }
        if (object != 0 && flag) {
            if (!((TechnoClass *)object)->unknown_slot275()) {
                return false;
            }
        }
    }
    return true;
}


// void Unknown_006F6570() (0x006F6570)
// Checks the selection for the group command (hypothesis): with nothing
// selected, or with a selected object that answers no to slot 275, the
// matching message is shown (MSG:NothingSelected / MSG:AttackMoveUnsupported);
// otherwise the flag 0x00AC1330 is raised.

void Unknown_006F6570()
{
    if (unknown_objects_a40c60.Count() == 0) {
        Show_Message(StringTable::Load_String("MSG:NothingSelected", 0, "D:\\Ra2\\UICmnds.cpp", 0x2E9), 0xF0);
    } else {
        int count = unknown_objects_a40c60.Count();
        for (int index = 0; index < count; index++) {
            ObjectClass *object = unknown_objects_a40c60[index];
            bool flag;
            if (object != 0) {
                flag = object->AbstractFlags.IsTechno;
            }
            if (object != 0 && flag) {
                if (!((TechnoClass *)object)->unknown_slot275()) {
                    Show_Message(StringTable::Load_String("MSG:AttackMoveUnsupported", 0, "D:\\Ra2\\UICmnds.cpp", 0x2EF), 0xF0);
                    return;
                }
            }
        }
    }
    unknown_flag_ac1330 = true;
}


// bool Unknown_Can_Force_Order_006F6670() -- 0x006F6670
// Purpose: true when the group flag at 0x00AC1330 is set; otherwise needs a
// force-fire key and a force-move key held, and every listed object that is
// flagged to answer slot 275 (hypothesis: the force-move-and-fire order is
// possible for the whole selection).

// A key binding is down (the bindings are ints; WWKeyboardClass takes 16-bit keys).
inline bool Key_Down(int key) { return WWKeyboard->Down(key); }

bool Unknown_Can_Force_Order_006F6670()
{
    if (unknown_flag_ac1330) {
        return true;
    }
    bool force_fire = Key_Down(KeyForceFire1) || Key_Down(KeyForceFire2);
    bool force_move = Key_Down(KeyForceMove1) || Key_Down(KeyForceMove2);
    if (!force_fire || !force_move) {
        return false;
    }
    int count = unknown_objects_a40c60.Count();
    for (int index = 0; index < count; index++) {
        ObjectClass *object = unknown_objects_a40c60[index];
        bool flag;
        if (object != 0) {
            flag = object->AbstractFlags.IsTechno;
        }
        if (object != 0 && flag) {
            if (!((TechnoClass *)object)->unknown_slot275()) {
                return false;
            }
        }
    }
    return true;
}


class Leaf_6f6730 { public: unsigned int get(); };
unsigned int Leaf_6f6730::get() { return 0x47; }


class Leaf_6f6740 { public: unsigned int get(); };
unsigned int Leaf_6f6740::get() { return 0x16; }


class Target_6f6750 { public: unsigned int target(unsigned int p0, unsigned int p1); };
class Leaf_6f6750 { public: unsigned int get(); };
unsigned int Leaf_6f6750::get() { return ((Target_6f6750*)(0x8324e0))->target((unsigned int)(0x4d), (unsigned int)(0)); }


class Target_6f6760 { public: unsigned int target(unsigned int p0, unsigned int p1); };
class Leaf_6f6760 { public: unsigned int get(); };
unsigned int Leaf_6f6760::get() { return ((Target_6f6760*)(0x8324e0))->target((unsigned int)(0x4c), (unsigned int)(0)); }


// void Unknown_Click_End_006F6770() (0x006F6770): clears the flag at 0x00AC1330
// (Mouse_Left_Release 0x0049BB20 calls it after a placement or selection click).

void Unknown_Click_End_006F6770()
{
    unknown_flag_ac1330 = false;
}


// void __fastcall Unknown_006f6780(unsigned int state) (0x006F6780): sets the command
// state word 0x00AC132C (the combatant/experience commands test 1 and 4) and clears
// the flag at 0x00AC1330.

extern int unknown_state_ac132c;

void __fastcall Unknown_006f6780(unsigned int state)
{
    unknown_state_ac132c = state;
    unknown_flag_ac1330 = false;
}


// void __fastcall Unknown_006F6790(wchar_t *buffer) (0x006F6790)
// Copies the message that tells why the selection cannot be used (hypothesis:
// the cross-map / cross-screen selection commands): nothing selected, a
// selection across the map (flag 0x00AC133C) or across the screen.

extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);

void __fastcall Unknown_006F6790(wchar_t *buffer)
{
    if (unknown_objects_a40c60.Count() == 0) {
        wcscpy(buffer, StringTable::Load_String("MSG:NothingSelected", 0, "D:\\Ra2\\UICmnds.cpp", 0x382));
        return;
    }
    if (unknown_flag_ac133c) {
        wcscpy(buffer, StringTable::Load_String("MSG:SelAcrossMap", 0, "D:\\Ra2\\UICmnds.cpp", 0x384));
        return;
    }
    wcscpy(buffer, StringTable::Load_String("MSG:SelAcrossScreen", 0, "D:\\Ra2\\UICmnds.cpp", 0x386));
}


// void __fastcall Unknown_006F6810(wchar_t *buffer, wchar_t const *name) (0x006F6810)
// Words the "units worth" message (hypothesis): MSG:NavEmpty when there is
// no list at 0x00AC1344 or no name; otherwise the name is upper-cased and either
// MSG:NoUnitsSel (nothing selected) or MSG:UnitsWorth (count, name, the summed
// cost of the selected objects' types) is formatted into buffer.

extern UnknownPointerList *unknown_list_ac1344;
extern "C" wchar_t *__cdecl wcscpy(wchar_t *destination, wchar_t const *source);
extern "C" wchar_t *__cdecl _wcsupr(wchar_t *string);

void __fastcall Unknown_006F6810(wchar_t *buffer, wchar_t const *name)
{
    if (unknown_list_ac1344->Count == 0 || name == 0) {
        wcscpy(buffer, StringTable::Load_String("MSG:NavEmpty", 0, "D:\\Ra2\\UICmnds.cpp", 0x38F));
        return;
    }
    wchar_t upper[100];
    wcscpy(upper, name);
    _wcsupr(upper);
    int count = unknown_objects_a40c60.Count();
    if (count == 0) {
        swprintf(buffer, StringTable::Load_String("MSG:NoUnitsSel", 0, "D:\\Ra2\\UICmnds.cpp", 0x399), upper);
        return;
    }
    int total = 0;
    for (int index = 0; index < count; index++) {
        ObjectClass *object = unknown_objects_a40c60[index];
        bool flag;
        if (object != 0) {
            flag = object->AbstractFlags.IsTechno;
        }
        if (object != 0 && flag) {
            TechnoClass *techno = (TechnoClass *)object;
            HouseClass *owner = techno->House;
            total += techno->Class_Of()->Cost_Of(owner);
        }
    }
    swprintf(buffer, StringTable::Load_String("MSG:UnitsWorth", 0, "D:\\Ra2\\UICmnds.cpp", 0x3AD), count, upper, total);
}


// DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument) (0x006F6930)
// The technos (0x00A40C20) for which test(techno, argument) holds, in a new list.

DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument)
{
    DynamicVectorClass<TechnoClass *> *list = new DynamicVectorClass<TechnoClass *>(0, 0);
    int count = Technos.Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = Technos[index];
        if (test(techno, argument)) {
            list->Add(techno);
        }
    }
    return list;
}


// DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument) (0x006F69F0)
// The technos of the selection (0x0083996C) for which test(object, argument) holds, in a new list.

DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument)
{
    DynamicVectorClass<TechnoClass *> *list = new DynamicVectorClass<TechnoClass *>(0, 0);
    int count = unknown_selected_83996c->Count();
    for (int index = 0; index < count; index++) {
        ObjectClass *object = unknown_selected_83996c->Get(index);
        if (object->Is_Techno()) {
            TechnoClass *techno = (TechnoClass *)object;
            if (test(techno, argument)) {
                list->Add(techno);
            }
        }
    }
    return list;
}


// DynamicVectorClass<TechnoClass *> *Unknown_006F6AD0() (0x006F6AD0)
// The selected objects (0x00A40C60; flag +0x14 bit 0) in a new list.

DynamicVectorClass<TechnoClass *> *Unknown_006F6AD0()
{
    DynamicVectorClass<TechnoClass *> *list = new DynamicVectorClass<TechnoClass *>(0, 0);
    int count = unknown_objects_a40c60.Count();
    for (int index = 0; index < count; index++) {
        ObjectClass *object = unknown_objects_a40c60[index];
        bool flag;
        if (object != 0) {
            flag = object->AbstractFlags.IsTechno;
        }
        if (object != 0 && flag) {
            list->Add((TechnoClass *)object);
        }
    }
    return list;
}


// void __fastcall Unknown_006F6B90(UnknownPointerList *list, UnknownVisit visit, int argument) (0x006F6B90)
// Calls visit(item, argument) for every item of the list.


void __fastcall Unknown_006F6B90(UnknownPointerList *list, UnknownVisit visit, int argument)
{
    int count = list->Count;
    for (int index = 0; index < count; index++) {
        visit(list->Items[index], argument);
    }
}


// void __fastcall Unknown_006F6BC0(UnknownPointerList *list, UnknownTest test, UnknownVisit visit, int test_argument, int visit_argument) (0x006F6BC0)
// Calls visit(item, visit_argument) for every item of the list for which test(item, test_argument) holds.


void __fastcall Unknown_006F6BC0(UnknownPointerList *list, UnknownTest test, UnknownVisit visit, int test_argument, int visit_argument)
{
    int count = list->Count;
    for (int index = 0; index < count; index++) {
        void *item = list->Items[index];
        if (test(item, test_argument)) {
            visit(item, visit_argument);
        }
    }
}


// bool __fastcall Unknown_006F6C10(UnknownPointerList *list, UnknownTest test, int argument) (0x006F6C10)
// True when test(item, argument) holds for every item of the list (stops at the first that fails).


bool __fastcall Unknown_006F6C10(UnknownPointerList *list, UnknownTest test, int argument)
{
    int count = list->Count;
    for (int index = 0; index < count; index++) {
        if (!test(list->Items[index], argument)) {
            return false;
        }
    }
    return true;
}


// bool __fastcall Unknown_006F6C50(UnknownPointerList *list, UnknownTest test, int argument) (0x006F6C50)
// True when test(item, argument) holds for some item of the list (stops at the first that does).


bool __fastcall Unknown_006F6C50(UnknownPointerList *list, UnknownTest test, int argument)
{
    int count = list->Count;
    for (int index = 0; index < count; index++) {
        if (test(list->Items[index], argument)) {
            return true;
        }
    }
    return false;
}


// int __fastcall unknown_find_name_index(DynamicVectorClass<char const *> const &names, char const *name) -- 0x006F6C90
// Purpose: the index of name (exact, strcmp) in a list of names; -1 when absent.

int __fastcall unknown_find_name_index(DynamicVectorClass<char const *> const &names, char const *name)
{
    for (int index = 0; index < names.Count(); index++) {
        if (strcmp(name, names[index]) == 0) {
            return index;
        }
    }
    return -1;
}


extern int unknown_state_ac132c;
void __fastcall Unknown_006F58E0(wchar_t const *text, int delay);
void __fastcall Unknown_006F6790(wchar_t *buffer);
int __fastcall Unknown_006F6FE0(TechnoClass *techno, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);

// Whether the techno is a selectable combatant (not a building, kind 6; type flag +0xB58).
static bool Is_Combatant(TechnoClass *techno)
{
    return techno->Kind_Of() != 6 && techno->Techno_Type_Class()->IsSelectableCombatant;
}

// Whether no combatant of the list is on the screen (+0x77 clear on every one).
static bool None_On_Screen(DynamicVectorClass<TechnoClass *> *list)
{
    int count = list->Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = (*list)[index];
        if (Is_Combatant(techno) && !techno->unknown_77) {
            return false;
        }
    }
    return true;
}

// void __fastcall Unknown_006F6D00(bool flag) (0x006F6D00)
// The select-combatants command (state 1): the player's selected combatants on the screen, else (flag 0x00AC133C,
// kept while the command repeats) every player combatant on the map; selects them (slot 80) and shows 0x006F6790's
// message through 0x006F58E0.
void __fastcall Unknown_006F6D00(bool flag)
{
    if (unknown_int_a3d548 != 0) {
        return;
    }
    unknown_selected_83996c->Unknown_006AAD70();
    if (unknown_state_ac132c != 1) {
        unknown_flag_ac133c = false;
    }
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = Unknown_006F6930((UnknownTest)Unknown_006F6FE0, 0);
    } else {
        list = Unknown_006F69F0((UnknownTest)Unknown_006F6FE0, 0);
        if (None_On_Screen(list)) {
            unknown_flag_ac133c = true;
        }
        if (unknown_flag_ac133c) {
            delete list;
            list = Unknown_006F6930((UnknownTest)Unknown_006F6FE0, 0);
        }
    }
    if (flag) {
        unknown_selected_83996c->Unknown_006AAD40();
    }
    int total = list->Count();
    for (int item = 0; item < total; item++) {
        TechnoClass *techno = (*list)[item];
        if (Is_Combatant(techno)) {
            techno->unknown_slot80();
        }
    }
    wchar_t buffer[256];
    Unknown_006F6790(buffer);
    Unknown_006F58E0(buffer, -1);
    delete list;
    unknown_state_ac132c = 1;
    unknown_flag_ac1330 = false;
}


// int __fastcall Unknown_006F6FE0(TechnoClass *techno, int argument) (0x006F6FE0)
// Whether a techno (in play, +0x7D) belongs to the player's side: its house
// flag +0x135 in a single-player game, else the house's slot 0x004F52B0 answer.


int __fastcall Unknown_006F6FE0(TechnoClass *techno, int argument)
{
    if (techno != 0 && techno->unknown_7d) {
        bool mine;
        if (unknown_game_mode_a3d298 == 0) {
            mine = techno->House->unknown_135;
        } else {
            mine = techno->House->Is_Player_Control();
        }
        if (mine) {
            return 1;
        }
    }
    return 0;
}


// Not exact (RA2_NON_MATCHING): FUN_006f7020 at 0x006F7020, best score 52.31, 2026-11-13; differs: 66 instructions in the original, 129 in the draft (shape 64.62)
#ifdef RA2_NON_MATCHING
int __fastcall Unknown_006F7190(TechnoClass *techno, char const *name);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);


void __fastcall Unknown_006F7020(char const *name)
{
    UnknownTest test = (UnknownTest)Unknown_006F7190;
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = new DynamicVectorClass<TechnoClass *>(0, 0);
        int count = Technos.Count();
        for (int index = 0; index < count; index++) {
            TechnoClass *techno = Technos[index];
            if (test(techno, (int)name)) {
                list->Add(techno);
            }
        }
    } else {
        list = Unknown_006F69F0(test, (int)name);
    }
    int total = list->Count();
    for (int index = 0; index < total; index++) {
        (*list)[index]->Unselect();
    }
    delete list;
}
#endif

// int __fastcall Unknown_006F7190(TechnoClass *techno, char const *name) (0x006F7190)
// Whether a player-side techno in play has the type name (0x005D27D0).


int __fastcall Unknown_006F7190(TechnoClass *techno, char const *name)
{
    if (techno != 0 && techno->unknown_7d) {
        if (Is_Player_Side(techno)) {
            if (techno->unknown_func_5d27d0(name)) {
                return 1;
            }
        }
    }
    return 0;
}


// Not exact (RA2_NON_MATCHING): FUN_006f71e0 at 0x006F71E0, best score 48.78, 2026-11-13; differs: 69 instructions in the original, 136 in the draft (shape 65.37)
#ifdef RA2_NON_MATCHING
int __fastcall Unknown_006F7190(TechnoClass *techno, char const *name);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);
#include "shared/techno.hpp"

void __fastcall Unknown_006F71E0(char const *name)
{
    unsigned char sound = unknown_sound_enabled_7d88d6;
    unknown_sound_enabled_7d88d6 = false;
    UnknownTest test = (UnknownTest)Unknown_006F7190;
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = new DynamicVectorClass<TechnoClass *>(0, 0);
        int count = Technos.Count();
        for (int index = 0; index < count; index++) {
            TechnoClass *techno = Technos[index];
            if (test(techno, (int)name)) {
                list->Add(techno);
            }
        }
    } else {
        list = Unknown_006F69F0(test, (int)name);
    }
    int total = list->Count();
    for (int index = 0; index < total; index++) {
        (*list)[index]->unknown_slot80();
    }
    delete list;
    unknown_sound_enabled_7d88d6 = sound != 0;
}
#endif

// Not exact (RA2_NON_MATCHING): FUN_006f7360 at 0x006F7360, best score 37.13, 2026-11-13; differs: 234 instructions in the original, 240 in the draft (shape 92.41)
#ifdef RA2_NON_MATCHING
extern int unknown_state_ac132c;
bool __fastcall unknown_matches_any_name(TechnoClass *techno, DynamicVectorClass<char const *> const &names);
void __fastcall Unknown_006F58E0(wchar_t const *text, int delay);
void __fastcall Unknown_006F6790(wchar_t *buffer);
int __fastcall Unknown_006F6FE0(TechnoClass *techno, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);
DynamicVectorClass<char const *> *Unknown_006F76C0();

// Whether no techno of the list with one of the names is on the screen (+0x77 clear).
static bool None_Named_On_Screen(DynamicVectorClass<TechnoClass *> *list, DynamicVectorClass<char const *> *names)
{
    int count = list->Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = (*list)[index];
        if (unknown_matches_any_name(techno, *names) && !techno->unknown_77) {
            return false;
        }
    }
    return true;
}

// Calls slot 80 on each techno of the list whose type is one of the names.
static void Select_Named(DynamicVectorClass<TechnoClass *> *list, DynamicVectorClass<char const *> *names)
{
    int total = list->Count();
    for (int item = 0; item < total; item++) {
        TechnoClass *techno = (*list)[item];
        if (unknown_matches_any_name(techno, *names)) {
            techno->unknown_slot80();
        }
    }
}

void __fastcall Unknown_006F7360()
{
    if (unknown_int_a3d548 != 0) {
        return;
    }
    unknown_selected_83996c->Unknown_006AAD70();
    if (unknown_state_ac132c != 2) {
        unknown_flag_ac133c = false;
    }
    DynamicVectorClass<char const *> *names = Unknown_006F76C0();
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = Unknown_006F6930((UnknownTest)Unknown_006F6FE0, 0);
    } else {
        list = Unknown_006F69F0((UnknownTest)Unknown_006F6FE0, 0);
        if (None_Named_On_Screen(list, names)) {
            unknown_flag_ac133c = true;
        }
        if (unknown_flag_ac133c) {
            delete list;
            list = Unknown_006F6930((UnknownTest)Unknown_006F6FE0, 0);
        }
    }
    Select_Named(list, names);
    wchar_t buffer[256];
    Unknown_006F6790(buffer);
    Unknown_006F58E0(buffer, -1);
    unknown_state_ac132c = 2;
    unknown_flag_ac1330 = false;
    delete names;
    delete list;
}
#endif

// bool __fastcall unknown_matches_any_name(UnknownObject_006f7610 *object, UnknownNameList_006f7610 const &names) -- 0x006F7610
// Purpose: whether the object's type ID matches any of the names (the
// ObjectClass test 0x005D27D0 for each).
// Assumptions: the list is a DynamicVectorClass of names (elements at
// +0x04, count at +0x10); views only.

struct UnknownObject_006f7610 {
    bool unknown_func_5d27d0(char const *name) const;   // 0x005D27D0
};
struct UnknownNameList_006f7610 {
    void *vtable_00;
    char const **Vector;                    // +0x04
    int VectorMax;
    int unknown_0c;
    int ActiveCount;                        // +0x10
};

bool __fastcall unknown_matches_any_name(UnknownObject_006f7610 *object, UnknownNameList_006f7610 const &names)
{
    int count = names.ActiveCount;
    for (int index = 0; index < count; index++) {
        char const *name = names.Vector[index];
        if (object->unknown_func_5d27d0(name)) {
            return true;
        }
    }
    return false;
}


// void __fastcall Unknown_006F7670() (0x006F7670)
// When the stopwatch of 0x006F7650 is running it is stopped; if it ran at most 500 ms
// (timeGetTime difference, absolute) 0x006F7360 runs and the screen is redrawn completely.

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
extern bool unknown_flag_ac133d;
extern unsigned long unknown_time_ac1340;
void __fastcall Unknown_006F7360();

void __fastcall Unknown_006F7670()
{
    if (unknown_flag_ac133d) {
        unknown_flag_ac133d = false;
        if (abs((int)(timeGetTime() - unknown_time_ac1340)) <= 500) {
            Unknown_006F7360();
            Map.Flag_To_Redraw(1);
        }
    }
}


// bool Unknown_006F76B0() (0x006F76B0): the flag at 0x00AC133D (shared/tactical.hpp).

extern bool unknown_flag_ac133d;

bool Unknown_006F76B0()
{
    return unknown_flag_ac133d;
}


// DynamicVectorClass<char const *> *Unknown_006F76C0() (0x006F76C0)
// The distinct type names (ID, +0x24 of the object's type) of the selected objects (0x00A40C60),
// in order of first appearance, in a new list. The name search is the inline strcmp loop of
// 0x006F6C90.

int __fastcall unknown_find_name_index(DynamicVectorClass<char const *> const &names, char const *name);

DynamicVectorClass<char const *> *Unknown_006F76C0()
{
    DynamicVectorClass<char const *> *names = new DynamicVectorClass<char const *>(0, 0);
    int count = unknown_objects_a40c60.Count();
    for (int index = 0; index < count; index++) {
        char const *name = unknown_objects_a40c60[index]->Class_Of()->ID;
        if (unknown_find_name_index(*names, name) == -1) {
            names->Add(name);
        }
    }
    return names;
}


// void __fastcall Unknown_006F7800(char const *name, UnknownVisit visit, int argument) (0x006F7800)
// Calls visit(techno, argument) for every player-side techno with the type name: from all technos
// (0x00A40C20) when the flag 0x00AC133C is set, else from the selection (0x006F69F0 expanded inline).
// The test is 0x006F7190 called through an UnknownTest, so the caller reads its answer as a bool
// (test al, al) although the function itself returns an int. The list is built, walked and deleted.

void __fastcall Unknown_006F7800(char const *name, UnknownVisit visit, int argument)
{
    UnknownTest test = (UnknownTest)Unknown_006F7190;
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = new DynamicVectorClass<TechnoClass *>(0, 0);
        int count = Technos.Count();
        for (int index = 0; index < count; index++) {
            TechnoClass *techno = Technos[index];
            if (test(techno, (int)name)) {
                list->Add(techno);
            }
        }
    } else {
        list = Unknown_006F69F0(test, (int)name);
    }
    int total = list->Count();
    for (int index = 0; index < total; index++) {
        visit((*list)[index], argument);
    }
    delete list;
}


// void __fastcall Unknown_006F7970(void *item) (0x006F7970)
// Removes the last entry equal to item from the list at 0x00AC1344 (searched from the end).

extern UnknownPointerList *unknown_list_ac1344;

void __fastcall Unknown_006F7970(void *item)
{
    if (unknown_list_ac1344 != 0) {
        for (int index = unknown_list_ac1344->Count - 1; index >= 0; index--) {
            if (unknown_list_ac1344->Items[index] == item) {
                unknown_list_ac1344->Delete(index);
                break;
            }
        }
    }
}


extern UnknownPointerList *unknown_list_ac1344;
int __fastcall Unknown_006F7DF0(TechnoClass *techno, int argument);
DynamicVectorClass<TechnoClass *> *Unknown_006F6AD0();
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);

void Unknown_006F79C0()
{
    if (unknown_list_ac1344 != 0) {
        delete (DynamicVectorClass<TechnoClass *> *)unknown_list_ac1344;
        unknown_list_ac1344 = 0;
    }
    unknown_list_ac1344 = (UnknownPointerList *)Unknown_006F6AD0();
    if (unknown_list_ac1344->Count == 0) {
        delete (DynamicVectorClass<TechnoClass *> *)unknown_list_ac1344;
        unknown_list_ac1344 = 0;
        unknown_list_ac1344 = (UnknownPointerList *)Unknown_006F69F0((UnknownTest)Unknown_006F7DF0, 0);
    }
}


extern UnknownPointerList *unknown_list_ac1344;
extern int unknown_state_ac132c;
extern int unknown_state_7f8bd8;
extern bool unknown_flag_ac1330;
extern wchar_t const unknown_empty_wide_839d4c[];
void __fastcall Unknown_006F6810(wchar_t *buffer, wchar_t const *name);
int __fastcall Unknown_006F7DF0(TechnoClass *techno, int argument);
DynamicVectorClass<TechnoClass *> *Unknown_006F6AD0();
void Unknown_006F79C0();
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);

static wchar_t const *__fastcall Health_Message(int state)
{
    if (state == 0) {
        return StringTable::Load_String("MSG:Critical", 0, "D:\\Ra2\\UICmnds.cpp", 0x5ae);
    }
    if (state == 1) {
        return StringTable::Load_String("MSG:HeavilyDamaged", 0, "D:\\Ra2\\UICmnds.cpp", 0x5b0);
    }
    if (state == 2) {
        return StringTable::Load_String("MSG:Healthy", 0, "D:\\Ra2\\UICmnds.cpp", 0x5b2);
    }
    return unknown_empty_wide_839d4c;
}

static int __fastcall State_Index(int state)
{
    if (state == -1) {
        return 0;
    }
    if (state == 0) {
        return 1;
    }
    return state == 1 ? 2 : 0;
}

static wchar_t const *Selection_Message(int kind)
{
    wchar_t const *text = Health_Message(kind);
    DynamicVectorClass<TechnoClass *> *objects = Unknown_006F6AD0();
    int objects_count = objects->Count();
    for (int check = 0; check < objects_count; check++) {
        if ((*objects)[check]->unknown_func_5d46b0() != kind) {
            text = StringTable::Load_String("MSG:Mixed", 0, "D:\\Ra2\\UICmnds.cpp", 0x599);
            break;
        }
    }
    delete objects;
    return text;
}

void __fastcall Unknown_006F7B80(bool flag)
{
    if (unknown_int_a3d548 != 0) {
        return;
    }
    int kind;
    if (unknown_state_ac132c != 3) {
        Unknown_006F79C0();
        unknown_selected_83996c->Unknown_006AAD40();
        kind = 0;
    } else {
        kind = State_Index(unknown_state_7f8bd8);
        if (flag) {
            unknown_selected_83996c->Unknown_006AAD40();
        }
    }
    DynamicVectorClass<TechnoClass *> *list = (DynamicVectorClass<TechnoClass *> *)unknown_list_ac1344;
    int count = list->Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = (*list)[index];
        if (techno->unknown_func_5d46b0() == kind) {
            techno->unknown_slot80();
        }
    }
    wchar_t const *text = Selection_Message(kind);
    wchar_t buffer[256];
    Unknown_006F6810(buffer, text);
    Show_Message(buffer, 0xF0);
    unknown_state_7f8bd8 = kind;
    int commands = unknown_list_ac1344->Count;
    unknown_state_ac132c = 0;
    if (commands != 0) {
        unknown_state_ac132c = 3;
    }
    unknown_flag_ac1330 = false;
}


// int __fastcall Unknown_006F7DF0(TechnoClass *techno, int argument) (0x006F7DF0)
// Whether a player-side techno in play is not a building (kind 6).


int __fastcall Unknown_006F7DF0(TechnoClass *techno, int argument)
{
    if (techno != 0 && techno->unknown_7d) {
        if (Is_Player_Side(techno)) {
            if (techno->Kind_Of() != 6) {
                return 1;
            }
        }
    }
    return 0;
}


extern UnknownPointerList *unknown_list_ac1344;
extern int unknown_state_ac132c;
extern int unknown_state_7f8bdc;
extern bool unknown_flag_ac1330;
extern wchar_t const unknown_empty_wide_839d4c[];
void __fastcall Unknown_006F6810(wchar_t *buffer, wchar_t const *name);
int __fastcall Unknown_006F7DF0(TechnoClass *techno, int argument);
DynamicVectorClass<TechnoClass *> *Unknown_006F6AD0();
void Unknown_006F79C0();
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, int argument);

static wchar_t const *__fastcall Veterancy_Message(int state)
{
    if (state == 0) {
        return StringTable::Load_String("MSG:Elite", 0, "D:\\Ra2\\UICmnds.cpp", 0x5f8);
    }
    if (state == 1) {
        return StringTable::Load_String("MSG:Veteran", 0, "D:\\Ra2\\UICmnds.cpp", 0x5fa);
    }
    if (state == 2) {
        return StringTable::Load_String("MSG:LittleExperience", 0, "D:\\Ra2\\UICmnds.cpp", 0x5fc);
    }
    return unknown_empty_wide_839d4c;
}

static int __fastcall Rank_Index(int state)
{
    if (state == -1) {
        return 0;
    }
    if (state == 0) {
        return 1;
    }
    return state == 1 ? 2 : 0;
}

static wchar_t const *Experience_Message(int kind)
{
    wchar_t const *text = Veterancy_Message(kind);
    DynamicVectorClass<TechnoClass *> *objects = Unknown_006F6AD0();
    int objects_count = objects->Count();
    for (int check = 0; check < objects_count; check++) {
        TechnoClass *item = (*objects)[check];
        if (item->unknown_118.Remaining_Levels() != kind) {
            text = StringTable::Load_String("MSG:Mixed", 0, "D:\\Ra2\\UICmnds.cpp", 0x5e4);
            break;
        }
    }
    delete objects;
    return text;
}

void __fastcall Unknown_006F7EC0(bool flag)
{
    if (unknown_int_a3d548 != 0) {
        return;
    }
    int kind;
    if (unknown_state_ac132c != 4) {
        Unknown_006F79C0();
        unknown_selected_83996c->Unknown_006AAD40();
        kind = 0;
    } else {
        kind = Rank_Index(unknown_state_7f8bdc);
        if (flag) {
            unknown_selected_83996c->Unknown_006AAD40();
        }
    }
    DynamicVectorClass<TechnoClass *> *list = (DynamicVectorClass<TechnoClass *> *)unknown_list_ac1344;
    int count = list->Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = (*list)[index];
        if (techno->unknown_118.Remaining_Levels() == kind) {
            techno->unknown_slot80();
        }
    }
    wchar_t const *text = Experience_Message(kind);
    wchar_t buffer[256];
    Unknown_006F6810(buffer, text);
    Show_Message(buffer, 0xF0);
    unknown_state_7f8bdc = kind;
    int commands = unknown_list_ac1344->Count;
    unknown_state_ac132c = 0;
    if (commands != 0) {
        unknown_state_ac132c = 4;
    }
    unknown_flag_ac1330 = false;
}


// DynamicVectorClass<char const *> *__fastcall Unknown_Type_Names_006F84A0(DynamicVectorClass<ObjectClass *> const &objects) (0x006F84A0)
// The distinct type names (ID, +0x24 of the object's type) of a list of
// objects, in order of first appearance, in a new list (0x006F76C0 for any
// list; here the name search and the add are expanded inline). No callers
// in the image.

int __fastcall unknown_find_name_index(DynamicVectorClass<char const *> const &names, char const *name);

DynamicVectorClass<char const *> *__fastcall Unknown_Type_Names_006F84A0(DynamicVectorClass<ObjectClass *> const &objects)
{
    DynamicVectorClass<char const *> *names = new DynamicVectorClass<char const *>(0, 0);
    int count = objects.Count();
    for (int index = 0; index < count; index++) {
        char const *name = objects[index]->Class_Of()->ID;
        if (unknown_find_name_index(*names, name) == -1) {
            names->Add(name);
        }
    }
    return names;
}


// char *__fastcall Unknown_006F85E0(objects, name, limit, best) (0x006F85E0)
// Of the distinct object types in a list picks the dearest by total cost under limit other than name, or the
// costliest; a copy of its ID in a new 100-byte buffer, the total in *best. Caller 0x006F81B0.

int __fastcall unknown_find_name_index(DynamicVectorClass<char const *> const &names, char const *name);

char *__fastcall Unknown_006F85E0(DynamicVectorClass<TechnoClass *> const &objects, char const *name, int limit, int *best)
{
    DynamicVectorClass<char const *> *names = Unknown_Type_Names_006F84A0((DynamicVectorClass<ObjectClass *> const &)objects);
    DynamicVectorClass<int> *costs = new DynamicVectorClass<int>(0, 0);
    int count = names->Count();
    for (int index = 0; index < count; index++) {
        costs->Add(0);
    }
    int total = objects.Count();
    for (int item = 0; item < total; item++) {
        TechnoClass *object = objects[item];
        char const *id = object->Class_Of()->ID;
        int found = unknown_find_name_index(*names, id);
        HouseClass *house = object->House;
        int sum = (*costs)[found];
        int cost = ((TechnoTypeClass *)object->Class_Of())->Cost_Of(house);
        (*costs)[found] = sum + cost;
    }
    *best = 0;
    int pick = -1;
    if (name != 0) {
        for (int entry = 0; entry < count; entry++) {
            if (strcmp(name, (*names)[entry]) != 0) {
                int cost = (*costs)[entry];
                if (cost < limit && cost > *best) {
                    *best = cost;
                    pick = entry;
                }
            }
        }
    }
    if (name == 0 || *best == 0) {
        int most = 0;
        int which = -1;
        for (int slot = 0; slot < count; slot++) {
            if ((*costs)[slot] > most) {
                most = (*costs)[slot];
                which = slot;
            }
        }
        *best = most;
        pick = which;
    }
    char const *selected = name;
    if (pick != -1) {
        selected = (*names)[pick];
    }
    char *result = new char[100];
    if (selected != 0) {
        strcpy(result, selected);
    }
    delete names;
    delete costs;
    return result;
}


// void Unknown_006F88F0() (0x006F88F0; the jump-to-radar-event command 0x0051C640): centres the view on the
// newest radar event cell, or on the previous one when pressed again within 50 frames, lifted by half the cell
// height; then (when 0x00833684 is set) calls 0x004994D0 with the unit's header-static cell (unknown_geometry_k
// at 0x00AC12D0; the result is dropped into the cell local).
extern int unknown_radar_jump_frame_7f8bd4;     // 0x007F8BD4 the frame of the last jump
extern int unknown_833684;                      // 0x00833684

void Unknown_006F88F0()
{
    Cell cell;
    if (Frame - unknown_radar_jump_frame_7f8bd4 < 50) {
        cell = RadarEventClass_unknown_previous_cell();
    } else {
        cell = RadarEventClass_unknown_newest_cell();
    }
    unknown_radar_jump_frame_7f8bd4 = Frame;
    if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
        CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        int height = Map[coord].unknown_11b * 256 / 2;
        coord -= CoordStruct(height, height, 0);
        unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
        if (unknown_833684 != 0) {
            Map.Unknown_004994D0(unknown_geometry_k);
        }
    }
}


// void Unknown_006F89E0() (0x006F89E0)
// Resets the group and radar-jump repeat state after a load (Scenario.cpp's
// post-load refresh 0x0065D2C0 and Clear_Scenario call it): no group, and both
// press frames far enough back that the next press is not a repeat.
extern int unknown_group_frame_7f8bcc;          // 0x007F8BCC
extern int unknown_group_7f8bd0;                // 0x007F8BD0
extern int unknown_radar_jump_frame_7f8bd4;     // 0x007F8BD4

void Unknown_006F89E0()
{
    unknown_group_7f8bd0 = -1;
    unknown_group_frame_7f8bcc = -100;
    unknown_radar_jump_frame_7f8bd4 = -100;
}


// 0x006f8a00: function not yet identified
//
// Body: 29 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall FUN_006f8a00(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006f8a00 {

extern "C" unsigned int DAT_00ac1334;
extern "C" unsigned int DAT_00ac1338;

void __fastcall FUN_006f8a00(int object_address)
{
    if (*(int *)(*(int *)(object_address + 0x418) + 0xba0) == 5) {
        DAT_00ac1338 = object_address;
        return;
    }
    DAT_00ac1334 = object_address;
    return;
}

}  // namespace seed_006f8a00


// void __fastcall Unknown_006F8A20(BuildingClass *building) (0x006F8A20)
// Forgets a building pending placement: null forgets both pending slots,
// otherwise the slot that holds the building (0x006F8A00 files them).
extern BuildingClass *unknown_pending_building_ac1334;     // 0x00AC1334
extern BuildingClass *unknown_pending_building_ac1338;     // 0x00AC1338 (owner +0xBA0 == 5)

void __fastcall Unknown_006F8A20(BuildingClass *building)
{
    if (building == 0) {
        unknown_pending_building_ac1334 = 0;
        unknown_pending_building_ac1338 = 0;
        return;
    }
    if (building == unknown_pending_building_ac1334) {
        unknown_pending_building_ac1334 = 0;
        return;
    }
    if (building == unknown_pending_building_ac1338) {
        unknown_pending_building_ac1338 = 0;
    }
}


class Leaf_6f8a50 { public: unsigned int get(); };
unsigned int Leaf_6f8a50::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int global_value; unsigned int unused_value; global_value = *(unsigned int*)(0xac1334); return global_value; }


class Leaf_6f8a60 { public: unsigned int get(); };
unsigned int Leaf_6f8a60::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int global_value; unsigned int unused_value; global_value = *(unsigned int*)(0xac1338); return global_value; }


// bool AbstractClass::Is_Techno() const (0x006F8A70): the COMDAT body of the header inline
// (shared/abstract_class.hpp, from ra2-decomp include/abstract.h:92), emitted by this unit
// because Unknown_006F6D00 refuses the inline inside its expanded Unknown_006F69F0.
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors


extern "C" unsigned char table_6f8b90[]; extern "C" void* __cdecl storage_6f8b90(unsigned int);
class Storage_6f8b90 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_6f8b90::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_6f8b90; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_6f8b90(requested << 2); owns=1; } return this; }
