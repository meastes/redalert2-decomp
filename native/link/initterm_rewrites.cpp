// Native rewrites of the explicit _initterm entries whose accepted sources are
// seed/scaffold text (absolute-address stores, truncated function pointers):
// native/tools/native_units.py calls Initterm_<ENTRY>() at the entry's original
// position when this file defines it. Behaviour is the scaffold's, stated on
// the recovered names; not original code, and each entry stays unrecovered in
// the exact tree until its real initialiser is accepted.
//
// Natively every global starts zero (as the loader gave .bss), so an entry
// that only zeroes storage nobody names has no observable effect; those are
// written as empty functions with the stores they stand for.
#include "shared/crt_memory.hpp"
#include "shared/rect.hpp"
#include "shared/system_timer.hpp"
#define RA2_CDTIMER_BASIC_BASE
#include "shared/cd_timer.hpp"
#include "shared/application.hpp"
#include "shared/win_util.hpp"
#include "shared/wol_array_list.hpp"

extern "C" int __cdecl atexit(void (__cdecl *function)(void));
extern ApplicationClass unknown_application_object_84c7dc;           // 0x0084C7DC
extern BasicTimerClass<SystemTimerClass> GameTimer;                  // 0x00ABD4C0
extern CDTimerClass<SystemTimerClass> unknown_timer_b292e8;          // 0x00B292E8
extern int unknown_radar_event_word_ab67b8;                          // 0x00AB67B8 (two words)
extern WolStringArray unknown_strings_b29828;                        // 0x00B29828
void Destroy_Wonline_Strings_0073bd40();                             // 0x0073BD40
extern int buddy_column_007fd308, buddy_column_007fd30c;             // 0x007FD308..
extern int buddy_column_007fd318, buddy_column_007fd31c;
extern int buddy_column_b29460, buddy_column_b26cb4;

namespace OwnerDraw {
struct UnknownTooltipState {
    Rect rect_00;
    void *surface_10;
    unsigned short text_14[128];
    int shown_114;
    int drawn_118;
    int unknown_11c;
};
extern UnknownTooltipState unknown_tooltip_a737f0;                    // 0x00A737F0
}

// Header statics: four dwords zeroed at an address no recovered source names
// (a Rect(0, 0, 0, 0) after the unit header's named statics). Each native unit
// carries its own copy of its header statics, so there is nothing to store.
#define RA2_ZERO_STATIC(entry, address) \
    void Initterm_##entry() { /* *(Rect *)address = Rect(0, 0, 0, 0): unnamed, unit-local */ }
RA2_ZERO_STATIC(0040BC60, 0x00832370)
RA2_ZERO_STATIC(0040F020, 0x00839A48)
RA2_ZERO_STATIC(0040FEF0, 0x00839C78)
RA2_ZERO_STATIC(00411400, 0x00839D60)
RA2_ZERO_STATIC(00423F70, 0x0084C710)
RA2_ZERO_STATIC(00431020, 0x0084E9C0)
RA2_ZERO_STATIC(00432270, 0x0084E9D8)
RA2_ZERO_STATIC(00434920, 0x0084EA68)
RA2_ZERO_STATIC(00468F30, 0x00850718)
RA2_ZERO_STATIC(0047D130, 0x00850C48)
RA2_ZERO_STATIC(00480B00, 0x00850D00)
RA2_ZERO_STATIC(004955B0, 0x00851CB8)
RA2_ZERO_STATIC(00496D10, 0x00852468)
RA2_ZERO_STATIC(004981A0, 0x008524A0)
RA2_ZERO_STATIC(004BBEE0, 0x00865DB0)
RA2_ZERO_STATIC(0050B8B0, 0x00A411B8)
RA2_ZERO_STATIC(00522540, 0x00A51FC0)
RA2_ZERO_STATIC(005383A0, 0x00A6E888)
RA2_ZERO_STATIC(00595170, 0x00A70CE0)
RA2_ZERO_STATIC(005B5DC0, 0x00A71918)
RA2_ZERO_STATIC(0060D0F0, 0x00A765A0)
RA2_ZERO_STATIC(00698740, 0x00ABD490)
RA2_ZERO_STATIC(006EB2C0, 0x00AC0BE8)
RA2_ZERO_STATIC(00734100, 0x00B25BA0)
RA2_ZERO_STATIC(00735670, 0x00B25DB8)
RA2_ZERO_STATIC(00736210, 0x00B25F18)

