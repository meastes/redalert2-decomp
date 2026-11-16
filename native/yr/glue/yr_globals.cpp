// The class-typed YR globals no unit of the YR tree defines (lane Y2): their storage comes from
// native/yr/tools/yr_data.py (zero, sized for the native layout) and gamemd.exe constructed them
// in compiler-made static initialisers (startup.cpp's SYNTHETIC entries, not in the tree). The
// native build constructs them here, after the original data is in place and before the YR
// units' own initialisers (yr_host_glue.cpp), with each class's default constructor.
#include "always.h"
#include "ccini.h"
#include "random.h"
#include "cell.h"
#include <new>

extern CCINIClass INI_RA2MD;     // 0x008870C0
extern CCINIClass INI_AI;        // 0x00887128
extern CCINIClass INI_UIMD;      // 0x00887208
extern Random2Class NonCriticalRandomNumber;
extern CellClass BlubCell;

// yr_data.py gives each at least twice its original extent (0x68 bytes for a CCINIClass).
static_assert(sizeof(CCINIClass) <= 2 * 0x68, "CCINIClass outgrows its storage");

void yr_construct_globals()
{
	new (&INI_AI) CCINIClass;
	new (&INI_UIMD) CCINIClass;
	new (&INI_RA2MD) CCINIClass;
	new (&NonCriticalRandomNumber) Random2Class;
	new (&BlubCell) CellClass;
}

// The radio's default message parameter (0x00A8EC30, .bss): pointer-sized natively, because the
// radio passes object pointers through it (fixups.json, lane Y6), so defined here rather than
// given 4 bytes of storage by yr_data.py.
intptr_t LParam = 0;
