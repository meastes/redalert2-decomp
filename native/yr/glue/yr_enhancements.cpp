// Native additions for yr-native (lane Y8; not in the original game): the Enhancements settings
// that RA2's native/link implements against RA2's classes, written again against the YR tree's
// classes and hooked by native fixups (yr/fixups.json). The settings are the host's
// (platform/sdl/enhancements.cpp, ra2_enh_*: the app's own settings file); each is off by
// default and in the Original preset, and off answers every question with the original's value.
//
// Queue five with Shift (RA2: link/native_queue_five.cpp, lane C16). A left click on a build icon
// (SidebarClass::StripClass::SelectClass::Action, gamemd.exe 0x006AAD00, glue/sidebar_native.inc)
// adds the game's own PRODUCE event; with the setting on and Shift held it is added five times, as
// five clicks would add it, stopping when OutList is full. A right click with Shift needs nothing:
// Yuri's Revenge's own click already abandons every queued copy (ABANDON_ALL, the click's
// modifier), where RA2's setting removes five.
//
// Controls: Classic / Modern (RA2: link/native_modern_controls.cpp, lane C16; the same rules,
// see there). Modern, on the map outside the placement, repair, sell, power, super weapon,
// waypoint and beacon modes: a left click selects (never an order), on open ground it clears the
// selection (Shift keeps it), Shift toggles a unit, a click on a unit of a group makes it the
// selection, a double click selects the player's units of its type (YR's type select, the T key's
// UICommands 0x007327D0); the band as in the original, with Shift over objects all selected
// already it unselects them; a right click gives the order the original's left click would give
// there (ScrollClass::What_Action, performed by DisplayClass::Mouse_Left_Release, so the events
// are the original's) and never selects; the right drag still scrolls. Ctrl or Alt held keeps the
// original's left click. On the radar the buttons swap. Hooks (yr/fixups.json): ScrollClass::
// Message_Handler (gamemd.exe 0x006930A0) WM_LBUTTONUP and WM_RBUTTONUP, DisplayClass::
// Mouse_Left_Release (0x004AB9B0) the band's walk, RadarClass::RTacticalClass::Action
// (0x006539D0) the flags. The touch tap across player-controlled houses (lane C18) is RA2's
// campaign case (Allied mission 2's Tanya); YR keeps the original's move there.
#include "always.h"
#include "geometry.h"
#include "display.h"
#include "_keyboar.h"
#include "_map.h"
#include "_tactica.h"
#include "abstype.h"
#include "event.h"
#include "globals.h"
#include "house.h"
#include "keyboard.h"
#include "object.h"
#include "objtype.h"
#include "queue.h"
#include "tactical.h"
#include "conquer.h"
#include <stdio.h>

namespace Game { void __fastcall UICommands_TypeSelect_7327D0(char const * name); }

extern "C" long long ra2port_now_ms(void);
// The settings (platform/sdl/enhancements.cpp). Weak defaults: the original behaviour.
extern "C" __attribute__((weak)) int ra2_enh_queue_five(void) { return 0; }
extern "C" __attribute__((weak)) int ra2_enh_modern_controls(void) { return 0; }

namespace {

constexpr unsigned short kShift = 0x10, kCtrl = 0x11, kAlt = 0x12;   // VK_SHIFT, VK_CONTROL, VK_MENU
constexpr int kQueueFive = 5;
constexpr long long kDoubleClickMs = 500;                           // Windows' default double-click time

bool Held(unsigned short key) { return Keyboard != NULL && Keyboard->Down((KeyNumType)key); }

// A mode in which the original's clicks stay (DisplayClass::Mouse_Right_Release cancels each).
bool In_Mode()
{
    ObjectClass *pending = Map.PendingObjectPtr;
    return (pending != NULL && pending->Is_Techno()) || Map.PendingObject != NULL || Map.IsRepairMode || Map.IsSellMode
           || Map.IsPowerMode || Map.IsTargettingMode != SUPER_NONE || Map.IsWaypointMode || Map.PlaceBeaconMode;
}

bool Selectable(ObjectClass *object) { return object != NULL && object->CanBeSelected(); }

ObjectClass *last_object = NULL;
long long last_ms = 0;
ObjectClass *type_select = NULL;    // a double click's object: Yr_Modern_Left_Done selects its type

bool Type_Selectable(ObjectClass *object)
{
    if (!Selectable(object) || !object->Is_Techno() || object->Fetch_RTTI() == RTTI_BUILDING) return false;
    HouseClass *house = object->Owner_HouseClass();
    return house != NULL && house->Is_Player_Control();
}

// What the band's select (Bandbox_Selection_Callback 0x004AC2B0) takes.
bool Band_Takes(ObjectClass *object)
{
    HouseClass *house = object->Owner_HouseClass();
    return house != NULL && house->Is_Player_Control() && object->CanBeSelected() && object->CanBeSelectedNow()
           && !object->IsInLimbo;
}
int band_objects = 0, band_unselected = 0;
void Count_Band(ObjectClass *object)
{
    if (!Band_Takes(object)) return;
    ++band_objects;
    if (!object->IsSelected) ++band_unselected;
}
void Unselect_Band(ObjectClass *object)
{
    if (Band_Takes(object) && object->IsSelected) object->Unselect();
}

}  // namespace

