// void Unknown_Stopwatch_Start_006F7650() (0x006F7650)
// Starts a stopwatch: once, flag 0x00AC133D is raised and timeGetTime is stamped at 0x00AC1340.

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();
extern bool unknown_flag_ac133d;
extern unsigned long unknown_time_ac1340;

void Unknown_Stopwatch_Start_006F7650()
{
    if (!unknown_flag_ac133d) {
        unknown_flag_ac133d = true;
        unknown_time_ac1340 = timeGetTime();
    }
}
