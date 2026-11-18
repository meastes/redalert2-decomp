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
//   select <ID> <n> the selection becomes the player's n-th unit of type ID (0 the first; lane Y15); <n> `all`
//                   adds every one of them, ID `*` the player's fighting vehicles (lane Y16);
//   go <dx> <dy>    every selected techno of the player's moves to the cell dx,dy from the first one's (one
//                   MISSION_MOVE event each in one frame: a group's move click; lane Y16);
//   groups <ID>     logs `harness-groups <ID> <g>[*] ...`: the team (-1 none, 0 Team 1) of each of the player's
//                   units of type ID in order, * when selected (lane Y15).
//   spawn <ID> [enemy|<dx> <dy>]  a techno of type ID (a vehicle, infantry or a building) for the player, or with
//                   `enemy` for the strongest house that is not the player's ally (most buildings, not the civilians), placed on the first
//                   free cell near the player's first building (2 cells and further off, or dx,dy); logs
//                   `harness-spawn <ID> h<house> @x,y &<addr>` (lane Y16: the stand-ins' tests);
//   fire <ID> <TARGET>  the player's first unit of type ID attacks the nearest techno of type TARGET (the enemy's,
//                   else any house's) (MISSION_ATTACK: a force-fire click's order); logs `harness-fire ...`;
//   enter <ID> <TARGET> [capture|enter]  the player's first unit of type ID is sent into the nearest techno of type
//                   TARGET not the player's (MISSION_CAPTURE, a hijacker's or spy's click; or MISSION_ENTER);
//   owner <ID>      logs `harness-owner <ID> h<house> p|e &<addr>` for each techno of type ID (p: the player's);
//   waves           logs `harness-waves <n> lasers <n>`: the waves (WaveClass::Array) and lasers being drawn;
//   super <name> enemy  the player's superweapon whose type ID starts with name is granted, charged and placed at
//                   the cell of the enemy house's first building (SuperClass::Discharged, the map click's event);
//                   logs `harness-super <ID> @x,y`;
//   wave <type>     a wave of that type (0 sonic, 3 magnetron) from the player's first vehicle to its first building
//                   (else the enemy's first techno);
//   money           logs `harness-money <credits>` (the player's);
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
#include "conquer.h"
#include "houstype.h"
#include "_map.h"
#include "building.h"
#include "super.h"
#include "suprtype.h"
#include "wave.h"
#include "laser.h"
#include "objtype.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Rect TacticalRect;
extern Rect VisibleRect;
extern Rect * DAT_00B0FC58;	// the command bar's area (FUN_0072FC60)
extern bool DAT_00B0FC0C;	// the command bar laid out