// The sidebar's PRODUCE event (glue/sidebar_native.inc): five copies with the setting on and
// Shift held. Answers OutList.Add's result for the first copy.
bool Yr_Queue_Five_Add(EventClass const &event)
{
    bool added = OutList.Add(event);
    if (!added || !ra2_enh_queue_five() || !Held(kShift)) return added;
    int copies = 1;
    while (copies < kQueueFive && OutList.Add(event)) ++copies;
    fprintf(stderr, "queue-five: event %d added %d times\n", (int)event.Type, copies);
    return added;
}

// The left button released over the map (WM_LBUTTONUP): the action Mouse_Left_Release performs
// for the original's action there (band: the selection band is being dragged, DisplayClass::
// IsRubberBand). May clear the selection or the object.
ActionType Yr_Modern_Left_Up(ObjectClass *&object, ActionType action, bool band)
{
    type_select = NULL;
    if (!ra2_enh_modern_controls() || In_Mode() || Held(kCtrl) || Held(kAlt)) return action;
    bool shift = Held(kShift);
    if (band) {
        // Mouse_Left_Release walks the band; when it took nothing the original goes on as a click
        // at the release point; Modern orders nothing there.
        last_object = NULL;
        return Selectable(object) && action == ACTION_TOGGLE_SELECT ? ACTION_TOGGLE_SELECT : ACTION_NONE;
    }
    if (!Selectable(object)) {
        last_object = NULL;
        if (!shift) Unselect_All();
        object = NULL;
        return ACTION_NONE;
    }
    long long now = ra2port_now_ms();
    bool twice = object == last_object && now - last_ms <= kDoubleClickMs;
    last_object = twice ? NULL : object;
    last_ms = now;
    if (twice && Type_Selectable(object)) {
        if (!shift) Unselect_All();
        type_select = object;
        object = NULL;
        return ACTION_NONE;
    }
    if (shift) return ACTION_TOGGLE_SELECT;
    if (object->IsSelected && CurrentObject.Count() > 1) return ACTION_SELECT;   // the group becomes this one
    return ACTION_NONE;    // selects it unless selected (Mouse_Left_Release's ACTION_NONE)
}

// After Mouse_Left_Release: a double click selects the player's units of the object's type on
// the screen, as YR's type select does.
void Yr_Modern_Left_Done()
{
    ObjectClass *object = type_select;
    type_select = NULL;
    if (object == NULL) return;
    Game::UICommands_TypeSelect_7327D0(object->Class_Of()->IniName);
    fprintf(stderr, "modern-controls: double click: %d selected\n", CurrentObject.Count());
}

// The right button released over the map (WM_RBUTTONUP): true when Modern takes it as an order
// (no scroll ran, no mode is on); false sends it to Mouse_Right_Release as before.
bool Yr_Modern_Right_Order(bool scrolled)
{
    return ra2_enh_modern_controls() && !scrolled && !In_Mode();
}

// The action a Modern right click performs for the original's left-click action there: none
// when that would only select.
ActionType Yr_Modern_Right_Action(ActionType action)
{
    last_object = NULL;
    if (action == ACTION_NONE || action == ACTION_SELECT || action == ACTION_TOGGLE_SELECT) return ACTION_NONE;
    fprintf(stderr, "modern-controls: right click: action %d\n", (int)action);
    return action;
}

// The band's walk (Mouse_Left_Release): with Shift over objects all selected already, they are
// unselected instead of selected.
void (*Yr_Modern_Band_Walk(void (*select)(ObjectClass *)))(ObjectClass *)
{
    if (!ra2_enh_modern_controls() || !Held(kShift) || TacticalMap == NULL) return select;
    // A dry run over the band (the walk may move its corner: put it back for the real one).
    Point2D start = TacticalMap->RubberBandStart;
    band_objects = band_unselected = 0;
    TacticalMap->Select_Rubber_Band(Count_Band);
    TacticalMap->RubberBandStart = start;
    if (band_objects > 0 && band_unselected == 0) {
        fprintf(stderr, "modern-controls: shift band: %d selected objects unselected\n", band_objects);
        return Unselect_Band;
    }
    return select;
}

// The radar's mouse flags (RTacticalClass::Action): Modern swaps the buttons (LEFTPRESS 0x01,
// LEFTRELEASE 0x04, RIGHTPRESS 0x10, RIGHTRELEASE 0x40).
unsigned Yr_Modern_Radar_Flags(unsigned flags)
{
    if (!ra2_enh_modern_controls()) return flags;
    unsigned out = flags & ~0x55u;
    if (flags & 0x01) out |= 0x10;
    if (flags & 0x04) out |= 0x40;
    if (flags & 0x10) out |= 0x04;
    if (flags & 0x40) out |= 0x01;
    return out;
}
