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
// @ref ?TacticalRect@@3VRect@@A=00839630
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
    SelectableCount = 0;
    bool has_selectable_buildings = Contains_Selectable_Buildings(TacticalRect);

    int xoffset = LastTacPixelX - TacPixelX;
    int yoffset = LastTacPixelY - TacPixelY;

    if (drawpass == 0) {
        if (abs(xoffset) < TacticalRect.Width && abs(yoffset) < TacticalRect.Height) {
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
    } else if (!IsFirstRender && !has_selectable_buildings && TacticalLists::DirtyAreas.Count() == 0 && !IsToRedraw && TacticalCoord.X == LastTacticalCoord.X && TacticalCoord.Y == LastTacticalCoord.Y) {
        render = false;
    } else if (has_selectable_buildings) {
        render = true;
    } else {
        render = IsToRedraw || TacticalLists::DirtyAreas.Count() != 0 || TacPixelX != LastTacPixelX || TacPixelY != LastTacPixelY;
    }

    if (render) {
        unsigned panwidth = abs(xoffset);
        unsigned panheight = abs(yoffset);
        int width = TacticalDimensions.Width - panwidth;
        int height = TacticalDimensions.Height - panheight;
        int ystart = (yoffset < 0) ? -yoffset : 0;
        int xstart = (xoffset < 0) ? -xoffset : 0;

        Rect sourcerect(TacticalDimensions.X + xstart, TacticalDimensions.Y + ystart,
            width, height);
        Rect destrect(TacticalDimensions.X + ((xoffset <= 0) ? 0 : xoffset),
            TacticalDimensions.Y + ((yoffset <= 0) ? 0 : yoffset),
            width, height);

        if (xoffset != 0 || yoffset != 0) {
            if (drawpass == 0 || drawpass == 3) {
                CompositeSurface->Copy_From(destrect, *TileSurface, sourcerect, false, true);
            }
            if (drawpass == 0 || drawpass == 3) {
                DepthBuffer->Pan(-xoffset, -yoffset, 0xFFFF);
            }
            if (drawpass == 0 || drawpass == 3) {
                AlphaBuffer->Pan(-xoffset, -yoffset, 0x7F);
            }
            if (drawpass == 0 || drawpass == 3) {
                Surface *oldcomp = CompositeSurface;
                CompositeSurface = TileSurface;
                TileSurface = oldcomp;
                surface = CompositeSurface;
                LogicalSurface = CompositeSurface;
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
            Register_Dirty_Buildings(Rect(TacticalRect.X - 64, TacticalRect.Y - 64,
                TacticalRect.Width + 128, TacticalRect.Height + 128));
        }

        Surface *prevlogical = LogicalSurface;
        LogicalSurface = TileSurface;
        LogicalSurface->Lock();

        if (drawpass == 1 || drawpass == 3) {
            bool scrolled_left = LastTacPixelX > TacPixelX;
            bool scrolled_right = LastTacPixelX < TacPixelX;
            bool scrolled_up = LastTacPixelY > TacPixelY;
            bool scrolled_down = LastTacPixelY < TacPixelY;

            Rect leftstrip(TacticalRect.X, TacticalRect.Y, panwidth, TacticalRect.Height);
            Rect rightstrip(TacticalRect.X + TacticalRect.Width - panwidth, TacticalRect.Y, panwidth, TacticalRect.Height);
            Rect leftclip = Intersect(leftstrip, TacticalRect);
            Rect rightclip = Intersect(rightstrip, TacticalRect);

            Rect topstrip;
            Rect bottomstrip;
            if (scrolled_left) {
                topstrip = Rect(TacticalRect.X + panwidth, TacticalRect.Y, TacticalRect.Width - panwidth, panheight);
                bottomstrip = Rect(TacticalRect.X + panwidth, TacticalRect.Y + TacticalRect.Height - panheight, TacticalRect.Width - panwidth, panheight);
            } else if (scrolled_right) {
                topstrip = Rect(TacticalRect.X, TacticalRect.Y, TacticalRect.Width - panwidth, panheight);
                bottomstrip = Rect(TacticalRect.X, TacticalRect.Y + TacticalRect.Height - panheight, TacticalRect.Width - panwidth, panheight);
            } else {
                topstrip = Rect(TacticalRect.X, TacticalRect.Y, TacticalRect.Width, panheight);
                bottomstrip = Rect(TacticalRect.X, TacticalRect.Y + TacticalRect.Height - panheight, TacticalRect.Width, panheight);
            }
            Rect topclip = Intersect(topstrip, TacticalRect);
            Rect bottomclip = Intersect(bottomstrip, TacticalRect);

            Rect bounds(TacticalRect.X + (scrolled_left ? panwidth : 0),
                TacticalRect.Y + (scrolled_up ? panheight : 0),
                TacticalRect.Width - panwidth, TacticalRect.Height - panheight);

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

            LogicalSurface->Unlock();
            LogicalSurface = prevlogical;
            CellRedrawCount = 0;
        }
    }
    }

    LastTacticalCoord.X = TacticalCoord.X;
    LastTacticalCoord.Y = TacticalCoord.Y;
    LastTacPixelX = TacPixelX;
    LastTacPixelY = TacPixelY;

    if (drawpass == 1 || drawpass == 3) {
        LogicalSurface->Copy_From(TacticalRect, *TileSurface, TacticalRect, false, true);
    }

    if (drawpass == 1) {
        return;
    }

    if (drawpass == 2 || drawpass == 3) {
        int i;

        Add_Buildings_To_Selectable(TacticalRect);
        surface->Lock();
        Surface *savedlogical = LogicalSurface;
        LogicalSurface = surface;
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
        BeaconManager.Unknown_0042E290(LogicalSurface, TacticalRect);
        Draw_Rubber_Band();
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
            if (techno->House->Is_Player_Control()) {
                if (!planning && techno->IsSelected) {
                    if (TechnoClass_ActionLines) {
                        techno->unknown_slot243(false, 0);
                    }
                    UnknownTechno230 *capture = (UnknownTechno230 *)techno->CaptureManager;
                    if (capture != 0 && capture->unknown_24 != 0) {
                        ((CaptureManagerClass *)capture)->Draw_Link();
                    }
                }
            } else if (Unknown_004386F0(techno) && techno != 0 && techno->AbstractFlags.IsFoot && !techno->IsInLimbo /* NATIVE: RA2_OBJECT_IS_FOOT inline spelled out (techno is non-null here) */) {
                Unknown_004CAB90(techno);
            }
        }

        if (planning) {
            Unknown_00618090();
        }

        Draw_Pixel_Effects(TacticalRect, TacticalRect);
        LogicalSurface = savedlogical;
        surface->Unlock();

        int line = 0;
        if (Scen->MissionTimer.Started != -1) {
            Draw_Timer(0, ColorSchemes[PlayerPtr->ColorSchemeIndex], Scen->MissionTimer / 15,
                Scen->MissionTimerText, 0, 0);
            line = 1;
        }

        count = TimedSuperWeapons.Count();
        for (i = 0; i < count; i++) {
            if (!TimedSuperWeapons[i]->unknown_0x58 || unknown_game_mode_a3d298 != 0
                    || TimedSuperWeapons[i]->RechargeTimer != TimedSuperWeapons[i]->Type->RechargeTime) {
                SuperClass *super = TimedSuperWeapons[i];
                Draw_Timer(line, ColorSchemes[super->House->ColorSchemeIndex], super->RechargeTimer / 15, super->Type->UIName,
                    (unsigned __int64 *)&super->unknown_0x3C.unknown_4, &super->unknown_0x3C.unknown_0);
                line++;
            }
        }

        count = Houses.Count();
        for (i = 0; i < count; i++) {
            int time = Houses[i]->PowerBlackoutTimer;
            if (time > 0) {
                Draw_Timer(line, ColorSchemes[Houses[i]->ColorSchemeIndex], time / 15,
                    StringTable::Load_String("MSG:BlackoutTimer", 0, "D:\\Ra2\\Tactical.CPP", 444), 0, 0);
                line++;
            }
        }

        Draw_Screen_Text((wchar_t const *)&unknown_24);
        IsFirstRender = false;
        IsToRedraw = false;
    }
}
