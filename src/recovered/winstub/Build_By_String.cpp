// 0x00737b00: function not yet identified
//
// Body: 23 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: char * __fastcall Build_By_String_00737b00(char *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00737b00 {

extern "C" unsigned int __cdecl _strncpy(...);  // @ref __strncpy=007823d0
extern "C" char s_gsmallwoo_007d8904[];

char * __fastcall Build_By_String_00737b00(char *destination)
{
    _strncpy(destination,s_gsmallwoo_007d8904,0x20);
    return destination;
}

}  // namespace seed_00737b00
