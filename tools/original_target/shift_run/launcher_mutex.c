/* Holds the named mutex Red Alert 2's launcher holds while the game runs (CopyProtection_Check_Already_Running
   0x4918F0 bails without it), then sleeps. Research helper for running the rebuilt executable under Wine. */
void * __stdcall CreateMutexA(void *attributes, int initial_owner, char const *name);
void __stdcall Sleep(unsigned long milliseconds);
int main(void)
{
    CreateMutexA(0, 0, "48BC11BD-C4D7-466b-8A31-C6ABBAD47B3E");
    for (;;) Sleep(60000);
    return 0;
}
