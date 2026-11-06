// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void MultiplayerGameMode::Read_Game_Modes_5b8490(Wstring const &section, InitializerBase &initializer) -- 0x005B8490
// From ra2-decomp 0x005D7590 (YR twin Read_Game_Modes, src/multiplayer.cpp), on Scout810's twin-shaped draft and
// LoadDlg485's r8490. The twin's direct MPModeIndex reads (unknown_28) in the insertion scan let the inlined
// mode-list accessor expand the list constructor at that site as in the original.
// Reads the [section] section of MPModes.ini: each entry is "id=title,description,ini,unknown_4[,flag]"
// (title and description are string-table labels, flag is true unless "false"); creates the mode
// through the initializer and inserts it into the mode list ordered by id.
// @ref ??_C@_0M@JELC@MPModes?4ini?$AA@=007e6140
// @ref ??_C@_01KMAE@?0?$AA@=007ce9c0
// @ref ??_C@_01FCOA@?5?$AA@=007e613c
// @ref ??_C@_0BH@ODPE@D?3?2Ra2?2Multiplayer?4cpp?$AA@=007e6124
// @ref ??_C@_05NAGO@false?$AA@=007d24d0
// @ref ??0GenericList@@QAE@XZ=00490e80
// @ref ??_GINIClass@@UAEPAXI@Z=004deef0
#include "shared/multiplayer_game_mode.hpp"
#include "shared/ini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/string_table.hpp"
#include "shared/lobby.hpp"

static inline void Trim_Trailing(Wstring &string)
{
    string.Trim_Trailing(" ");
}

void __fastcall MultiplayerGameMode::Read_Game_Modes_5b8490(Wstring const &section, InitializerBase &init)
{
    CCFileClass file("MPModes.ini");
    if (file.Is_Available()) {
        INIClass ini;
        if (ini.Load(file, false) > 0) {
            int count = ini.Entry_Count(section);
            for (int i = 0; i < count; i++) {
                Wstring entry = ini.Get_Entry(section, i);
                int index = entry.As_Integer();
                Wstring value;
                int length = ini.Get_String(section, entry, unknown_string_83c55c, value);
                if (length <= 0) continue;

                Wstring name;
                int pos = value.Get_Token(0, ",", name);
                if (!name.Length()) continue;
                name.Trim_Leading(" ");
                Trim_Trailing(name);
                WideString title((unsigned short const *)StringTable::Load_String(name.Get(), 0, "D:\\Ra2\\Multiplayer.cpp", 0x305));

                Wstring tip;
                pos = value.Get_Token(pos, ",", tip);
                if (!tip.Length()) continue;
                tip.Trim_Leading(" ");
                Trim_Trailing(tip);
                WideString tooltip((unsigned short const *)StringTable::Load_String(tip.Get(), 0, "D:\\Ra2\\Multiplayer.cpp", 0x310));

                Wstring inifile;
                pos = value.Get_Token(pos, ",", inifile);
                if (!inifile.Length()) continue;
                inifile.Trim_Leading(" ");
                Trim_Trailing(inifile);

                Wstring mapfilter;
                pos = value.Get_Token(pos, ",", mapfilter);
                mapfilter.Trim_Leading(" ");
                Trim_Trailing(mapfilter);

                Wstring ai;
                value.Get_Token(pos, ",", ai);
                ai.Trim_Leading(" ");
                Trim_Trailing(ai);
                bool aiallowed = true;
                if (ai.Length() > 0) {
                    aiallowed = Compare_Narrow_00781f20(ai.Get(), "false") != 0;
                }

                if (title.Length() <= 0 || inifile.Length() <= 0 || mapfilter.Length() <= 0) continue;

                MultiplayerGameMode *mode = init.Create(title, tooltip, inifile, mapfilter, aiallowed, index);
                int at;
                for (at = 0; at < unknown_modes_5b6c30().Count(); at++) {
                    if (unknown_modes_5b6c30()[at]->unknown_28 > mode->unknown_28) {
                        break;
                    }
                }
                if (at == 0) {
                    unknown_modes_5b6c30().Add_Head(mode);
                } else {
                    unknown_modes_5b6c30().Insert_After(mode, at - 1);
                }
            }
        }
    }
}
