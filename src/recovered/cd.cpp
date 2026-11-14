// cd.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cd.json carries the addresses and the rows that
// stay in their own files.


#define THEME_SOUND_CONTROLS
#include "shared/cd.hpp"
#include "shared/cd_file_class.hpp"
#include "shared/mouse.hpp"
#include "shared/keyboard.hpp"
#include "shared/mix_file_class.hpp"


// CD.CPP dynamic initializer (0x0046E4A0)
// Constructs the unit's 16-byte static at 0x00850818 (four zeroed words).
// Its address sits in the C++ initializer table next to cdctrl.cpp's
// (0x0046EA40), which places it at the start of CD.CPP.
// Assumes: the object's type, layout and use are unknown (honest
// placeholder names).

struct UnknownStatic850818 {
    UnknownStatic850818() : unknown_00(0), unknown_04(0), unknown_08(0), unknown_0C(0) {}
    int unknown_00;     // +0x00
    int unknown_04;     // +0x04
    int unknown_08;     // +0x08
    int unknown_0C;     // +0x0C
};

UnknownStatic850818 unknown_static_850818;     // 0x00850818


// int __fastcall Unknown_0046E4C0(int timeout) (0x0046E4C0)
// The disk index (Get_CD_Index) of the current CD drive.
// Assumes: name and the meaning of the second Get_CD_Index argument
// (a timeout in the cdfile notes) are hypotheses.

int __fastcall Unknown_0046E4C0(int timeout)
{
    return Get_CD_Index(CDFileClass::CurrentCDDrive, timeout);
}


// void __fastcall CD::Set_Required_CD(int cd) (0x0046E4D0)
// Records the disk the next file needs; any disk (-2) for a local install.
// Assumes: name and the IsLocal meaning are hypotheses.

void __fastcall CD::Set_Required_CD(int cd)
{
    RequiredCD = -2;
    if (IsLocal != true) {
        RequiredCD = cd;
    }
}


// bool CD::Force_Available(int cd) (0x0046E4F0, slot 0)
// Makes sure the disk is available: nothing to do for -2 (any disk) or a
// local installation, else searches for it with the music theme reset.
// Assumes: the -2/IsLocal meanings and the callee's purpose (hypotheses).

bool CD::Force_Available(int cd)
{
    if (cd == -2) {
        return true;
    }
    if (IsLocal == true) {
        return true;
    }
    LastTheme = THEME_NONE;
    return Unknown_004985C0(cd);
}


// CD prompt handling, complete original 0x0046E520 body.

bool CD::Insert_Disk(int disk)
{
    wchar_t prompt[256];
    wchar_t name[128];
    if (disk == -1) {
        swprintf(prompt, StringTable::Load_String("TXT_CD_DIALOG_1", 0, "D:\\Ra2\\CD.CPP", 0x92));
    } else if (disk == 0) {
        swprintf(prompt, StringTable::Load_String("TXT_CD_DIALOG_A", 0, "D:\\Ra2\\CD.CPP", 0x97));
    } else if (disk == 1) {
        swprintf(prompt, StringTable::Load_String("TXT_CD_DIALOG_S", 0, "D:\\Ra2\\CD.CPP", 0x9C));
    } else {
        swprintf(name, StringTable::Load_String("TXT_DISK_NAME", 0, "D:\\Ra2\\CD.CPP", 0xA0), disk + 1);
        swprintf(prompt, StringTable::Load_String("TXT_CD_DIALOG_2", 0, "D:\\Ra2\\CD.CPP", 0xA1), disk + 1, name);
    }
    if (LastTheme == THEME_NONE) LastTheme = Theme.What_Is_Playing();
    Theme.Stop();
    int hidden = WWMouse->Get_Mouse_State() < 0;
    while (WWMouse->Get_Mouse_State() < 0) WWMouse->Show_Mouse();
    WWKeyboard->Clear();
    bool result = true;
    if (disk == 0) {
        if (!Unknown_00600C80(StringTable::Load_String("TXT_CD_DIALOG_A", 0, "D:\\Ra2\\CD.CPP", 0xC2))) result = false;
    } else if (disk == 1) {
        if (!Unknown_00600C80(StringTable::Load_String("TXT_CD_DIALOG_S", 0, "D:\\Ra2\\CD.CPP", 0xCA))) result = false;
    } else {
        if (!Unknown_00600C80(StringTable::Load_String("TXT_CD_DIALOG_1", 0, "D:\\Ra2\\CD.CPP", 0xD1))) result = false;
    }
    WWKeyboard->Clear();
    while (hidden--) WWMouse->Hide_Mouse();
    if (!result && LastTheme != THEME_NONE) {
        Theme.Queue_Song(LastTheme);
        LastTheme = THEME_NONE;
    }
    return result;
}


