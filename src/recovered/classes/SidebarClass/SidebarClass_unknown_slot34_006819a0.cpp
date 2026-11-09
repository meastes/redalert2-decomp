// void SidebarClass::unknown_slot34() (0x006819A0, slot 34)
// Lays the sidebar out again: after the power bar's layout it places the
// sidebar at the right of the tactical area (or at 0 when it is on the left),
// moves the button columns with it (the 18/26 command bar offsets), loads the
// three background shapes once, repositions every sidebar control and the
// strips' select buttons (each control's OffsetX keeps the sidebar's -X), and
// registers their help regions: the four tabs (0xCB..), the select buttons
// (1000..), sell (0x66), repair (0x65) and the two scroll buttons (0xC8, 0xC9).
#include "shared/sidebar.hpp"
#include "shared/sidebar_screen.hpp"
#include "shared/tool_tip.hpp"
#include "shared/mix_file_class.hpp"
// @ref ??_C@_0BA@IOHI@TXT_REPAIR_MODE?$AA@=007dcedc
// @ref ??_C@_0N@HNAG@SIDEGDI2?4SHP?$AA@=007f3f34
// @ref ??_C@_0N@JNGJ@SIDEGDI1?4SHP?$AA@=007f3f44
// @ref ??_C@_0N@NNCD@SIDEGDI3?4SHP?$AA@=007f3f24
// @ref ??_C@_0O@IOEL@TXT_SELL_MODE?$AA@=007dcf10
// @ref ?unknown_slot34@PowerClass@@UAEXXZ=0061d130
// @ref ?unknown_flag_a40b30@@3_NA=00a40b30
// @ref ?unknown_int_839630@@3HA=00839630
// @ref ?unknown_int_839638@@3HA=00839638
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?unknown_rect_839620@@3VRect@@A=00839620
// @ref ?unknown_int_839634@@3HA=00839634
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ?unknown_int_83962c@@3HA=0083962c
// @ref ?unknown_int_839624@@3HA=00839624
// @ref ?unknown_abcd64@@3HA=00abcd64
// @ref ?Retrieve@MixFileClass@@SIPBXPBD@Z=005956c0
// @ref ?Set_Position@GadgetClass@@UAEXHH@Z=004cf660
// @ref ?Set_Size@GadgetClass@@UAEXHH@Z=004cf680
// @ref ?Flag_To_Redraw@GadgetClass@@UAEXXZ=004cf5a0
// @ref ?unknown_surface_839948@@3PAVSurface@@A=00839948
// @ref ?ToolTips@@3PAVCCToolTip@@A=008399b0
// @ref ?Remove@ToolTipManager@@QAEXI@Z=006eba10
// @ref ?Add@ToolTipManager@@QAE_NABVToolTip@@@Z=006eb860

// The rows of icons that fit, as the YR twin's file-static Max_Visible_Cameos: the
// strip top is read into a local before the 26/18 bottom piece, so VC6 subtracts the
// piece before it (out-of-line SidebarClass::Unknown_006820A0 keeps the other order).
static inline int Max_Visible_Cameos()
{
    int top = unknown_abcd64;
    int reserved;
    if (unknown_id_source_a3d290->unknown_2239) {
        reserved = 26;
    } else {
        reserved = 18;
    }
    return (unknown_int_839624 + unknown_int_83962c - top - reserved - 7) / 50;
}

