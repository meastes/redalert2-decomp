// Test harness orders for yr-native (lane Y12; RA2's link/harness_orders.cpp and harness_save.cpp's
// force-end, written against the YR tree's classes). Test scaffolding, never game behaviour: nothing
// here runs unless its environment variable is set.
//
// RA2_NATIVE_ORDERS="<frame> <order>; ..." (loop frames of yr_native_main_loop, counted over the
// process): at that frame
//   list            logs every techno as `harness-list h<house> <ID> r<rtti> s<strength> @x,y &<addr>`
//                   (p marks the player's);
//   health          logs the player's technos as `harness-health <ID> s<strength> &<addr>`;
//   shoot <ID|*>    the player's first unit of type ID attacks the player's second one (MISSION_ATTACK
//                   with it as the target: the order a force-fire click queues), so a weapon is seen at
//                   work with no enemy to find; `*` takes the first vehicle type the player has two of.
//                   Logs `harness-shoot <ID> &<attacker> &<target> s<target strength>`;
//   watch           from then on, every 50 frames, the shoot target's strength (`harness-watch s<n>`)
//                   and `harness-killed <ID> at frame <f>` once it has left the game;
//   hunt            every combat unit of the player's gets MISSION_HUNT (the AI attack teams' order);
//   deploy <ID>     the player's first unit of type ID deploys (MISSION_UNLOAD, as EventClass DEPLOY queues
//                   it: an MCV unpacks into its Construction Yard) (lane Y14);
//   move <ID> <dx> <dy>  the player's first unit of type ID moves dx,dy cells from its cell (MISSION_MOVE);
//   where <ID>      logs `harness-where <ID> px<x>,<y> &<addr>`: the player's first techno of type ID on the
//                   tactical view (window pixels of the 800x600 view, TacticalClass::Coord_To_Pixel), or
//                   `off` when not on the view;
//   rects           logs the layout (`harness-rects tactical ... commandbar ...`) and how much of the command
//                   bar's rows on the screen are drawn (`harness-rects commandbar on screen <n>% drawn`);
//   noise <frames>  for that many frames, counts in the composed tactical view (CompositeSurface, RGB565)
//                   the pixels coloured as a broken draw's noise (saturated magenta, pure green: 16-bit
//                   pixels written into the eight-bit compositing surface show as random palette entries);
//                   logs `harness-noise <n> at frame <f>` for each frame over 60 and at the end
//                   `harness-noise-max <n> over <frames> frames` (lane Y14).
// RA2_NATIVE_FORCE_END=<f1>[,<f2>...][,lose]: at the first loop frame at or past each entry the game
// ends as the house AI ends it on a win or defeat (PlayerWins / PlayerLoses, read by the main loop);
// logs `force-end: win|loss at frame <f> in <scenario>`, so a campaign goes on mission by mission.
#include "always.h"
#include "globals.h"
#include "techno.h"
#include "techtype.h"
#include "house.h"
#include "target.h"
#include "queue.h"
#include "scenario.h"
#include "mission.hh"
#include "rtti.hh"
#include "_tactica.h"
#include "_surface.h"
#include "surface.h"
#include "tactical.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Rect TacticalRect;
extern Rect VisibleRect;
extern Rect * DAT_00B0FC58;	// the command bar's area (FUN_0072FC60)
extern bool DAT_00B0FC0C;	// the command bar laid out

