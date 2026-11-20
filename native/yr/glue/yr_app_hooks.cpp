// The host's app hooks for yr-native (lane Y7; platform/sdl/host.cpp declares weak defaults, the
// RA2 game's strong ones are native/link/app_hooks.cpp): the touch layer's map pan and the
// settings' save, written against the YR tree's own classes.
//   ra2_native_scroll_map: a swipe over the tactical view (touch.cpp; trackpad panning on the Mac)
//     moves the view by the fingers' pixels: Tactical::Get_Tactical_Position (gamemd.exe
//     0x006D6170) and Set_Tactical_Position (0x006D6000, which clamps to the map and flags the
//     redraw), as the RA2 hook does with RA2's (0x006A67B0, 0x006A6640). Without it a swipe over
//     the map turned the wheel.
//   ra2_native_team_button_at (lane Y15): the command-bar team button (1-3) under a game pixel,
//     else 0, for the touch host's hold on a team button (platform/sdl/touch.cpp): the bar's buttons
//     (tab.cpp FUN_006CFD40, gamemd.exe's button of a command; Team01..Team03 commands
//     DAT_00B0CC20..DAT_00B0CD28) while the bar is open (TabClass::ThumbActive), as the RA2 hook.
//   ra2_native_save_settings: RA2MD.INI saved as the main menu's Exit Game does
//     (GameOptionsClass::Save_Settings).
//   yr_native_main_loop / yr_native_play_over (fixups.json, conquer.cpp's game loop): the host's
//     per-frame work and in-play mark, as the RA2 game's Main_Loop seam gives them
//     (link/harness_skirmish.cpp): ra2_enh_in_play (the touch toolbar shows in play only) and
//     ra2port_frame (the host's frame: the touch layer's held buttons and pans, the input
//     script's frame-numbered lines, the frame-rate line). Main_Loop's result is returned as is.
#include "always.h"
#include "geometry.h"
#include "tactical.h"
#include "_tactica.h"
#include "goptions.h"
#include "globals.h"
#include "mainloop.h"
#include "saveload.h"
#include "mouse.h"
#include "shapebtn.h"
#include "session.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" void ra2_enh_in_play(int on);   // platform/sdl/enhancements.cpp
extern "C" void ra2port_frame(int frame);  // port/src/run_progress.cpp
extern "C" void ra2port_progress(const char *milestone, const char *detail);   // port/src/run_progress.cpp
extern "C" int ra2port_skirmish_frames(void);                                   // the same: --frames=N

extern "C" int ra2_native_scroll_map(int, int, int dx, int dy)
{
    Tactical *tactical = TacticalMap;
    if (!tactical) return 0;
    Point2D view = tactical->Get_Tactical_Position();
    view.X += dx;
    view.Y += dy;
    tactical->Set_Tactical_Position(view);
    return 1;
}

extern MouseClass Map;
extern int DAT_00B0CC20;   // tab.cpp: the Team01 command
extern int DAT_00B0CD28;   // tab.cpp: the Team03 command
ShapeButtonClass * __fastcall FUN_006CFD40(int button);   // tab.cpp (gamemd.exe 0x006CFD40)

extern "C" int ra2_native_team_button_at(int x, int y)
{
	if (!Map.ThumbActive) return 0;
	for (int command = DAT_00B0CC20; command >= 0 && command <= DAT_00B0CD28; command++) {
		ShapeButtonClass * button = FUN_006CFD40(command);
		if (button && button->Is_Enabled() && x >= button->X && x < button->X + button->Width &&
			y >= button->Y && y < button->Y + button->Height) {
			return command - DAT_00B0CC20 + 1;
		}
	}
	return 0;
}

// Test scaffolding (lane N3): an input script's point "@cmd=<command>" is the middle of the command
// bar's button for that command (ra2port_set_point_resolver, port/src/run_progress.cpp; the host
// asks it for any point that is not "@team=N"). Known commands: the bar's buttons the tests press.
extern int DAT_00B0CB38;   // tab.cpp: the TypeSelect command
extern int DAT_00B0CB3C;   // tab.cpp: the Beacon command
extern int DAT_00B0CC1C;   // tab.cpp: the PlanningMode command
static int Resolve_Bar_Point(const char *spec, int *x, int *y)
{
	if (strncmp(spec, "cmd=", 4) || !Map.ThumbActive) return 0;
	int command = -1;
	if (!strcmp(spec + 4, "TypeSelect")) command = DAT_00B0CB38;
	else if (!strcmp(spec + 4, "Beacon")) command = DAT_00B0CB3C;
	else if (!strcmp(spec + 4, "PlanningMode")) command = DAT_00B0CC1C;
	ShapeButtonClass * button = command >= 0 ? FUN_006CFD40(command) : NULL;
	if (button == NULL || !button->Is_Enabled()) {
		fprintf(stderr, "input-script: no command bar button for %s (command %d)\n", spec, command);
		return 0;
	}
	*x = button->X + button->Width / 2;
	*y = button->Y + button->Height / 2;
	return 1;
}
extern "C" void ra2port_set_point_resolver(int (*resolver)(const char *spec, int *x, int *y));   // port/src/run_progress.cpp
static int const bar_point_registered = (ra2port_set_point_resolver(Resolve_Bar_Point), 0);

