// void SidebarClass::StripClass::Unknown_0067F800(bool complete) (0x0067F800)
// Draws the strip's visible cameos: shape, hover frame, darkening, flash,
// caption, queue count, ready/hold text and the build clock.
#include "shared/sidebar.hpp"
#include "shared/sidebar_screen.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor.hpp"
#include "shared/radar.hpp"
#include "shared/string_table.hpp"
// @ref ?Unknown_004F4F30@UnknownSidebarHouse@@QAE_NPAVTechnoTypeClass@@@Z=004f4f30
// @ref ?Unknown_004EB580@UnknownSidebarHouse@@QAEPAVFactoryClass@@H_NH@Z=004eb580
// @ref ??_C@_08JP@TXT_HOLD?$AA@=007f3eb4
// @ref ??_C@_15KLDP@?$AA?$CF?$AAd?$AA?$AA@=007d1598
// @ref ??_C@_09IMGH@TXT_READY?$AA@=007d1610
// @ref ??_C@_0BD@KJOF@D?3?2Ra2?2Sidebar?4CPP?$AA@=007f3ea0
// @ref ?Unknown_004B9900@UnknownFactory@@QAE_NXZ=004b9900
// @ref ?Unknown_00682140@@YIXPBGABUPoint2D@@ABVRect@@H@Z=00682140

static inline int Bottom_Extra()
{
    if (unknown_id_source_a3d290->unknown_2239) {
        return 26;
    }
    return 18;
}

