// 0x006220d0: function not yet identified
//
// Body: 8 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined * __fastcall Name_From_Quarry(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006220d0 {

extern "C" void* QuarryName;

unsigned char * __fastcall Name_From_Quarry(int index)
{
    return (unsigned char *)((&QuarryName)[index]);
}

}  // namespace seed_006220d0
