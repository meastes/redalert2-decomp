// wolpersonainformation.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wolpersonainformation.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/ini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/wol_sink.hpp"
#include "shared/wol_registry_options.hpp"
#include "shared/win32_api.hpp"
#include "shared/string_table.hpp"
#include "shared/wide_string.hpp"


// Header-static dynamic initializers of the unit at 0x00738450: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00738450-0x00738631); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00738470, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x007384A0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x007384C0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x007384E0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x00738500, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x00738520, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x00738540, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x00738580, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x007385B0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x007385E0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x00738610, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// Dynamic initializer (0x00738620, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00738450.cpp; no source of its own.


// rc_ptr<PlayerProfile> __fastcall PlayerProfile::Load_Default() (0x00738670): the profile in
// wolinfo.ini's [_Default_] section (unknown_wstring_b26418): Server, Locale, Country and Color
// (-2 when absent); creates wolinfo.ini when it does not exist. Name is a hypothesis.
// The section test goes through an inline Find_Section(Wstring const &) (ini_class.hpp): the
// original spends one more inline site here (its GenericList constructor stays out of line).

extern Wstring unknown_wstring_b26418;

rc_ptr<PlayerProfile> __fastcall PlayerProfile::Load_Default()
{
    Wstring server;
    int locale = 0;
    int country = -2;
    int color = -2;
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    if (file.Is_Available()) {
        INIClass ini;
        ini.Load(file, false);
        file.Close();
        if (ini.Find_Section(unknown_wstring_b26418)) {
            ini.Get_String(unknown_wstring_b26418, "Server", server, server);
            locale = ini.Get_Int(unknown_wstring_b26418, "Locale", 0);
            country = ini.Get_Int(unknown_wstring_b26418, "Country", -2);
            color = ini.Get_Int(unknown_wstring_b26418, "Color", -2);
        }
    }
    return rc_ptr<PlayerProfile>(new PlayerProfile(server, locale, false, 0, country, color, -1, -1, -1, -1, -1));
}


// rc_ptr<PlayerProfile> __fastcall PlayerProfile::Load_00738880(Wstring const &name) (0x00738880):
// the profile in wolinfo.ini's section of the lowercased nickname (Server, Locale, LocaleVerified,
// Country, Color, Rank, Points, Wins, Losses, Disconnects); Load_Default when the file or the
// section is missing. Creates wolinfo.ini when it does not exist.

rc_ptr<PlayerProfile> __fastcall PlayerProfile::Load_00738880(Wstring const &name)
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    if (!file.Is_Available()) {
        return Load_Default();
    }
    INIClass ini;
    ini.Load(file, false);
    file.Close();
    Wstring section(name);
    section.To_Lower();
    if (!ini.Find_Section(section)) {
        return Load_Default();
    }
    Wstring server;
    ini.Get_String(section, "Server", server, server);
    int locale = ini.Get_Int(section, "Locale", 0);
    bool verified = ini.Get_Bool(section.Get(), "LocaleVerified", false);
    int country = ini.Get_Int(section, "Country", -2);
    int color = ini.Get_Int(section, "Color", -2);
    int rank = ini.Get_Int(section, "Rank", -1);
    int points = ini.Get_Int(section, "Points", -1);
    int wins = ini.Get_Int(section, "Wins", -1);
    int losses = ini.Get_Int(section, "Losses", -1);
    int disconnects = ini.Get_Int(section, "Disconnects", -1);
    return rc_ptr<PlayerProfile>(new PlayerProfile(server, locale, verified, verified ? 1 : 3, country, color,
                                                   rank, points, wins, losses, disconnects));
}


// PlayerProfile::PlayerProfile(Wstring const &server, int locale, bool verified, int unknown_18, int country,
//                              int color, int rank, int points, int wins, int losses, int disconnects) (0x00738C40)
// The profile of a WOnline player: the server name, locale, verified flag and the ladder numbers
// (Load_Default 0x00738670 builds one from wolinfo.ini with -1 for the numbers; the field names are
// hypotheses after the parameters).

PlayerProfile::PlayerProfile(Wstring const &server, int locale, bool verified, int unknown_18_value, int country,
                             int color, int rank, int points, int wins, int losses, int disconnects)
    : unknown_08(false), Server(server), unknown_10(locale), Verified(verified), unknown_18(unknown_18_value),
      Country(country), Color(color), Rank(rank), Points(points), Wins(wins), Losses(losses),
      Disconnects(disconnects)
{
}