void SidebarClass::StripClass::Unknown_0067F800(bool complete)
{
    if (unknown_18 && (unknown_38 || complete)) {
        unknown_38 = false;
        unknown_flag_abcd84 = true;
        Rect cliprect;
        cliprect.X = 0;
        cliprect.Y = 0;
        cliprect.Width = unknown_surface_839948->Get_Width();
        cliprect.Height = unknown_surface_839948->Get_Height();

        bool busy = false;
        for (int i = 0; i < BuildableCount; i++) {
            if (Buildables[i].unknown_10 == 1 || Buildables[i].unknown_10 == 2) {
                busy = true;
                break;
            }
        }

        int rows = (unknown_int_839624 + unknown_int_83962c - unknown_abcd64 - Bottom_Extra() - 7) / 50 + (unknown_3b ? 1 : 0);
        int columns = 2;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                bool draw = complete;
                int stage = 0;
                void const *shape = 0;
                UnknownFactory *factory = 0;
                wchar_t const *state = 0;
                int index = col + (unknown_40 + row) * columns;
                bool production = false;
                bool completed = false;
                bool darken = false;
                bool hover = false;
                int x = unknown_abcd68 * col - unknown_rect_839620.X + unknown_1c;
                int y = unknown_abcd6c * row + 1 + unknown_20;
                SelectClass *button = &SelectButton[row * columns + unknown_34 * 60 + col];
                if (unknown_3b) {
                    y += unknown_48 - unknown_abcd6c;
                }
                wchar_t const *name = 0;
                TechnoTypeClass *obj = 0;

                if (index < BuildableCount) {
                    hover = button->Unknown_00681690();
                    if (Buildables[index].unknown_04 != 31) {
                        obj = (TechnoTypeClass *)Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
                        if (obj) {
                            name = obj->UIName;
                            bool isbusy = false;
                            if (obj->Kind_Of() == 7 && busy) {
                                isbusy = true;
                            } else if (obj->Who_Can_Build_Me(true, true, true, unknown_player_a35db4) == 0) {
                                isbusy = true;
                            } else {
                                TechnoTypeClass *type = (TechnoTypeClass *)Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
                                bool none = unknown_player_a35db4->Can_Build(type, false, false) == -1;
                                bool limited = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004F4F30(type);
                                if (none || limited) {
                                    isbusy = true;
                                }
                            }
                            shape = obj->unknown_slot38();
                            factory = Buildables[index].Factory;
                            if (factory) {
                                production = true;
                                completed = factory->Unknown_004B9900();
                                if (completed) {
                                    state = StringTable::Load_String("TXT_READY", 0, "D:\\Ra2\\Sidebar.CPP", 2845);
                                } else {
                                    draw = true;
                                }
                                stage = Unknown_0067F7C0(index);
                                darken = false;
                            } else {
                                production = false;
                                darken = isbusy;
                                switch (Buildables[index].unknown_10) {
                                case 1:
                                    draw = true;
                                    production = true;
                                    stage = Unknown_0067F7C0(index);
                                    darken = false;
                                    break;
                                case 3:
                                    production = true;
                                    completed = true;
                                    darken = false;
                                    break;
                                case 2:
                                    production = true;
                                    completed = false;
                                    darken = true;
                                    stage = Unknown_0067F7C0(index);
                                    break;
                                case 0:
                                    if (!complete) {
                                        continue;
                                    }
                                    break;
                                }
                            }
                        } else {
                            darken = false;
                        }
                    } else {
                        int id = Buildables[index].unknown_00;
                        name = SuperWeaponTypes[id]->UIName;
                        shape = Unknown_0067E740(id);
                        production = true;
                        completed = !unknown_player_a35db4->Supers[id]->unknown_69e990();
                        state = unknown_player_a35db4->Supers[id]->State_String();
                        stage = unknown_player_a35db4->Supers[id]->Anim_Stage();
                        darken = false;
                        draw = true;
                    }
                } else {
                    production = false;
                }

                Rect window(0, 0, x + 60, y + 48);
                if (draw) {
                    if (shape) {
                        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323b0, (ShapeFileStruct const *)shape, 0,
                            Point2D(x, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                    if (hover) {
                        RGBClass color = unknown_color_ac1028;
                        unknown_surface_839948->Draw_Rect(window, Rect(x, y, 60, 48),
                            (int)(((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8) | ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8) | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0)));
                    }
                    if (darken) {
                        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab9480, 0,
                            Point2D(x, y), window, 0x401, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                    if (Buildables[index].unknown_2c > Frame && Frame % 16 > 8) {
                        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab9480, 0,
                            Point2D(x, y), window, 0x404, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                    if (name) {
                        Unknown_00682140(name, Point2D(x, y + 36), cliprect, 60);
                    }
                    bool hasqueue = false;
                    unsigned pixel = ((unknown_color_ac1028.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                        | ((unknown_color_ac1028.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0)
                        | ((unknown_color_ac1028.Green >> unknown_shift_852fcc) << unknown_shift_852fc8);
                    if (obj) {
                        FactoryClass *queue = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB580(obj->Kind_Of(), obj->Naval, 0);
                        if (queue) {
                            int total = queue->Count_Type_004B9E40(obj);
                            if (total > 1 || total > 0 && (!queue->Object || queue->Object->unknown_slot32() && queue->Object->unknown_slot32() != obj)) {
                                wchar_t buffer[256];
                                swprintf(buffer, L"%d", total);
                                Point2D point(x + 60, y + 1);
                                Rect rect = Text_Rect(buffer, point.X, point.Y, 0x242, 2, 1);
                                Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                                Plain_Text_Print(L"%d", unknown_surface_839948, &cliprect, &point, pixel, 0, 0x242, total);
                                hasqueue = true;
                            }
                        }
                    }
                    if (production) {
                        if (state) {
                            Point2D point(x + 30, y + 1);
                            Rect rect = Text_Rect(state, point.X, point.Y, 0x142, 2, 1);
                            Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                            Plain_Text_Print(state, unknown_surface_839948, &cliprect, &point, pixel, 0, 0x142);
                        }
                        if (!completed) {
                            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[7], stage + 1,
                                Point2D(x, y), window, 0x404, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                            if (factory && (!factory->unknown_34 || factory->unknown_6c) || Buildables[index].unknown_10 == 2) {
                                wchar_t const *hold = StringTable::Load_String("TXT_HOLD", 0, "D:\\Ra2\\Sidebar.CPP", 3070);
                                if (hasqueue) {
                                    Point2D point(x + 2, y + 1);
                                    Rect rect = Text_Rect(hold, point.X, point.Y, 0x42, 2, 1);
                                    Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                                    Plain_Text_Print(hold, unknown_surface_839948, &cliprect, &point, pixel, 0, 0x42);
                                } else {
                                    Point2D point(x + 30, y + 1);
                                    Rect rect = Text_Rect(hold, point.X, point.Y, 0x142, 2, 1);
                                    Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                                    Plain_Text_Print(hold, unknown_surface_839948, &cliprect, &point, pixel, 0, 0x142);
                                }
                            }
                        }
                    }
                }
            }
        }
        unknown_4c = unknown_48;
    }
}
