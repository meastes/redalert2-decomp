// void __fastcall Command_Planning_Mode_Off(bool arg) (0x006F64F0)
// 0x006147B0, then unless the flag 0x00A3D548 is set 0x00616EC0 (arg, false).

extern int MuteSWLaunches;
void Unknown_006147B0();
void __fastcall Unknown_00616EC0(bool arg1, bool arg2);

void __fastcall Command_Planning_Mode_Off(bool arg)
{
    Unknown_006147B0();
    if (MuteSWLaunches == 0) {
        Unknown_00616EC0(arg, false);
    }
}
