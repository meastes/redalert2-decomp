// getcd.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/getcd.json carries the addresses and the rows that
// stay in their own files.


#include "shared/get_cd.hpp"
#include "shared/debug_printf.hpp"
#include "shared/cd_control.hpp"
#include "shared/crt_string.hpp"


// GetCDClass::GetCDClass() (0x004D2940)
// Lists the CD-ROM drives: C: to Z: first; when there is none, A: and B:.

GetCDClass::GetCDClass()
{
    char root[4] = "a:\\";
    Count = 0;
    Cursor = 0;
    Debug_Printf("GetCDClass constructor\n");
    memset(Drives, 0xFF, 26);
    int upper = 'C';
    for (char letter = 'c'; letter <= 'z'; letter++, upper++) {
        root[0] = letter;
        if (GetDriveTypeA(root) == DRIVE_CDROM) {
            Drives[Count++] = upper - 'A';
            Debug_Printf("CD drive found - %c:\n", upper);
        }
    }
    if (Count == 0) {
        char letter = 'a';
        int index = 0;
        for (; letter <= 'b'; letter++, index++) {
            root[0] = letter;
            if (GetDriveTypeA(root) == DRIVE_CDROM) {
                Drives[Count++] = index;
            }
        }
        if (Count == 0) {
            Debug_Printf("No CD drives found\n");
        }
    }
}


// GetCDClass::~GetCDClass() (0x004D2A10): nothing to release.

GetCDClass::~GetCDClass()
{
}
