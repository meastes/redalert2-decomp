// power.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/power.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/house.hpp"
#include "shared/power.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/surface.hpp"
#include "shared/abstract.hpp"
#include "shared/palette.hpp"
#include "shared/tool_tip.hpp"
#include "shared/map_view.hpp"


// Header-static dynamic initializers of the unit at 0x0061C260: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0061C260-0x0061C441); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0061C280, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C2B0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C2D0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C2F0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C310, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C330, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C350, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C390, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C3C0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C3F0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C420, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// Dynamic initializer (0x0061C430, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0061c260.cpp; no source of its own.


// PowerClass::PowerClass() (0x0061C450)
// Constructs the radar, then the power bar's timers (started on the system
// clock) and counters.
// Power.CPP sees the full HouseClass (the AI and the bar segments call its
// IHouse power methods); included first, it takes the place of
// shared/house_screen.hpp, the view shared/power.hpp gives the other units.

PowerClass::PowerClass()
    : IsToRedraw(false), FlashCount(0), GreenPipCount(0), YellowPipCount(0), RedPipCount(0), HasChanged(false),
      RecordedDrain(-1), RecordedPower(-1)
{
}


// void PowerClass::Init_Clear() (0x0061C4D0, slot 7)
// Clears the radar, then restarts the power bar's timers and counters.

void PowerClass::Init_Clear()
{
    RadarClass::Init_Clear();
    RecordedDrain = -1;
    RecordedPower = -1;
    FlashTimer = CDTimerClass<SystemTimerClass>();
    FlashCount = 0;
    UpdateTimer = CDTimerClass<SystemTimerClass>();
    GreenPipCount = 0;
    YellowPipCount = 0;
    RedPipCount = 0;
    HasChanged = false;
}


// void PowerClass::One_Time() (0x0061C550, slot 5)
// Only the radar's one-time setup (a tail jump).

void PowerClass::One_Time()
{
    RadarClass::One_Time();
}


// void PowerClass::Init_For_House() (0x0061C560, slot 53)
// After the radar's shapes, loads the power bar shape "POWERP.SHP".

void PowerClass::Init_For_House()
{
    RadarClass::Init_For_House();
    PowerPipShape = Unknown_Load_Shape_00495270("POWERP.SHP", PowerPipShapeAllocated);
}


// void PowerClass::DisposeOfArt() (0x0061C580, slot 50)
// After the radar's, frees the power bar shape when it was loaded.

void PowerClass::DisposeOfArt()
{
    RadarClass::DisposeOfArt();
    if (PowerPipShapeAllocated) {
        operator delete((void *)PowerPipShape);
        PowerPipShapeAllocated = false;
    }
}


// void PowerClass::Flash_Power() (0x0061C5B0)
// Sets the first counter to 10 and restarts the first timer at 3 ticks.

void PowerClass::Flash_Power()
{
    FlashCount = 10;
    FlashTimer = 3;
}


// int PowerClass::Desired_Power_Height() (0x0061C5F0)
// The bar segments left unlit for the player's buildings: the share of the
// bar the output (less the drain) leaves, 400 / (power + 400) of the
// segments, clamped to the bar.

int PowerClass::Desired_Power_Height()
{
    HouseClass *house = unknown_player_a35db4;
    int segments = Max_Power_Height();
    int drain = 0;
    int output = 0;
    for (int index = 0; index < house->Buildings.Count(); index++) {
        BuildingClass *building = house->Buildings[index];
        drain += building->Type->PowerDrain_c6c;
        output += building->Type->Power;
    }
    int total = output + drain;
    double bar = 400.0 / ((double)total + 400.0);
    int lit = (int)(bar * segments);
    lit = (lit > 0) ? lit : 0;
    if (lit >= segments - 1) {
        lit = segments - 1;
    }
    return segments - lit;
}


// int PowerClass::Update_Delay() (0x0061C690)
// The three counters' total, capped at Desired_Power_Height, as a share of it
// times 5. No direct callers (inlined elsewhere).

int PowerClass::Update_Delay()
{
    int value = Desired_Power_Height();
    int total = RedPipCount + YellowPipCount + GreenPipCount;
    if (total > value) {
        total = value;
    }
    return (int)((double)total / value * 5.0);
}


// int PowerClass::Max_Power_Height() (0x0061C6E0, static)
// One power bar segment per 3 pixels of 0x00ABCD70, rounded up. No direct
// callers (inlined elsewhere).

int PowerClass::Max_Power_Height()
{
    return (unknown_int_abcd70 + 3) / 3;
}


