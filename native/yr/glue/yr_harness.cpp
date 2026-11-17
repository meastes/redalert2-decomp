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
//   hunt            every combat unit of the player's gets MISSION_HUNT (the AI attack teams' order).
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace {

struct Order { int frame; char text[64]; };
Order orders[32];
int order_count = -1;
int order_done = 0;

TechnoClass * shoot_target = NULL;
char shoot_type[32] = "";
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
	return(-1);
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
	Force_End(frame);
}