namespace {

struct Order { int frame; char text[64]; };
Order orders[128];
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
	for (char const * p = env; p && *p && order_count < 128;) {
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

// `*` in select: the player's vehicles that fight (not an MCV, a harvester or a miner).
bool Is_Fighting_Vehicle(TechnoClass * techno)
{
	if (techno->Fetch_RTTI() != RTTI_UNIT) return(false);
	char const * name = Type_Name(techno);
	return(!strstr(name, "MCV") && !strstr(name, "HARV") && !strstr(name, "CMIN") && !strstr(name, "SMIN") && !strstr(name, "PCV"));
}

// The house the stand-ins' tests fight: not the player's, not allied, not defeated, the most buildings.
HouseClass * Enemy_House(void)
{
	HouseClass * best = NULL;
	for (int i = 0; i < Houses.Count(); ++i) {
		HouseClass * house = Houses[i];
		if (house == NULL || house == PlayerPtr || house->IsDefeated || (PlayerPtr && PlayerPtr->Is_Ally(house))) continue;
		if (house->Class && house->Class->IsMultiplayPassive) continue;	// the civilians
		if (best == NULL || house->Buildings.Count() > best->Buildings.Count()) best = house;
	}
	return(best);
}

TechnoClass * First_Of(char const * id, HouseClass * house, bool not_house = false)
{
	for (int i = 0; i < Technos.Count(); ++i) {
		TechnoClass * techno = Technos[i];
		if (techno == NULL || techno->IsInLimbo || strcmp(Type_Name(techno), id)) continue;
		if (house != NULL && (not_house ? techno->House == house : techno->House != house)) continue;
		return(techno);
	}
	return(NULL);
}

// spawn: the first free cell in rings around the player's first building (or first techno).
int Spawn(char const * id, bool enemy, int dx, int dy, bool offset)
{
	ObjectTypeClass const * type = ObjectTypeClass::From_Name(id);
	HouseClass * house = enemy ? Enemy_House() : PlayerPtr;
	if (type == NULL || house == NULL || PlayerPtr == NULL) {
		fprintf(stderr, "harness-spawn: no type %s or no house\n", id);
		return(-1);
	}
	Coord home;
	if (PlayerPtr->Buildings.Count() > 0) home = PlayerPtr->Buildings[0]->Center_Coord();
	else {
		TechnoClass * any = First_Of(Type_Name(Technos[0]), PlayerPtr);
		for (int i = 0; i < Technos.Count() && any == NULL; ++i) if (Technos[i] && Technos[i]->House == PlayerPtr) any = Technos[i];
		if (any == NULL) return(-1);
		home = any->Center_Coord();
	}
	ObjectClass * object = type->Create_One_Of(house);
	if (object == NULL) return(-1);
	for (int ring = offset ? 0 : 2; ring <= 12; ++ring) {
		for (int k = 0; k < 8 * (ring ? ring : 1); ++k) {
			int x = ring ? (k % (2 * ring)) - ring : 0, y = ring ? (k < 4 * ring ? -ring : ring) : 0;
			if (ring && (k / (2 * ring)) % 2) { int t = x; x = (k < 4 * ring ? ring : -ring); y = t; }
			Cell cell(home.X / 256 + (offset ? dx : 0) + x, home.Y / 256 + (offset ? dy : 0) + y);
			Coord at(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
			at.Z = Map.Get_Height_GL(at);
			if (object->Unlimbo(at, DIR_N)) {
				fprintf(stderr, "harness-spawn %s h%d @%d,%d &%p\n", id, house->HeapID, cell.X, cell.Y, (void *)object);
				return(1);
			}
		}
	}
	fprintf(stderr, "harness-spawn: no room for %s\n", id);
	delete object;
	return(-1);
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
	if (!strcmp(word[0], "spawn") && words >= 2) {
		bool enemy = words >= 3 && !strcmp(word[2], "enemy");
		bool offset = words >= 4;
		return(Spawn(word[1], enemy, offset ? atoi(word[2]) : 0, offset ? atoi(word[3]) : 0, offset));
	}
	if ((!strcmp(word[0], "fire") || !strcmp(word[0], "enter")) && words >= 3) {
		TechnoClass * unit = First_Of(word[1], PlayerPtr);
		bool enter = word[0][0] == 'e';
		// the nearest techno of type TARGET: the enemy's, else any not the player's, else (fire) the player's own
		TechnoClass * target = NULL;
		HouseClass * enemy = Enemy_House();
		for (int pass = 0; pass < 3 && target == NULL && unit != NULL; ++pass) {
			int best = 0;
			for (int i = 0; i < Technos.Count(); ++i) {
				TechnoClass * techno = Technos[i];
				if (techno == NULL || techno == unit || techno->IsInLimbo || strcmp(Type_Name(techno), word[2])) continue;
				if (pass == 0 ? techno->House != enemy : pass == 1 ? techno->House == PlayerPtr : enter) continue;
				Coord a = techno->Center_Coord(), b = unit->Center_Coord();
				int d = abs(a.X - b.X) + abs(a.Y - b.Y);
				if (target == NULL || d < best) target = techno, best = d;
			}
		}
		if (unit == NULL || target == NULL || unit == target) {
			fprintf(stderr, "harness-%s: no %s or no target %s\n", word[0], word[1], word[2]);
			return(-1);
		}
		MissionType mission = !enter ? MISSION_ATTACK : (words >= 4 && !strcmp(word[3], "enter")) ? MISSION_ENTER : MISSION_CAPTURE;
		TargetClass who(unit);
		TargetClass tarcom(target);
		TargetClass navcom = enter ? TargetClass(target) : TargetClass();
		Queue_Mission(who, mission, tarcom, navcom);
		fprintf(stderr, "harness-%s %s &%p -> %s h%d &%p\n", word[0], word[1], (void *)unit, word[2],
			target->House ? target->House->HeapID : -1, (void *)target);
		return(1);
	}
	if (!strcmp(word[0], "owner") && words >= 2) {
		int n = 0;
		for (int i = 0; i < Technos.Count(); ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == NULL || strcmp(Type_Name(techno), word[1])) continue;
			fprintf(stderr, "harness-owner %s h%d %s%s &%p\n", word[1], techno->House ? techno->House->HeapID : -1,
				techno->House == PlayerPtr ? "p" : "e", techno->IsInLimbo ? " limbo" : "", (void *)techno);
			++n;
		}
		if (!n) fprintf(stderr, "harness-owner %s none\n", word[1]);
		return(n ? n : 1);
	}
	if (!strcmp(word[0], "waves")) {
		fprintf(stderr, "harness-waves %d lasers %d\n", WaveClass::Array.Count(), LaserDrawClass::LaserDraws.Count());
		return(1);
	}
	if (!strcmp(word[0], "wave") && words >= 2) {
		// a wave of the given type (0 sonic, 1 big laser, 2 laser, 3 magnetron) from the player's first vehicle
		// to its first building (else the enemy's first techno), the vehicle's target set to the building as a firing unit's is
		HouseClass * enemy = Enemy_House();
		TechnoClass * building = PlayerPtr && PlayerPtr->Buildings.Count() ? (TechnoClass *)PlayerPtr->Buildings[0] : NULL;
		for (int i = 0; i < Technos.Count() && building == NULL; ++i) {
			if (Technos[i] && Technos[i]->House == enemy && !Technos[i]->IsInLimbo) building = Technos[i];
		}
		if (PlayerPtr == NULL || building == NULL) return(-1);
		TechnoClass * unit = NULL;
		for (int i = 0; i < Technos.Count() && unit == NULL; ++i) {
			if (Technos[i] && Technos[i]->House == PlayerPtr && !Technos[i]->IsInLimbo && Technos[i]->Fetch_RTTI() == RTTI_UNIT) unit = Technos[i];
		}
		if (unit == NULL) return(-1);
		unit->TarCom = building;
		new WaveClass(unit->Center_Coord(), building->Center_Coord(), unit, (WaveType)atoi(word[1]), building);
		fprintf(stderr, "harness-wave %d %s -> %s\n", atoi(word[1]), Type_Name(unit), Type_Name(building));
		return(1);
	}
	if (!strcmp(word[0], "go") && words >= 3) {
		// every selected techno of the player's moves to the cell dx,dy from the first one's (MISSION_MOVE, one
		// event each in the same frame: a group's move click)
		TechnoClass * lead = NULL;
		int count = 0;
		for (int i = 0; i < CurrentObject.Count(); ++i) {
			ObjectClass * object = CurrentObject[i];
			if (object == NULL || !object->Is_Techno() || ((TechnoClass *)object)->House != PlayerPtr) continue;
			TechnoClass * techno = (TechnoClass *)object;
			if (lead == NULL) lead = techno;
			Coord coord = lead->Center_Coord();
			Cell cell(coord.X / 256 + atoi(word[1]), coord.Y / 256 + atoi(word[2]));
			TargetClass who(techno), none, dest(cell);
			Queue_Mission(who, MISSION_MOVE, none, dest);
			++count;
		}
		fprintf(stderr, "harness-go %d units %s,%s\n", count, word[1], word[2]);
		return(count ? count : -1);
	}
	if (!strcmp(word[0], "money")) {
		fprintf(stderr, "harness-money %d\n", PlayerPtr ? PlayerPtr->Available_Money() : -1);
		return(1);
	}
	if (!strcmp(word[0], "super") && words >= 3) {
		HouseClass * enemy = Enemy_House();
		if (PlayerPtr == NULL || enemy == NULL || enemy->Buildings.Count() == 0) return(-1);
		SuperClass * super = NULL;
		for (int i = 0; i < PlayerPtr->SuperWeapon.Count() && super == NULL; ++i) {
			SuperClass * candidate = PlayerPtr->SuperWeapon[i];
			if (candidate && candidate->Class && !strncmp(candidate->Class->Name(), word[1], strlen(word[1]))) super = candidate;
		}
		if (super == NULL) {
			fprintf(stderr, "harness-super: the player has no %s\n", word[1]);
			return(-1);
		}
		super->Grant(true, false, false);
		super->Forced_Charge(true);
		Cell cell = enemy->Buildings[0]->Center_Coord().As_Cell();
		super->Discharged(true, cell);
		fprintf(stderr, "harness-super %s @%d,%d\n", super->Class->Name(), cell.X, cell.Y);
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
	if ((!strcmp(word[0], "select") && words >= 3) || (!strcmp(word[0], "groups") && words >= 2)) {
		bool select = word[0][0] == 's';
		bool every = select && !strcmp(word[2], "all");
		int wanted = select ? atoi(word[2]) : -1, n = 0;
		char line[512];
		int at = snprintf(line, sizeof(line), "harness-groups %s", word[1]);
		if (select) Unselect_All();
		for (int i = 0; i < Technos.Count(); ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == NULL || techno->House != PlayerPtr || techno->IsInLimbo) continue;
			if (!strcmp(word[1], "*") ? !Is_Fighting_Vehicle(techno) : strcmp(Type_Name(techno), word[1]) != 0) continue;
			if (every) {
				techno->Select();
				++n;
				continue;
			}
			if (select && n == wanted) {
				techno->Select();
				fprintf(stderr, "harness-select %s %d &%p\n", word[1], wanted, (void *)techno);
				return(1);
			}
			if (!select && at < (int)sizeof(line) - 16) at += snprintf(line + at, sizeof(line) - at, " %d%s", techno->Group, techno->IsSelected ? "*" : "");
			++n;
		}
		if (every && n) {
			fprintf(stderr, "harness-select %s all: %d\n", word[1], n);
			return(n);
		}
		if (select) {
			fprintf(stderr, "harness-select: no unit %d of type %s\n", wanted, word[1]);
			return(-1);
		}
		fprintf(stderr, "%s\n", line);
		return(1);
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
