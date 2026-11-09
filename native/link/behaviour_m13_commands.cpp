// Behaviour rows (Scout1115, native M13; NOT EXACT, native/behaviour_rewrites.json):
// the keyboard commands' small virtuals (UICmnds.cpp, 0x0051AE70-0x0051DA30) that the
// player's hotkeys reach. Each is written from its seed and pass-6a decompile: the
// command calls an accepted UICmnds.cpp helper (0x006F5xxx), the sidebar's scroll or
// the display's mode switches. The three CommandClass defaults and the overrides
// answer the key's modifier bits (0x100 shift, 0x800 alt/release as the seeds test).
#include "shared/command.hpp"
#include "shared/mouse_class.hpp"
#include "shared/queue.hpp"
#include "shared/object.hpp"

extern MouseClass Map;                 // 0x008324E0 (as shared/map_view.hpp declares it)

extern int unknown_game_mode_a3d298;   // 0x00A3D298 the session's game mode (Session.Type)

void __fastcall Unknown_006F61F0(int group);    // 0x006F61F0 adds the selection to a group
void __fastcall Unknown_006F5D90(int group);    // 0x006F5D90 makes a group of the selection
void __fastcall Unknown_006F5F00(int group);    // 0x006F5F00 selects a group
void __fastcall Unknown_006F60D0(int group);    // 0x006F60D0 centres on a group
void Unknown_006F5C50();                        // 0x006F5C50 cheer
void Unknown_006F6380();                        // 0x006F6380 the alliance event
void Unknown_006F64B0();                        // 0x006F64B0 beacon placement (not recovered)
void Unknown_006F88F0();                        // 0x006F88F0 the radar event jump
void Unknown_006F5940();                        // 0x006F5940 deploy
void __fastcall Unknown_006F5B10();             // 0x006F5B10 guard
void Unknown_006F5D10();                        // 0x006F5D10 scatter
void Unknown_006F5BC0();                        // 0x006F5BC0 stop
void __fastcall Unknown_006F7670();             // 0x006F7670
void Unknown_006F7650();                        // 0x006F7650
void unknown_dispatch();                        // 0x006F6490 delete

// CommandClass defaults (0x0051AE70 / 0x0051AE80 / 0x0051AE90).
bool CommandClass::unknown_slot5(int key) const { return false; }
bool CommandClass::unknown_slot6(int key) const { return ((unsigned)~key >> 11) & 1; }
bool CommandClass::unknown_slot7(int key) const { return false; }

// Overrides answering the shift bit (0x0051BA80, 0x0051BBE0, 0x0051BC80, 0x0051BB20).
bool CombatantSelectCommandClass::unknown_slot5(int key) const { return ((unsigned)key >> 8) & 1; }
bool HealthNavCommandClass::unknown_slot5(int key) const { return ((unsigned)key >> 8) & 1; }
bool VeterancyNavCommandClass::unknown_slot5(int key) const { return ((unsigned)key >> 8) & 1; }
bool TypeSelectCommandClass::unknown_slot5(int key) const { return ((unsigned)key >> 8) & 1; }
bool TypeSelectCommandClass::unknown_slot6(int key) const { return true; }                         // 0x0051BB30
bool TypeSelectCommandClass::unknown_slot7(int key) const { return ((unsigned)~key >> 11) & 1; }    // 0x0051BB40
bool PlanningModeCommandClass::unknown_slot6(int key) const { return true; }                       // 0x0051B9D0
bool PlanningModeCommandClass::unknown_slot7(int key) const { return ((unsigned)~key >> 11) & 1; }  // 0x0051B9E0

// 0x0051BB50: on release (0x800) 0x006F7670, else 0x006F7650.
void TypeSelectCommandClass::Execute(int key) const
{
    if (key & 0x800) {
        Unknown_006F7670();
        return;
    }
    Unknown_006F7650();
}