// bool __fastcall Get_Auto_Login_00738cb0() (0x00738cb0)
// The [Ask] AutoLogin= option of wolinfo.ini (true when absent); false without the file.

bool __fastcall Get_Auto_Login_00738cb0()
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        return false;
    }
    INIClass ini;
    ini.Load(file, false);
    file.Close();
    return ini.Get_Bool("Ask", "AutoLogin", true);
}


// void __fastcall Set_Auto_Login_00738da0(bool value) (0x00738da0)
// Writes the [Ask] AutoLogin= option to wolinfo.ini (created when missing).

void __fastcall Set_Auto_Login_00738da0(bool value)
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    INIClass ini;
    if (file.Is_Available()) {
        ini.Load(file, false);
        ini.Put_Bool("Ask", "AutoLogin", value);
        ini.Save(file);
    }
}


// bool __fastcall Get_Add_Match_Buddies_00738eb0() (0x00738eb0)
// The [Ask] AddMatchBuddies= option of wolinfo.ini (true when absent); false without the file.

bool __fastcall Get_Add_Match_Buddies_00738eb0()
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        return false;
    }
    INIClass ini;
    ini.Load(file, false);
    file.Close();
    return ini.Get_Bool("Ask", "AddMatchBuddies", true);
}


// void __fastcall Set_Add_Match_Buddies_00738fa0(bool value) (0x00738fa0)
// Writes the [Ask] AddMatchBuddies= option to wolinfo.ini (created when missing).

void __fastcall Set_Add_Match_Buddies_00738fa0(bool value)
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    INIClass ini;
    if (file.Is_Available()) {
        ini.Load(file, false);
        ini.Put_Bool("Ask", "AddMatchBuddies", value);
        ini.Save(file);
    }
}


// Wstring __fastcall WOL_Auto_Login_Persona() (0x007390B0): the [DefaultPersona]
// AutoLogin entry of wolinfo.ini; null when the file is absent. Name is a hypothesis.
// Fastcall: the hidden result pointer arrives in ecx (mov esi, ecx), and the Wstring
// temporaries are built before the pushes (implicit conversions).

Wstring __fastcall WOL_Auto_Login_Persona()
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        return 0;
    }
    INIClass ini;
    ini.Load(file, false);
    file.Close();
    Wstring persona;
    ini.Get_String("DefaultPersona", "AutoLogin", persona, persona);
    return persona;
}


// void __fastcall WOL_Set_Auto_Login_Persona(Wstring const &persona) (0x007391F0): writes
// [DefaultPersona] AutoLogin= to wolinfo.ini, creating the file first. Name is a hypothesis.
// The section and entry literals convert implicitly to Put_String's Wstring overload.

void __fastcall WOL_Set_Auto_Login_Persona(Wstring const &persona)
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    INIClass ini;
    if (file.Is_Available()) {
        ini.Load(file, false);
        ini.Put_String("DefaultPersona", "AutoLogin", persona);
        ini.Save(file);
    }
}


// Wstring __fastcall WOL_Last_Valid_Persona() (0x00739330): the [DefaultPersona]
// LastValid entry of wolinfo.ini; null when the file is absent. Name is a hypothesis.

Wstring __fastcall WOL_Last_Valid_Persona()
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        return 0;
    }
    INIClass ini;
    ini.Load(file, false);
    file.Close();
    Wstring persona;
    ini.Get_String("DefaultPersona", "LastValid", persona, persona);
    return persona;
}


// void __fastcall WOL_Set_Last_Valid_Persona(Wstring const &persona) (0x00739470): writes
// [DefaultPersona] LastValid= to wolinfo.ini, creating the file first. Name is a hypothesis.

void __fastcall WOL_Set_Last_Valid_Persona(Wstring const &persona)
{
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    INIClass ini;
    if (file.Is_Available()) {
        ini.Load(file, false);
        ini.Put_String("DefaultPersona", "LastValid", persona);
        ini.Save(file);
    }
}