namespace {

struct Order { int frame; char text[64]; };
Order orders[32];
int order_count = -1;
int order_done = 0;

TechnoClass * shoot_target = NULL;
char shoot_type[32] = "";
int noise_frames = 0;
int noise_total = 0;
int noise_max = 0;
bool watching = false;
bool killed = false;

void Read_Orders(void)
{
	order_count = 0;
	char const * env = getenv("RA2_NATIVE_ORDERS");
	for (char const * p = env; p && *p && order_count < 32;) {
		while (*p == ' ' || *p == ';') ++p;
		if (!*p) break;
		Order & o = orders[order_count];
		o.frame = atoi(p);
		while (*p >= '0' && *p <= '9') ++p;
		while (*p == ' ') ++p;
		int n = 0;
		while (*p && *p != ';') { if (n < 63) o.text[n++] = *p; ++p; }
		while (n > 0 && o.text[n - 1] == ' ') --n;
		o.text[n] = 0;
		++order_count;
	}
}

char const * Type_Name(TechnoClass * techno)
{
	TechnoTypeClass const * type = techno->Techno_Type_Class();
	return(type != NULL ? type->Name() : "?");
}

bool Is_Live(TechnoClass * techno)
{
	for (int i = 0; i < Technos.Count(); ++i) {
		if (Technos[i] == techno) return(true);
	}
	return(false);
}

// The share (percent) of the command bar's rows on the screen (VisibleSurface) that are not black.
int Bar_Drawn(Rect const & bar)
{
	Surface * surface = VisibleSurface;
	if (surface == NULL || surface->Bytes_Per_Pixel() != 2 || bar.Height < 8) return(-1);
	unsigned char * base = (unsigned char *)surface->Lock();
	if (base == NULL) return(-1);
	int lit = 0, total = 0;
	for (int y = bar.Y + 2; y < bar.Y + bar.Height - 2 && y < surface->Get_Height(); ++y) {
		unsigned short const * row = (unsigned short const *)(base + (size_t)y * surface->Stride());
		for (int x = bar.X; x < bar.X + bar.Width && x < surface->Get_Width(); x += 2) {
			int r = (row[x] >> 11) << 3, g = ((row[x] >> 5) & 63) << 2, b = (row[x] & 31) << 3;
			++total;
			if (r > 24 || g > 24 || b > 24) ++lit;
		}
	}
	surface->Unlock();
	return(total ? lit * 100 / total : -1);
}

int Run_Order(int frame, char const * text)
{
	char word[4][32];
	int words = 0;
	for (char const * p = text; *p && words < 4;) {
		while (*p == ' ') ++p;
		if (!*p) break;
		int n = 0;
		while (*p && *p != ' ') { if (n < 31) word[words][n++] = *p; ++p; }
		word[words++][n] = 0;
	}
	if (words < 1) return(0);
	if (!strcmp(word[0], "list") || !strcmp(word[0], "health")) {
		bool all = !strcmp(word[0], "list");
		int listed = 0;
		for (int i = 0; i < Technos.Count(); ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == NULL || (!all && techno->House != PlayerPtr)) continue;
			Coord coord = techno->Center_Coord();
			if (all) {
				fprintf(stderr, "harness-list h%d%s %s r%d s%d @%d,%d &%p\n", techno->House ? techno->House->HeapID : -1,
					techno->House == PlayerPtr ? "p" : "", Type_Name(techno), (int)techno->Fetch_RTTI(), techno->Strength,
					coord.X / 256, coord.Y / 256, (void *)techno);
			} else {
				fprintf(stderr, "harness-health %s s%d &%p\n", Type_Name(techno), techno->Strength, (void *)techno);
			}
			++listed;
		}
		return(listed);
	}
	if (!strcmp(word[0], "shoot") && words >= 2) {
		TechnoClass * first = NULL;
		TechnoClass * second = NULL;
		bool any = !strcmp(word[1], "*");
		for (int i = 0; i < Technos.Count() && second == NULL; ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == NULL || techno->House != PlayerPtr || techno->IsInLimbo) continue;
			if (any) {
				if (techno->Fetch_RTTI() != RTTI_UNIT) continue;
				if (first != NULL && strcmp(Type_Name(first), Type_Name(techno))) continue;
				if (first == NULL) {
					// the first vehicle type the player has two of
					int same = 0;
					for (int j = i; j < Technos.Count(); ++j) {
						TechnoClass * other = Technos[j];
						if (other && other->House == PlayerPtr && !other->IsInLimbo && other->Fetch_RTTI() == RTTI_UNIT
							&& !strcmp(Type_Name(other), Type_Name(techno))) ++same;
					}
					if (same < 2) continue;
				}
			} else if (strcmp(Type_Name(techno), word[1])) {
				continue;
			}
			if (first == NULL) first = techno;
			else second = techno;
		}
		if (second == NULL) {
			fprintf(stderr, "harness-shoot: no two units of type %s\n", word[1]);
			return(-1);
		}
		TargetClass target(second);
		TargetClass none;
		Queue_Mission(TargetClass(first), MISSION_ATTACK, target, none);
		shoot_target = second;
		snprintf(shoot_type, sizeof shoot_type, "%s", Type_Name(second));
		fprintf(stderr, "harness-shoot %s &%p &%p s%d\n", shoot_type, (void *)first, (void *)second, second->Strength);
		return(1);
	}
	if (!strcmp(word[0], "watch")) {
		watching = true;
		return(1);
	}
	if (!strcmp(word[0], "hunt")) {
		int count = 0;
		for (int i = 0; i < Technos.Count(); ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == NULL || techno->House != PlayerPtr || techno->IsInLimbo) continue;
			RTTIType rtti = techno->Fetch_RTTI();
			if (rtti != RTTI_UNIT && rtti != RTTI_INFANTRY) continue;
			TargetClass none;
			TargetClass none2;
			Queue_Mission(TargetClass(techno), MISSION_HUNT, none, none2);
			++count;
		}
		return(count);
	}
	if (!strcmp(word[0], "rects")) {
		Rect bar = DAT_00B0FC58 != NULL ? *DAT_00B0FC58 : Rect(0, 0, 0, 0);
		fprintf(stderr, "harness-rects tactical %d,%d %dx%d visible %dx%d commandbar %d,%d %dx%d (%s) composite %dx%d\n",
			TacticalRect.X, TacticalRect.Y, TacticalRect.Width, TacticalRect.Height, VisibleRect.Width, VisibleRect.Height,
			bar.X, bar.Y, bar.Width, bar.Height, DAT_00B0FC0C ? "laid out" : "not laid out",
			CompositeSurface ? CompositeSurface->Get_Width() : 0, CompositeSurface ? CompositeSurface->Get_Height() : 0);
		fprintf(stderr, "harness-rects commandbar on screen %d%% drawn\n", Bar_Drawn(bar));
		return(1);
	}
	if (!strcmp(word[0], "noise") && words >= 2) {
		noise_frames = noise_total = atoi(word[1]);
		noise_max = 0;
		return(noise_frames);
	}
	if ((!strcmp(word[0], "deploy") || !strcmp(word[0], "move") || !strcmp(word[0], "where")) && words >= 2) {
		TechnoClass * found = NULL;
		for (int i = 0; i < Technos.Count() && found == NULL; ++i) {
			TechnoClass * techno = Technos[i];
			if (techno != NULL && techno->House == PlayerPtr && !techno->IsInLimbo && !strcmp(Type_Name(techno), word[1])) {
				found = techno;
			}
		}
		if (found == NULL) {
			fprintf(stderr, "harness-%s: no unit of type %s\n", word[0], word[1]);
			return(-1);
		}
		if (word[0][0] == 'w') {
			Point2D pixel;
			if (TacticalMap->Coord_To_Pixel(found->Center_Coord(), pixel)) {
				fprintf(stderr, "harness-where %s px%d,%d &%p\n", word[1], pixel.X, pixel.Y, (void *)found);
			} else {
				fprintf(stderr, "harness-where %s off &%p\n", word[1], (void *)found);
			}
			return(1);
		}
		TargetClass none;
		TargetClass none2;
		if (word[0][0] == 'd') {
			Queue_Mission(TargetClass(found), MISSION_UNLOAD, none, none2);
		} else {
			if (words < 4) return(-1);
			Coord coord = found->Center_Coord();
			Cell cell(coord.X / 256 + atoi(word[2]), coord.Y / 256 + atoi(word[3]));
			TargetClass dest(cell);
			Queue_Mission(TargetClass(found), MISSION_MOVE, none, dest);
		}
		fprintf(stderr, "harness-%s %s &%p\n", word[0], word[1], (void *)found);
		return(1);
	}
	return(-1);
}

