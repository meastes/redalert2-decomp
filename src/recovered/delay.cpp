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


// DelayTimerClass::Is_Count_Up_Active (0x00496A90): a timed change is running in reverse (mischa85 names.csv:
// DelayTimerClass::Is_Count_Up_Active).

bool DelayTimerClass::Is_Count_Up_Active() const
{
    if (IsActive && IsToCountUp) {
        return true;
    }
    return false;
}


// DelayTimerClass::Is_Count_Down_Active (0x00496AB0): a timed change is running forward (DelayTimerClass::Is_Count_Down_Active).

bool DelayTimerClass::Is_Count_Down_Active() const
{
    if (IsActive && !IsToCountUp) {
        return true;
    }
    return false;
}


bool DelayTimerClass::Has_Ended() const
{
    if (IsActive && Timer.TotalTime) {
        if ((double)(Timer.TotalTime - Timer.Value()) / Timer.TotalTime != 1.0) return false;
    }
    return true;
}


// DelayTimerClass::Is_Count_Up_Complete (0x00496B30): the reverse change has finished (DelayTimerClass::Is_Count_Up_Complete).

bool DelayTimerClass::Is_Count_Up_Complete() const
{
    if (!IsActive && IsToCountUp == true) {
        return true;
    }
    return false;
}


// DelayTimerClass::Is_Count_Down_Complete (0x00496B50): the forward change has finished (DelayTimerClass::Is_Count_Down_Complete).

bool DelayTimerClass::Is_Count_Down_Complete() const
{
    if (!IsActive && !IsToCountUp) {
        return true;
    }
    return false;
}


void DelayTimerClass::Count_Up(double time)
{
    if (IsActive || IsToCountUp != true) {
        time *= 900.0;
        IsActive = true;
        Duration = time;
        Timer = UnknownDurationTimer423F60((long)time);
        IsToCountUp = true;
    }
}


void DelayTimerClass::Count_Down(double time)
{
    if (IsActive || IsToCountUp != false) {
        time *= 900.0;
        IsActive = true;
        Duration = time;
        Timer = UnknownDurationTimer423F60((long)time);
        IsToCountUp = false;
    }
}


void DelayTimerClass::Reverse()
{
    if (IsActive) {
        Timer.Reverse();
        IsToCountUp = !IsToCountUp;
    }
}


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
