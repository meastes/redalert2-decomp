// void __fastcall Unknown_WOL_Load_Settings_0073C7C0() (0x0073C7C0)
// Reads the seven World Online lobby switches from [WOnline] of RA2.INI (default 1) and
// then runs 0x005CD380.
#include "shared/ccini_class.hpp"
#include "shared/wol_settings.hpp"

void __fastcall Unknown_WOL_Load_Settings_0073C7C0()
{
    unknown_wol_allow_page_b290a0 = ConfigINI.Get_Int("WOnline", "AllowPage", 1);
    unknown_wol_allow_find_b2a818 = ConfigINI.Get_Int("WOnline", "AllowFind", 1);
    unknown_wol_lang_filter_b2a7e4 = ConfigINI.Get_Int("WOnline", "LangFilter", 1);
    unknown_wol_lob_music_b297c8 = ConfigINI.Get_Int("WOnline", "LobMusic", 1);
    unknown_wol_show_all_b2a634 = ConfigINI.Get_Int("WOnline", "ShowAll", 1);
    unknown_wol_display_asian_b2a804 = ConfigINI.Get_Int("WOnline", "DisplayAsian", 1);
    unknown_wol_display_latin_b2a7e8 = ConfigINI.Get_Int("WOnline", "DisplayLatin", 1);
    Unknown_005CD380();
}
