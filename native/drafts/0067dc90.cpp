// NON-EXACT DRAFT 0x67DC90 SidebarClass::AI (slot 10)
// entry 0x67DC90; exact-match score 50.75 (misses_index best); plan "draft, verify"
// origin lane: Scout626 (build/class-work/Scout626/w/ai0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Best recorded 62.90 is codex2 r2875 (Windows, not mirrored); this is the local 50.75 form of the same structure.
// void SidebarClass::AI(int &input, Point2D const &xy) (0x0067DC90, slot 10)
// The sidebar's per-frame update: runs the strips with the mouse position
// relative to the sidebar, steps the 0xABCD30 animation, follows the
// player's +0x230 count for the +0x4EE5 state, handles the sidebar's own
// buttons (0x8066 / 0x8065 mode switches, tabs 0x80CB-0x80CE, the scroll
// buttons), redraws flashing tabs, releases the mode buttons when their
// modes ended, and then runs the power bar.
#include "shared/sidebar.hpp"
#include "shared/sidebar_screen.hpp"
#include "shared/object.hpp"
#include "shared/rules.hpp"
// @ref ?unknown_mouse_8324e0@@3VMouseClass@@A=008324e0
// @ref ?unknown_rect_839620@@3VRect@@A=00839620
// @ref ?unknown_flag_a40d13@@3_NA=00a40d13
// @ref ?AI@StripClass@SidebarClass@@QAE_NAAHABUPoint2D@@@Z=0067f050
// @ref ?unknown_shapes_abcd20@@3PAPBXA=00abcd20
// @ref ?unknown_player_a35db4@@3PAVHouseClass@@A=00a35db4
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?Unknown_00713DB0@@YIHHMHH@Z=00713db0
// @ref ?Unknown_0049C7C0@DisplayClass@@QAEXH@Z=0049c7c0
// @ref ?Unknown_0049CA20@DisplayClass@@QAEXH@Z=0049ca20
// @ref ?Turn_Off@ToggleClass@@QAEXXZ=006eb170
// @ref ?Turn_On@ToggleClass@@QAEXXZ=006eb160
// @ref ?Unknown_006820A0@SidebarClass@@QAEHXZ=006820a0
// @ref ?Unknown_006820F0@SidebarClass@@QAEHXZ=006820f0
// @ref ?Unknown_0067C850@SidebarClass@@QAEXXZ=0067c850
// @ref ?Unknown_00675320@ShapeButtonClass@@QAE_NXZ=00675320
// @ref ?unknown_flag_8373c3@@3_NA=008373c3
// @ref ?unknown_flag_abcd84@@3_NA=00abcd84
// @ref ?AI@PowerClass@@UAEXAAHABUPoint2D@@@Z=0061cc30
// @ref ?Zap@GadgetClass@@UAEXXZ=00538ff0
// @ref ?Add_A_Button@GScreenClass@@QAEXAAVGadgetClass@@@Z=004e0ac0
// @ref ?Remove_A_Button@GScreenClass@@QAEXAAVGadgetClass@@@Z=004e0b00

void SidebarClass::AI(int &input, Point2D const &xy)
{
    Point2D point(xy.X - unknown_rect_839620.X, xy.Y);
    if (!unknown_flag_a40d13) {
        unknown_slot54(1);
        for (int index = 0; index < 4; index++) {
            Column[index].AI(input, point);
        }
    }

    if (unknown_4ee1) {
        if (unknown_4ed4 == 1) {
            unknown_4ed0++;
            if (unknown_4ed0 > ((ShapeFileStruct const *)unknown_shapes_abcd20[4])->FrameCount) {
                unknown_4ed4 = 0;
                unknown_4ed0 = 0;
            }
        } else if (unknown_4ed4 == -1) {
            unknown_4ed0--;
            if (unknown_4ed0 < 0) {
                unknown_4ed4 = 0;
                unknown_4ed0 = 0;
            }
        }

        Unknown_0067C4C0(unknown_player_a35db4->unknown_230 > 0);

        if (input == 0x8066) {
            Unknown_Play_Sound(Rules->unknown_180);
            Unknown_0049C7C0(-1);
        } else if (input == 0x8065) {
            Unknown_Play_Sound(Rules->unknown_180);
            Unknown_0049CA20(-1);
        } else if (input >= 0x80CB && input < 0x80CF) {
            Select_Tab(input - 0x80CB);
            Unknown_Play_Sound(Rules->unknown_188);
        } else if ((input & ~0x4000) == (unknown_button_abcbe8.ID | 0x8000)) {
            unknown_button_abcbe8.IsPressed = false;
            StripClass &strip = Column[unknown_4ed8];
            int rows = StripClass::Max_Visible_Rows();
            if ((StripClass::Max_Visible_Rows() + strip.unknown_40) * 2 < strip.BuildableCount) {
                strip.unknown_44 += rows;
                Unknown_Play_Sound(Rules->unknown_188);
            } else {
                Unknown_Play_Sound(Rules->unknown_620);
            }
        } else if ((input & ~0x4000) == (unknown_button_abccc0.ID | 0x8000)) {
            unknown_button_abccc0.IsPressed = false;
            StripClass &strip = Column[unknown_4ed8];
            if (strip.unknown_40) {
                strip.unknown_44 -= StripClass::Max_Visible_Rows();
                Unknown_Play_Sound(Rules->unknown_188);
            } else {
                Unknown_Play_Sound(Rules->unknown_620);
            }
        }

        bool flashed = false;
        for (int tab = 0; tab < 4; tab++) {
            if (unknown_buttons_ab9508[tab].Unknown_00675320()) {
                flashed = true;
            }
        }
        if (flashed) {
            unknown_4ee2 = true;
            unknown_flag_abcd84 = true;
            unknown_flag_8373c3 = true;
            Flag_To_Redraw(false);
        }
    }

    if (!unknown_11ad && unknown_button_ab96b8.IsOn) {
        unknown_button_ab96b8.Turn_Off();
    }
    if (!unknown_11ac && unknown_button_abcc58.IsOn) {
        unknown_button_abcc58.Turn_Off();
    }
    PowerClass::AI(input, xy);
}
