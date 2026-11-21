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
//   deploy <ID>     the unit of type ID the last `spawn`/`deploy` of the type named (lane N16), else the player's
//                   first, deploys (MISSION_UNLOAD, as EventClass DEPLOY queues it: an MCV unpacks into its
//                   Construction Yard) (lane Y14);
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
//   spawn <ID> [enemy] [<dx> <dy>]  a techno of type ID (a vehicle, infantry or a building) for the player, or with
//                   `enemy` for the strongest house that is not the player's ally (most buildings, not the civilians), placed on the first
//                   free cell near the player's first building (2 cells and further off, or dx,dy); logs
//                   `harness-spawn <ID> h<house> @x,y &<addr>` (lane Y16: the stand-ins' tests); `spawn <ID> [enemy]
//                   near <OTHER> <dx> <dy>` places it from the player's first techno of type OTHER instead (lane N6);
//   fire <ID> <TARGET>  the player's first unit of type ID attacks the nearest techno of type TARGET (the enemy's,
//                   else any house's) (MISSION_ATTACK: a force-fire click's order); logs `harness-fire ...`;
//   enter <ID> <TARGET> [capture|enter|hijack]  the player's first unit of type ID is sent into the nearest techno of
//                   type TARGET not the player's (MISSION_CAPTURE, a spy's click; or MISSION_ENTER; hijack: MISSION_CAPTURE
//                   with the vehicle as the destination only, a vehicle thief's click);
//   owner <ID>      logs `harness-owner <ID> h<house> p|e &<addr>` for each techno of type ID (p: the player's);
//   waves           logs `harness-waves <n> lasers <n>`: the waves (WaveClass::Array) and lasers being drawn;
//   super <name> enemy  the player's superweapon whose type ID starts with name is granted, charged and placed at
//                   the cell of the enemy house's first building (SuperClass::Discharged, the map click's event);
//                   logs `harness-super <ID> @x,y`;
//   wave <type>     a wave of that type (0 sonic, 3 magnetron) from the player's first vehicle to its first building
//                   (else the enemy's first techno);
//   money           logs `harness-money <credits>` (the player's);
//   thief <ID>      infantry type ID becomes a vehicle thief (InfantryTypeClass::IsThief and IsVehicleThief: no type in
//                   Yuri's Revenge's rules sets Thief= or VehicleThief=, so a hijack is only seen this way); logs
//                   `harness-thief <ID>` (lane P1);
//   create <BLDG> on <ID>  building type BLDG for the player placed with BuildingClass::ForceCreate on the cell of the
//                   player's first unit of type ID, as the trigger action "create building at" does (Create_One_Of,
//                   ForceCreate, Grand_Opening); the units there are moved aside; logs `harness-create <BLDG> ok|failed
//                   @x,y` (lane N3);
//   bolt enemy      a lightning bolt (IonStormClass::Lightning_Bolt, the trigger action "ion lightning strike") on the
//                   cell of the enemy house's first building; logs `harness-bolt @x,y` (lane N3);
//   blowhut         the first bridge repair hut is blown as C4 blows it: its bridge is destroyed (lane N3);
//   repairhut <ID>  an engineer of type ID placed next to the first bridge repair hut is sent into it (an engineer's
//                   click on the hut); logs `harness-repairhut <ID> @x,y bridge <0|1>` (lane N3);
//   bridge          logs `harness-bridge <0|1>`: the first hut's bridge needs repair (MapClass::Can_Repair_Bridge; lane N3);
//   produce <FACTORY> <ID>  a unit of type ID for the player leaves the player's first building of type FACTORY as
//                   finished production does (BuildingClass::Exit_Object; a war factory unloads it with
//                   Do_MISSION_UNLOAD); logs `harness-produce <FACTORY> <ID> exit <result> &<addr>` (lane N3);
//   produced        logs `harness-produced <ID> cell x,y factory x,y limbo <0|1> mission <m> factory-mission <m>` for
//                   the last unit `produce` made (lane N3);
//   mission <ID>    logs `harness-mission <ID> <mission> &<addr>` for the player's first techno of type ID (lane N3);
//   plan <dx> <dy>  every selected techno of the player's is ordered to the cell dx,dy from the first one's as a map
//                   click orders it (Player_Assign_Mission): in planning mode a planned waypoint (lane N3);
//   plans           logs `harness-plans mode <0|1> tokens <n> nodes <n>`: planning mode on or off, the planning tokens
//                   and the planned nodes (lane N3);
//   centermap       centres the view on the selection (DisplayClass::Center_Map) (lane N23);
//   planloop        logs `harness-planloop picked <i> hover <i> refused <0|1> loop <start> <count> path <n>`: the planned
//                   nodes picked up and hovered with the mouse, a refusal shown this frame, the loop and the nodes
//                   of the first node's first member's path; then each node's place and FUN_0063AC70's verdict
//                   (lane N23);
//   beaconmode      the beacon placement mode as the Beacon command gives it (FUN_00731A30: the command bar's
//                   button in network games, the key; a skirmish's bar has no Beacon button); logs
//                   `harness-beaconmode on|off` (lane N3);
//   beacons         logs `harness-beacons <n>`: the beacons placed (BeaconManagerClass::AllocatedCount; lane N3);
//   stormstart      starts a lightning storm on the enemy house's first building now, as the trigger action "ion storm
//                   start" does with no deferment (LightningStorm::Start, FUN_00539EB0); logs `harness-stormstart @x,y
//                   active|not active` (lane N3);
//   stormend        ends a running lightning storm as the trigger action "ion storm stop" (only while one is active);
//                   logs `harness-stormend active|none` (lane N3);
//   cmd <name>      executes the keyboard command with that unique name (AllCommands; key 0, a press without
//                   Shift); logs `harness-cmd <name>` (lane N3: HealthNav, VeterancyNav);
//   occupiable      logs the building types infantry can garrison (`harness-occupiable <ID> max <n>`; lane N4);
//   click <ID> <TARGET>  the player's first unit of type ID clicked onto the nearest techno of type TARGET (What_Action,
//                   Active_Click_With: the mouse click's order); logs `harness-click <ID> action <n> -> <TARGET> ok|refused`;
//   camo <ID> <frames>  for that many frames, while the first techno of type ID shows the player its disguise
//                   (IsDisguised, not IsClearlyVisibleTo), counts leaf-green and blue/white pixels in a box above it in
//                   the composed view; logs `harness-camo seen <frames> green <avg> blue <avg>` (lane N4); and
//                   compares the second disguise cycle's morph (the fade out, the tree, the fade in) with the first's,
//                   frame by frame: `harness-camo-morph compared <n> differ <max> at frame <f>` (lane N12;
//                   RA2_NATIVE_CAMO_DUMP=<dir> writes the boxes as PPM files);
//   disguise <ID>   logs `harness-disguise <ID> <0|1> as <TYPE> r<rtti> clear <0|1>` (lane N4);
//   banims <BLDG>   logs `harness-banim <BLDG> <slot> <anim> live <0|1> ... layer <n> in-layer <n> ...` for each animation
//                   of the first building of type BLDG: the type's animations and the playing ones (in-layer: the
//                   display layer it is drawn from, -1 none) (lane N4);
//   banimhide <BLDG> <slot> [show]  hides (shows) that animation of every building of type BLDG (lane N4);
//   scenario        logs `harness-scenario <file>` (lane N4);
//   artkey <section> <key>  logs the value as ART(MD).INI gave it, and as a layer name (lane N4);
//   lights [add]    logs `harness-lights <n>`: the combat lights alive (SpotLights); with `add`, first a combat light
//                   at the player's first techno (a SpotLightClass, as Combat_Lighting makes for a bright warhead;
//                   lane N5: Delete_All_Objects deletes the living ones when a game is loaded or a new one starts);
//   occupants <BLDG>  logs `harness-occupants <BLDG> h<house> <n>[p]`: the garrison of each building of type BLDG;
// "+<n>[/<p>] <order>" (lane N25, the mission soak): n game frames (Unsorted::CurrentFrame) into each scenario, then
// every p frames; a new scenario starts the count again.
// RA2_NATIVE_SOAK=<period> and the rest of the mission soak (lane N25; RA2's link/harness_soak.cpp, the same lines):
//   soak-mission <file> loop <n> triggers <live> types <n>; soak-trigger <frame> gone|new <ID> <name>;
//   soak-trigger <frame> tripped <ID> <bits>; soak-timer <frame> started|stopped <left>; soak-house <frame> <i>:<name>
//   defeated; soak-digest <frame> <hash> technos <n> player <n> credits <n> rng <hash> every <period> game frames;
//   RA2_NATIVE_SOAK_DUMP=<from>[-<to>] every techno at those frames; RA2_NATIVE_SOAK_SEED=<n> the custom seed
//   (DAT_00A8ED98, which Game::InitRandom 0x0052FC20 then takes instead of GetTickCount; set by a static
//   initialiser, which runs after the original data, glue/yr_host_glue.cpp); RA2_NATIVE_SOAK_SPEED=<0-6> a campaign's
//   frame delay (Main_Loop keeps it at 2 ticks unless DAT_00A8EDDC, -SPEEDCONTROL's flag, is set: the harness sets it and
//   Options.GameSpeed); RA2_NATIVE_SOAK_END=<frames>[@<n>] ends the run (milestone soak-end) when the n-th scenario
//   reaches that frame, or is won or lost.
// RA2_NATIVE_FORCE_END=<f1>[,<f2>...][,lose]: at the first loop frame at or past each entry the game
// ends as the house AI ends it on a win or defeat (PlayerWins / PlayerLoses, read by the main loop);
// logs `force-end: win|loss at frame <f> in <scenario>`, so a campaign goes on mission by mission. With
// `flag` (lane N21) the player's house is flagged instead (HouseClass::Flag_To_Lose / Flag_To_Win, as the
// house AI flags a defeat or a victory): the end-of-game banner (GRFXTXT.SHP) and EVA, then the end after
// the savour delay.
#include "always.h"
#include "globals.h"
#include "techno.h"
#include "techtype.h"
#include "house.h"
#include "target.h"
#include "queue.h"
#include "scenario.h"
#include "trigger.h"
#include "trigtype.h"
#include "options.h"
#include "goptions.h"
#include "mission.hh"
#include "rtti.hh"
#include "_tactica.h"
#include "_surface.h"
#include "surface.h"
#include "tactical.h"
#include "conquer.h"
#include "houstype.h"
#include "_map.h"
#include "display.h"
#include "isotype.h"
#include "cell.h"
#include "ccini.h"
extern CCINIClass ArtINI;
namespace Unsorted { extern int CurrentFrame; }
LayerType Layer_From_Name(char const * name);
#include "building.h"
#include "super.h"
#include "suprtype.h"
#include "wave.h"
#include "laser.h"
#include "objtype.h"
#include "infatype.h"
#include "infantry.h"
#include "unit.h"
#include "ion.h"
#include "command.h"
#include "_command.h"
#include "builtype.h"
#include "beacon.h"
#include "planning.h"
#include "ovrlight.h"
#include "cell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Rect TacticalRect;
extern PlanningNodeClass * DAT_00AC4CCC;   // planmgr.cpp: the planned node under the mouse
extern PlanningNodeClass * DAT_00AC4C38;   // planmgr.cpp: the planned node picked up with the mouse
extern bool DAT_00AC4C08;                  // planmgr.cpp: a planning refusal was shown
int __fastcall FUN_0063AC70(PlanningNodeClass * node);   // planmgr.cpp: whether the selection's paths can take the node

