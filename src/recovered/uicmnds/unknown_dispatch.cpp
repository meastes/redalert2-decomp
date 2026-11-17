// void unknown_dispatch() -- 0x006F6490
// Purpose: runs 0x00614AA0 on the object 0x00617A00 returns, or, when
// there is none, 0x0042E990(-1, -1) on the object at 0x0084E8D0.
// Assumptions: the objects are unidentified; /O1 like its neighbours.

struct UnknownTarget_006f6490 {
    void unknown_call_614aa0();             // 0x00614AA0
};
struct UnknownFallback_006f6490 {
    void unknown_call_42e990(int arg1, int arg2);   // 0x0042E990
};
UnknownTarget_006f6490 *unknown_find_617a00();      // 0x00617A00
extern UnknownFallback_006f6490 BeaconManager;

void unknown_dispatch()
{
    UnknownTarget_006f6490 *target = unknown_find_617a00();
    if (target != 0) {
        target->unknown_call_614aa0();
        return;
    }
    BeaconManager.unknown_call_42e990(-1, -1);
}