void CreateTeamCommandClass::Execute(int key) const { Unknown_006F5D90(TeamNumber); }   // 0x0051AF60
void SelectTeamCommandClass::Execute(int key) const { Unknown_006F5F00(TeamNumber); }   // 0x0051B320
void AddTeamCommandClass::Execute(int key) const { Unknown_006F61F0(TeamNumber); }      // 0x0051B410
void CenterTeamCommandClass::Execute(int key) const { Unknown_006F60D0(TeamNumber); }   // 0x0051B500
void AllToCheerCommandClass::Execute(int key) const { Unknown_006F5C50(); }             // 0x0051BEA0
void AllianceCommandClass::Execute(int key) const { Unknown_006F6380(); }               // 0x0051C430
void CenterREventCommandClass::Execute(int key) const { Unknown_006F88F0(); }           // 0x0051C640
void DeployCommandClass::Execute(int key) const { Unknown_006F5940(); }                 // 0x0051BF20
void GuardCommandClass::Execute(int key) const { Unknown_006F5B10(); }                  // 0x0051BFA0
void ScatterCommandClass::Execute(int key) const { Unknown_006F5D10(); }                // 0x0051C020
void StopCommandClass::Execute(int key) const { Unknown_006F5BC0(); }                   // 0x0051BE20
void DeleteCommandClass::Execute(int key) const { unknown_dispatch(); }                 // 0x0051DA30
void OptionsCommandClass::Execute(int key) const { Queue_Options(); }                   // 0x0051CD80

// 0x0051C5B0: beacons only in a multiplayer game (mode not 0 or 5).
void BeaconPlacementCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
        Unknown_006F64B0();
    }
}

// 0x0051D8F0: the page in an internet game (mode 4).
void PageUserCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 == 4) {
        Queue_Event_22();
    }
}

// 0x0051C0A0: centres the view on the selected objects and redraws.
void CenterViewCommandClass::Execute(int key) const
{
    if (unknown_objects_a40c60.Count() > 0) {
        Map.Unknown_0049E3B0();
        Map.Flag_To_Redraw(true);
    }
}

// 0x0051C4B0 / 0x0051C530: toggle the repair / sell mode.
void ToggleRepairCommandClass::Execute(int key) const { Map.Unknown_0049CA20(-1); }
void ToggleSellCommandClass::Execute(int key) const { Map.Unknown_0049C7C0(-1); }

// The sidebar's columns: a row (Scroll 0x0067CDD0) or a page (Scroll_Page 0x0067CE90),
// up or down, in the current column (-1), the left (0) or the right one (1).
void SidebarUpCommandClass::Execute(int key) const { Map.Scroll(true, -1); }               // 0x0051C6C0
void SidebarDownCommandClass::Execute(int key) const { Map.Scroll(false, -1); }            // 0x0051CA20
void SidebarPageUpCommandClass::Execute(int key) const { Map.Scroll_Page(true, -1); }      // 0x0051C870
void SidebarPageDownCommandClass::Execute(int key) const { Map.Scroll_Page(false, -1); }   // 0x0051CBD0
void LSidebarUpCommandClass::Execute(int key) const { Map.Scroll(true, 0); }               // 0x0051C750
void LSidebarDownCommandClass::Execute(int key) const { Map.Scroll(false, 0); }            // 0x0051CAB0
void LSidebarPageUpCommandClass::Execute(int key) const { Map.Scroll_Page(true, 0); }      // 0x0051C900
void LSidebarPageDownCommandClass::Execute(int key) const { Map.Scroll_Page(false, 0); }   // 0x0051CC60
void RSidebarUpCommandClass::Execute(int key) const { Map.Scroll(true, 1); }               // 0x0051C7E0
void RSidebarDownCommandClass::Execute(int key) const { Map.Scroll(false, 1); }            // 0x0051CB40
void RSidebarPageUpCommandClass::Execute(int key) const { Map.Scroll_Page(true, 1); }      // 0x0051C990
void RSidebarPageDownCommandClass::Execute(int key) const { Map.Scroll_Page(false, 1); }   // 0x0051CCF0

// 0x006A20B0 TabClass slot 33 (seed): the tooltip text of a control: SidebarClass's
// (0x00681ED0, called directly).
wchar_t const *TabClass::unknown_slot33(int id)
{
    return SidebarClass::unknown_slot33(id);
}

// Native adapter (not a row): RadarClass slot 51 0x0062FBD0 (the cell under a screen point)
// on Map, for the Set_Cursor_Pos draft 0x004994D0 whose unit sees a light MouseClass.
Cell Native_Radar_Cell_At(Point2D const &xy)
{
    return Map.unknown_slot51(xy);
}
