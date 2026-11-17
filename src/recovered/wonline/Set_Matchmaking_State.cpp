// WOL matchmaking state transition, 0x00759DF0 (39 bytes plus nine frozen NOPs).
// State/result and timeout are separate 32-bit words; entering a state refreshes its timestamp.
#include "shared/crt_time.hpp"
extern int unknown_b2ac2c;
extern int unknown_b26898;
extern int unknown_b2ab88;
extern long unknown_b2a8a4;
void __fastcall Set_Matchmaking_State_00759df0(int state,int timeout,int result)
{
    unknown_b2ac2c=result;
    unknown_b26898=state;
    unknown_b2ab88=timeout;
    unknown_b2a8a4=time(0);
    return;
}
