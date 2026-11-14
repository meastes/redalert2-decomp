// special.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/special.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/special.hpp"
#include "shared/win32_api.hpp"
#include "shared/crt_memory.hpp"


// Header-static dynamic initializers of the unit at 0x0068B130: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0068B130-0x0068B311); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0068B150, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B180, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B1A0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B1C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B1E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B200, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B220, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B260, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B290, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B2C0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B2F0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// Dynamic initializer (0x0068B300, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0068b130.cpp; no source of its own.


// void SpecialClass::Init() (0x0068B320)
// The default options: bits 0-2 clear, TiberiumSpreads and DestroyableBridges
// set, EscapeMovies from HKLM\SOFTWARE\Westwood\Red Alert 2\ EscapeMovies
// (a REG_DWORD, bit 0), everything else clear. `size` is set before the key
// copy: after it, the constant 4 is kept in a register and the size store and
// type test no longer use immediates.

void SpecialClass::Init()
{
    unknown_bits_0 = 0;
    IsFromInstall = 0;
    bool escape = false;
    HKEY handle;
    DWORD size = 4;
    char key[128];
    strcpy(key, "SOFTWARE\\Westwood\\Red Alert 2\\");
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_ALL_ACCESS, &handle) == 0) {
        DWORD type;
        DWORD value = 0;
        if (RegQueryValueExA(handle, "EscapeMovies", 0, &type, (BYTE *)&value, &size) == 0 && type == 4) {
            escape = value & 1;
        }
        RegCloseKey(handle);
    }
    EscapeMovies = escape;
    unknown_bit_4 = 0;
    Inert = 0;
    TiberiumGrows = 0;
    TiberiumSpreads = 1;
    MCVDeploy = 0;
    InitialVeteran = 0;
    FixedAlliance = 0;
    HarvesterImmune = 0;
    FogOfWar = 0;
    unknown_bit_13 = 0;
    TiberiumExplosive = 0;
    DestroyableBridges = 1;
    Meteorites = 0;
    IonStorms = 0;
    Visceroids = 0;
}


// bool SpecialClass::Equals_Options(SpecialClass const &other) const (0x0068B3F0)
// Whether bits 0-18 (every option) are equal; the original tests the XOR's low
// byte and then bits 8-18. The if/return form returns the bool in al.
bool SpecialClass::Equals_Options(SpecialClass const &other) const
{
    if (unknown_bits_0 == other.unknown_bits_0
        && IsFromInstall == other.IsFromInstall
        && EscapeMovies == other.EscapeMovies
        && unknown_bit_4 == other.unknown_bit_4
        && Inert == other.Inert
        && TiberiumGrows == other.TiberiumGrows
        && TiberiumSpreads == other.TiberiumSpreads
        && MCVDeploy == other.MCVDeploy
        && InitialVeteran == other.InitialVeteran
        && FixedAlliance == other.FixedAlliance
        && HarvesterImmune == other.HarvesterImmune
        && FogOfWar == other.FogOfWar
        && unknown_bit_13 == other.unknown_bit_13
        && TiberiumExplosive == other.TiberiumExplosive
        && DestroyableBridges == other.DestroyableBridges
        && Meteorites == other.Meteorites
        && IonStorms == other.IonStorms
        && Visceroids == other.Visceroids) {
        return true;
    }
    return false;
}


// void SpecialClass::Unknown_0068B410() (0x0068B410)
// Empty in this build (Read_Scenario_INI calls it on the global options).

void SpecialClass::Unknown_0068B410()
{
}


class Leaf_68b420 { public: void get(); };
void Leaf_68b420::get() {  }


// void SpecialClass::Write_INI(INIClass &ini) const (0x0068B430, name hypothesis)
// Writes the thirteen named options to [SpecialFlags].

// The original keeps this pointer (to "SpecialFlags") as function-local static data.
extern char *unknown_special_section_7f43f0;    // 0x007f43f0

void SpecialClass::Write_INI(INIClass &ini) const
{
    ini.Put_Bool(unknown_special_section_7f43f0, "TiberiumGrows", TiberiumGrows);
    ini.Put_Bool(unknown_special_section_7f43f0, "TiberiumSpreads", TiberiumSpreads);
    ini.Put_Bool(unknown_special_section_7f43f0, "TiberiumExplosive", TiberiumExplosive);
    ini.Put_Bool(unknown_special_section_7f43f0, "DestroyableBridges", DestroyableBridges);
    ini.Put_Bool(unknown_special_section_7f43f0, "MCVDeploy", MCVDeploy);
    ini.Put_Bool(unknown_special_section_7f43f0, "InitialVeteran", InitialVeteran);
    ini.Put_Bool(unknown_special_section_7f43f0, "FixedAlliance", FixedAlliance);
    ini.Put_Bool(unknown_special_section_7f43f0, "HarvesterImmune", HarvesterImmune);
    ini.Put_Bool(unknown_special_section_7f43f0, "FogOfWar", FogOfWar);
    ini.Put_Bool(unknown_special_section_7f43f0, "Inert", Inert);
    ini.Put_Bool(unknown_special_section_7f43f0, "IonStorms", IonStorms);
    ini.Put_Bool(unknown_special_section_7f43f0, "Meteorites", Meteorites);
    ini.Put_Bool(unknown_special_section_7f43f0, "Visceroids", Visceroids);
}


// void SpecialClass::Read_INI(INIClass &ini) (0x0068B5A0, name hypothesis)
// Reads the options from [SpecialFlags]; the Tiberium, bridge, alliance, fog,
// inert and harvester options are only read outside multiplayer-style games
// (0x00A3D298 is zero) or when 0x00A40D13 is set.

extern int unknown_a3d298;      // 0x00A3D298
extern bool Debug_Map;     // 0x00A40D13

// The original keeps this pointer (to "SpecialFlags") as function-local static data.
extern char *unknown_special_section_7f43f4;    // 0x007f43f4

void SpecialClass::Read_INI(INIClass &ini)
{
    TiberiumExplosive = ini.Get_Bool(unknown_special_section_7f43f4, "TiberiumExplosive", TiberiumExplosive);
    MCVDeploy = ini.Get_Bool(unknown_special_section_7f43f4, "MCVDeploy", MCVDeploy);
    InitialVeteran = ini.Get_Bool(unknown_special_section_7f43f4, "InitialVeteran", InitialVeteran);
    IonStorms = ini.Get_Bool(unknown_special_section_7f43f4, "IonStorms", IonStorms);
    Meteorites = ini.Get_Bool(unknown_special_section_7f43f4, "Meteorites", Meteorites);
    Visceroids = ini.Get_Bool(unknown_special_section_7f43f4, "Visceroids", Visceroids);
    if (unknown_a3d298 == 0 || Debug_Map) {
        TiberiumGrows = ini.Get_Bool(unknown_special_section_7f43f4, "TiberiumGrows", TiberiumGrows);
        TiberiumSpreads = ini.Get_Bool(unknown_special_section_7f43f4, "TiberiumSpreads", TiberiumSpreads);
        DestroyableBridges = ini.Get_Bool(unknown_special_section_7f43f4, "DestroyableBridges", DestroyableBridges);
        FixedAlliance = ini.Get_Bool(unknown_special_section_7f43f4, "FixedAlliance", FixedAlliance);
        FogOfWar = ini.Get_Bool(unknown_special_section_7f43f4, "FogOfWar", FogOfWar);
        Inert = ini.Get_Bool(unknown_special_section_7f43f4, "Inert", Inert);
        HarvesterImmune = ini.Get_Bool(unknown_special_section_7f43f4, "HarvesterImmune", HarvesterImmune);
    }
}
