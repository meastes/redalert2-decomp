// NON-EXACT DRAFT 0x67E210 FUN_0067e210
// entry 0x67E210; exact-match score 99.04 (misses_index best); plan "draft as-is"
// origin lane: Scout1018 (build/class-work/Scout1018/keep/slot54_67e210_M1a_9904.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native (Scout1089): its file-static Max_Visible_Cameos renamed Max_Visible_Cameos_67e210
// (the accepted 0x6819A0 in the same unit now defines one); do not refresh blindly.
// bool SidebarClass::unknown_slot54(int control) (0x0067E210, slot 54)
// Shows (1), hides (0) or toggles (-1) the sidebar, adding or removing its
// buttons and the active strip; true when the state changed.
#include "shared/sidebar.hpp"
#include "shared/sidebar_screen.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_flags_a3f600@@3EA=00a3f600
// @ref ?unknown_game_mode_a3d298@@3HA=00a3d298
// @ref ?Set_Buttons@RadarClass@@QAEX_N@Z=0062f2d0
// @ref ?Zap@LinkClass@@UAEXXZ=00538ff0
// @ref ?Add_A_Button@GScreenClass@@UAE_NAAVGadgetClass@@@Z=004e0ac0
// @ref ?Remove_A_Button@GScreenClass@@UAE_NAAVGadgetClass@@@Z=004e0b00
// @ref ?Enable@GadgetClass@@UAEXXZ=004cf140
// @ref ?Disable@GadgetClass@@UAEXXZ=004cf150
// @ref ?Is_Enabled@GadgetClass@@QBE_NXZ=004cf160
// @ref ?Turn_On@ToggleClass@@QAEXXZ=006eb160
// @ref ?Unknown_005A09E0@@YAXXZ=005a09e0
// @ref ?RadarButton@RadarClass@@2VRTacticalClass@@A=00ab6478
// @ref ?SelectButton@SidebarClass@@2PAVSelectClass@StripClass@1@A=00ab9740
// @ref ?unknown_flag_8373c3@@3_NA=008373c3
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?unknown_int_83962c@@3HA=0083962c
// @ref ?unknown_int_839624@@3HA=00839624
// @ref ?unknown_abcd64@@3HA=00abcd64

static inline int Bottom_Extra_67e210()
{
    if (unknown_id_source_a3d290->unknown_2239) {
        return 26;
    }
    return 18;
}

static inline int Max_Visible_Cameos_67e210()
{
    return (unknown_int_839624 + unknown_int_83962c - unknown_abcd64 - Bottom_Extra_67e210() - 7) / 50;
}

bool SidebarClass::unknown_slot54(int control)
{
    bool old = unknown_4ee1;
    if ((unknown_flags_a3f600 & 2) && unknown_game_mode_a3d298 && unknown_game_mode_a3d298 != 5) {
        return old;
    }
    if (control != -1) {
        unknown_4ee1 = (control == 1);
    } else {
        unknown_4ee1 = !old;
    }
    bool changed = (unknown_4ee1 != old);
    if (changed) {
        Set_Buttons(unknown_4ee1);
        if (unknown_4ee1) {
            Reposition_Sidebar();
            unknown_4ee2 = true;
            unknown_button_ab96b8.Zap();
            Add_A_Button(unknown_button_ab96b8);
            unknown_button_abcc58.Zap();
            Add_A_Button(unknown_button_abcc58);
            int index;
            for (index = 0; index < 4; index++) {
                unknown_buttons_ab9508[index].Zap();
                unknown_buttons_ab9508[index].IsPressed = false;
                Add_A_Button(unknown_buttons_ab9508[index]);
            }
            unknown_button_abcbe8.Zap();
            Map.Add_A_Button(unknown_button_abcbe8);
            unknown_button_abccc0.Zap();
            Map.Add_A_Button(unknown_button_abccc0);
            Background.Zap();
            Add_A_Button(Background);
            RadarButton.Zap();
            Add_A_Button(RadarButton);
            for (index = 0; index < 4; index++) {
                Column[index].unknown_40 = 0;
                if (Column[index].BuildableCount > 0) {
                    unknown_buttons_ab9508[index].Enable();
                }
            }
            unknown_buttons_ab9508[unknown_4ed8].Turn_On();
            Column[unknown_4ed8].Activate();
            Unknown_0067C850();
        } else {
            Unknown_005A09E0();
            Remove_A_Button(unknown_button_ab96b8);
            Remove_A_Button(unknown_button_abcc58);
            Remove_A_Button(Background);
            for (int index = 0; index < 4; index++) {
                Remove_A_Button(unknown_buttons_ab9508[index]);
            }
            Remove_A_Button(unknown_button_abcbe8);
            Remove_A_Button(unknown_button_abccc0);
            StripClass &current = Column[unknown_4ed8];
            int count = Max_Visible_Cameos_67e210() * 2;
            for (int slot = 0; slot < count; slot++) {
                Map.Remove_A_Button(SelectButton[current.unknown_34 * 60 + slot]);
            }
            Remove_A_Button(RadarButton);
        }
        Flag_To_Redraw(2);
    }
    return changed;
}