// bool CD::Swap_Disk(int cd) (0x0046E750, slot 2)
// Switches to another game disk: stops the music, releases the five per-disk
// mixfiles and, when the disk's volume checks out, reopens MAIN.MIX,
// MAPS<n>.MIX, MULTI.MIX, THEME.MIX (rescanning the themes) and
// MOVIES<n>.MIX for disk n = cd + 1. False when an archive cannot be
// created; true without reopening when the volume check fails.
// Assumes: the global names, FastKey and VolumeCheck's result meaning are
// hypotheses; the theme save mirrors Insert_Disk.

bool CD::Swap_Disk(int cd)
{
    char name[64];

    Debug_Printf("***** Swapping to disk %d *****\n", cd);

    if (LastTheme == THEME_NONE) {
        ThemeType theme = Theme.Score;
        if (theme == THEME_NONE) {
            theme = Theme.Pending;
        }
        LastTheme = theme;
    }
    Theme.Stop();

    if (MoviesMix != 0) {
        Debug_Printf("     Releasing %s\n", MoviesMix->Filename);
        delete MoviesMix;
        MoviesMix = 0;
    }
    if (ThemeMix != 0) {
        Debug_Printf("     Releasing %s\n", ThemeMix->Filename);
        delete ThemeMix;
        ThemeMix = 0;
    }
    if (MultiMix != 0) {
        Debug_Printf("     Releasing %s\n", MultiMix->Filename);
        delete MultiMix;
        MultiMix = 0;
    }
    if (MapsMix != 0) {
        Debug_Printf("     Releasing %s\n", MapsMix->Filename);
        delete MapsMix;
        MapsMix = 0;
    }
    if (MainMix != 0) {
        Debug_Printf("     Releasing %s\n", MainMix->Filename);
        delete MainMix;
        MainMix = 0;
    }

    if (!VolumeCheck()) {
        return true;
    }

    MainMix = new MixFileClass("MAIN.MIX", &FastKey);
    Debug_Printf("     Initialized %s\n", MainMix->Filename);

    cd++;
    sprintf(name, "MAPS%02d.MIX", cd);
    Debug_Printf("     Initializing %s\n", name);
    MapsMix = new MixFileClass(name, &FastKey);
    if (MapsMix == 0) {
        Debug_Printf("     FAILED!\n");
        return false;
    }

    Debug_Printf("     Initializing Multi.Mix\n");
    MultiMix = new MixFileClass("MULTI.MIX", &FastKey);
    if (MultiMix == 0) {
        Debug_Printf("     FAILED!\n");
        return false;
    }

    Debug_Printf("     Initializing Themes.MIX\n");
    ThemeMix = new MixFileClass("THEME.MIX", &FastKey);
    Theme.Scan();

    sprintf(name, "MOVIES%02d.MIX", cd);
    Debug_Printf("     Initializing %s\n", name);
    MoviesMix = new MixFileClass(name, &FastKey);
    if (MoviesMix == 0) {
        Debug_Printf("     FAILED!\n");
        return false;
    }

    return VolumeCheck();
}
