
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/tactical.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

#include "beacon.h"
#include "ebolt.h"
#include "linetrail.h"
#include "radbeam.h"
#include "capmgr.h"
#include "super.h"
#include "suprtype.h"
#include "stringtable.h"

bool __fastcall FUN_00637AA0(void);   // planning.cpp: planning mode is on
void __fastcall FUN_0063B0A0(void);
void __fastcall FUN_0063B150(void);
extern BeaconManagerClass BeaconManager;
extern DynamicVectorClass<SuperClass *> ShowTimers;

void Tactical::Render(Surface & surface, bool fullredraw, int drawpass)
{
	/*
	 * Set true when the view scrolled further than one screen, so the still-valid region can no
	 * longer be blitted and a full redraw is forced. It persists across the passes of a frame.
	 */
	static bool scrolled_off_screen = false;

	/*
	 * The composite surface to draw the foreground objects onto. This tracks the front buffer
	 * across the surface swap performed during the pan pass.
	 */
	Surface * composite = &surface;

	SelectableCount = 0;
	bool has_selectable_buildings = Contains_Selectable_Buildings(TacticalRect);

	/*
	 * Work out how far the view has panned since the last frame. A pan smaller than the view can be
	 * satisfied by blitting the still-valid region and only redrawing the strips that scrolled in;
	 * a larger pan forces a full redraw.
	 */
	int xoffset = LastTacPixelX - TacPixelX;
	int yoffset = LastTacPixelY - TacPixelY;

	if (drawpass == DRAW_PASS_PAN) {
		if (abs(xoffset) < TacticalRect.Width && abs(yoffset) < TacticalRect.Height) {
			scrolled_off_screen = false;
		} else {
			fullredraw = true;
			scrolled_off_screen = true;
		}
	} else {
		if (scrolled_off_screen) {
			fullredraw = true;
		}
	}

	/*
	 * Either the whole view, or just the strips the pan exposed.
	 */
	if (fullredraw) {
		xoffset = 0;
		yoffset = 0;
	}

	/*
	 * Decide whether the cached layers actually need to be rebuilt this pass. A pending redraw,
	 * selectable buildings, dirty areas or a pixel pan all force a rebuild; but once the first
	 * frame has been rendered, a pan is only honoured if the world coordinate also changed. The
	 * foreground pass never rebuilds the cached layers.
	 */
	bool render;
	/*
	 * Either the whole view, or just the strips the pan exposed.
	 */
	if (fullredraw) {
		render = true;
	} else if (IsFirstRender) {
		if (has_selectable_buildings) {
			render = true;
		} else {
			render = IsToRedraw || DirtyAreas.Count() != 0 || TacPixelX != LastTacPixelX || TacPixelY != LastTacPixelY;
		}
	} else if (has_selectable_buildings) {
		render = true;
	} else if (DirtyAreas.Count() == 0 && IsToRedraw) {
		render = true;
	} else if (DirtyAreas.Count() == 0 && TacticalCoord.X == LastTacticalCoord.X && TacticalCoord.Y == LastTacticalCoord.Y) {
		render = false;
	} else {
		render = IsToRedraw || DirtyAreas.Count() != 0 || TacPixelX != LastTacPixelX || TacPixelY != LastTacPixelY;
	}

	if (drawpass != DRAW_PASS_FOREGROUND && render) {

		/*
		 * The magnitude of the pan, and the source/destination rectangles for shifting the
		 * still-valid portion of the cached surface across by that amount.
		 */
		unsigned panwidth = abs(xoffset);
		unsigned panheight = abs(yoffset);
		int ystart = (yoffset >= 0) ? 0 : -yoffset;
		int xstart = (xoffset >= 0) ? 0 : -xoffset;

		Rect sourcerect(TacticalDimensions.X + xstart, TacticalDimensions.Y + ystart,
					TacticalDimensions.Width - panwidth, TacticalDimensions.Height - panheight);
		Rect destrect(TacticalDimensions.X + ((xoffset <= 0) ? 0 : xoffset),
					TacticalDimensions.Y + ((yoffset <= 0) ? 0 : yoffset),
					TacticalDimensions.Width - panwidth, TacticalDimensions.Height - panheight);

		if (xoffset != 0 || yoffset != 0) {
			if (drawpass == DRAW_PASS_PAN || drawpass == DRAW_PASS_FULL) {
				CompositeSurface->Blit_From(destrect, *TileSurface, sourcerect);
			}
			if (drawpass == DRAW_PASS_PAN || drawpass == DRAW_PASS_FULL) {
				DepthBuffer->Pan(-xoffset, -yoffset, 0xFFFFu);
			}
			if (drawpass == DRAW_PASS_PAN || drawpass == DRAW_PASS_FULL) {
				AlphaBuffer->Pan(-xoffset, -yoffset, 0x7Fu);
			}
			if (drawpass == DRAW_PASS_PAN || drawpass == DRAW_PASS_FULL) {

				/*
				 * Swap the composite and tile surfaces so the freshly-panned image becomes the
				 * render target.
				 */
				Surface * oldcomp = CompositeSurface;
				CompositeSurface = TileSurface;
				TileSurface = oldcomp;
				composite = CompositeSurface;
				LogicalSurface = CompositeSurface;
			}
		} else {
			if (fullredraw && (drawpass == DRAW_PASS_PAN || drawpass == DRAW_PASS_FULL)) {
				DepthBuffer->Fill(0xFFFFu);
				AlphaBuffer->Fill(127u);
			}
		}

		if (drawpass == DRAW_PASS_PAN) {
			return;
		}

		if (!Debug_Map) {
			Register_Dirty_Buildings(Rect(TacticalRect.X - 64, TacticalRect.Y - 64, TacticalRect.Width + 128, TacticalRect.Height + 128));
		}

		Surface * prevlogical = LogicalSurface;
		LogicalSurface = TileSurface;
		LogicalSurface->Lock();

		if (drawpass == DRAW_PASS_BACKGROUND || drawpass == DRAW_PASS_FULL) {

			/*
			 * Build the two redraw strips that the pan exposed. 'xpanrect' is the full-height strip
			 * revealed by horizontal scrolling (left edge if scrolled right, right edge if scrolled
			 * left), 'ypanrect' the strip revealed by vertical scrolling. 'bounds' is the region
			 * that the blit above preserved, used to clip the depth wipe and shroud.
			 */
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
			if (scrolled_left || scrolled_right) {
				int stripx = scrolled_left ? TacticalRect.X + panwidth : TacticalRect.X;
				topstrip = Rect(stripx, TacticalRect.Y, TacticalRect.Width - panwidth, panheight);
				bottomstrip = Rect(stripx, TacticalRect.Y + TacticalRect.Height - panheight, TacticalRect.Width - panwidth, panheight);
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

			for (int index = DirtyAreas.Count() - 1; index >= 0; index--) {
				DirtyAreas.Delete_Index(index);
			}

			LogicalSurface->Unlock();
			LogicalSurface = prevlogical;
			CellRedrawCount = 0;
		}
	}

	/*
	 * Remember this frame's position for the next pan calculation.
	 */
	LastTacticalCoord.X = TacticalCoord.X;
	LastTacticalCoord.Y = TacticalCoord.Y;
	LastTacPixelX = TacPixelX;
	LastTacPixelY = TacPixelY;

	if (drawpass == DRAW_PASS_BACKGROUND || drawpass == DRAW_PASS_FULL) {
		LogicalSurface->Blit_From(TacticalRect, *TileSurface, TacticalRect);
	}

	if (drawpass == DRAW_PASS_FOREGROUND || drawpass == DRAW_PASS_FULL) {
		/*
		**	NATIVE: YR's foreground pass, read from gamemd.exe (0x006D45F2..0x006D4B37) on TibSun's
		**	shape. Not written yet: the Tactical helper at 0x006DBE20 (after the beams), the
		**	enemy units' FootClass::FUN_004DC340 lines behind the unnamed test 0x0043B150, the
		**	airstrike designator's laser (0x00705860), the planning-mode nodes (0x0063B2F0) and
		**	the Tactical helper at 0x006D7840 (TacticalRect, TacticalRect).
		*/
		Add_Buildings_To_Selectable(TacticalRect);
		composite->Lock();
		Surface * savedlogical = LogicalSurface;
		LogicalSurface = composite;
		Draw_Waypoints(false);
		Draw_Rally_Points(false);
		Draw_Placement(false);

		IonBlastClass::Draw_All();
		Draw_Objects(true);
		SpotLightClass::Draw_All();
		LaserDrawClass::Draw_All();
		EBolt::FUN_004C2830();
		LineTrail::Process_All();
		RadBeam::FUN_006591B0();
		BeaconManager.Draw(LogicalSurface, TacticalRect);

		Draw_Rubber_Band();
		Draw_Waypoints(true);
		Draw_Rally_Points(true);
		Draw_Placement(true);

		bool planning = FUN_00637AA0();
		if (planning) {
			FUN_0063B0A0();
			FUN_0063B150();
		}

		for (int i = 0; i < TechnoClass::Array.Count(); i++) {
			TechnoClass * techno = TechnoClass::Array[i];
			if (planning) {
				continue;
			}
			if (techno->House->Is_Player_Control() && techno->IsSelected && TechnoClass::ActionLines) {
				techno->Draw_Action_Line(false, 0);
			}
			if (techno->Strength) {
				if (techno->CaptureManager != NULL && techno->CaptureManager->NeedsToDrawLinks()) {
					techno->CaptureManager->DrawLinks();
				}
				if (techno->Strength && techno->MindControlledBy != NULL && techno->MindControlledBy->CaptureManager != NULL
					&& techno->MindControlledBy->CaptureManager->NeedsToDrawLinks()) {
					techno->MindControlledBy->CaptureManager->DrawLinks();
				}
			}
		}

		LogicalSurface = savedlogical;
		composite->Unlock();

		/*
		**	The countdown timers: the mission timer, the super weapons on show, the houses'
		**	power blackouts (gamemd.exe shows the remaining frames / 15).
		*/
		int index = 0;
		if (Scen->MissionTimer.Started != -1) {
			DrawTimer(index++, ColorSchemes[PlayerPtr->Scheme], Scen->MissionTimer.Value() / 15, Scen->MissionTimerTextCSF, NULL, NULL);
		}
		for (int timer = 0; timer < ShowTimers.Count(); timer++) {
			SuperClass * super = ShowTimers[timer];
			int remaining = super->Control.Value();
			if (super->IsSuspended && Session.Type == GAME_NORMAL && remaining == super->GetRechargeTime()) {
				continue;
			}
			DrawTimer(index++, ColorSchemes[super->House->Scheme], remaining / 15, super->Class->UIName, (unsigned __int64 *)&super->BlinkTimer, &super->BlinkState);
		}
		for (int h = 0; h < Houses.Count(); h++) {
			HouseClass * house = Houses[h];
			int remaining = house->PowerBlackoutTimer.Value();
			if (remaining > 0) {
				DrawTimer(index++, ColorSchemes[house->Scheme], remaining / 15, StringTable::LoadString("MSG:BlackoutTimer", NULL, "D:\\ra2mdpost\\Tactical.CPP", 0x1C4), NULL, NULL);
			}
		}

		Draw_Screen_Text(ScreenText);

		IsFirstRender = false;
		IsToRedraw = false;
	}
}
