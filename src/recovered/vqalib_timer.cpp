// vqalib_timer.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_timer.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"


// void __fastcall VQA_SetTimer(VQAHandleP *vqap, long time) -- 0x0077CB20
// Purpose: sets the movie clock (60 ticks per second from the timer callback
// and refresh rate, offset by TickOffset) so that it reads `time` now.

void __fastcall VQA_SetTimer(VQAHandleP *vqap, long time)
{
    vqap->TickOffset = 0;
    unsigned long curtime = VQA_GetTime(vqap);
    vqap->TickOffset = time - curtime;
}


// void __fastcall VQA_StepTimer(VQAHandleP *vqap, long step) -- 0x0077CB60

void __fastcall VQA_StepTimer(VQAHandleP *vqap, long step)
{
    vqap->TickOffset += step;
}


// unsigned long __fastcall VQA_GetTime(VQAHandleP *vqap) -- 0x0077CB70

unsigned long __fastcall VQA_GetTime(VQAHandleP *vqap)
{
    unsigned long ticks = (vqap->Config.TimerCallback((VQAHandle *)vqap) * 60) / vqap->Config.RefreshRate;
    ticks += vqap->TickOffset;
    return ticks;
}


// int __fastcall VQA_Unknown_0077CBA0(VQAHandle *vqa) -- 0x0077CBA0
// Purpose: VQA_GetMovieTime (TS): the clock of a public handle.

int __fastcall VQA_Unknown_0077CBA0(VQAHandle *vqa)
{
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQAConfig *config = &vqap->Config;
    unsigned long rate = config->RefreshRate;
    unsigned long ticks = (config->TimerCallback((VQAHandle *)vqap) * 60) / rate;
    ticks += vqap->TickOffset;
    return ticks;
}
