// getcpu.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/getcpu.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cpu_info.hpp"
#include "shared/crt_string.hpp"


// CPU identification (0x004D2A20): runs the original handwritten CPUID probes,
// then reports the family byte and copies the unavailable-feature text.


extern unsigned char unknown_cpu_probe_77f14d();
extern void unknown_cpu_family_probe_77f20f();
extern char unknown_cpu_family_8015a3;
extern char unknown_cpu_text_8015a5[];
extern "C" char *__cdecl strncpy(char *, const char *, unsigned int);

void __fastcall Get_CPU_Info(int *family, unsigned char *has_cpuid, char *text, unsigned int size)
{
    *has_cpuid = unknown_cpu_probe_77f14d();
    unknown_cpu_family_probe_77f20f();
    *family = unknown_cpu_family_8015a3;
    if (text != 0) {
        strncpy(text, unknown_cpu_text_8015a5, size);
    }
}


// int Get_CPU_Type() (0x004D2A60): the CPU type index from the CPUID family and model and the
// vendor text (Get_CPU_Info inlined): -1 unknown, 0 486, 1 Pentium, 2 Pentium Pro, 3 Pentium II,
// 4 Pentium III, 5 Celeron, 6 Pentium 4, 7 AMD, 8 Cyrix (the names table 0x007D8838).


extern char unknown_cpu_model_8015a4;

int Get_CPU_Type()
{
    int family;
    unsigned char has_cpuid;
    char vendor[64];
    Get_CPU_Info(&family, &has_cpuid, vendor, sizeof(vendor));
    if (family == 0 || unknown_cpu_model_8015a4 == 0) {
        return -1;
    }
    if (_stricmp(vendor, "AuthenticAMD") == 0) {
        return 7;
    }
    if (_stricmp(vendor, "CyrixInstead") == 0) {
        return 8;
    }
    switch (family) {
    case 4:
        return 0;
    case 5:
        return 1;
    case 6:
        switch (unknown_cpu_model_8015a4) {
        case 3:
        case 5:
            return 3;
        case 6:
            return 5;
        case 7:
        case 8:
        case 10:
            return 4;
        default:
            return 2;
        }
    default:
        return 6;
    }
}