// Test scaffolding (lane N23): an input script's point "@node=N" is the N-th planned node's lead on the screen
// (glue/yr_app_hooks.cpp's resolver asks it).
extern "C" int yr_harness_node_point(int n, int * x, int * y)
{
	if (n < 0 || n >= PlanningNodeClass::Unknown1.Count()) {
		fprintf(stderr, "input-script: no planned node %d (%d planned)\n", n, PlanningNodeClass::Unknown1.Count());
		return(0);
	}
	Point2D pixel = PlanningNodeClass::Unknown1[n]->Lead_Pixel();
	*x = pixel.X + TacticalRect.X;
	*y = pixel.Y + TacticalRect.Y;
	return(1);
}
extern Rect VisibleRect;
extern Rect * DAT_00B0FC58;	// the command bar's area (FUN_0072FC60)
extern bool DAT_00B0FC0C;	// the command bar laid out

extern BeaconManagerClass BeaconManager;   // beacon.cpp
void FUN_00731A30(void);   // uicmnds.cpp: the Beacon command (beacon placement mode on or off)
void __fastcall FUN_00539EB0(int duration, int deferment, Cell cell, HouseClass * owner);   // ion.cpp: LightningStorm::Start

namespace {

struct Order { int frame; char text[64]; };
Order orders[128];
int order_count = -1;
int order_done = 0;
// "+<n>[/<p>]" orders (lane N25): run at game frame n of each scenario, then every p frames
struct Relative { int at; int every; int due; char text[64]; };
Relative relative[32];
int relative_count = 0;

TechnoClass * shoot_target = NULL;
TechnoClass * produced = NULL;		// produce: the last unit made and its factory (lane N3)
TechnoClass * produced_from = NULL;
char shoot_type[32] = "";
int noise_frames = 0;
int noise_total = 0;
int noise_max = 0;
TechnoClass * camo_unit = NULL;	// camo: the disguised unit watched, frames left, frames seen as its disguise
int camo_frames = 0;
int camo_seen = 0;
int camo_green = 0;
int camo_blue = 0;
// camo, lane N12: the owner sees the disguise come and go every 256 frames (TechnoClass::IsClearlyVisibleTo and
// GetDisguiseFlags: the tank fades out, the tree, the tank fades in). The box around the unit on each frame of the
// first cycle's morph (phases 64-127) is kept and the second cycle's same frames are compared with it: a still
// unit's morph must look the same every time (the user's Duo fault: corrupted only the first time).
int const kCamoBoxW = 100, kCamoBoxH = 110;
unsigned short * camo_first = NULL;	// [64][kCamoBoxH * kCamoBoxW]
bool camo_kept[64];
unsigned camo_created = 0;
int camo_compared = 0;
int camo_diff_max = 0;
int camo_diff_frame = -1;
bool watching = false;
bool killed = false;

void Read_Orders(void)
{
	order_count = 0;
	char const * env = getenv("RA2_NATIVE_ORDERS");
	for (char const * p = env; p && *p && order_count < 128;) {
		while (*p == ' ' || *p == ';') ++p;
		if (!*p) break;
		if (*p == '+') {
			Relative & r = relative[relative_count < 32 ? relative_count : 31];
			r.at = atoi(p + 1);
			r.every = 0;
			r.due = -1;
			++p;
			while ((*p >= '0' && *p <= '9') || *p == '/') {
				if (*p == '/') r.every = atoi(p + 1);
				++p;
			}
			while (*p == ' ') ++p;
			int n = 0;
			while (*p && *p != ';') { if (n < 63) r.text[n++] = *p; ++p; }
			while (n > 0 && r.text[n - 1] == ' ') --n;
			r.text[n] = 0;
			if (relative_count < 32) ++relative_count;
			continue;
		}
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

// deploy: the techno each type's last `spawn` (the player's) or `deploy` named, for `deploy` and `doing` (lane N13;
// lane N16: the spawned one, not the player's first of the type, which in a skirmish is a starting soldier)
enum { kDeployKept = 16 };
char deploy_type[kDeployKept][32];
TechnoClass * deploy_last[kDeployKept];
int deploy_count = 0;

void Track(char const * id, TechnoClass * techno)
{
	int k = 0;
	while (k < deploy_count && strcmp(deploy_type[k], id)) ++k;
	if (k >= kDeployKept) return;
	snprintf(deploy_type[k], sizeof(deploy_type[k]), "%s", id);
	deploy_last[k] = techno;
	if (k == deploy_count) ++deploy_count;
}

// the techno Track last named for type ID; `*gone` when there is one but it has left the map (killed, or in limbo)
TechnoClass * Tracked(char const * id, bool * gone)
{
	*gone = false;
	for (int k = 0; k < deploy_count; ++k) {
		if (strcmp(deploy_type[k], id)) continue;
		// the same object only if it is still the player's of that type: a freed unit's memory may hold a new object
		// (lane N25: a mission soak's tracked MCV of an earlier mission came back as an enemy submarine and was deployed)
		for (int i = 0; i < Technos.Count(); ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == deploy_last[k] && !techno->IsInLimbo && techno->House == PlayerPtr && !strcmp(Type_Name(techno), id)) {
				return(techno);
			}
		}
		*gone = true;
	}
	return(NULL);
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
int Spawn(char const * id, bool enemy, int dx, int dy, bool offset, char const * beside = NULL)
{
	ObjectTypeClass const * type = ObjectTypeClass::From_Name(id);
	HouseClass * house = enemy ? Enemy_House() : PlayerPtr;
	if (type == NULL || house == NULL || PlayerPtr == NULL) {
		fprintf(stderr, "harness-spawn: no type %s or no house\n", id);
		return(-1);
	}
	Coord home;
	TechnoClass * anchor = beside ? First_Of(beside, PlayerPtr) : NULL;
	if (beside && anchor == NULL) {
		fprintf(stderr, "harness-spawn: no %s of the player's to place %s near\n", beside, id);
		return(-1);
	}
	if (anchor) home = anchor->Center_Coord();
	else if (PlayerPtr->Buildings.Count() > 0) home = PlayerPtr->Buildings[0]->Center_Coord();
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
				if (!enemy && object->Is_Techno()) Track(id, (TechnoClass *)object);
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
	char word[8][32];		// lane N6: 8 words (was 4: `spawn <ID> enemy <dx> <dy>` lost its offset)
	int words = 0;
	for (char const * p = text; *p && words < 8;) {
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
		int first = enemy ? 3 : 2;		// `spawn <ID> enemy <dx> <dy>`: the enemy's, placed at the offset (lane P1)
		char const * beside = NULL;		// `... near <OTHER> <dx> <dy>`: the offset from the player's first OTHER (lane N6)
		if (words >= first + 2 && !strcmp(word[first], "near")) {
			beside = word[first + 1];
			first += 2;
		}
		bool offset = words >= first + 2;
		return(Spawn(word[1], enemy, offset ? atoi(word[first]) : 0, offset ? atoi(word[first + 1]) : 0, offset, beside));
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
		// hijack: the capture click's assignment (FootClass::Active_Click_With: MISSION_CAPTURE, no target, the
		// vehicle as the destination; a target as well would keep it a firing target)
		bool hijack = enter && words >= 4 && !strcmp(word[3], "hijack");
		TargetClass tarcom = hijack ? TargetClass() : TargetClass(target);
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
			Cell cell = techno->Center_Coord().As_Cell();	// where it stands (lane N6: the hijack check's diagnosis)
			fprintf(stderr, "harness-owner %s h%d %s%s &%p @%d,%d m%d\n", word[1], techno->House ? techno->House->HeapID : -1,
				techno->House == PlayerPtr ? "p" : "e", techno->IsInLimbo ? " limbo" : "", (void *)techno, cell.X, cell.Y,
				(int)techno->CurrentMission);
			++n;
		}
		if (!n) fprintf(stderr, "harness-owner %s none\n", word[1]);
		return(n ? n : 1);
	}
	if (!strcmp(word[0], "lights")) {
		if (words >= 2 && !strcmp(word[1], "add") && PlayerPtr != NULL) {
			for (int i = 0; i < Technos.Count(); ++i) {
				if (Technos[i] && Technos[i]->House == PlayerPtr && !Technos[i]->IsInLimbo) {
					new SpotLightClass(Technos[i]->Center_Coord(), 63);	// what Combat_Lighting makes for a bright warhead
					break;
				}
			}
		}
		fprintf(stderr, "harness-lights %d\n", SpotLights.Count());
		return(1);
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
	if (!strcmp(word[0], "blowhut")) {
		// the first bridge repair hut (BuildingTypeClass::IsBridgeRepairHut) is blown as C4 blows it (IsGoingToBlow with
		// the count-down run out): its next AI destroys the bridge beside it (DestroyWoodBridgeAt / DestroyConcreteBridgeAt)
		for (int i = 0; i < Buildings.Count(); ++i) {
			BuildingClass * hut = Buildings[i];
			if (hut == NULL || hut->IsInLimbo || !hut->Class->IsBridgeRepairHut) continue;
			hut->IsGoingToBlow = true;
			hut->CountDown = 0;
			Cell cell = hut->Get_Cell();
			fprintf(stderr, "harness-blowhut %s @%d,%d &%p\n", Type_Name(hut), cell.X, cell.Y, (void *)hut);
			return(1);
		}
		fprintf(stderr, "harness-blowhut: no bridge repair hut\n");
		return(-1);
	}
	if (!strcmp(word[0], "repairhut") && words >= 2) {
		// an engineer of type word[1] for the player is placed next to the first bridge repair hut and sent into it
		// (MISSION_CAPTURE with the hut as the destination: an engineer's click on a hut); logs `harness-repairhut
		// <ID> @x,y bridge <0|1>`, the second the hut's cell can repair a bridge (MapClass::Can_Repair_Bridge)
		for (int i = 0; i < Buildings.Count(); ++i) {
			BuildingClass * hut = Buildings[i];
			if (hut == NULL || hut->IsInLimbo || !hut->Class->IsBridgeRepairHut) continue;
			ObjectTypeClass const * type = ObjectTypeClass::From_Name(word[1]);
			TechnoClass * engineer = type ? (TechnoClass *)type->Create_One_Of(PlayerPtr) : NULL;
			if (engineer == NULL) return(-1);
			Cell home = hut->Get_Cell();
			bool placed = false;
			for (int ring = 1; ring <= 4 && !placed; ++ring) {
				for (int dy = -ring; dy <= ring && !placed; ++dy) {
					for (int dx = -ring; dx <= ring && !placed; ++dx) {
						Cell cell(home.X + dx, home.Y + dy);
						Coord at(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
						at.Z = Map.Get_Height_GL(at);
						placed = engineer->Unlimbo(at, DIR_N);
					}
				}
			}
			if (!placed) { delete engineer; return(-1); }
			TargetClass who(engineer), none, dest(hut);
			Queue_Mission(who, MISSION_CAPTURE, none, dest);
			Cell at = engineer->Get_Cell();
			fprintf(stderr, "harness-repairhut %s @%d,%d bridge %d\n", word[1], at.X, at.Y, Map.Can_Repair_Bridge(home) ? 1 : 0);
			return(1);
		}
		fprintf(stderr, "harness-repairhut: no bridge repair hut\n");
		return(-1);
	}
	if (!strcmp(word[0], "bridgecells")) {
		// logs the 5x5 cells around the first bridge repair hut: `harness-bridgecell x,y it <ITType> sub <n> ov <Overlay>
		// [bridge|trainbridge]` (lane N4)
		for (int i = 0; i < Buildings.Count(); ++i) {
			BuildingClass * hut = Buildings[i];
			if (hut == NULL || hut->IsInLimbo || !hut->Class->IsBridgeRepairHut) continue;
			Cell home = hut->Get_Cell();
			for (int y = -3; y < 4; ++y) for (int x = -3; x < 6; ++x) {
				CellClass & c = Map[Cell(home.X + x, home.Y + y)];
				int it = c.ITType;
				fprintf(stderr, "harness-bridgecell %d,%d it %d sub %d ov %d under %d was %d%s\n", home.X + x, home.Y + y, it, (int)c.SubTile, (int)c.Overlay,
					c.IsUnderBridge ? 1 : 0, c.WasUnderBridge ? 1 : 0,
					it >= IsometricTileTypeClass::BridgeSet && it < IsometricTileTypeClass::BridgeSet + 16 ? " bridge" :
					it >= IsometricTileTypeClass::TrainBridgeSet && it < IsometricTileTypeClass::TrainBridgeSet + 16 ? " trainbridge" : "");
			}
			for (int k = 0; k < Map.ZoneConnections.Count(); ++k) {
				ZoneConnectionClass & z = Map.ZoneConnections[k];
				if (abs(z.From.X - home.X) > 999 || abs(z.From.Y - home.Y) > 999) continue;
				fprintf(stderr, "harness-zonecon %d,%d -> %d,%d passable %d type %d\n", z.From.X, z.From.Y, z.To.X, z.To.Y,
					z.IsPassable ? 1 : 0, (int)z.Type);
				int dx = z.To.X > z.From.X ? 1 : z.To.X < z.From.X ? -1 : 0, dy = z.To.Y > z.From.Y ? 1 : z.To.Y < z.From.Y ? -1 : 0;
				for (Cell c = z.From; c.X != z.To.X + dx || c.Y != z.To.Y + dy; c.X += dx, c.Y += dy) {
					CellClass & cc = Map[c];
					fprintf(stderr, "harness-span %d,%d under %d ov %d it %d sub %d\n", c.X, c.Y, cc.IsUnderBridge ? 1 : 0, (int)cc.Overlay, (int)cc.ITType, (int)cc.SubTile);
					if (!dx && !dy) break;
				}
			}
			return(1);
		}
		return(-1);
	}
	if (!strcmp(word[0], "bridge")) {
		// logs `harness-bridge <0|1>`: the first bridge repair hut's bridge needs repair (MapClass::Can_Repair_Bridge)
		for (int i = 0; i < Buildings.Count(); ++i) {
			BuildingClass * hut = Buildings[i];
			if (hut == NULL || hut->IsInLimbo || !hut->Class->IsBridgeRepairHut) continue;
			int broken = Map.Can_Repair_Bridge(hut->Get_Cell()) ? 1 : 0;
			fprintf(stderr, "harness-bridge %d\n", broken);
			return(1);
		}
		fprintf(stderr, "harness-bridge: no bridge repair hut\n");
		return(-1);
	}
	if (!strcmp(word[0], "produce") && words >= 3) {
		// a unit of type word[2] for the player leaves the player's first building of type word[1] as finished
		// production leaves a factory (BuildingClass::Exit_Object: a war factory takes the vehicle in and unloads it,
		// BuildingClass::Do_MISSION_UNLOAD)
		TechnoClass * factory = First_Of(word[1], PlayerPtr);
		ObjectTypeClass const * type = ObjectTypeClass::From_Name(word[2]);
		if (factory == NULL || factory->Fetch_RTTI() != RTTI_BUILDING || type == NULL) {
			fprintf(stderr, "harness-produce: no %s or no type %s\n", word[1], word[2]);
			return(-1);
		}
		TechnoClass * unit = (TechnoClass *)type->Create_One_Of(PlayerPtr);
		int result = unit != NULL ? factory->Exit_Object(unit, CELL_NONE) : 0;
		produced = unit;
		produced_from = factory;
		fprintf(stderr, "harness-produce %s %s exit %d &%p\n", word[1], word[2], result, (void *)unit);
		return(result ? 1 : -1);
	}
	if (!strcmp(word[0], "produced")) {
		// logs `harness-produced <ID> cell x,y factory x,y limbo <0|1> mission <m> factory-mission <m>` for the last
		// unit `produce` made (lane N3)
		if (produced == NULL || !Is_Live(produced) || produced_from == NULL) {
			fprintf(stderr, "harness-produced none\n");
			return(-1);
		}
		Cell at = produced->Get_Cell(), home = produced_from->Get_Cell();
		fprintf(stderr, "harness-produced %s cell %d,%d factory %d,%d limbo %d mission %d factory-mission %d\n", Type_Name(produced),
			at.X, at.Y, home.X, home.Y, produced->IsInLimbo ? 1 : 0, (int)produced->Get_Mission(), (int)produced_from->Get_Mission());
		return(1);
	}
	if (!strcmp(word[0], "mission") && words >= 2) {
		// logs `harness-mission <ID> <mission> &<addr>` for the player's first techno of type ID
		TechnoClass * techno = First_Of(word[1], PlayerPtr);
		if (techno == NULL) return(-1);
		fprintf(stderr, "harness-mission %s %d &%p\n", word[1], (int)techno->Get_Mission(), (void *)techno);
		return(1);
	}
	if (!strcmp(word[0], "doing") && words >= 2) {
		// logs `harness-doing <ID> do<n> m<mission> deployer <0|1> deployed <0|1> &<addr>` for the first techno of
		// type ID (the player's, else any): an infantry soldier's animation (DoType; 0x1b deploy, 0x1c deployed idle,
		// 0x1f undeploy) and its type's Deployer; a vehicle's deployed state (lane N13, the Guardian GI)
		// the one the last `spawn` or `deploy` of that type named (the player's first of a type changes as soldiers go
		// into limbo), else the player's first, else anyone's; `harness-doing <ID> gone` when the named one has left
		// the map (lane N16: a starting GI killed by the Desolator's radiation was replaced by another, unordered one)
		bool gone = false;
		TechnoClass * techno = Tracked(word[1], &gone);
		if (gone) {
			fprintf(stderr, "harness-doing %s gone\n", word[1]);
			return(-1);
		}
		if (techno == NULL) techno = First_Of(word[1], PlayerPtr);
		if (techno == NULL) techno = First_Of(word[1], NULL);
		if (techno == NULL) {
			fprintf(stderr, "harness-doing %s none\n", word[1]);
			return(-1);
		}
		int doing = -1, deployer = 0, deployed = 0;
		if (techno->Fetch_RTTI() == RTTI_INFANTRY) {
			InfantryClass * infantry = (InfantryClass *)techno;
			doing = (int)infantry->Doing;
			deployer = infantry->Class->Deployer ? 1 : 0;
			deployed = infantry->FUN_00522510() ? 1 : 0;
		} else if (techno->Fetch_RTTI() == RTTI_UNIT) {
			deployed = ((UnitClass *)techno)->Deployed ? 1 : 0;
		}
		fprintf(stderr, "harness-doing %s do%d m%d deployer %d deployed %d s%d &%p\n", word[1], doing,
			(int)techno->Get_Mission(), deployer, deployed, techno->Strength, (void *)techno);
		return(1);
	}
	if (!strcmp(word[0], "deployed") && words >= 2) {
		// logs `harness-deployed <ID> <deployed> of <n> (<deploying> deploying) dead <0..n>` over every soldier of type
		// ID on the map (any house; deployed: the deployed idle or firing animations 0x1c-0x1e, deploying: 0x1b),
		// the mission's scripted deploy (lane N13)
		int n = 0, deployed = 0, deploying = 0, dead = 0;
		for (int i = 0; i < Infantry.Count(); ++i) {
			InfantryClass * infantry = Infantry[i];
			if (infantry == NULL || infantry->IsInLimbo || strcmp(Type_Name(infantry), word[1])) continue;
			++n;
			if (infantry->Strength <= 0) ++dead;
			if (infantry->Doing == (DoType)0x1b) ++deploying;
			else if (infantry->Doing == (DoType)0x1c || infantry->Doing == (DoType)0x1d || infantry->Doing == (DoType)0x1e) ++deployed;
		}
		fprintf(stderr, "harness-deployed %s %d of %d (%d deploying) dead %d\n", word[1], deployed, n, deploying, dead);
		return(n ? n : 1);
	}
	if (!strcmp(word[0], "plan") && words >= 3) {
		// every selected techno of the player's is ordered to the cell dx,dy from the first one's as a map click
		// orders it (TechnoClass::Player_Assign_Mission, MISSION_MOVE): in planning mode that plans a waypoint
		TechnoClass * lead = NULL;
		int count = 0;
		for (int i = 0; i < CurrentObject.Count(); ++i) {
			ObjectClass * object = CurrentObject[i];
			if (object == NULL || !object->Is_Techno() || ((TechnoClass *)object)->House != PlayerPtr) continue;
			TechnoClass * techno = (TechnoClass *)object;
			if (lead == NULL) lead = techno;
			Coord coord = lead->Center_Coord();
			Cell cell(coord.X / 256 + atoi(word[1]), coord.Y / 256 + atoi(word[2]));
			techno->Player_Assign_Mission(MISSION_MOVE, NULL, &Map[cell], NULL);
			++count;
		}
		fprintf(stderr, "harness-plan %d units %s,%s\n", count, word[1], word[2]);
		return(count ? count : -1);
	}
	if (!strcmp(word[0], "thief") && words >= 2) {
		ObjectTypeClass const * type = ObjectTypeClass::From_Name(word[1]);
		if (type == NULL || type->Fetch_RTTI() != RTTI_INFANTRYTYPE) {
			fprintf(stderr, "harness-thief: no infantry type %s\n", word[1]);
			return(-1);
		}
		// Both flags: InfantryClass::Theft_AI (gamemd.exe 0x005202F0, the walk-up hijack) tests the one the tree
		// reads from "Thief", the movement and target choice the one from "VehicleThief".
		((InfantryTypeClass *)type)->IsThief = true;
		((InfantryTypeClass *)type)->IsVehicleThief = true;
		fprintf(stderr, "harness-thief %s\n", word[1]);
		return(1);
	}
	if (!strcmp(word[0], "create") && words >= 4 && !strcmp(word[2], "on")) {
		TechnoClass * unit = First_Of(word[3], PlayerPtr);
		int type = BuildingTypeClass::From_Name(word[1]);
		if (unit == NULL || type < 0 || BuildingTypes[type] == NULL) {
			fprintf(stderr, "harness-create: no %s or no type %s\n", word[3], word[1]);
			return(-1);
		}
		Cell cell = unit->Get_Cell();
		Coord coord = Map[cell].Center_Coord();
		BuildingClass * building = (BuildingClass *)BuildingTypes[type]->Create_One_Of(PlayerPtr);
		bool ok = building != NULL && building->ForceCreate(coord, 0);
		if (building != NULL && !ok) building->Delete_Me();
		else if (ok) building->Grand_Opening(false);
		fprintf(stderr, "harness-create %s %s @%d,%d\n", word[1], ok ? "ok" : "failed", cell.X, cell.Y);
		return(ok ? 1 : -1);
	}
	if (!strcmp(word[0], "bolt") && words >= 2) {
		HouseClass * enemy = Enemy_House();
		if (enemy == NULL || enemy->Buildings.Count() == 0) return(-1);
		Cell cell = enemy->Buildings[0]->Get_Cell();
		IonStormClass::Lightning_Bolt(cell);
		fprintf(stderr, "harness-bolt @%d,%d\n", cell.X, cell.Y);
		return(1);
	}
	if (!strcmp(word[0], "plans")) {
		fprintf(stderr, "harness-plans mode %d tokens %d nodes %d\n", PlanningNodeClass::PlanningModeActive ? 1 : 0,
			PlanningTokenClass::Array.Count(), PlanningNodeClass::Unknown1.Count());
		return(PlanningNodeClass::Unknown1.Count());
	}
	if (!strcmp(word[0], "centermap")) {
		// the view is centred on the selection (DisplayClass::Center_Map, the Home key's centring) (lane N23)
		Map.Center_Map();
		fprintf(stderr, "harness-centermap %d selected\n", CurrentObject.Count());
		return(1);
	}
	if (!strcmp(word[0], "planloop")) {
		// logs `harness-planloop picked <i> hover <i> refused <0|1> loop <start> <count> path <n>`: the planned node picked up
		// with the mouse (DAT_00AC4C38) and the hovered one (DAT_00AC4CCC) as indexes into the planned nodes (-1:
		// none), whether a planning refusal was shown this frame (DAT_00AC4C08), and the closed loop and node count of the
		// first node's first member's path (lane N23)
		TechnoClass * lead = PlanningNodeClass::Unknown1.Count() ? PlanningNodeClass::Unknown1[0]->Member_Owner(0) : NULL;
		PlanningTokenClass * token = lead != NULL ? lead->FUN_00705D20() : NULL;
		Point2D node = PlanningNodeClass::Unknown1.Count() ? PlanningNodeClass::Unknown1[0]->Lead_Pixel() : Point2D(-1, -1);
		fprintf(stderr, "harness-planloop picked %d hover %d refused %d loop %d %d path %d (node 0 at %d,%d, mouse %d,%d)\n",
			PlanningNodeClass::Unknown1.ID(DAT_00AC4C38), PlanningNodeClass::Unknown1.ID(DAT_00AC4CCC), DAT_00AC4C08 ? 1 : 0,
			token ? token->Get_Loop_Start() : -2, token ? token->ClosedLoopNodeCount : -2, token ? token->Node_Count() : -2, node.X, node.Y,
			MouseCursor->Get_Mouse_X(), MouseCursor->Get_Mouse_Y());
		for (int i = 0; i < PlanningNodeClass::Unknown1.Count(); ++i) {
			Point2D pixel = PlanningNodeClass::Unknown1[i]->Lead_Pixel();
			fprintf(stderr, "harness-planloop node %d at %d,%d state %d\n", i, pixel.X, pixel.Y, FUN_0063AC70(PlanningNodeClass::Unknown1[i]));
		}
		return(1);
	}
	if (!strcmp(word[0], "beaconmode")) {
		FUN_00731A30();
		fprintf(stderr, "harness-beaconmode %s\n", Map.PlaceBeaconMode ? "on" : "off");
		return(1);
	}
	if (!strcmp(word[0], "beacons")) {
		fprintf(stderr, "harness-beacons %d\n", BeaconManager.AllocatedCount);
		return(BeaconManager.AllocatedCount);
	}
	if (!strcmp(word[0], "stormstart")) {
		HouseClass * enemy = Enemy_House();
		if (enemy == NULL || enemy->Buildings.Count() == 0) return(-1);
		Cell cell = enemy->Buildings[0]->Get_Cell();
		FUN_00539EB0(300, 0, cell, NULL);
		fprintf(stderr, "harness-stormstart @%d,%d %s\n", cell.X, cell.Y, IonStormClass::Is_Ion_Storm_Active() ? "active" : "not active");
		return(1);
	}
	if (!strcmp(word[0], "stormend")) {
		bool active = IonStormClass::Is_Ion_Storm_Active();
		if (active) IonStormClass::Ion_Storm_End();
		fprintf(stderr, "harness-stormend %s\n", active ? "active" : "none");
		return(1);
	}
	if (!strcmp(word[0], "cmd") && words >= 2) {
		for (int i = 0; i < AllCommands.Count(); ++i) {
			CommandClass const * command = AllCommands[i];
			if (command != NULL && !strcmp(command->Get_Unique_Name(), word[1])) {
				command->Execute(0);
				fprintf(stderr, "harness-cmd %s\n", word[1]);
				return(1);
			}
		}
		fprintf(stderr, "harness-cmd: no command %s\n", word[1]);
		return(-1);
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
	if (!strcmp(word[0], "camo") && words >= 3) {
		camo_unit = First_Of(word[1], NULL);
		camo_frames = atoi(word[2]);
		camo_seen = camo_green = camo_blue = 0;
		camo_compared = camo_diff_max = 0;
		camo_diff_frame = -1;
		camo_created = 0;
		memset(camo_kept, 0, sizeof(camo_kept));
		return(camo_unit != NULL ? 1 : -1);
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
	if (!strcmp(word[0], "occupiable")) {
		// logs `harness-occupiable <ID> max <n>` for each building type that infantry can garrison (lane N4)
		int n = 0;
		for (int i = 0; i < BuildingTypes.Count(); ++i) {
			BuildingTypeClass * type = BuildingTypes[i];
			if (type == NULL || !type->CanBeOccupied) continue;
			if (n < 40) fprintf(stderr, "harness-occupiable %s max %d\n", type->Name(), type->MaxNumberOccupants);
			++n;
		}
		return(n ? n : -1);
	}
	if (!strcmp(word[0], "click") && words >= 3) {
		// the player's first unit of type ID clicked onto the nearest techno of type TARGET as the mouse click does
		// (What_Action, then Active_Click_With: the order event); logs `harness-click <ID> action <n> -> <TARGET>` (lane N4)
		TechnoClass * unit = First_Of(word[1], PlayerPtr);
		TechnoClass * target = NULL;
		int best = 0;
		for (int i = 0; i < Technos.Count() && unit != NULL; ++i) {
			TechnoClass * techno = Technos[i];
			if (techno == NULL || techno == unit || techno->IsInLimbo || strcmp(Type_Name(techno), word[2])) continue;
			Coord a = techno->Center_Coord(), b = unit->Center_Coord();
			int d = abs(a.X - b.X) + abs(a.Y - b.Y);
			if (target == NULL || d < best) target = techno, best = d;
		}
		if (unit == NULL || target == NULL) {
			fprintf(stderr, "harness-click: no %s or no %s\n", word[1], word[2]);
			return(-1);
		}
		ActionType action = unit->What_Action(target, true);
		bool done = unit->Active_Click_With(action, target, false);
		fprintf(stderr, "harness-click %s action %d -> %s %s\n", word[1], (int)action, word[2], done ? "ok" : "refused");
		return(done ? 1 : -1);
	}
	if (!strcmp(word[0], "scenario")) {
		// logs `harness-scenario <file>`: the scenario being played (lane N4)
		fprintf(stderr, "harness-scenario %s\n", Scen ? Scen->ScenarioName : "?");
		return(1);
	}
	if (!strcmp(word[0], "artkey") && words >= 3) {
		// logs `harness-artkey <section> <key> <value>` from ART(MD).INI as the game read it, and `harness-layer-of
		// <value> = <n>`: the value read as a layer name (Layer_From_Name; lane N4)
		char value[128];
		ArtINI.Get_String(word[1], word[2], "<absent>", value, sizeof(value));
		fprintf(stderr, "harness-artkey %s %s %s\n", word[1], word[2], value);
		fprintf(stderr, "harness-layer-of %s = %d\n", value, (int)Layer_From_Name(value));	// the value as a layer name
		return(1);
	}
	if (!strcmp(word[0], "banimhide") && words >= 3) {
		// every building of type BLDG hides (or with `show`, shows) its animation in slot n (lane N4)
		int slot = atoi(word[2]);
		bool show = words >= 4 && !strcmp(word[3], "show");
		int n = 0;
		for (int i = 0; i < Buildings.Count(); ++i) {
			BuildingClass * b = Buildings[i];
			if (b == NULL || slot < 0 || slot >= BANIM_COUNT || b->Anims[slot] == NULL || strcmp(Type_Name(b), word[1])) continue;
			b->Anims[slot]->Invisible = !show;
			++n;
		}
		return(n ? n : -1);
	}
	if (!strcmp(word[0], "banims") && words >= 2) {
		// logs `harness-banim <BLDG> <slot> <anim> live <0|1> powered <0|1>` for each animation slot the type names
		// (BuildingTypeClass::AnimData) of the first building of type BLDG, whether it is playing (BuildingClass::Anims)
		// (lane N4)
		BuildingClass * found = NULL;
		for (int i = 0; i < Buildings.Count() && found == NULL; ++i) {
			if (Buildings[i] && !Buildings[i]->IsInLimbo && !strcmp(Type_Name(Buildings[i]), word[1])) found = Buildings[i];
		}
		if (found == NULL) return(-1);
		int n = 0;
		for (int slot = 0; slot < BANIM_COUNT; ++slot) {
			BuildingTypeClass::AnimDataType const & data = found->Class->AnimData[slot];
			if (!data.Anim[0] && found->Anims[slot] == NULL) continue;
			AnimClass * anim = found->Anims[slot];
			fprintf(stderr, "harness-banim %s %d %s live %d powered %d", word[1], slot, data.Anim[0] ? data.Anim : "-",
				anim != NULL ? 1 : 0, data.Powered ? 1 : 0);
			if (anim != NULL) {
				Cell cell = anim->Get_Cell();
				fprintf(stderr, " stage %d/%d invisible %d poweroff %d paused %d limbo %d @%d,%d", anim->Animation.Fetch_Stage(),
					anim->Stage_Count(), anim->Invisible, anim->PowerOff, anim->Paused, anim->IsInLimbo, cell.X, cell.Y);
				fprintf(stderr, " delay %d fogged %d detail %d image %d layer %d", anim->LoopDelay, anim->IsFogged,
					anim->Type->DetailLevel, anim->Get_Image_Data() != NULL ? 1 : 0, (int)anim->In_Which_Layer());
				int in = -1;
				for (int layer = 0; layer < LAYER_COUNT && in < 0; ++layer) {
					for (int k = 0; k < DisplayClass::Layer[layer].Count(); ++k) if (DisplayClass::Layer[layer][k] == anim) in = layer;
				}
				fprintf(stderr, " in-layer %d owner %d", in, anim->OwnerObject != NULL ? 1 : 0);
			}
			fprintf(stderr, "\n");
			++n;
		}
		fprintf(stderr, "harness-banim %s slots %d\n", word[1], n);
		return(1);
	}
	if (!strcmp(word[0], "disguise") && words >= 2) {
		// logs `harness-disguise <ID> <0|1> as <TYPE> r<rtti> clear <0|1>` for the first techno of type ID: disguised,
		// the disguise's type, whether the player sees through it now (IsClearlyVisibleTo) (lane N4)
		TechnoClass * techno = First_Of(word[1], NULL);
		if (techno == NULL) return(-1);
		ObjectTypeClass const * as = techno->IsDisguised() ? techno->GetDisguise(true) : NULL;
		fprintf(stderr, "harness-disguise %s %d as %s r%d clear %d\n", word[1], techno->IsDisguised() ? 1 : 0,
			as ? as->Name() : "-", as ? (int)as->Fetch_RTTI() : -1, techno->IsClearlyVisibleTo(PlayerPtr) ? 1 : 0);
		return(1);
	}
	if (!strcmp(word[0], "occupants") && words >= 2) {
		// logs `harness-occupants <BLDG> h<house> <n>[p]` for each building of type BLDG (p: the player's) (lane N4)
		int n = 0;
		for (int i = 0; i < Buildings.Count(); ++i) {
			BuildingClass * building = Buildings[i];
			if (building == NULL || building->IsInLimbo || strcmp(Type_Name(building), word[1])) continue;
			fprintf(stderr, "harness-occupants %s h%d %d%s\n", word[1], building->House ? building->House->HeapID : -1,
				building->GetOccupantCount(), building->House == PlayerPtr ? "p" : "");
			++n;
		}
		if (!n) fprintf(stderr, "harness-occupants %s none\n", word[1]);
		return(n ? n : 1);
	}
	if ((!strcmp(word[0], "deploy") || !strcmp(word[0], "move") || !strcmp(word[0], "where")) && words >= 2) {
		TechnoClass * found = NULL;
		bool gone = false;
		// `deploy` orders the one the type's last `spawn` or `deploy` named: the spawned one, and a second `deploy` of
		// a type the same one again (to undeploy it; lanes N13, N16)
		if (word[0][0] == 'd') found = Tracked(word[1], &gone);
		if (gone) {
			fprintf(stderr, "harness-deploy: %s gone\n", word[1]);
			return(-1);
		}
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
			Track(word[1], found);
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

// camo: in the composed view, the pixels of a box above a unit (its disguise, a tree, stands there) that are
// leaf green (green the strongest channel) and that are blue or white (blue the strongest, or all channels
// high): the disguise drawn through the house's colours instead of the terrain's (lane N4) was blue and white.
void Camo_Pixels(Surface * surface, Point2D const & at, int & green, int & blue)
{
	if (surface == NULL || surface->Bytes_Per_Pixel() != 2) return;
	unsigned char * base = (unsigned char *)surface->Lock();
	if (base == NULL) return;
	for (int y = at.Y - 70; y < at.Y; ++y) {
		if (y < 0 || y >= surface->Get_Height()) continue;
		unsigned short const * row = (unsigned short const *)(base + (size_t)y * surface->Stride());
		for (int x = at.X - 40; x < at.X + 10; ++x) {
			if (x < 0 || x >= surface->Get_Width()) continue;
			int r = (row[x] >> 11) << 3, g = ((row[x] >> 5) & 63) << 2, b = (row[x] & 31) << 3;
			if (g > r + 8 && g > b + 8) ++green;
			else if ((b > r + 24 && b > g) || (r > 200 && g > 200 && b > 200)) ++blue;
		}
	}
	surface->Unlock();
}

// camo: copy (keep) or compare the box around the unit (centre at its pixel, the disguise above it); returns the
// pixels that differ from the kept copy.
int Camo_Box(Surface * surface, Point2D const & at, unsigned short * keep, bool compare, char const * dump)
{
	if (surface == NULL || surface->Bytes_Per_Pixel() != 2) return(0);
	unsigned char * base = (unsigned char *)surface->Lock();
	if (base == NULL) return(0);
	int differ = 0;
	FILE * f = dump != NULL ? fopen(dump, "wb") : NULL;
	if (f != NULL) fprintf(f, "P6 %d %d 255\n", kCamoBoxW, kCamoBoxH);
	for (int y = 0; y < kCamoBoxH; ++y) {
		int sy = at.Y - 85 + y;
		for (int x = 0; x < kCamoBoxW; ++x) {
			int sx = at.X - 55 + x;
			unsigned short c = 0;
			if (sy >= 0 && sy < surface->Get_Height() && sx >= 0 && sx < surface->Get_Width()) {
				c = ((unsigned short const *)(base + (size_t)sy * surface->Stride()))[sx];
			}
			if (compare) {
				// only where the disguise is drawn (Camo_Pixels' box: 40 left to 10 right of the unit, 70 above it):
				// soldiers standing beside and below the tank change pose and place between the cycles (lane N14:
				// the ctest's 289- to 691-pixel failures were the neighbouring GIs, not the morph)
				bool tree = sx >= at.X - 40 && sx < at.X + 10 && sy >= at.Y - 70 && sy < at.Y;
				differ += tree && keep[y * kCamoBoxW + x] != c;
			} else {
				keep[y * kCamoBoxW + x] = c;
			}
			if (f != NULL) {
				unsigned char rgb[3] = {(unsigned char)((c >> 11) << 3), (unsigned char)(((c >> 5) & 63) << 2), (unsigned char)((c & 31) << 3)};
				fwrite(rgb, 1, 3, f);
			}
		}
	}
	if (f != NULL) fclose(f);
	surface->Unlock();
	return(differ);
}

void Force_End(int frame)
{
	static int end_at[16];
	static int end_count = -1;
	static bool end_lose = false;
	static bool end_flag = false;
	static int end_done = 0;
	if (end_count < 0) {
		end_count = 0;
		char const * env = getenv("RA2_NATIVE_FORCE_END");
		for (char const * p = env; p && *p;) {
			if (*p == 'l') end_lose = true;
			else if (*p == 'f') end_flag = true;
			else if (*p >= '0' && *p <= '9' && end_count < 16) end_at[end_count++] = atoi(p);
			while (*p && *p != ',') ++p;
			if (*p == ',') ++p;
		}
	}
	if (end_done < end_count && frame >= end_at[end_done]) {
		++end_done;
		if (end_flag && PlayerPtr != NULL) {
			if (end_lose) PlayerPtr->Flag_To_Lose(false); else PlayerPtr->Flag_To_Win(false);
		} else if (end_lose) PlayerLoses = true; else PlayerWins = true;
		fprintf(stderr, "force-end: %s%s at frame %d in %s\n", end_flag ? "flagged " : "", end_lose ? "loss" : "win", frame,
			Scen ? Scen->ScenarioName : "?");
	}
}


// The mission soak (lane N25): see the file comment; the same lines as RA2's link/harness_soak.cpp.
} // namespace
extern "C" void ra2port_progress(const char * milestone, const char * detail);	// port/src/run_progress.cpp
namespace {
extern int DAT_00A8ED98;	// init.cpp: the custom seed (Red Alert's CustomSeed)
extern bool DAT_00A8EDDC;	// mainloop.cpp: -SPEEDCONTROL's flag (Red Alert 2's GameControlsShown)
int const soak_seed_set = (getenv("RA2_NATIVE_SOAK_SEED") ? (DAT_00A8ED98 = atoi(getenv("RA2_NATIVE_SOAK_SEED")), 1) : 0);

unsigned Soak_Mix(unsigned hash, unsigned value)
{
	for (int i = 0; i < 4; ++i) {
		hash ^= (value >> (8 * i)) & 0xFF;
		hash *= 16777619u;
	}
	return(hash);
}
unsigned Soak_Mix_Text(unsigned hash, char const * text)
{
	for (; text && *text; ++text) hash = Soak_Mix(hash, (unsigned char)*text);
	return(hash);
}
struct SoakSeen { TriggerClass * trigger; TriggerTypeClass * type; int tripped; };
int const kSoakTriggers = 4096;
SoakSeen soak_seen[kSoakTriggers];
int soak_seen_count = 0;
char soak_scenario[_MAX_PATH] = "";
int soak_last_frame = -1;
bool soak_timer = false;
bool soak_house_done[64];
int soak_missions = 0;

char const * Soak_Id(TriggerTypeClass * type) { return(type != NULL ? type->Name() : "?"); }
char const * Soak_Name(TriggerTypeClass * type) { return(type != NULL ? type->Full_Name() : "?"); }

void Soak_Triggers(bool report, int frame)
{
	static SoakSeen now[kSoakTriggers];
	int count = 0;
	for (int i = 0; i < Triggers.Count() && count < kSoakTriggers; ++i) {
		TriggerClass * trigger = Triggers[i];
		if (trigger == NULL) continue;
		now[count].trigger = trigger;
		now[count].type = trigger->Class;
		now[count].tripped = 0;
		for (int bit = 0; bit < 32; ++bit) {
			if (trigger->Is_Event_Tripped(bit)) now[count].tripped |= 1 << bit;
		}
		++count;
	}
	if (report) {
		for (int i = 0; i < soak_seen_count; ++i) {
			int k = 0;
			while (k < count && !(now[k].trigger == soak_seen[i].trigger && now[k].type == soak_seen[i].type)) ++k;
			if (k == count) {
				fprintf(stderr, "soak-trigger %d gone %s %s\n", frame, Soak_Id(soak_seen[i].type), Soak_Name(soak_seen[i].type));
			} else if (now[k].tripped != soak_seen[i].tripped) {
				fprintf(stderr, "soak-trigger %d tripped %s %x\n", frame, Soak_Id(now[k].type), (unsigned)now[k].tripped);
			}
		}
		for (int k = 0; k < count; ++k) {
			int i = 0;
			while (i < soak_seen_count && !(now[k].trigger == soak_seen[i].trigger && now[k].type == soak_seen[i].type)) ++i;
			if (i == soak_seen_count) fprintf(stderr, "soak-trigger %d new %s %s\n", frame, Soak_Id(now[k].type), Soak_Name(now[k].type));
		}
	}
	for (int k = 0; k < count; ++k) soak_seen[k] = now[k];
	soak_seen_count = count;
}

void Soak_Digest(int frame, bool dump)
{
	unsigned hash = 2166136261u;
	int player = 0;
	for (int i = 0; i < Technos.Count(); ++i) {
		TechnoClass * techno = Technos[i];
		if (techno == NULL) continue;
		Coord coord = techno->Center_Coord();
		int house = techno->House != NULL ? techno->House->ID : -1;
		int mission = (int)techno->Get_Mission();
		if (dump) {
			fprintf(stderr, "soak-techno %d #%d %s h%d %d,%d,%d s%d m%d\n", frame, i, Type_Name(techno), house, coord.X, coord.Y,
				coord.Z, techno->Strength, mission);
			continue;
		}
		hash = Soak_Mix_Text(hash, Type_Name(techno));
		hash = Soak_Mix(hash, house);
		hash = Soak_Mix(hash, coord.X);
		hash = Soak_Mix(hash, coord.Y);
		hash = Soak_Mix(hash, coord.Z);
		hash = Soak_Mix(hash, techno->Strength);
		hash = Soak_Mix(hash, mission);
		if (techno->House == PlayerPtr) ++player;
	}
	if (dump) return;
	for (int i = 0; i < Houses.Count(); ++i) {
		if (Houses[i] != NULL) hash = Soak_Mix(hash, Houses[i]->Credits);
	}
	unsigned rng = 2166136261u;
	unsigned char const * bytes = (unsigned char const *)&Scen->RandomNumber;
	for (unsigned i = 0; i < sizeof(Scen->RandomNumber); ++i) rng = Soak_Mix(rng, bytes[i]);
	fprintf(stderr, "soak-digest %d %08x technos %d player %d credits %d rng %08x\n", frame, hash, Technos.Count(), player,
		PlayerPtr != NULL ? PlayerPtr->Credits : -1, rng);
}

void Soak(int loop_frame)
{
	static int period = -2, speed = -2, dump_from = -1, dump_to = -2, end_frames = -2, end_mission = 1;
	if (period == -2) {
		char const * env = getenv("RA2_NATIVE_SOAK");
		period = env != NULL ? atoi(env) : -1;
		if (period == 0) period = 500;
		env = getenv("RA2_NATIVE_SOAK_SPEED");
		speed = env != NULL && *env ? atoi(env) : -1;
		if ((env = getenv("RA2_NATIVE_SOAK_DUMP")) != NULL) {
			dump_from = dump_to = atoi(env);
			for (char const * p = env; *p; ++p) if (*p == '-') dump_to = atoi(p + 1);
		}
		env = getenv("RA2_NATIVE_SOAK_END");
		end_frames = env != NULL ? atoi(env) : -1;
		for (char const * p = env; p != NULL && *p; ++p) if (*p == '@') end_mission = atoi(p + 1);
	}
	if (speed >= 0) {
		DAT_00A8EDDC = true;
		Options.GameSpeed = speed;
	}
	if (period < 0 || Scen == NULL) return;
	int frame = Unsorted::CurrentFrame;
	bool renamed = strncmp(soak_scenario, Scen->ScenarioName, sizeof(soak_scenario)) != 0;
	if (frame < soak_last_frame || renamed) {
		strncpy(soak_scenario, Scen->ScenarioName, sizeof(soak_scenario) - 1);
		++soak_missions;
		Soak_Triggers(false, frame);
		fprintf(stderr, "soak-mission %s loop %d triggers %d types %d\n", soak_scenario, loop_frame, soak_seen_count,
			TriggerTypes.Count());
		soak_timer = false;
		memset(soak_house_done, 0, sizeof(soak_house_done));
	} else if (frame != soak_last_frame) {
		Soak_Triggers(true, frame);
	}
	if (frame != soak_last_frame) {
		bool active = Scen->MissionTimer.Is_Active();
		if (active != soak_timer) {
			fprintf(stderr, "soak-timer %d %s %ld\n", frame, active ? "started" : "stopped", (long)Scen->MissionTimer);
			soak_timer = active;
		}
		for (int i = 0; i < Houses.Count() && i < 64; ++i) {
			HouseClass * house = Houses[i];
			if (house == NULL || soak_house_done[i] || !house->IsDefeated) continue;
			soak_house_done[i] = true;
			fprintf(stderr, "soak-house %d %d:%s defeated\n", frame, i, (char const *)house->IniName);
		}
		if (frame % period == 0) Soak_Digest(frame, false);
		if (frame >= dump_from && frame <= dump_to) Soak_Digest(frame, true);
	}
	soak_last_frame = frame;
	if (end_frames >= 0 && soak_missions == end_mission) {
		char const * how = NULL;
		if (PlayerWins) how = "won";
		else if (PlayerLoses || (PlayerPtr != NULL && PlayerPtr->IsDefeated)) how = "lost";
		else if (frame >= end_frames) how = "played";
		if (how != NULL) {
			char detail[_MAX_PATH + 64];
			snprintf(detail, sizeof(detail), "%s %s at frame %d", soak_scenario, how, frame);
			fprintf(stderr, "soak-end %s\n", detail);
			ra2port_progress("soak-end", detail);
			end_frames = -1;
		}
	}
}

// The "+<n>[/<p>]" orders of the scenario playing (Read_Orders keeps them apart).
void Relative_Orders(int frame)
{
	static int last = -1;
	static char scenario[_MAX_PATH] = "";
	if (Scen == NULL || relative_count == 0) return;
	int now = Unsorted::CurrentFrame;
	if (now < last || strncmp(scenario, Scen->ScenarioName, sizeof(scenario)) != 0) {
		strncpy(scenario, Scen->ScenarioName, sizeof(scenario) - 1);
		for (int i = 0; i < relative_count; ++i) relative[i].due = relative[i].at;
		deploy_count = 0;	// `deploy` tracks units of the scenario playing only
	}
	last = now;
	for (int i = 0; i < relative_count; ++i) {
		Relative & r = relative[i];
		if (r.due < 0 || now < r.due) continue;
		r.due = r.every > 0 ? now + r.every : -1;
		int result = Run_Order(frame, r.text);
		fprintf(stderr, "harness-order +%d %s: %d\n", now, r.text, result);
	}
}
} // namespace

void yr_native_harness_frame(int frame)
{
	if (order_count < 0) Read_Orders();
	Soak(frame);
	Relative_Orders(frame);
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
	if (camo_frames > 0 && camo_unit != NULL) {
		Point2D at;
		if (Is_Live(camo_unit) && camo_unit->IsDisguised() && TacticalMap->Coord_To_Pixel(camo_unit->Center_Coord(), at)) {
			// the morph: the first cycle's frames kept, the second cycle's compared with them (lane N12)
			// the composed view holds the previous frame's drawing: the first frame a disguise is seen still shows
			// the tank as it was before, so it is not kept
			bool first_seen = camo_created == 0;
			if (first_seen) camo_created = camo_unit->DisguiseCreationFrame;
			unsigned age = (unsigned)Unsorted::CurrentFrame - camo_unit->DisguiseCreationFrame + 64;
			int phase = (int)(age % 256), cycle = (int)(age / 256);
			char dump[1024];
			char const * dir = getenv("RA2_NATIVE_CAMO_DUMP");	// the boxes as PPM files, for a look
			if (dir != NULL) snprintf(dump, sizeof dump, "%s/f%05d_c%d_p%03d.ppm", dir, frame, cycle, phase);
			if (camo_first == NULL) camo_first = new unsigned short[64 * kCamoBoxH * kCamoBoxW];
			if (!first_seen && camo_unit->DisguiseCreationFrame == camo_created && phase >= 64 && phase < 128 && cycle <= 1) {
				unsigned short * keep = camo_first + (size_t)(phase - 64) * kCamoBoxH * kCamoBoxW;
				if (cycle == 0) {
					Camo_Box(CompositeSurface, at, keep, false, dir ? dump : NULL);
					camo_kept[phase - 64] = true;
				} else if (camo_kept[phase - 64]) {
					int differ = Camo_Box(CompositeSurface, at, keep, true, dir ? dump : NULL);
					++camo_compared;
					if (differ > camo_diff_max) {
						camo_diff_max = differ;
						camo_diff_frame = frame;
					}
				}
			}
		}
		if (!Is_Live(camo_unit)) {
			camo_frames = 1;
		} else if (camo_unit->IsDisguised() && !camo_unit->IsClearlyVisibleTo(PlayerPtr)) {
			Point2D pixel;
			if (TacticalMap->Coord_To_Pixel(camo_unit->Center_Coord(), pixel)) {
				int green = 0, blue = 0;
				Camo_Pixels(CompositeSurface, pixel, green, blue);
				++camo_seen;
				camo_green += green;
				camo_blue += blue;
			}
		}
		if (--camo_frames == 0) {
			fprintf(stderr, "harness-camo seen %d green %d blue %d\n", camo_seen,
				camo_seen ? camo_green / camo_seen : 0, camo_seen ? camo_blue / camo_seen : 0);
			fprintf(stderr, "harness-camo-morph compared %d differ %d at frame %d\n", camo_compared, camo_diff_max,
				camo_diff_frame);
		}
	}
	Force_End(frame);
}
