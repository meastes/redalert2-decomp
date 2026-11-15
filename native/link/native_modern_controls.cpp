// Modern controls (native addition, not in the original; lane C16, user 2026-11-14; the
// Enhancements page's "Controls: Classic / Modern", Classic by default and in the Original
// preset). Written from the behaviour the design reference describes (redalert2-recomp
// docs/modern-controls.md); no code taken from it, and the hooks are our own: native fixups
// (port/fixups.json) in ScrollClass::Message_Handler 0x0066A400 (the map's mouse buttons),
// DisplayClass::Mouse_Left_Release 0x0049BB20 (the selection band's walk) and
// RTacticalClass::Action 0x0062FE30 (the radar) ask the functions below. Classic answers
// every question with the original's own value, so with Classic the game runs as before.
//
// Only local input decisions change: which of the game's own actions a click resolves to
// (DisplayClass::Mouse_Left_Release still performs it, and the orders it sends are the
// original's events), which objects are selected (a local state) and where the view is.
//
// Modern, on the map (no placement, sell, repair, power, super weapon, waypoint or beacon
// mode: those keep the original's clicks, the right click cancelling them):
//   left click on a selectable object   selects it (never an order); on one already in a
//                                       group of several, the group becomes that one; with
//                                       Shift it is added or removed (the original's toggle
//                                       action 8)
//   left click on open ground or on an  clears the selection without an order; with Shift
//   object that cannot be selected      the selection stays
//   left double click on a unit         selects the player's units of its type on the
//                                       screen (the click with the type-select key held:
//                                       0x006F71E0); with Shift they are added
//   left drag                           the selection band, as in the original (Shift adds:
//                                       Mouse_Left_Release reads Shift); with Shift over
//                                       objects that are all selected already, they are
//                                       removed instead
//   right click                         the order the original's left click would give
//                                       there (ScrollClass::What_Action, performed by
//                                       Mouse_Left_Release); a click that would only select
//                                       does nothing: a right click never selects. On a
//                                       selected unit that deploys it is its deploy, on a
//                                       selected factory its primary (the original's own
//                                       "self" action)
//   right drag                          scrolls, as in the original
// With Ctrl or Alt held (force fire, force move; Ctrl and Shift: attack-move) a left click
// keeps the original's meaning, so those keys order with either button, and the touch
// toolbar's Fire, Move and A-Move modes (which hold them) still order with a tap.
// On the radar, Modern swaps the buttons: a left click centres the view there, a right
// click gives the order (the radar's gadget sees no right release, so the order is given at
// the right press).
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/map_view.hpp"
#include "shared/tactical.hpp"
#include "shared/techno.hpp"
#include "shared/object.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/keyboard.hpp"
#include <stdio.h>

void __cdecl Clear_Selected_Objects_00482bc0();         // 0x00482BC0 every selected object's Unselect
void __fastcall Unknown_0049C3F0(ObjectClass *object);  // 0x0049C3F0 the selection band's select

extern "C" long long ra2port_now_ms(void);
// The setting (platform/sdl/enhancements.cpp). Weak default: Classic.
extern "C" __attribute__((weak)) int ra2_enh_modern_controls(void) { return 0; }

namespace {

constexpr KeyNumType kShift = 0x10, kCtrl = 0x11, kAlt = 0x12;   // VK_SHIFT, VK_CONTROL, VK_MENU
constexpr int kNone = 0, kSelect = 7, kToggleSelect = 8;        // the original's actions (Mouse_Left_Release)
constexpr int kKindBuilding = 6;
constexpr long long kDoubleClickMs = 500;                       // Windows' default double-click time

bool Held(KeyNumType key) { return WWKeyboard != 0 && WWKeyboard->Down(key); }

// A mode in which the original's clicks stay (DisplayClass::Mouse_Right_Release cancels
// each, in its order: a techno waiting for placement, repair, sell, power, a super weapon's
// target, the waypoint mode and the beacon mode), or a type waiting for placement.
bool In_Mode()
{
    ObjectClass *pending = Map.unknown_11a0;
    return (pending != 0 && pending->AbstractFlags.IsTechno) || Map.unknown_11a4 != 0 || Map.unknown_11ac
           || Map.unknown_11ad || Map.unknown_11ae || Map.unknown_11b4 != -1 || Map.unknown_11af || Map.unknown_11b0;
}

bool Selectable(ObjectClass *object) { return object != 0 && object->CanBeSelected(); }

// The last plain left click on an object (the double click's first half).
ObjectClass *last_object = 0;
long long last_ms = 0;
ObjectClass *type_select = 0;    // a double click's object: Native_Modern_Left_Done selects its type

// A unit of the player's (not a building) whose type a double click selects.
bool Type_Selectable(ObjectClass *object)
{
    if (!Selectable(object) || !object->AbstractFlags.IsTechno || object->Kind_Of() == kKindBuilding) return false;
    HouseClass *house = object->Owning_House();
    return house != 0 && house->Is_Player_Control();
}

// The objects the selection band's select (0x0049C3F0) takes: one of the player's
// selectable objects in play, a building only when it undeploys and is not a
// construction yard type.
bool Band_Takes(ObjectClass *object)
{
    HouseClass *house = object->Owning_House();
    if (house == 0 || !house->Is_Player_Control() || !object->CanBeSelected() || object->unknown_76) return false;
    if (object->Kind_Of() == kKindBuilding) {
        BuildingTypeClass *type = static_cast<BuildingClass *>(object)->Type;
        if (type->UndeploysInto == 0) return false;
        for (int i = 0; i < Rules->unknown_7c8.Count(); ++i)
            if (Rules->unknown_7c8[i] == type) return false;
    }
    return true;
}
int band_objects = 0, band_unselected = 0;
void __fastcall Count_Band(ObjectClass *object)
{
    if (!Band_Takes(object)) return;
    ++band_objects;
    if (!object->unknown_77) ++band_unselected;
}
void __fastcall Unselect_Band(ObjectClass *object)
{
    if (Band_Takes(object) && object->unknown_77) object->Unselect();
}

}  // namespace

