// void __fastcall Unknown_WOL_Save_Settings_0073C890() (0x0073C890)
// Writes the seven World Online lobby switches to [WOnline] of RA2.INI, saves the INI
// file and then runs 0x005CD3A0.
#include "shared/raw_file_class.hpp"
#include "shared/ccini_class.hpp"
#include "shared/wol_settings.hpp"

void __fastcall Unknown_WOL_Save_Settings_0073C890()
{
    RawFileClass file("RA2.INI");
    ConfigINI.Put_Int("WOnline", "AllowPage", unknown_wol_allow_page_b290a0);
    ConfigINI.Put_Int("WOnline", "AllowFind", unknown_wol_allow_find_b2a818);
    ConfigINI.Put_Int("WOnline", "LangFilter", unknown_wol_lang_filter_b2a7e4);
    ConfigINI.Put_Int("WOnline", "LobMusic", unknown_wol_lob_music_b297c8);
    ConfigINI.Put_Int("WOnline", "ShowAll", unknown_wol_show_all_b2a634);
    ConfigINI.Put_Int("WOnline", "DisplayAsian", unknown_wol_display_asian_b2a804);
    ConfigINI.Put_Int("WOnline", "DisplayLatin", unknown_wol_display_latin_b2a7e8);
    ConfigINI.Save(file, false);
    Unknown_005CD3A0();
}