// int PowerClass::Desired_Levels(int &green, int &yellow, int &red) (0x0061C700)
// The three power bar counters' targets: the segments 0x0061C5F0 leaves are
// split by the shares of the player's drain (red), of the surplus
// output up to 100 (yellow) and of the rest of
// the surplus (green); what the truncations lose goes to
// red. Returns the bar's segment count (0x0061C6E0).

int PowerClass::Desired_Levels(int &green, int &yellow, int &red)
{
    int segments = Max_Power_Height();
    int count = Desired_Power_Height();
    int surplus = unknown_player_a35db4->Power_Output() - unknown_player_a35db4->Power_Drain();
    double value = surplus;
    double first = 100.0;
    double second;
    if (value < 0.0) {
        first = 0.0;
        second = 0.0;
    } else {
        if (value < 100.0) {
            first = value;
        }
        second = value - first;
    }
    double drain_share = 1.0;
    double second_share = 0.0;
    double first_share = 0.0;
    double total = unknown_player_a35db4->Power_Drain();
    total += first;
    total += second;
    if (total > 0.0) {
        drain_share = unknown_player_a35db4->Power_Drain() / total;
        second_share = second / total;
        first_share = first / total;
    }
    double bar = count;
    double drain_part = bar * drain_share;
    red = (int)drain_part;
    double first_part = bar * first_share;
    yellow = (int)first_part;
    double second_part = bar * second_share;
    green = (int)second_part;
    red = (int)((first_part - yellow) + (second_part - green) + (drain_part - red) + red + 0.01);
    return segments;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void PowerClass::Draw_It(bool complete) (0x0061C8C0, slot 16)
// Draws the power bar when it needs it: one shape frame per segment, down
// the sidebar's edge, three pixels apart. The segments the counters do not
// cover come first (frame 0), then a blinking one (frame 4) while the first
// counter is odd... and the three counters' segments (frames 1, 2, 3).
// Then the radar's drawing. Draft: Display245/Power338; the side-dependent
// x and y offsets take the shape of ra2-decomp PowerClass::Draw_It (YR
// 0x0063FB20, a fuzzy twin): the side is read again for the y offset, whose
// arms agree, as in the twin (Scout836).

void PowerClass::Draw_It(bool complete)
{
    if ((complete || IsToRedraw) && unknown_flag_8373c1) {
        IsToRedraw = false;
        unknown_flag_abcd84 = true;
        bool side = unknown_id_source_a3d290->unknown_2239;
        int shift;
        if (side) {
            shift = 5;
        } else {
            shift = 0;
        }
        side = unknown_id_source_a3d290->unknown_2239;
        int yoff;
        if (side) {
            yoff = 0x45;
        } else {
            yoff = 0x45;
        }
        int y = unknown_int_839624 + yoff;
        Rect window = unknown_surface_839948->Get_Rect();
        int unlit = Max_Power_Height() - RedPipCount - YellowPipCount - GreenPipCount;

        for (int index = 0; index < unlit; index++) {
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerPipShape, 0,
                          Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            y += 3;
        }
        int first = 0;
        if (FlashCount > 0 && FlashCount % 2 == 0) {
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerPipShape, 4,
                          Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            y += 3;
            first = 1;
        }
        if (GreenPipCount > 0) {
            for (; first < GreenPipCount; first++) {
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerPipShape, 1,
                              Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += 3;
            }
            first = 0;
        }
        if (YellowPipCount > 0) {
            for (; first < YellowPipCount; first++) {
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerPipShape, 2,
                              Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += 3;
            }
            first = 0;
        }
        if (RedPipCount > 0) {
            for (; first < RedPipCount; first++) {
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)PowerPipShape, 3,
                              Point2D(shift, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += 3;
            }
        }
    }
    RadarClass::Draw_It(complete);
}


// void PowerClass::Remove_Pip() (0x0061CB50)
// Steps one of the power bar's counters down toward the target 0x0061C700
// gives it: +0x1528 first, then +0x1530, then +0x152C.

void PowerClass::Remove_Pip()
{
    int green;
    int red;
    int yellow;
    Desired_Levels(green, yellow, red);
    if (GreenPipCount > green) {
        GreenPipCount--;
        return;
    }
    if (RedPipCount > red) {
        RedPipCount--;
        return;
    }
    if (YellowPipCount > yellow) {
        YellowPipCount--;
    }
}


// void PowerClass::Add_Pip() (0x0061CBC0)
// Steps one of the power bar's counters up toward the target 0x0061C700
// gives it: +0x1530 first, then +0x1528, then +0x152C (the counterpart of
// 0x0061CB50; no direct callers).

void PowerClass::Add_Pip()
{
    int green;
    int red;
    int yellow;
    Desired_Levels(green, yellow, red);
    if (RedPipCount < red) {
        RedPipCount++;
        return;
    }
    if (GreenPipCount < green) {
        GreenPipCount++;
        return;
    }
    if (YellowPipCount < yellow) {
        YellowPipCount++;
    }
}


// void PowerClass::AI(int &input, Point2D const &xy) (0x0061CC30, slot 10)
// While the power bar is shown: counts the first counter down once per
// expiry of the first timer (redrawing the bar), restarts it when the
// player's power output or drain changed, then once per expiry of the
// second timer moves one of the three counters a step toward its target
// and restarts that timer for the share of the bar they fill. Ends with
// the radar's AI.

void PowerClass::AI(int &input, Point2D const &xy)
{
    if (unknown_flag_8373c1) {
        if (!HasChanged && FlashCount > 0 && FlashTimer.Value() == 0) {
            IsToRedraw = true;
            FlashCount--;
            unknown_flag_8373c2 = true;
            Map.Flag_To_Redraw(false);
            FlashTimer = 3;
        }
        if (unknown_player_a35db4->Power_Drain() != RecordedDrain || unknown_player_a35db4->Power_Output() != RecordedPower || HasChanged) {
            IsToRedraw = true;
            unknown_flag_8373c2 = true;
            Map.Flag_To_Redraw(false);
            if (unknown_player_a35db4->Power_Drain() != RecordedDrain || unknown_player_a35db4->Power_Output() != RecordedPower) {
                HasChanged = true;
                FlashCount = 10;
                FlashTimer = 3;
            }
            RecordedDrain = unknown_player_a35db4->Power_Drain();
            RecordedPower = unknown_player_a35db4->Power_Output();
            int green;
            int yellow;
            int red;
            Desired_Levels(green, yellow, red);
            if (UpdateTimer.Value() == 0) {
                HasChanged = false;
                if (RedPipCount != red) {
                    HasChanged = true;
                    if (RedPipCount > red) {
                        RedPipCount--;
                        Add_Pip();
                    } else {
                        RedPipCount++;
                        Remove_Pip();
                    }
                } else if (GreenPipCount != green) {
                    HasChanged = true;
                    if (GreenPipCount > green) {
                        GreenPipCount--;
                        Add_Pip();
                    } else {
                        GreenPipCount++;
                        Remove_Pip();
                    }
                } else if (YellowPipCount != yellow) {
                    HasChanged = true;
                    if (YellowPipCount > yellow) {
                        YellowPipCount--;
                        Add_Pip();
                    } else {
                        YellowPipCount++;
                        Remove_Pip();
                    }
                }
                if (HasChanged) {
                    int segments = Desired_Power_Height();
                    int total = YellowPipCount + GreenPipCount + RedPipCount;
                    if (total > segments) {
                        total = segments;
                    }
                    UpdateTimer = (int)((double)total / segments * 5.0);
                }
            }
        }
    }
    RadarClass::AI(input, xy);
}


// void PowerClass::Reposition_Sidebar() (0x0061D130, slot 34)
// After the radar's, registers the power bar's help region (id 999): 8
// pixels wide at the sidebar origin (5 pixels further right when
// unknown_id_source_a3d290->unknown_2239 is set), 0x45 below its top.

void PowerClass::Reposition_Sidebar()
{
    RadarClass::Reposition_Sidebar();
    if (ToolTips != 0) {
        ToolTip tip;
        tip.Label = 0;
        tip.ID = 999;
        int shift;
        if (unknown_id_source_a3d290->unknown_2239) {
            shift = 5;
        } else {
            shift = 0;
        }
        tip.Region.X = unknown_int_839620 + shift;
        tip.Region.Y = unknown_int_839624 + 0x45;
        tip.Region.Width = 8;
        tip.Region.Height = unknown_int_abcd70;
        ToolTips->Remove(999);
        ToolTips->Add(tip);
    }
}


// wchar_t const *PowerClass::Help_Text(int id) (0x0061D1C0, slot 33)
// Help text of a button: the power bar (id 999) shows the player's power
// output and drain; other ids are the radar's.

wchar_t const *PowerClass::Help_Text(int id)
{
    if (id == 999) {
        swprintf(unknown_text_a767e0, StringTable::Load_String("TXT_POWER_DRAIN", 0, "D:\\Ra2\\Power.CPP", 662),
                 unknown_player_a35db4->Power, unknown_player_a35db4->Drain);
        return unknown_text_a767e0;
    }
    return RadarClass::Help_Text(id);
}


// PowerClass scalar deleting destructor (0x0061D220, slot 4)
// Compiler generated (implicit destructor): RadarClass's destructor, then
// operator delete when bit 0 of the flags is set.
