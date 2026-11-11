// NON-EXACT DRAFT 0x6A45A0 FUN_006a45a0
// entry 0x6A45A0; exact-match score 64.69 (misses_index best), this text 56.45; plan "review: rewrite likely"
// origin lane: Scout958 (build/class-work/Scout958/w/keep/render_b3_5645.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): RA2_OBJECT_IS_FOOT inline spelled out; each changed line is marked NATIVE.
// void Tactical::Render(Surface *surface, bool fullredraw, int drawpass) (0x006A45A0): draws one pass of the
// tactical view (TibSun Tactical::Render).
#include "shared/tactical.hpp"
#include "shared/tactical_lists.hpp"
#include "shared/display.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/surface.hpp"
#include "shared/blit.hpp"
#include "shared/building_class.hpp"
#include "shared/techno.hpp"
#include "shared/house_screen.hpp"
#include "shared/house.hpp"
#include "shared/super.hpp"
#include "shared/id_source.hpp"
#include "shared/color_scheme.hpp"
#include "shared/string_table.hpp"
#include "shared/veinhole.hpp"
#include "shared/spot_light.hpp"
#include "shared/ebolt.hpp"
#include "shared/line_trail.hpp"
#include "shared/capture_manager.hpp"
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?DirtyAreas@TacticalLists@@2V?$DynamicVectorClass@UDirtyAreaStruct@@@@A=00abe5b0
// @ref ??_C@_0BC@KAME@MSG?3BlackoutTimer?$AA@=007f6a70
// @ref ??_C@_0BE@LNLL@D?3?2Ra2?2Tactical?4CPP?$AA@=007f6a84
// @ref ?Unknown_004386F0@@YI_NPAVTechnoClass@@@Z=004386f0
// @ref ?Unknown_004CAB90@@YIXPAVTechnoClass@@@Z=004cab90
// @ref ?Unknown_00521FB0@@YAXXZ=00521fb0
// @ref ?Unknown_005334D0@@YAXXZ=005334d0
// @ref ?Unknown_00617EF0@@YAXXZ=00617ef0
// @ref ?Unknown_00618090@@YAXXZ=00618090
// @ref ?Unknown_006354E0@@YAXXZ=006354e0
// @ref ?Draw_Pixel_Effects@Tactical@@QAEXABVRect@@0@Z=006a7e80
// @ref ?Draw_Timer@@YIXHPAVColorScheme@@HPB_WPA_KPA_N@Z=006a5280
// @ref ?Draw_Objects@Tactical@@QAEX_N@Z=006a93f0
// @ref ?Contains_Selectable_Buildings@Tactical@@QAE_NVRect@@@Z=006aa180
// @ref ?Add_Buildings_To_Selectable@Tactical@@QAEXVRect@@@Z=006aa300
// @ref ?Draw_Radial_Indicators@Tactical@@QAEXXZ=006ac420
// @ref ?unknown_scrolled_off_screen_abfd6c@@3_NA=00abfd6c
// @ref ?Planning_Active@@YA_NXZ=00614840
// @ref ?ColorSchemes@@3V?$DynamicVectorClass@PAVColorScheme@@@@A=00ab6ef8

// Every techno (0x00A40C20; declared in shared/planning.hpp, which this unit cannot include).
extern DynamicVectorClass<TechnoClass *> unknown_list_a40c20;
extern bool unknown_scrolled_off_screen_abfd6c;     // 0x00ABFD6C
void Unknown_00521FB0();                            // 0x00521FB0 (TibSun IonBlastClass::Draw_All; hypothesis)
void Unknown_005334D0();                            // 0x005334D0 (TibSun LaserDrawClass::Draw_All; hypothesis)
void Unknown_006354E0();                            // 0x006354E0 (RadBeam drawing; hypothesis)
bool Planning_Active();                             // 0x00614840
void Planning_Redraw();                             // 0x00617E40
void Unknown_00617EF0();                            // 0x00617EF0 (planning nodes; hypothesis)
void Unknown_00618090();                            // 0x00618090 (planning nodes; hypothesis)
bool __fastcall Unknown_004386F0(TechnoClass *techno);  // 0x004386F0 (hypothesis)
void __fastcall Draw_Timer(int line, ColorScheme *scheme, int value, wchar_t const *text,
    unsigned __int64 *flash_time, bool *flash);                         // 0x006A5280
