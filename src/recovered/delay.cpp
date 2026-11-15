/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/delay.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/delay.cpp where both define the function (names only).

// delay.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/delay.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/techno.hpp"


// Header-static dynamic initializers of the unit at 0x00496820: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00496820-0x00496A01); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00496840, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x00496870, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x00496890, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x004968B0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x004968D0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x004968F0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x00496910, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x00496950, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x00496980, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x004969B0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x004969E0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


// Dynamic initializer (0x004969F0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00496820.cpp; no source of its own.


void __fastcall Wait_Timer_00496A20(long ticks)
{
    CDTimerClass<SystemTimerClass> wait(ticks);
    while (wait.Value() != 0) {}
}


/// <summary>
/// Creates a delay timer in its idle state.
/// The new timer is not counting in either direction and reports itself as having
/// already ended, so an owner may safely query it before ever starting a count.
/// </summary>
DelayTimerClass::DelayTimerClass() : IsActive(false), IsToCountUp(false)
{
}


// 0x00496a10: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00496a10(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00496a10 {

void  FUN_00496a10(void)
{
    return;
}

}  // namespace seed_00496a10


/// <summary>
/// Is a count up currently running?
/// This is the query behind a door reporting itself as opening.
/// </summary>
/// <returns>bool; Is the timer running forwards?</returns>
// (0x00496A90)
// DelayTimerClass::Is_Count_Up_Active).

bool DelayTimerClass::Is_Count_Up_Active() const
{
    if (IsActive && IsToCountUp) {
        return true;
    }
    return false;
}


/// <summary>
/// Is a count down currently running?
/// This is the query behind a door reporting itself as closing.
/// </summary>
/// <returns>bool; Is the timer running backwards?</returns>
// (0x00496AB0)

bool DelayTimerClass::Is_Count_Down_Active() const
{
    if (IsActive && !IsToCountUp) {
        return true;
    }
    return false;
}


/// <summary>
/// Determines if the delay has run its course.
/// This routine is polled by the timer's own AI; when it answers true the count is
/// ended and the timer settles at whichever extreme it was heading for.
/// </summary>
/// <returns>bool; Is the timer either idle or expired?</returns>
bool DelayTimerClass::Has_Ended() const
{
    if (IsActive && Timer.TotalTime) {
        if ((double)(Timer.TotalTime - Timer.Value()) / Timer.TotalTime != 1.0) return false;
    }
    return true;
}


/// <summary>
/// Has the count up finished?
/// This is the query behind a door reporting itself fully open.
/// </summary>
/// <returns>bool; Is the timer idle at the top of its travel?</returns>
// (0x00496B30)

bool DelayTimerClass::Is_Count_Up_Complete() const
{
    if (!IsActive && IsToCountUp == true) {
        return true;
    }
    return false;
}


/// <summary>
/// Has the count down finished?
/// A timer that has never been started answers true, since a door that was never opened
/// is closed as far as the game is concerned.
/// </summary>
/// <returns>bool; Is the timer idle at the bottom of its travel?</returns>
// (0x00496B50)

bool DelayTimerClass::Is_Count_Down_Complete() const
{
    if (!IsActive && !IsToCountUp) {
        return true;
    }
    return false;
}


/// <summary>
/// Starts the timer counting up.
/// This routine is used to begin a delay that runs forwards -- a building door swinging
/// open, for example. The count runs until the duration is used up, at which point the
/// timer reports its count up as complete.
/// </summary>
/// <param name="duration">The length of the count, expressed in game minutes.</param>
void DelayTimerClass::Count_Up(double duration)
{
    if (IsActive || IsToCountUp != true) {
        duration *= 900.0;
        IsActive = true;
        Duration = duration;
        Timer = UnknownDurationTimer423F60((long)duration);
        IsToCountUp = true;
    }
}


/// <summary>
/// Starts the timer counting down.
/// This routine is used to begin a delay that runs backwards -- a building door swinging
/// shut, for example. The count runs until the duration is used up, at which point the
/// timer reports its count down as complete.
/// </summary>
/// <param name="duration">The length of the count, expressed in game minutes.</param>
void DelayTimerClass::Count_Down(double duration)
{
    if (IsActive || IsToCountUp != false) {
        duration *= 900.0;
        IsActive = true;
        Duration = duration;
        Timer = UnknownDurationTimer423F60((long)duration);
        IsToCountUp = false;
    }
}


/// <summary>
/// Reverses the direction of the running count.
/// Use this routine when something changes its mind part way through. A door told to
/// close while it is still opening backs out from wherever it happened to get to,
/// rather than snapping to the far end of its travel.
/// </summary>
/// <remarks>A timer that is not running is left alone.</remarks>
void DelayTimerClass::Reverse()
{
    if (IsActive) {
        Timer.Reverse();
        IsToCountUp = !IsToCountUp;
    }
}


/// <summary>
/// Fetches how far the delay has progressed.
/// This routine is used by the animation code that rides on the timer, so that it can
/// pick the stage of its sequence to display.
/// </summary>
/// <returns>Returns with the progress through the delay, from 0.0 to 1.0. An idle timer
/// reports itself as fully complete.</returns>
double DelayTimerClass::Percent_Complete() const
{
    if (IsActive) {
        return Timer.Percentage();
    }
    return 1.0;
}


// 0x00496c50: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_00496c50(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00496c50 {

void __fastcall FUN_00496c50(int timer_address)
{
    *(unsigned char *)(timer_address + 0x18) = 0;
    *(unsigned char *)(timer_address + 0x19) = 1;
    return;
}

}  // namespace seed_00496c50


// 0x00496c60: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_00496c60(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00496c60 {

void __fastcall FUN_00496c60(int timer_address)
{
    *(unsigned char *)(timer_address + 0x18) = 0;
    *(unsigned char *)(timer_address + 0x19) = 0;
    return;
}

}  // namespace seed_00496c60


// 0x00496ce0: function not yet identified
//
// Body: 38 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall End_Count_00496ce0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00496ce0 {

void __fastcall End_Count_00496ce0(int timer_address)
{
    if (*(char *)(timer_address + 0x18) != '\0') {
        if (*(char *)(timer_address + 0x19) != '\0') {
            *(unsigned char *)(timer_address + 0x18) = 0;
            *(unsigned char *)(timer_address + 0x19) = 1;
        }
        if ((*(char *)(timer_address + 0x18) != '\0') && (*(char *)(timer_address + 0x19) == '\0')) {
            *(unsigned char *)(timer_address + 0x18) = 0;
            *(unsigned char *)(timer_address + 0x19) = 0;
        }
    }
    return;
}

}  // namespace seed_00496ce0