int Noise_Pixels(Surface * surface)
{
	if (surface == NULL || surface->Bytes_Per_Pixel() != 2) return(-1);
	unsigned char * base = (unsigned char *)surface->Lock();
	if (base == NULL) return(-1);
	int count = 0;
	for (int y = 0; y < surface->Get_Height(); ++y) {
		unsigned short const * row = (unsigned short const *)(base + (size_t)y * surface->Stride());
		for (int x = 0; x < surface->Get_Width(); ++x) {
			int r = (row[x] >> 11) << 3, g = ((row[x] >> 5) & 63) << 2, b = (row[x] & 31) << 3;
			if ((r > 180 && b > 180 && g < 110) || (g > 190 && r < 110 && b < 110)) ++count;
		}
	}
	surface->Unlock();
	return(count);
}

void Force_End(int frame)
{
	static int end_at[16];
	static int end_count = -1;
	static bool end_lose = false;
	static int end_done = 0;
	if (end_count < 0) {
		end_count = 0;
		char const * env = getenv("RA2_NATIVE_FORCE_END");
		for (char const * p = env; p && *p;) {
			if (*p == 'l') end_lose = true;
			else if (*p >= '0' && *p <= '9' && end_count < 16) end_at[end_count++] = atoi(p);
			while (*p && *p != ',') ++p;
			if (*p == ',') ++p;
		}
	}
	if (end_done < end_count && frame >= end_at[end_done]) {
		++end_done;
		if (end_lose) PlayerLoses = true; else PlayerWins = true;
		fprintf(stderr, "force-end: %s at frame %d in %s\n", end_lose ? "loss" : "win", frame,
			Scen ? Scen->ScenarioName : "?");
	}
}

} // namespace

void yr_native_harness_frame(int frame)
{
	if (order_count < 0) Read_Orders();
	while (order_done < order_count && frame >= orders[order_done].frame) {
		int result = Run_Order(frame, orders[order_done].text);
		fprintf(stderr, "harness-order %d %s: %d\n", frame, orders[order_done].text, result);
		++order_done;
	}
	if (watching && !killed && shoot_target != NULL) {
		if (!Is_Live(shoot_target) || shoot_target->Strength <= 0) {
			killed = true;
			fprintf(stderr, "harness-killed %s at frame %d\n", shoot_type, frame);
		} else if (frame % 50 == 0) {
			fprintf(stderr, "harness-watch s%d at frame %d\n", shoot_target->Strength, frame);
		}
	}
	if (noise_frames > 0) {
		int n = Noise_Pixels(CompositeSurface);
		if (n > noise_max) noise_max = n;
		if (n > 60) fprintf(stderr, "harness-noise %d at frame %d\n", n, frame);
		if (--noise_frames == 0) fprintf(stderr, "harness-noise-max %d over %d frames\n", noise_max, noise_total);
	}
	Force_End(frame);
}
