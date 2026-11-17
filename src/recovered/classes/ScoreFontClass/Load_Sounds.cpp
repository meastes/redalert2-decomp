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