// Empty bodies in the original (a static with a trivial constructor).
void Initterm_0040F040() {}
void Initterm_0040F050() {}
void Initterm_0040F060() {}
void Initterm_00610760() {}
void Initterm_0063B5A0() {}

// 0x00426B00: the COM application object's constructor only installs its
// vtable (ApplicationClass, ??_7ApplicationClass@@6B@).
void Initterm_00426B00()
{
    ApplicationClass constructed;
    memcpy((void *)&unknown_application_object_84c7dc, (void *)&constructed, sizeof(constructed));
}

// 0x005EDF60: OwnerDraw::UnknownTooltipState's constructor for the tooltip
// state: empty rectangle, no surface, empty text, not shown or drawn.
void Initterm_005EDF60()
{
    OwnerDraw::UnknownTooltipState &tip = OwnerDraw::unknown_tooltip_a737f0;
    memset(&tip.rect_00, 0, sizeof(tip.rect_00));
    tip.surface_10 = 0;
    tip.text_14[0] = 0;
    tip.shown_114 = 0;
    tip.drawn_118 = 0;
    tip.unknown_11c = 0;
}

// 0x0063B590: both radar-event words cleared.
void Initterm_0063B590() { unknown_radar_event_word_ab67b8 = 0; }

// 0x0065B4B0: a CDTimerClass<SystemTimerClass> at 0x00AB6EA8 that no recovered
// source names (Started = the system timer, accumulated 0; destructor 0x0065B4E0
// registered with atexit, empty). Nothing to construct natively.
void Initterm_0065B4B0() {}

// 0x00698CD0: GameTimer (BasicTimerClass<SystemTimerClass>: Started = the system
// timer; its empty destructor 0x00698CF0 registered with atexit).
void Initterm_00698CD0()
{
    BasicTimerClass<SystemTimerClass> constructed;
    memcpy((void *)&GameTimer, (void *)&constructed, sizeof(constructed));
}

// 0x00735D70: the font cache starts empty; 0x00735D90 clears it at exit.
void Initterm_00735D70()
{
    memset(&unknown_font_cache_b25dc8, 0, sizeof(unknown_font_cache_b25dc8));
    atexit(unknown_clear_font_cache_735d90);
}

// 0x0073BC80 / 0x0073BCA0: widths of two buddy-list columns from their edges.
void Initterm_0073BC80() { buddy_column_b29460 = buddy_column_007fd30c - buddy_column_007fd308 - 4; }
void Initterm_0073BCA0() { buddy_column_b26cb4 = buddy_column_007fd31c - buddy_column_007fd318 - 4; }

// 0x0073BD20: the WOL string list starts empty; 0x0073BD40 destroys it at exit.
void Initterm_0073BD20()
{
    memset((void *)&unknown_strings_b29828, 0, sizeof(unknown_strings_b29828));
    atexit(Destroy_Wonline_Strings_0073bd40);
}

// 0x0073BFC0: unknown_timer_b292e8 (CDTimerClass<SystemTimerClass>: Started =
// the system timer, DelayTime 0; empty destructor 0x0073BFF0 at exit).
void Initterm_0073BFC0()
{
    CDTimerClass<SystemTimerClass> constructed;
    memcpy((void *)&unknown_timer_b292e8, (void *)&constructed, sizeof(constructed));
}

// 0x00769390 / 0x007693D0: one dword each (0x00B2AD60, 0x00B2AD70) zeroed and a
// destructor (0x007693B0, 0x007693F0) registered; no recovered source names
// either object. Nothing to construct natively.
void Initterm_00769390() {}
void Initterm_007693D0() {}