void __fastcall Unknown_004CAB90(TechnoClass *techno);  // 0x004CAB90 (hypothesis)

void Tactical::Render(Surface *surface, bool fullredraw, int drawpass)
{
    unknown_db0 = 0;
    bool has_selectable_buildings = Contains_Selectable_Buildings(unknown_rect_839630);

    int xoffset = LastTacPixelX - TacPixelX;
    int yoffset = LastTacPixelY - TacPixelY;

    if (drawpass == 0) {
        if (abs(xoffset) < unknown_rect_839630.Width && abs(yoffset) < unknown_rect_839630.Height) {
            unknown_scrolled_off_screen_abfd6c = false;
        } else {
            fullredraw = true;
            unknown_scrolled_off_screen_abfd6c = true;
        }
    } else {
        if (unknown_scrolled_off_screen_abfd6c) {
            fullredraw = true;
        }
    }

    if (fullredraw) {
        xoffset = 0;
        yoffset = 0;
    }

    if (drawpass != 2) {
    bool render;
    if (fullredraw) {
        render = true;
    } else if (!unknown_d7c && !has_selectable_buildings && TacticalLists::DirtyAreas.Count() == 0 && !unknown_d7d && unknown_d64.X == unknown_d6c.X && unknown_d64.Y == unknown_d6c.Y) {
        render = false;
    } else if (has_selectable_buildings) {
        render = true;
    } else {
        render = unknown_d7d || TacticalLists::DirtyAreas.Count() != 0 || TacPixelX != LastTacPixelX || TacPixelY != LastTacPixelY;
    }

    if (render) {
        unsigned panwidth = abs(xoffset);
        unsigned panheight = abs(yoffset);
        int width = unknown_rect_abe560.Width - panwidth;
        int height = unknown_rect_abe560.Height - panheight;
        int ystart = (yoffset < 0) ? -yoffset : 0;
        int xstart = (xoffset < 0) ? -xoffset : 0;

        Rect sourcerect(unknown_rect_abe560.X + xstart, unknown_rect_abe560.Y + ystart,
            width, height);
        Rect destrect(unknown_rect_abe560.X + ((xoffset <= 0) ? 0 : xoffset),
            unknown_rect_abe560.Y + ((yoffset <= 0) ? 0 : yoffset),
            width, height);

        if (xoffset != 0 || yoffset != 0) {
            if (drawpass == 0 || drawpass == 3) {
                unknown_surface_839964->Copy_From(destrect, *unknown_surface_839944, sourcerect, false, true);
            }
            if (drawpass == 0 || drawpass == 3) {
                DepthBuffer->Pan(-xoffset, -yoffset, 0xFFFF);
            }
            if (drawpass == 0 || drawpass == 3) {
                AlphaBuffer->Pan(-xoffset, -yoffset, 0x7F);
            }
            if (drawpass == 0 || drawpass == 3) {
                Surface *oldcomp = unknown_surface_839964;
                unknown_surface_839964 = unknown_surface_839944;
                unknown_surface_839944 = oldcomp;
                surface = unknown_surface_839964;
                unknown_surface_83995c = unknown_surface_839964;
            }
        } else {
            if (fullredraw && (drawpass == 0 || drawpass == 3)) {
                DepthBuffer->Fill(0xFFFF);
                AlphaBuffer->Fill(0x7F);
            }
        }

        if (drawpass == 0) {
            return;
        }

        if (!Debug_Map) {
            Register_Dirty_Buildings(Rect(unknown_rect_839630.X - 64, unknown_rect_839630.Y - 64,
                unknown_rect_839630.Width + 128, unknown_rect_839630.Height + 128));
        }

        Surface *prevlogical = unknown_surface_83995c;
        unknown_surface_83995c = unknown_surface_839944;
        unknown_surface_83995c->Lock();

        if (drawpass == 1 || drawpass == 3) {
            bool scrolled_left = LastTacPixelX > TacPixelX;
            bool scrolled_right = LastTacPixelX < TacPixelX;
            bool scrolled_up = LastTacPixelY > TacPixelY;
            bool scrolled_down = LastTacPixelY < TacPixelY;

            Rect leftstrip(unknown_rect_839630.X, unknown_rect_839630.Y, panwidth, unknown_rect_839630.Height);
            Rect rightstrip(unknown_rect_839630.X + unknown_rect_839630.Width - panwidth, unknown_rect_839630.Y, panwidth, unknown_rect_839630.Height);
            Rect leftclip = Intersect(leftstrip, unknown_rect_839630);
            Rect rightclip = Intersect(rightstrip, unknown_rect_839630);

            Rect topstrip;
            Rect bottomstrip;
            if (scrolled_left) {
                topstrip = Rect(unknown_rect_839630.X + panwidth, unknown_rect_839630.Y, unknown_rect_839630.Width - panwidth, panheight);
                bottomstrip = Rect(unknown_rect_839630.X + panwidth, unknown_rect_839630.Y + unknown_rect_839630.Height - panheight, unknown_rect_839630.Width - panwidth, panheight);
            } else if (scrolled_right) {
                topstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y, unknown_rect_839630.Width - panwidth, panheight);
                bottomstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y + unknown_rect_839630.Height - panheight, unknown_rect_839630.Width - panwidth, panheight);
            } else {
                topstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y, unknown_rect_839630.Width, panheight);
                bottomstrip = Rect(unknown_rect_839630.X, unknown_rect_839630.Y + unknown_rect_839630.Height - panheight, unknown_rect_839630.Width, panheight);
            }
            Rect topclip = Intersect(topstrip, unknown_rect_839630);
            Rect bottomclip = Intersect(bottomstrip, unknown_rect_839630);

            Rect bounds(unknown_rect_839630.X + (scrolled_left ? panwidth : 0),
                unknown_rect_839630.Y + (scrolled_up ? panheight : 0),
                unknown_rect_839630.Width - panwidth, unknown_rect_839630.Height - panheight);

            Rect xpanrect(0, 0, 0, 0);
            Rect ypanrect(0, 0, 0, 0);
            if (scrolled_left) {
                xpanrect = leftclip;
            } else if (scrolled_right) {
                xpanrect = rightclip;
            }
            if (scrolled_up) {
                ypanrect = topclip;
            } else if (scrolled_down) {
                ypanrect = bottomclip;
            }

            Wipe_Depth(fullredraw, xoffset, yoffset, bounds);
            Render_Shroud(xpanrect, ypanrect, bounds, fullredraw);
            Render_Tiles(xpanrect, ypanrect, fullredraw);
            Render_Fogged_Objects(xpanrect, ypanrect, fullredraw);
            Render_Overlays(xpanrect, ypanrect, fullredraw);
            Render_Terrain(xpanrect, ypanrect, fullredraw);
            Render_Tile_Shadows(xpanrect, ypanrect, fullredraw);
            Render_Buildings(xpanrect, ypanrect, fullredraw);

            for (int index = TacticalLists::DirtyAreas.Count() - 1; index >= 0; index--) {
                TacticalLists::DirtyAreas.Delete(index);
            }

            unknown_surface_83995c->Unlock();
            unknown_surface_83995c = prevlogical;
            unknown_e0 = 0;
        }
    }
    }

    unknown_d6c.X = unknown_d64.X;
    unknown_d6c.Y = unknown_d64.Y;
    LastTacPixelX = TacPixelX;
    LastTacPixelY = TacPixelY;

    if (drawpass == 1 || drawpass == 3) {
        unknown_surface_83995c->Copy_From(unknown_rect_839630, *unknown_surface_839944, unknown_rect_839630, false, true);
    }

    if (drawpass == 1) {
        return;
    }

    if (drawpass == 2 || drawpass == 3) {
        int i;

        Add_Buildings_To_Selectable(unknown_rect_839630);
        surface->Lock();
        Surface *savedlogical = unknown_surface_83995c;
        unknown_surface_83995c = surface;
        Draw_Waypoints(false);
        Draw_Rally_Points(false);
        Draw_Placement(false);
        VeinholeMonsterClass::Draw_All();
        Unknown_00521FB0();
        Draw_Objects(true);
        SpotLightClass::Draw_All();
        Unknown_005334D0();
        EBolt::Update_All_004B2210();
        LineTrail::Update_All();
        Unknown_006354E0();
        Draw_Radial_Indicators();
        unknown_object_84e8d0.Unknown_0042E290(unknown_surface_83995c, unknown_rect_839630);
        Unknown_006AA790();
        Draw_Waypoints(true);
        Draw_Rally_Points(true);
        Draw_Placement(true);

        bool planning = Planning_Active();
        if (planning) {
            Planning_Redraw();
            Unknown_00617EF0();
        }

        int count = unknown_list_a40c20.Count();
        for (i = 0; i < count; i++) {
            TechnoClass *techno = unknown_list_a40c20[i];
            if (techno->House->Unknown_004F52B0()) {
                if (!planning && techno->unknown_77) {
                    if (unknown_bool_7f7110) {
                        techno->unknown_slot243(false, 0);
                    }
                    UnknownTechno230 *capture = (UnknownTechno230 *)techno->unknown_230;
                    if (capture != 0 && capture->unknown_24 != 0) {
                        ((CaptureManagerClass *)capture)->Draw_Link();
                    }
                }
            } else if (Unknown_004386F0(techno) && techno != 0 && techno->AbstractFlags.IsFoot && !techno->unknown_76 /* NATIVE: RA2_OBJECT_IS_FOOT inline spelled out (techno is non-null here) */) {
                Unknown_004CAB90(techno);
            }
        }

        if (planning) {
            Unknown_00618090();
        }

        Draw_Pixel_Effects(unknown_rect_839630, unknown_rect_839630);
        unknown_surface_83995c = savedlogical;
        surface->Unlock();

        int line = 0;
        if (unknown_id_source_a3d290->MissionTimer.Started != -1) {
            Draw_Timer(0, ColorSchemes[unknown_player_a35db4->ColorSchemeIndex], unknown_id_source_a3d290->MissionTimer / 15,
                unknown_id_source_a3d290->MissionTimerText, 0, 0);
            line = 1;
        }

        count = unknown_supers_a35db8.Count();
        for (i = 0; i < count; i++) {
            if (!unknown_supers_a35db8[i]->unknown_0x58 || unknown_game_mode_a3d298 != 0
                    || unknown_supers_a35db8[i]->RechargeTimer != unknown_supers_a35db8[i]->Type->RechargeTime) {
                SuperClass *super = unknown_supers_a35db8[i];
                Draw_Timer(line, ColorSchemes[super->House->ColorSchemeIndex], super->RechargeTimer / 15, super->Type->UIName,
                    (unsigned __int64 *)&super->unknown_0x3C.unknown_4, &super->unknown_0x3C.unknown_0);
                line++;
            }
        }

        count = Houses.Count();
        for (i = 0; i < count; i++) {
            int time = Houses[i]->unknown_1ec;
            if (time > 0) {
                Draw_Timer(line, ColorSchemes[Houses[i]->ColorSchemeIndex], time / 15,
                    StringTable::Load_String("MSG:BlackoutTimer", 0, "D:\\Ra2\\Tactical.CPP", 444), 0, 0);
                line++;
            }
        }

        Draw_Screen_Text((wchar_t const *)&unknown_24);
        unknown_d7c = false;
        unknown_d7d = false;
    }
}
