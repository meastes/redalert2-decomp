// void __fastcall Command_Planning_Mode_On(bool arg) (0x006F64D0)
// Unless the flag 0x00A3D548 is set: 0x00614760, then 0x00616EC0 (arg, true).

extern int MuteSWLaunches;
void Unknown_00614760();
void __fastcall Unknown_00616EC0(bool arg1, bool arg2);

void __fastcall Command_Planning_Mode_On(bool arg)
{
    if (MuteSWLaunches == 0) {
        Unknown_00614760();
        Unknown_00616EC0(arg, true);
    }
}
