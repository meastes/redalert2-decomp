// void ScoreFontClass::Load_Sounds() (0x00667900)
// Loads the three typing sounds (TEXT1-3.AUD) when sound is available:
// from the mixfiles, else read whole from a loose file (owned then). The
// original opens the file through FileClass (virtual Set_Name and Open)
// and closes it through CCFileClass.
#include "shared/score_font.hpp"
#include "shared/sound_available.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/debug_printf.hpp"
// @ref ?AudioDeviceAvailable@@YAHXZ=00406f30
// @ref ??0CCFileClass@@QAE@XZ=00468fd0
// @ref ?Close@CCFileClass@@UAEXXZ=004691f0
// @ref ?Load_Alloc_Data@@YIPAXAAVFileClass@@@Z=00495230
// @ref ?Retrieve@MixFileClass@@SIPBXPBD@Z=005956c0
// @ref ?Debug_Printf@@YAXPBDZZ=004068f0
// @ref ??1Buffer@@QAE@XZ=00438410
// @ref ??1BufferIOFileClass@@UAE@XZ=0042f350
// @ref ?text_sounds@@3PAUScoreFontSound@@A=00ab6fa8
// @ref ??_C@_09CNEE@TEXT1?4AUD?$AA@=007f2ae0
// @ref ??_C@_09MNCL@TEXT2?4AUD?$AA@=007f2ab4
// @ref ??_C@_09GNAO@TEXT3?4AUD?$AA@=007f2a88
// @ref ??_C@_0BP@OHAE@ScoreScreen?3?5Loaded?5TEXT1?4AUD?6?$AA@=007f2ac0
// @ref ??_C@_0BP@FJOE@ScoreScreen?3?5Loaded?5TEXT2?4AUD?6?$AA@=007f2a94
// @ref ??_C@_0BP@MMEE@ScoreScreen?3?5Loaded?5TEXT3?4AUD?6?$AA@=007f2a68
void *__fastcall Load_Alloc_Data(class FileClass &file);

void ScoreFontClass::Load_Sounds()
{
    if (AudioDeviceAvailable()) {
        CCFileClass ccfile;
        FileClass &file = ccfile;
        text_sounds[0].Data = MixFileClass::Retrieve("TEXT1.AUD");
        if (text_sounds[0].Data == 0) {
            file.Set_Name("TEXT1.AUD");
            file.Open(1);
            text_sounds[0].Data = Load_Alloc_Data(file);
            text_sounds[0].IsAllocated = true;
            ccfile.Close();
            Debug_Printf("ScoreScreen: Loaded TEXT1.AUD\n");
        }
        text_sounds[1].Data = MixFileClass::Retrieve("TEXT2.AUD");
        if (text_sounds[1].Data == 0) {
            file.Set_Name("TEXT2.AUD");
            file.Open(1);
            text_sounds[1].Data = Load_Alloc_Data(file);
            text_sounds[1].IsAllocated = true;
            ccfile.Close();
            Debug_Printf("ScoreScreen: Loaded TEXT2.AUD\n");
        }
        text_sounds[2].Data = MixFileClass::Retrieve("TEXT3.AUD");
        if (text_sounds[2].Data == 0) {
            file.Set_Name("TEXT3.AUD");
            file.Open(1);
            text_sounds[2].Data = Load_Alloc_Data(file);
            text_sounds[2].IsAllocated = true;
            ccfile.Close();
            Debug_Printf("ScoreScreen: Loaded TEXT3.AUD\n");
        }
    }
}
