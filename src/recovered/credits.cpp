// credits.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/credits.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/tab.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/radar.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/id_source.hpp"
#include "shared/crt_memory.hpp"


// Header-static dynamic initializers of the unit at 0x00494320: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00494320-0x00494501); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00494340, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x00494370, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x00494390, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x004943B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x004943D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x004943F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x00494410, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x00494450, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x00494480, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x004944B0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x004944E0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// Dynamic initializer (0x004944F0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00494320.cpp; no source of its own.


// CreditClass::CreditClass() (0x00494510): TibSun's CreditClass constructor.

CreditClass::CreditClass() :
    unknown_00(0),
    unknown_04(0),
    unknown_08(false),
    unknown_09(false),
    unknown_0a(false),
    unknown_0c(0)
{
}


// CreditClass::Unknown_00494530(bool forced) (0x00494530): TibSun's CreditClass::Graphic_Logic.

void CreditClass::Unknown_00494530(bool forced)
{
    if (forced || unknown_08) {
        int xx = unknown_surface_839948->Get_Width() / 2;

        if (unknown_0a && Rules->CreditTicks.Count() >= 2) {
            if (unknown_09) {
                Voc_Play(Rules->CreditTicks[0], 0.5f, 0x2000, 0);
            } else {
                Voc_Play(Rules->CreditTicks[1], 0.5f, 0x2000, 0);
            }
        }

        Map.Unknown_006A1710();

        int color = Build_Hicolor_Pixel(unknown_color_ac1028.Red, unknown_color_ac1028.Green, unknown_color_ac1028.Blue);
        Plain_Text_Print(L"%ld", unknown_surface_839948, &unknown_surface_839948->Get_Rect(), &Point2D(xx, 2), color, 0, 0x4108, unknown_04);

        unknown_08 = false;
        unknown_0a = false;
    }
}


// CreditClass::Unknown_00494650(bool forced) (0x00494650): TibSun's CreditClass::AI.

void CreditClass::Unknown_00494650(bool forced)
{
    static int _last = 0;

    _last = Frame;

    unknown_00 = unknown_player_a35db4->As_IHouse()->Available_Money();
    unknown_00 = unknown_00 > 0 ? unknown_00 : 0;

    if (unknown_id_source_a3d290->MissionTimer.Is_Active() || unknown_id_source_a3d290->MissionTimer) {
        unknown_08 = true;
        Map.Flag_To_Redraw();
    }

    if (unknown_04 == unknown_00) return;

    if (forced) {
        unknown_0a = false;
        unknown_04 = unknown_00;
    } else {
        if (unknown_0c) unknown_0c--;

        int adder = unknown_00 - unknown_04;

        if (adder > 0) {
            unknown_0c = 1;
        } else {
            unknown_0c = 3;
        }

        adder = abs(adder);
        adder >>= 3;
        if (adder < 1) {
            adder = 1;
        } else if (adder > 71+72) {
            adder = 71+72;
        }
        if (unknown_04 > unknown_00) adder = -adder;
        unknown_04 += adder;
        if (unknown_04 - adder != unknown_04) {
            unknown_0a = true;
            unknown_09 = (adder > 0);
        }
    }
    unknown_08 = true;
    Map.Flag_To_Redraw();
    Map.unknown_4ee4 = true;
}
