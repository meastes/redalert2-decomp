
// ---------------------------------------------------------------------------------------------
// Native glue (lane Y2), appended to the uicmnds2 unit. StringTable::LoadString (0x00734E60) is
// defined here after `#undef LoadString`; units that include windows.h first spell the calls
// LoadStringA (the Win32 macro), as VC6 did without harm (one TU's view of the name). The native
// link gives the one definition both spellings.
// ---------------------------------------------------------------------------------------------
__asm__(".globl __ZN11StringTable11LoadStringAEPKcPPcS1_i\n"
        "__ZN11StringTable11LoadStringAEPKcPPcS1_i = __ZN11StringTable10LoadStringEPKcPPcS1_i");