void SidebarClass::unknown_slot34()
{
    PowerClass::unknown_slot34();

    if (unknown_flag_a40b30) {
        unknown_rect_839620.X = unknown_int_839638 + unknown_int_839630;
    } else {
        unknown_rect_839620.X = 0;
    }
    unknown_abcd60 = unknown_rect_839620.X + 0x16;
    if (unknown_id_source_a3d290->unknown_2239) {
        unknown_abcd48 = unknown_rect_839620.X + 0x14;
        unknown_abcd54 = unknown_rect_839620.X + 0x1A;
        unknown_abcd74 = unknown_rect_839620.X + 0x27;
    } else {
        unknown_abcd48 = unknown_rect_839620.X + 0x21;
        unknown_abcd54 = unknown_rect_839620.X + 0x14;
        unknown_abcd74 = unknown_rect_839620.X + 0x27;
    }

    if (unknown_shapes_abcd20[0] == 0) {
        unknown_shapes_abcd20[0] = MixFileClass::Retrieve("SIDEGDI1.SHP");
        unknown_shapes_abcd20[1] = MixFileClass::Retrieve("SIDEGDI2.SHP");
        unknown_shapes_abcd20[2] = MixFileClass::Retrieve("SIDEGDI3.SHP");
    }

    Background.Set_Position(unknown_rect_839620.X + 0x10, unknown_int_839634);
    Background.Flag_To_Redraw();
    int top = RadarButton.Y + RadarButton.Height;
    if (unknown_flag_a40b30) {
        Background.Set_Position(unknown_rect_839630.X + unknown_rect_839630.Width, top);
    } else {
        Background.Set_Position(0, top);
    }
    Background.Set_Size(unknown_surface_839948->Get_Width(), unknown_surface_839948->Get_Height() - top);

    unknown_button_abcc58.Set_Position(unknown_abcd48, unknown_abcd4c);
    unknown_button_abcc58.Flag_To_Redraw();
    unknown_button_abcc58.OffsetX = -unknown_rect_839620.X;

    unknown_button_ab96b8.Set_Position(unknown_button_abcc58.X + unknown_abcd50, unknown_button_abcc58.Y);
    unknown_button_ab96b8.Flag_To_Redraw();
    unknown_button_ab96b8.OffsetX = -unknown_rect_839620.X;

    for (int tab = 0; tab < 4; tab++) {
        unknown_buttons_ab9508[tab].Set_Position(unknown_abcd5c * tab + unknown_abcd54, unknown_abcd58);
        unknown_buttons_ab9508[tab].Flag_To_Redraw();
        unknown_buttons_ab9508[tab].OffsetX = -unknown_rect_839620.X;
    }

    unknown_button_abcbe8.Set_Position(unknown_abcd74, unknown_abcd78);
    unknown_button_abcbe8.Flag_To_Redraw();
    unknown_button_abcbe8.OffsetX = -unknown_rect_839620.X;

    unknown_button_abccc0.Set_Position(unknown_abcd74 + unknown_abcd7c, unknown_abcd78);
    unknown_button_abccc0.Flag_To_Redraw();
    unknown_button_abccc0.OffsetX = -unknown_rect_839620.X;

    int rows = Max_Visible_Cameos();
    int count = rows * 2;
    for (int column = 0; column < 4; column++) {
        StripClass::SelectClass *button = &SelectButton[column * 60];
        for (int row = 0; row < rows; row++) {
            for (int side = 0; side < 2; side++, button++) {
                button->Set_Position(unknown_abcd68 * side + unknown_abcd60, unknown_abcd6c * row + unknown_abcd64 + 1);
                button->Flag_To_Redraw();
            }
        }
    }

    if (ToolTips != 0) {
        ToolTip tip;
        for (int tab = 0; tab < 4; tab++) {
            tip.ID = tab + 0xCB;
            tip.Label = 0;
            tip.Region.X = unknown_buttons_ab9508[tab].X;
            tip.Region.Y = unknown_buttons_ab9508[tab].Y;
            tip.Region.Width = unknown_buttons_ab9508[tab].Width;
            tip.Region.Height = unknown_buttons_ab9508[tab].Height;
            ToolTips->Remove(tip.ID);
            ToolTips->Add(tip);
        }
        for (int index = 0; index < count; index++) {
            tip.ID = index + 1000;
            tip.Label = 0;
            tip.Region.X = SelectButton[index].X;
            tip.Region.Y = SelectButton[index].Y;
            tip.Region.Width = SelectButton[index].Width;
            tip.Region.Height = SelectButton[index].Height;
            tip.unknown_18 = true;
            ToolTips->Remove(tip.ID);
            ToolTips->Add(tip);
        }
        tip.unknown_18 = false;
        tip.ID = 0x66;
        tip.Label = "TXT_SELL_MODE";
        tip.Region.X = unknown_button_ab96b8.X;
        tip.Region.Y = unknown_button_ab96b8.Y;
        tip.Region.Width = unknown_button_ab96b8.Width;
        tip.Region.Height = unknown_button_ab96b8.Height;
        ToolTips->Remove(0x66);
        ToolTips->Add(tip);
        tip.ID = 0x65;
        tip.Label = "TXT_REPAIR_MODE";
        tip.Region.X = unknown_button_abcc58.X;
        tip.Region.Y = unknown_button_abcc58.Y;
        tip.Region.Width = unknown_button_abcc58.Width;
        tip.Region.Height = unknown_button_abcc58.Height;
        ToolTips->Remove(0x65);
        ToolTips->Add(tip);
        tip.ID = 0xC8;
        tip.Label = 0;
        tip.Region.X = unknown_button_abccc0.X;
        tip.Region.Y = unknown_button_abccc0.Y;
        tip.Region.Width = unknown_button_abccc0.Width;
        tip.Region.Height = unknown_button_abccc0.Height;
        ToolTips->Remove(0xC8);
        ToolTips->Add(tip);
        tip.ID = 0xC9;
        tip.Label = 0;
        tip.Region.X = unknown_button_abcbe8.X;
        tip.Region.Y = unknown_button_abcbe8.Y;
        tip.Region.Width = unknown_button_abcbe8.Width;
        tip.Region.Height = unknown_button_abcbe8.Height;
        ToolTips->Remove(0xC9);
        ToolTips->Add(tip);
    }
}