extern "C" unsigned char data_7395b0_7af554[];
class Call_7395b0_0 {public: unsigned int target();};
class Call_7395b0_1 {public: unsigned int target();};
class Leaf_7395b0 { public: unsigned int get(); };
unsigned int Leaf_7395b0::get() { *(unsigned int*)(((unsigned int)this)) = (unsigned int)data_7395b0_7af554; unsigned int unused_member_return = ((Call_7395b0_0*)(((unsigned int)this) + 0xc))->target(); unsigned int base_return = ((Call_7395b0_1*)((unsigned int)this))->target(); return base_return; }


// void PlayerProfile::Save_007395d0(Wstring const &name) (0x007395D0): when the profile changed
// (+0x08), writes it into wolinfo.ini's section of the lowercased nickname (the _Default_ section
// for an empty name; a named profile only when the registry options allow it): Server, Locale,
// LocaleVerified, Country, Color, Rank, Points, Wins, Losses and Disconnects, then clears the flag.
// Creates wolinfo.ini when it does not exist.

extern Wstring unknown_wstring_b26418;

void PlayerProfile::Save_007395d0(Wstring const &name)
{
    WolRegistryOptions options;
    options.Read_004ff200();
    if (!unknown_08) {
        return;
    }
    Wstring section(name);
    if (name.Length() == 0) {
        section = unknown_wstring_b26418;
    } else {
        section.To_Lower();
        if (!options.unknown_004ff2c0()) {
            return;
        }
    }
    CDFileClass file("wolinfo.ini");
    if (!file.Is_Available()) {
        file.Create();
        file.Close();
        file.Open("wolinfo.ini", 1);
    }
    if (file.Is_Available()) {
        INIClass ini;
        ini.Load(file, false);
        ini.Put_String(section, "Server", Server);
        ini.Put_Int(section, "Locale", Unknown_Value_10());
        ini.Put_Bool(section, "LocaleVerified", Verified);
        ini.Put_Int(section, "Country", Country);
        ini.Put_Int(section, "Color", Color);
        ini.Put_Int(section, "Rank", Rank);
        ini.Put_Int(section, "Points", Points);
        ini.Put_Int(section, "Wins", Wins);
        ini.Put_Int(section, "Losses", Losses);
        ini.Put_Int(section, "Disconnects", Disconnects);
        ini.Save(file);
        file.Close();
        unknown_08 = false;
    }
}


// The persona statistics page's text setup (0x00739A50; WOLPersonaInformation.cpp, name
// hypothesis): with clear set it empties control 0x628 (message 0x4B4) and repaints it, then
// empties control 0x6EF and shows the "GUI:UnknownStats" text in controls 0x6F0, 0x6F1,
// 0x701, 0x6F4 and 0x6F5 (message 0x4B2). Called by 0x00753A60.

void __fastcall Unknown_Persona_Stats_Text_00739a50(HWND window, bool clear)
{
    HWND control;
    if (clear) {
        control = GetDlgItem(window, 0x628);
        SendMessageA(control, 0x4B4, 0, (LPARAM)"");
        InvalidateRect(control, 0, FALSE);
    }
    control = GetDlgItem(window, 0x6EF);
    SendMessageA(control, 0x4B2, 0, (LPARAM)L"");
    WideString text(StringTable::Load_String("GUI:UnknownStats", 0, "D:\\Ra2\\WOLPersonaInformation.cpp", 639));
    control = GetDlgItem(window, 0x6F0);
    SendMessageA(control, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
    control = GetDlgItem(window, 0x6F1);
    SendMessageA(control, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
    control = GetDlgItem(window, 0x701);
    SendMessageA(control, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
    control = GetDlgItem(window, 0x6F4);
    SendMessageA(control, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
    control = GetDlgItem(window, 0x6F5);
    SendMessageA(control, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
}


// Compiler-generated PlayerProfile scalar deleting destructor.
// The existing accepted constructor in this population emits the vtable and implicit destructor.


// 0x0073a0c0: function not yet identified
//
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_0073a0c0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0073a0c0 {

extern void __fastcall rc_ptr_base_Destructor_00639510(void);
extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007af54c;

struct Owner_0073a0c0 { unsigned int * FUN_0073a0c0(unsigned char deletion_flags); };

unsigned int * Owner_0073a0c0::FUN_0073a0c0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007af54c);
    rc_ptr_base_Destructor_00639510();
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_0073a0c0
