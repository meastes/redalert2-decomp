// 0x005b69b0: function not yet identified
//
// Body: 43 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 * __thiscall MSSfx_Constructor_005b69b0(undefined4 *param_1,undefined4 param_2)
// Callees with accepted source:
//   FUN_00406f30 -> src/recovered/00406f30.cpp
//   FUN_0078d816 -> src/recovered/0078d816.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005b69b0 {

extern unsigned int  FUN_00406f30(void);
extern unsigned int  FUN_0078d816(char *source_text);

struct Owner_005b69b0 { unsigned int * MSSfx_Constructor_005b69b0(unsigned int source_address, int unused_1); };

unsigned int * Owner_005b69b0::MSSfx_Constructor_005b69b0(unsigned int source_address, int unused_1)
{
    unsigned int * object_words = (unsigned int *)this;
    int audio_status;
    unsigned int sample_value;

    *object_words = 0;
    object_words[1] = 0;
    *(unsigned char *)(object_words + 2) = 0;
    audio_status = (int)(FUN_00406f30());
    if (audio_status != 0) {
        sample_value = (unsigned int)(FUN_0078d816((char *)(source_address)));
        *object_words = sample_value;
    }
    return object_words;
}

}  // namespace seed_005b69b0
