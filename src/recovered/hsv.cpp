// hsv.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/hsv.json carries the addresses and the rows that
// stay in their own files.


#include "shared/wol_sink.hpp"
#include "shared/crt_string.hpp"


// 0x004ff1f0: function not yet identified
//
// Body: 11 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool FUN_004ff1f0(void)
// Callees with accepted source:
//   FUN_004ff1e0 -> src/recovered/004ff1e0.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004ff1f0 {

extern unsigned int  FUN_004ff1e0(void);

bool  FUN_004ff1f0(void)
{
    char result_byte;

    result_byte = (char)(FUN_004ff1e0());
    return result_byte == '\0';
}

}  // namespace seed_004ff1f0


// bool WolRegistryOptions::Read_004ff200() (0x004FF200)
// Reads the DWORD "Options" of HKLM\SOFTWARE\Westwood\WOLAPI; HasOptions tells whether it was there.

bool WolRegistryOptions::Read_004ff200()
{
    DWORD size = 4;
    HasOptions = false;
    char subkey[128];
    strcpy(subkey, "SOFTWARE\\Westwood\\WOLAPI\\");
    HKEY key;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, 0xF003F, &key) == 0) {
        DWORD value = 0;
        DWORD type;
        if (RegQueryValueExA(key, "Options", 0, &type, (BYTE *)&value, &size) == 0 && type == 4) {
            HasOptions = true;
            Options = value;
        }
        RegCloseKey(key);
    }
    return HasOptions;
}


// bool WolRegistryOptions::unknown_004ff2b0() const (0x004FF2B0)
// Bit 0 of Options is clear (same form as 0x004FF2C0: the dword loaded, the byte inverted).

bool WolRegistryOptions::unknown_004ff2b0() const
{
    unsigned int options = Options;
    bool result = (unsigned char)~options & 1;
    return result;
}


// bool WolRegistryOptions::unknown_004ff2c0() const (0x004FF2C0)
// Bit 1 of Options is clear (the option the WOL login flow tests before saving the nickname).
// Written as the compiler wants it: the dword is loaded, the byte inverted and shifted.

bool WolRegistryOptions::unknown_004ff2c0() const
{
    unsigned int options = Options;
    bool result = ((unsigned char)~options >> 1) & 1;
    return result;
}


// bool WolRegistryOptions::unknown_004ff2d0() const (0x004FF2D0)
// Bit 2 of Options is clear (same form as 0x004FF2C0).

bool WolRegistryOptions::unknown_004ff2d0() const
{
    unsigned int options = Options;
    bool result = ((unsigned char)~options >> 2) & 1;
    return result;
}


// bool WolRegistryOptions::Write_004ff2e0(unsigned int options) (0x004FF2E0)
// Stores the DWORD "Options" under HKLM\SOFTWARE\Westwood\WOLAPI (creating the key
// when it is missing), then reads the value back into Options / HasOptions.
// Returns whether the value was written.

bool WolRegistryOptions::Write_004ff2e0(unsigned int options)
{
    bool written = false;
    HKEY key;
    DWORD value;
    HKEY readhandle;
    DWORD size;
    DWORD type;
    DWORD disposition;
    char subkey[128];
    strcpy(subkey, "SOFTWARE\\Westwood\\WOLAPI\\");
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, 0xF003F, &key) != 0) {
        if (RegCreateKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, 0, 0, 0xF003F, 0, &key, &disposition) != 0) {
            return false;
        }
    }
    if (RegSetValueExA(key, "Options", 0, 4, (BYTE *)&options, 4) == 0) {
        written = true;
    }
    RegCloseKey(key);
    HasOptions = false;
    size = 4;
    char readkey[128];
    strcpy(readkey, "SOFTWARE\\Westwood\\WOLAPI\\");
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, readkey, 0, 0xF003F, &readhandle) == 0) {
        value = 0;
        if (RegQueryValueExA(readhandle, "Options", 0, &type, (BYTE *)&value, &size) == 0 && type == 4) {
            HasOptions = true;
            Options = value;
        }
        RegCloseKey(readhandle);
    }
    return written;
}