// The left button released over the map (ScrollClass::Message_Handler, WM_LBUTTONUP): the
// action Mouse_Left_Release performs for the original's action there. May clear the
// selection or the object (a double click: Native_Modern_Left_Done selects the type).
int Native_Modern_Left_Up(Cell const &cell, ObjectClass *&object, int action)
{
    (void)cell;
    type_select = 0;
    if (!ra2_enh_modern_controls() || In_Mode() || Held(kCtrl) || Held(kAlt)) return action;
    bool shift = Held(kShift);
    if (Map.unknown_11cb) {
        // The selection band: Mouse_Left_Release walks it. When it took nothing, the
        // original goes on as a click at the release point; Modern orders nothing there.
        last_object = 0;
        return Selectable(object) ? action == kToggleSelect ? kToggleSelect : kNone : kNone;
    }
    if (!Selectable(object)) {
        last_object = 0;
        if (!shift) Clear_Selected_Objects_00482bc0();
        object = 0;
        return kNone;
    }
    long long now = ra2port_now_ms();
    bool twice = object == last_object && now - last_ms <= kDoubleClickMs;
    last_object = twice ? 0 : object;
    last_ms = now;
    if (twice && Type_Selectable(object)) {
        if (!shift) Clear_Selected_Objects_00482bc0();
        type_select = object;
        object = 0;
        return kNone;
    }
    if (shift) return kToggleSelect;
    if (object->unknown_77 && unknown_objects_a40c60.Count() > 1) return kSelect;   // the group becomes this one
    return kNone;    // selects it unless selected (Mouse_Left_Release's action 0)
}

// After Mouse_Left_Release: a double click selects the player's units of the object's type on
// the screen, as a click does while the type-select key is held (0x006F71E0).
void Native_Modern_Left_Done()
{
    ObjectClass *object = type_select;
    type_select = 0;
    if (object == 0) return;
    Unknown_006F71E0(object->Class_Of()->ID);
    fprintf(stderr, "modern-controls: double click: %d selected\n", unknown_objects_a40c60.Count());
}

// The right button released over the map (WM_RBUTTONUP): true when Modern takes it as an
// order (no scroll ran, no mode is on); false sends it to Mouse_Right_Release as before.
bool Native_Modern_Right_Order(bool scrolled)
{
    return ra2_enh_modern_controls() && !scrolled && !In_Mode();
}

// The action a Modern right click performs for the original's left-click action there:
// none when that would only select.
int Native_Modern_Right_Action(ObjectClass *object, int action)
{
    (void)object;
    last_object = 0;
    if (action == kNone || action == kSelect || action == kToggleSelect) return kNone;
    return action;
}

// The selection band's walk (Mouse_Left_Release): with Shift over objects that are all
// selected already, they are unselected instead of selected.
void (__fastcall *Native_Modern_Band_Walk(void (__fastcall *select)(ObjectClass *)))(ObjectClass *)
{
    if (!ra2_enh_modern_controls() || !Held(kShift) || unknown_tactical_83996c == 0) return select;
    // A dry run over the band (the walk clears its corner: put it back for the real one).
    Point2D start = unknown_tactical_83996c->RubberBandStart;
    band_objects = band_unselected = 0;
    unknown_tactical_83996c->Unknown_006AA610(Count_Band);
    unknown_tactical_83996c->RubberBandStart = start;
    if (band_objects > 0 && band_unselected == 0) {
        fprintf(stderr, "modern-controls: shift band: %d selected objects unselected\n", band_objects);
        return Unselect_Band;
    }
    return select;
}

// The radar's mouse flags (RTacticalClass::Action): Modern swaps the buttons. The left press
// is taken as the right press (the original centres the view on it); the left release as the
// right release (nothing); the right press as the left release (the original's order).
unsigned Native_Modern_Radar_Flags(unsigned flags)
{
    if (!ra2_enh_modern_controls()) return flags;
    unsigned out = flags & ~0x55u;
    if (flags & 0x01) out |= 0x10;
    if (flags & 0x04) out |= 0x40;
    if (flags & 0x10) out |= 0x04;
    if (flags & 0x40) out |= 0x01;
    return out;
}