// The touch app's on-screen keyboard for the game's own text entry (lane N3; the RA2 game's hooks are
// native/link/native_text_entry.cpp, the host's weak defaults platform/sdl/host.cpp). Not original code.
//   ra2_native_new_edit_focused: Yuri's Revenge's player-name fields are a dialog's Edit that
//     OwnerDraw's FUN_0060F320 (gamemd.exe 0x0060F320) hides and replaces with a ListBox carrying a
//     NewEditLink ("NewEdit") in its GWL_USERDATA, drawn and typed into by OwnerDraw::NewEditHandler.
//     The focus is such a list box and it is shown. (The user's Duo report, 2026-11-19: the keyboard
//     did not come up in the skirmish name field; port/src/user32.cpp answered only for windows of
//     the classes Edit and NewEdit.)
//   ra2_native_message_edit_open: the edit line the game draws in the message list for chat and
//     beacon text (Session.Messages, MessageListClass::IsEdit) is open.
struct NewEditLink {   // ownrdraw.cpp (gamemd.exe 0x0060F320)
	const char *className;
	LONG style;
};
extern "C" intptr_t ra2port_get_window_ptr(const volatile void *window, int index);   // port/src/user32.cpp
extern "C" int ra2_native_new_edit_focused(void)
{
	HWND focus = GetFocus();
	if (!focus || !IsWindowVisible(focus)) return 0;
	NewEditLink const *link = (NewEditLink const *)ra2port_get_window_ptr(focus, -21);   // GWL_USERDATA
	return link && link->className && !strcmp(link->className, "NewEdit");
}
extern "C" int ra2_native_message_edit_open(void)
{
	return Session.Messages.Is_Edit() ? 1 : 0;
}

extern "C" void ra2_native_save_settings(void)
{
    Options.Save_Settings();
}

// Test harness (lane Y9; RA2's link/harness_save.cpp, test scaffolding, never game behaviour):
// RA2_NATIVE_SAVE_AT=<frame>[,<file>] saves the game at that loop frame as the options menu's
// Save ends in (Save_Game), under <file> (default SAVE0001.SAV) in the game's write folder.
static void yr_native_harness_save(int frame)
{
    static int at = -2;
    static char file[64] = "SAVE0001.SAV";
    if (at == -2) {
        at = -1;
        if (char const *env = getenv("RA2_NATIVE_SAVE_AT")) {
            at = atoi(env);
            if (char const *comma = strchr(env, ',')) snprintf(file, sizeof file, "%s", comma + 1);
        }
    }
    if (at < 0 || frame != at) return;
    bool saved = Save_Game(file, L"Native harness save", false);
    char detail[128];
    snprintf(detail, sizeof detail, "frame %d: %s %s", frame, saved ? "saved" : "save failed", file);
    fprintf(stderr, "harness-save: %s\n", detail);
    ra2port_progress("harness-save", detail);
}

void yr_native_harness_frame(int frame);

void Yr_Map_Zoom_Frame();   // glue/yr_map_zoom.cpp: a zoom gesture's step (lane N18)
void Yr_Map_Zoom_End();     // glue/yr_map_zoom.cpp: play is over (lane N21)
bool yr_native_main_loop(void)
{
    static int frames = 0;
    ra2_enh_in_play(1);
    bool over = Main_Loop();
    // Main_Loop returned true (won, lost, restarted or aborted: the game loop's end check runs next):
    // play is over and a zoomed game's last frame is cleared, as Red Alert 2 does at the same point
    // (link/native_map_zoom.cpp Native_Map_Zoom_End). Only marking play over (before lane N21) showed
    // that frame whole until the end check cleared it, or, after Abort Mission, through Do_Abort's
    // wait: the zoomed map small in the screen's top left corner (the user's Duo report, 2026-11-20).
    if (over) Yr_Map_Zoom_End();
    else Yr_Map_Zoom_Frame();
    ra2port_frame(++frames);
    yr_native_harness_save(frames);
    yr_native_harness_frame(frames);   // yr_harness.cpp: RA2_NATIVE_ORDERS, RA2_NATIVE_FORCE_END (lane Y12)
    // --run-to=frames --frames=N (lane Y10, as the RA2 game's Main_Loop seam): N frames of a game
    // started from the menus, or loaded (--load-save: the count starts with the loaded game).
    if (frames == ra2port_skirmish_frames()) {
        char detail[16];
        snprintf(detail, sizeof detail, "%d", frames);
        ra2port_progress("frames", detail);
    }
    return over;
}

void yr_native_play_over(void)
{
    ra2_enh_in_play(0);
}

// port/src/user32.cpp's question after a focus change (lane Y11, as native/link/app_hooks.cpp answers
// it for Red Alert 2): is the game's screen redraw mark still set? Yuri's Revenge's mark is the same
// SurfacesRestored (msengine.cpp sets it on WM_ACTIVATEAPP, init.cpp's menu redraw clears it).
// Without this hook the one-game app's Yuri's Revenge engine had a link-closure trap here and
// aborted whenever the app lost focus in the menus (user report 2026-11-17, the Duo's log).
extern bool SurfacesRestored;
extern "C" int ra2port_game_redraw_pending(void)
{
    return SurfacesRestored ? 1 : 0;
}

// The touch app hides the game's own cursor (lane Y11, user report 2026-11-17: a cursor showed on
// taps in Yuri's Revenge; Red Alert 2's hook is native/link/native_touch_cursor.cpp, Scout1249): one
// extra Hide_Mouse whenever the cursor is shown keeps its nesting count below zero, so the game's
// balanced hide/show pairs leave it hidden. Nothing else changes. The host calls it as it pumps events.
#include "xmouse.h"
extern Mouse *MouseCursor;      // gamemd.exe 0x00887640, WinMain's WWMouseClass
extern "C" int ra2_native_hide_game_cursor(void)
{
    if (!MouseCursor) return 0;
    if (!MouseCursor->Is_Hidden()) MouseCursor->Hide_Mouse();
    return 1;
}
