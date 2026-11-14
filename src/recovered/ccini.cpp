// ccini.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ccini.json carries the addresses and the rows that
// stay in their own files.


#define RA2_CCINI_UNIT_MEMBERS
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/ccini_class.hpp"
#include "shared/ccini_extra.hpp"
#include "shared/crt_memory.hpp"
#include "shared/sha_pipe.hpp"


// CCINI.CPP declares CCINIClass::Get_Buildings 0x0046AEA0 and Put_House_List 0x0046BE60 (shared/ccini_class.hpp).
// Header-static dynamic initializers of the unit at 0x004694C0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004694C0-0x004696A6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004694F0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469510, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469530, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469550, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469570, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469590, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x004695D0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469600, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469630, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469660, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469670, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// Dynamic initializer (0x00469690, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004694c0.cpp; no source of its own.


// CCINIClass::Load (0x004696B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Load(FileClass &file, bool withdigest, bool keep_comments)
{
    FileStraw straw(file);
    return Load(straw, withdigest, keep_comments);
}


// CCINIClass::Load (0x004697E0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Load(Straw &file, bool withdigest, bool keep_comments)
{
    int result = INIClass::Load(file, keep_comments);
    Invalidate_Message_Digest();
    if (result && withdigest) {
        unsigned char digest[20];
        int length = Get_UUBlock("Digest", digest, sizeof(digest));
        if (length == 0) {
            return 2;
        }
        if (length > 0) {
            Clear("Digest");
            Calculate_Message_Digest();
            if (memcmp(digest, Digest, sizeof(digest)) != 0) {
                return 2;
            }
        }
    }
    return result;
}


// CCINIClass::Save (0x004698F0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Save(FileClass &file, bool withdigest)
{
    FilePipe pipe(file);
    return Save(pipe, withdigest);
}


// CCINIClass::Save (0x00469A00).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Save(Pipe &pipe, bool withdigest)
{
    if (!withdigest) {
        return INIClass::Save(pipe);
    }
    Clear("Digest");
    Calculate_Message_Digest();
    Put_UUBlock("Digest", Digest, sizeof(Digest));
    int result = INIClass::Save(pipe);
    Clear("Digest");
    return result;
}


// CCINIClass::Get_Lepton (0x00469AE0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Lepton(char const *section, char const *entry, int defvalue) const
{
    double value = Get_Double(section, entry, -1.0);
    if (value == -1.0) {
        return defvalue;
    }
    return value * 256.0;
}


// CCINIClass::Put_Lepton (0x00469B20).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Lepton(char const *section, char const *entry, int lepton)
{
    return Put_Double(section, entry, lepton / 256.0);
}


// CCINIClass::Put_Facing (0x00469B50).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Facing(char const *section, char const *entry, int facing)
{
    char buffer[32];
    sprintf(buffer, "%d", (facing * 360) / 256);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Facing (0x00469BA0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Facing(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return (atoi(buffer) * 256) / 360;
    }
    return defvalue;
}


// CCINIClass::Get_Cell (0x00469C00).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

Cell CCINIClass::Get_Cell(char const *section, char const *entry, Cell const &defvalue) const
{
    char buffer[64];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int x;
        int y;
        sscanf(buffer, "%d,%d", &x, &y);
        return Cell(x, y);
    }
    return defvalue;
}


// CCINIClass::Put_Cell (0x00469C80).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Cell(char const *section, char const *entry, Cell const &cell)
{
    char buffer[64];
    sprintf(buffer, "%d,%d", cell.X, cell.Y);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Percent (0x00469CD0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Percent(char const *section, char const *entry, int defvalue) const
{
    int value = Get_Int(section, entry, -1);
    if (value == -1) {
        return defvalue;
    }
    if (value >= 100) {
        value = 100;
    }
    int result = ((value > 0 ? value : 0) * 256) / 100;
    if (result >= 255) {
        result = 255;
    }
    return result;
}


// CCINIClass::Put_Percent (0x00469D30).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Percent(char const *section, char const *entry, int value)
{
    return Put_Int(section, entry, (value * 100) / 256);
}


// CCINIClass::Get_PipType (0x00469D60).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_PipType(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, PipNames[defvalue].Name, buffer, sizeof(buffer));
    for (int index = 0; index < 5; index++) {
        if (_stricmp(buffer, PipNames[index].Name) == 0) {
            return PipNames[index].Value;
        }
    }
    return 1;
}


// CCINIClass::Put_PipType (0x00469DD0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_PipType(char const *section, char const *entry, int pip)
{
    if (pip != 0) {
        return Put_String(section, entry, PipNames[pip].Name);
    }
    return false;
}


// CCINIClass::Get_PipScale (0x00469E00).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_PipScale(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    if (Get_String(section, entry, "", buffer, sizeof(buffer)) > 0) {
        for (int index = 0; index < 4; index++) {
            if (_stricmp(buffer, PipScaleNames[index].Name) == 0) {
                return PipScaleNames[index].Value;
            }
        }
        return 0;
    }
    return defvalue;
}


// CCINIClass::Put_PipScale (0x00469E70): writes a pip scale by name; none (0) is not written
// (TibSun Put_PipScaleType).

bool CCINIClass::Put_PipScale(char const *section, char const *entry, int scale)
{
    if (scale != 0) {
        return Put_String(section, entry, PipScaleNames[scale].Name);
    }
    return false;
}


// CCINIClass::Get_CategoryType (0x00469EA0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

CategoryType CCINIClass::Get_CategoryType(char const *section, char const *entry, CategoryType defvalue) const
{
    char buffer[32];
    Get_String(section, entry, Name_From_Category(defvalue), buffer, sizeof(buffer));
    return Category_From_Name(buffer);
}


// CCINIClass::Put_CategoryType (0x00469EE0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_CategoryType(char const *section, char const *entry, CategoryType category)
{
    if (category != CATEGORY_SOLDIER) {
        return Put_String(section, entry, Name_From_Category(category));
    }
    return false;
}


// CCINIClass::Get_UnknownHex_469f10 (0x00469F10): the entry read with Get_Hex
// as an UnknownHexValue. Unreferenced in the original.

UnknownHexValue CCINIClass::Get_UnknownHex_469f10(char const *section, char const *entry, int defvalue) const
{
    return UnknownHexValue(Get_Hex(section, entry, defvalue));
}


// CCINIClass::Put_UnknownHex_469f30 (0x00469F30): writes the value with
// Put_Hex. Unreferenced in the original.

bool CCINIClass::Put_UnknownHex_469f30(char const *section, char const *entry, int value)
{
    return Put_Hex(section, entry, value);
}


// CCINIClass::Get_ColorScheme (0x00469F50).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_ColorScheme(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, ColorSchemes[defvalue]->Name, buffer, sizeof(buffer));
    for (int index = 0; index < ColorSchemes.Count(); index++) {
        if (_stricmp(ColorSchemes[index]->Name, buffer) == 0 && ColorSchemes[index]->unknown_0x310 != 1) {
            return index;
        }
    }
    return defvalue;
}


// CCINIClass::Put_ColorScheme (0x00469FE0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_ColorScheme(char const *section, char const *entry, int scheme)
{
    return Put_String(section, entry, ColorSchemes[scheme]->Name);
}


// CCINIClass::Get_RGB (0x0046A010).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

RGBClass CCINIClass::Get_RGB(char const *section, char const *entry, RGBClass const &defvalue) const
{
    char defbuffer[64];
    char buffer[64];
    sprintf(defbuffer, "%d,%d,%d", defvalue.Red, defvalue.Green, defvalue.Blue);
    if (Get_String(section, entry, defbuffer, buffer, sizeof(buffer))) {
        int red;
        int green;
        int blue;
        sscanf(buffer, "%d,%d,%d", &red, &green, &blue);
        return RGBClass(red, green, blue);
    }
    return defvalue;
}


// CCINIClass::Put_RGB (0x0046A0E0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_RGB(char const *section, char const *entry, RGBClass const &rgb)
{
    char buffer[64];
    sprintf(buffer, "%d,%d,%d", rgb.Red, rgb.Green, rgb.Blue);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_HSV (0x0046A130).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

HSVClass CCINIClass::Get_HSV(char const *section, char const *entry, HSVClass const &defvalue) const
{
    char defbuffer[64];
    char buffer[64];
    sprintf(defbuffer, "%d,%d,%d", defvalue.Hue, defvalue.Saturation, defvalue.Value);
    if (Get_String(section, entry, defbuffer, buffer, sizeof(buffer))) {
        int hue = 0;
        int saturation = 0;
        int value = 0;
        sscanf(buffer, "%d,%d,%d", &hue, &saturation, &value);
        return HSVClass(hue, saturation, value);
    }
    return defvalue;
}


// CCINIClass::Put_HSV (0x0046A210).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_HSV(char const *section, char const *entry, HSVClass const &hsv)
{
    char buffer[64];
    sprintf(buffer, "%d,%d,%d", hsv.Hue, hsv.Saturation, hsv.Value);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Foundation (0x0046A260).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Foundation(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, FoundationNames[defvalue].Name, buffer, sizeof(buffer));
    for (int index = 0; index < 22; index++) {
        if (_stricmp(buffer, FoundationNames[index].Name) == 0) {
            return FoundationNames[index].Value;
        }
    }
    return 0;
}


// CCINIClass::Put_Foundation (0x0046A2D0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Foundation(char const *section, char const *entry, int foundation)
{
    if (foundation != -1) {
        return Put_String(section, entry, FoundationNames[foundation].Name);
    }
    return false;
}


// CCINIClass::Get_MovementZone (0x0046A300).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_MovementZone(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, MovementZoneNames[defvalue], buffer, sizeof(buffer));
    for (int index = 0; index < 12; index++) {
        if (_stricmp(buffer, MovementZoneNames[index]) == 0) {
            return index;
        }
    }
    return -1;
}


// CCINIClass::Put_MovementZone (0x0046A370).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_MovementZone(char const *section, char const *entry, int zone)
{
    if (zone != -1) {
        return Put_String(section, entry, MovementZoneNames[zone]);
    }
    return false;
}


// CCINIClass::Get_ActionType (0x0046A3A0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_ActionType(char const *section, char const *entry, int defvalue) const
{
    char buffer[32];
    Get_String(section, entry, ActionNames[defvalue], buffer, sizeof(buffer));
    for (int index = 0; index < 66; index++) {
        if (_stricmp(buffer, ActionNames[index]) == 0) {
            return index;
        }
    }
    return 0;
}


// CCINIClass::Get_SuperWeaponType (0x0046A410).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_SuperWeaponType(char const *section, char const *entry, int defvalue) const
{
    char empty[28];
    char buffer[32];
    char const *defname;
    if (defvalue != -1) {
        defname = SuperWeaponTypes[defvalue]->ID;
    } else {
        empty[0] = '\0';
        defname = empty;
    }
    Get_String(section, entry, defname, buffer, sizeof(buffer));
    return SuperWeaponTypeClass::Find_Index(buffer);
}


// CCINIClass::Get_VoxType (0x0046A460).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_VoxType(char const *section, char const *entry, int defvalue) const
{
    char buffer[100];
    Get_String(section, entry, "", buffer, sizeof(buffer));
    if (strlen(buffer) != 0) {
        return VoxClass::Find_Index(buffer);
    }
    return defvalue;
}


// CCINIClass::Get_RTTIType (0x0046A4B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

RTTIType CCINIClass::Get_RTTIType(char const *section, char const *entry, RTTIType defvalue) const
{
    char buffer[32];
    Get_String(section, entry, Name_From_RTTI(defvalue), buffer, sizeof(buffer));
    return RTTI_From_Name(buffer);
}


// CCINIClass::Put_RTTIType (0x0046A4F0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_RTTIType(char const *section, char const *entry, RTTIType rtti)
{
    return Put_String(section, entry, Name_From_RTTI(rtti));
}


// CCINIClass::Get_BuildCat (0x0046A520).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

BuildCat CCINIClass::Get_BuildCat(char const *section, char const *entry, BuildCat defvalue) const
{
    char buffer[32];
    Get_String(section, entry, Name_From_BuildCat(defvalue), buffer, sizeof(buffer));
    return BuildCat_From_Name(buffer);
}


// CCINIClass::Put_BuildCat (0x0046A560).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_BuildCat(char const *section, char const *entry, BuildCat category)
{
    return Put_String(section, entry, Name_From_BuildCat(category));
}


// CCINIClass::Get_Owners (0x0046A590).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Owners(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    int bits = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        bits = 0;
        for (char const *name = strtok(buffer, ","); name != 0; name = strtok(0, ",")) {
            bits |= Owner_From_Name(name);
        }
    }
    return bits;
}


// CCINIClass::Put_Owners (0x0046A610).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Owners(char const *section, char const *entry, int owners)
{
    char buffer[128];
    buffer[0] = '\0';
    if (owners != 0) {
        for (int index = 0; index < HouseTypes.Count(); index++) {
            HouseTypeClass *house = HouseTypes[index];
            if ((owners & (1 << house->House)) != 0) {
                if (buffer[0] != '\0') {
                    strcat(buffer, ",");
                }
                strcat(buffer, house->ID);
            }
        }
        if (buffer[0] != '\0') {
            return Put_String(section, entry, buffer);
        }
    }
    return true;
}


// CCINIClass::Get_Houses (0x0046A720).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Houses(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    int bits = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        bits = 0;
        for (char const *name = strtok(buffer, ","); name != 0; name = strtok(0, ",")) {
            bits |= 1 << HouseClass::Find_Index(name);
        }
    }
    return bits;
}


// CCINIClass::Put_Houses (0x0046A7A0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Houses(char const *section, char const *entry, int houses)
{
    char buffer[128];
    buffer[0] = '\0';
    if (houses != 0) {
        for (int index = 0; index < Houses.Count(); index++) {
            HouseClass *house = Houses[index];
            if ((houses & (1 << house->ArrayIndex)) != 0) {
                if (buffer[0] != '\0') {
                    strcat(buffer, ",");
                }
                strcat(buffer, house->IniName);
            }
        }
        if (buffer[0] != '\0') {
            return Put_String(section, entry, buffer);
        }
    }
    return true;
}


// CCINIClass::Get_ArmorType (0x0046A8B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

ArmorType CCINIClass::Get_ArmorType(char const *section, char const *entry, ArmorType defvalue) const
{
    char buffer[128];
    Get_String(section, entry, ArmorNames[defvalue], buffer, sizeof(buffer));
    return Armor_From_Name(buffer);
}


// CCINIClass::Put_ArmorType (0x0046A900).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_ArmorType(char const *section, char const *entry, ArmorType armor)
{
    return Put_String(section, entry, ArmorNames[armor]);
}


// CCINIClass::Put_VocType (0x0046A920).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_VocType(char const *section, char const *entry, int voc)
{
    if ((char)voc == -1) {
        return Put_String(section, entry, "<none>");
    }
    return Put_String(section, entry, Voc_Name(voc));
}


// CCINIClass::Get_LandType (0x0046A970).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

LandType CCINIClass::Get_LandType(char const *section, char const *entry, LandType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, Name_From_Land(defvalue), buffer, sizeof(buffer))) {
        return Land_From_Name(buffer);
    }
    return defvalue;
}


// CCINIClass::Put_LandType (0x0046A9D0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_LandType(char const *section, char const *entry, LandType land)
{
    return Put_String(section, entry, Name_From_Land(land));
}


// CCINIClass::Get_HousesType (0x0046AA00).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_HousesType(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int index = HouseTypeClass::Find_Index(buffer);
        if (index == -1) {
            // An unknown name creates the house type.
            return HouseTypes.ID(new HouseTypeClass(buffer));
        }
        return index;
    }
    return defvalue;
}


// CCINIClass::Put_HousesType (0x0046AAB0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_HousesType(char const *section, char const *entry, int house)
{
    char const *name = "<none>";
    if (house == -2) {
        name = "<random>";
    } else if (house == -3) {
        name = "<random>";
    } else if (house != -1 && house >= 0 && house < HouseTypes.Count()) {
        name = HouseTypes[house]->ID;
    }
    return Put_String(section, entry, name);
}


// CCINIClass::Get_Side (0x0046AB30).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Side(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int index = SideClass::Find_Index(buffer);
        if (index == -1) {
            // An unknown name creates the side.
            return Sides.ID(new SideClass(buffer));
        }
        return index;
    }
    return defvalue;
}


// CCINIClass::Put_Side (0x0046ABE0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Side(char const *section, char const *entry, int side)
{
    char const *name = "<none>";
    if (side != -1) {
        name = Sides[side]->ID;
    }
    return Put_String(section, entry, name);
}


// CCINIClass::Get_Listed_Name (0x0046AC10).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_Listed_Name(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        int index = unknown_name_index_482e60(buffer);
        if (index != -1) {
            return index;
        }
    }
    return defvalue;
}


// CCINIClass::Put_Listed_Name (0x0046AC60).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Listed_Name(char const *section, char const *entry, unsigned index)
{
    if (index >= (unsigned)unknown_names_a71050.Count()) {
        return Put_String(section, entry, "<none>");
    }
    return Put_String(section, entry, unknown_names_a71050[index]);
}


// CCINIClass::Get_TheaterType (0x0046ACB0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

TheaterType CCINIClass::Get_TheaterType(char const *section, char const *entry, TheaterType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Theater_From_Name(buffer);
    }
    return defvalue;
}


// CCINIClass::Put_TheaterType (0x0046AD00).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_TheaterType(char const *section, char const *entry, TheaterType theater)
{
    return Put_String(section, entry, Theaters[theater].Name);
}


// CCINIClass::Get_ThemeType (0x0046AD30).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

int CCINIClass::Get_ThemeType(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Theme.From_Name(buffer);
    }
    return defvalue;
}


// CCINIClass::Put_ThemeType (0x0046AD90).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_ThemeType(char const *section, char const *entry, int theme)
{
    return Put_String(section, entry, Theme.Base_Name(theme));
}


// CCINIClass::Get_SourceType (0x0046ADC0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

SourceType CCINIClass::Get_SourceType(char const *section, char const *entry, SourceType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Source_From_Name(buffer);
    }
    return defvalue;
}


// CCINIClass::Put_SourceType (0x0046AE10).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_SourceType(char const *section, char const *entry, SourceType source)
{
    return Put_String(section, entry, SourceNames[source]);
}


// CCINIClass::Get_CrateType (0x0046AE30).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

CrateType CCINIClass::Get_CrateType(char const *section, char const *entry, CrateType defvalue) const
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Crate_From_Name(buffer);
    }
    return defvalue;
}


// CCINIClass::Put_CrateType (0x0046AE80).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_CrateType(char const *section, char const *entry, CrateType crate)
{
    return Put_String(section, entry, CrateNames[crate]);
}


// CCINIClass::Get_Buildings (0x0046AEA0; TibSun ccini.cpp): the building bitfield of a comma
// separated list: POWER, FACTORY, BARRACKS and RADAR set the top four bits, a building type ID
// its own bit (BuildingTypeClass::From_Name); defvalue when the entry is missing.

int CCINIClass::Get_Buildings(char const *section, char const *entry, int defvalue) const
{
    char buffer[128];
    int pre;

    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        pre = 0;
        char *token = strtok(buffer, ",");
        while (token != 0 && *token != '\0') {
            if (!_stricmp(token, "POWER")) {
                pre |= 1L << (32 - 1);
            }
            if (!_stricmp(token, "FACTORY")) {
                pre |= 1L << (32 - 2);
            }
            if (!_stricmp(token, "BARRACKS")) {
                pre |= 1L << (32 - 3);
            }
            if (!_stricmp(token, "RADAR")) {
                pre |= 1L << (32 - 4);
            }
            int building = BuildingTypeClass::From_Name(token);
            if (building != -1) {
                pre |= (1L << building);
            }
            token = strtok(0, ",");
        }
    } else {
        pre = defvalue;
    }

    return pre;
}


// CCINIClass::Put_Buildings (0x0046AFB0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Buildings(char const *section, char const *entry, int buildings)
{
    char buffer[128] = "";
    int count = BuildingTypes.Count() > 32 ? 32 : BuildingTypes.Count();
    for (int index = 0; index < count; index++) {
        if ((buildings & (1 << index)) != 0) {
            if (buffer[0] != '\0') {
                strcat(buffer, ",");
            }
            strcat(buffer, BuildingTypes[index]->ID);
        }
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Put_Voc_List (0x0046B0B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Voc_List(char const *section, char const *entry, TypeList<int> list)
{
    char buffer[128] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        strcat(buffer, Voc_Name(list[index]));
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Integer_List (0x0046B1B0).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> CCINIClass::Get_Integer_List(char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            list.Add(atoi(token));
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_Integer_List (0x0046B370).
// Layout and behaviour: shared/ccini_class.hpp.
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
extern "C" char *__cdecl strcat(char *destination, char const *source);
#pragma intrinsic(strcat)

bool CCINIClass::Put_Integer_List(char const *section, char const *entry, TypeList<int> const &list)
{
    char buffer[512] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        char number[12];
        sprintf(number, "%d", list[index]);
        strcat(buffer, number);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Anim_Target_List (0x0046B460).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> CCINIClass::Get_Anim_Target_List(char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            TargetClass target;
            int index = InfantryTypeClass::From_Name(token);
            if (index != -1) {
                target = TargetClass(InfantryTypes[index]);
            } else {
                index = UnitTypeClass::From_Name(token);
                if (index == -1) {
                    continue;
                }
                target = TargetClass(UnitTypes[index]);
            }
            if (target.Is_Valid()) {
                list.Add(target.As_Target());
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_Anim_Target_List (0x0046B680).
// Layout and behaviour: shared/ccini_class.hpp; TargetClass in
// CCINIClass_Get_Anim_Target_List_0046b460.cpp.

bool CCINIClass::Put_Anim_Target_List(char const *section, char const *entry, TypeList<int> const &list)
{
    char buffer[512] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        TargetClass target;
        target = TargetClass(list[index]);
        AbstractTypeClass *type = target.As_TechnoType();
        strcat(buffer, type->ID);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Float_Triple (0x0046B780).
// Layout and behaviour: shared/ccini_class.hpp.
extern "C" double __cdecl atof(char const *string);

INIVector3 CCINIClass::Get_Float_Triple(char const *section, char const *entry, INIVector3 const &defvalue)
{
    char buffer[512];
    INIVector3 value = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        value.X = atof(strtok(buffer, ","));
        value.Y = atof(strtok(0, ","));
        value.Z = atof(strtok(0, ","));
        return value;
    }
    return defvalue;
}


// CCINIClass::Get_Integer_Triple (0x0046B860).
// Layout and behaviour: shared/ccini_class.hpp.

INIPoint3D CCINIClass::Get_Integer_Triple(char const *section, char const *entry, INIPoint3D const &defvalue)
{
    char buffer[512];
    INIPoint3D value = defvalue;
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        value.X = atoi(strtok(buffer, ","));
        value.Y = atoi(strtok(0, ","));
        value.Z = atoi(strtok(0, ","));
        return value;
    }
    return defvalue;
}


// CCINIClass::Get_Techno_List (0x0046B930).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<TechnoTypeClass *> CCINIClass::Get_Techno_List(char const *section, char const *entry,
                                                        TypeList<TechnoTypeClass *> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<TechnoTypeClass *> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            for (int index = 0; index < TechnoTypes.Count(); index++) {
                if (_stricmp(token, TechnoTypes[index]->ID) == 0) {
                    list.Add(TechnoTypes[index]);
                    break;
                }
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_Techno_List (0x0046BB30).
// Layout and behaviour: shared/ccini_class.hpp; TechnoTypeClass in
// CCINIClass_Get_Techno_List_0046b930.cpp.

bool CCINIClass::Put_Techno_List(char const *section, char const *entry, TypeList<TechnoTypeClass *> const &list)
{
    char buffer[512] = "";
    for (int index = 0; index < list.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        strcat(buffer, list[index]->ID);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_House_List (0x0046BC00).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> CCINIClass::Get_House_List(char const *section, char const *entry, TypeList<int> defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            int index = HouseTypeClass::Find_Index(token);
            if (index != -1) {
                list.Add(index);
            } else {
                int side = SideClass::Find_Index(token);
                if (side != -1) {
                    SideClass *sideptr = Sides[side];
                    for (int house = 0; house < sideptr->Houses.Count(); house++) {
                        list.Add(sideptr->Houses[house]);
                    }
                }
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Put_House_List (0x0046BE60; TibSun ccini.cpp): stores a list of house types as a
// comma separated list of their IDs.

bool CCINIClass::Put_House_List(char const *section, char const *entry, TypeList<int> value)
{
    char buffer[128] = "";

    for (int index = 0; index < value.Count(); index++) {
        if (buffer[0] != '\0') {
            strcat(buffer, ",");
        }
        strcat(buffer, HouseTypes[value[index]]->ID);
    }
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Color_List (0x0046BF60).
// Layout and behaviour: shared/ccini_class.hpp, shared/rgb_class.hpp.

TypeList<RGBClass> CCINIClass::Get_Color_List(char const *section, char const *entry, TypeList<RGBClass> defvalue)
{
    char buffer[512];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<RGBClass> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            RGBClass color;
            bool valid = true;
            if (token != 0) {
                color.Red = atoi(token + 1);      // after the '('
            } else {
                valid = false;
            }
            token = strtok(0, ",");
            if (token != 0 && *token != '\0') {
                color.Green = atoi(token);
            } else {
                valid = false;
            }
            token = strtok(0, ",");
            if (token != 0 && *token != '\0') {
                token[strlen(token) - 1] = '\0';  // drops the ')'
                color.Blue = atoi(token);
                if (valid) {
                    list.Add(color);
                }
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Get_Unique_ID (0x0046C1C0).
// Layout and behaviour: shared/ccini_class.hpp (Calculate_Message_Digest,
// defined later in the unit, is expanded here).

int CCINIClass::Get_Unique_ID() const
{
    if (!IsDigestValid) {
        ((CCINIClass *)this)->Calculate_Message_Digest();
    }
    return CRCEngine()(&Digest[0], sizeof(Digest));
}


// CCINIClass::Calculate_Message_Digest (0x0046C260).
// Layout and behaviour: shared/ccini_class.hpp, shared/sha_pipe.hpp.

void CCINIClass::Calculate_Message_Digest()
{
    SHAPipe sha;
    INIClass::Save(sha);
    sha.Result(Digest);
    IsDigestValid = true;
}


// 0x0046c2e0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_0046c2e0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0046c2e0 {

void __fastcall FUN_0046c2e0(int object_address)
{
    *(unsigned char *)(object_address + 0x38) = 0;
    return;
}

}  // namespace seed_0046c2e0


// CCINIClass::Get_Object_Type (0x0046C2F0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

ObjectTypeClass *CCINIClass::Get_Object_Type(char const *section, char const *entry)
{
    char buffer[128];
    ObjectTypeClass *type = 0;
    Get_String(section, entry, "<none>", buffer, sizeof(buffer));
    int index = InfantryTypeClass::From_Name(buffer);
    if (index != -1) {
        type = InfantryTypes[index];
    } else {
        index = UnitTypeClass::From_Name(buffer);
        if (index != -1) {
            type = UnitTypes[index];
        } else {
            index = AircraftTypeClass::From_Name(buffer);
            if (index != -1) {
                type = AircraftTypes[index];
            } else {
                index = BuildingTypeClass::From_Name(buffer);
                if (index != -1) {
                    type = BuildingTypes[index];
                }
            }
        }
    }
    return type;
}


// CCINIClass::Put_Target_Type (0x0046C3B0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

bool CCINIClass::Put_Target_Type(char const *section, char const *entry, TargetClass const &target)
{
    AbstractTypeClass *type = target.As_TechnoType();
    if (type != 0) {
        return Put_String(section, entry, type->ID);
    }
    return Put_String(section, entry, "<none>");
}


// CCINIClass::Get_SpeedType (0x0046C400).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

SpeedType CCINIClass::Get_SpeedType(char const *section, char const *entry, SpeedType defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, Name_From_Speed(defvalue), buffer, sizeof(buffer))) {
        return Speed_From_Name(buffer);
    }
    return defvalue;
}


class Call_46c460_0 {public: unsigned int Name_From_Speed_00482f60();};
class Call_46c460_1 {public: unsigned int Put_String_0050e5f0(unsigned int target_section, unsigned int target_entry, unsigned int target_value);};
class Leaf_46c460 { public: unsigned int get(unsigned int section_name, unsigned int entry_name, unsigned int speed); };
unsigned int Leaf_46c460::get(unsigned int section_name, unsigned int entry_name, unsigned int speed) { unsigned int speed_name_address = ((Call_46c460_0*)(speed))->Name_From_Speed_00482f60(); unsigned int write_result = ((Call_46c460_1*)((unsigned int)this))->Put_String_0050e5f0((unsigned int)(section_name), (unsigned int)(entry_name), (unsigned int)(speed_name_address)); return write_result; }


// CCINIClass::Get_LayerType (0x0046C490).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

LayerType CCINIClass::Get_LayerType(char const *section, char const *entry, LayerType defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, Name_From_Layer(defvalue), buffer, sizeof(buffer))) {
        return Layer_From_Name(buffer);
    }
    return defvalue;
}


class Call_46c4f0_0 {public: unsigned int Name_From_Layer_00482fc0();};
class Call_46c4f0_1 {public: unsigned int Put_String_0050e5f0(unsigned int target_section, unsigned int target_entry, unsigned int target_value);};
class Leaf_46c4f0 { public: unsigned int get(unsigned int section_name, unsigned int entry_name, unsigned int layer); };
unsigned int Leaf_46c4f0::get(unsigned int section_name, unsigned int entry_name, unsigned int layer) { unsigned int layer_name_address = ((Call_46c4f0_0*)(layer))->Name_From_Layer_00482fc0(); unsigned int write_result = ((Call_46c4f0_1*)((unsigned int)this))->Put_String_0050e5f0((unsigned int)(section_name), (unsigned int)(entry_name), (unsigned int)(layer_name_address)); return write_result; }


// Get_Prerequisite_List (0x0046C520).
// Layout and behaviour: shared/ccini_class.hpp.

TypeList<int> __fastcall Get_Prerequisite_List(CCINIClass &ini, char const *section, char const *entry,
                                               TypeList<int> defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, "", buffer, sizeof(buffer))) {
        TypeList<int> list;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            bool found = false;
            if (_stricmp(token, "POWER") == 0) {
                list.Add(-1);
                found = true;
            }
            if (_stricmp(token, "FACTORY") == 0) {
                list.Add(-2);
                found = true;
            }
            if (_stricmp(token, "BARRACKS") == 0) {
                list.Add(-3);
                found = true;
            }
            if (_stricmp(token, "RADAR") == 0) {
                list.Add(-4);
                found = true;
            }
            if (_stricmp(token, "TECH") == 0) {
                list.Add(-5);
                found = true;
            }
            if (_stricmp(token, "PROC") == 0) {
                list.Add(-6);
                found = true;
            }
            if (!found) {
                int index = BuildingTypeClass::From_Name(token);
                if (index != -1) {
                    list.Add(index);
                }
            }
        }
        return list;
    }
    return defvalue;
}


// CCINIClass::Get_Integers (0x0046C880).
// Layout and behaviour: shared/ccini_class.hpp.

void CCINIClass::Get_Integers(char const *section, char const *entry, int &first, int &second, int &third)
{
    char buffer[512] = "";
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        char *token = strtok(buffer, ",");
        if (token != 0) {
            first = atoi(token);
        }
        token = strtok(0, ",");
        if (token != 0) {
            second = atoi(token);
        }
        token = strtok(0, ",");
        if (token != 0) {
            third = atoi(token);
        }
    }
}


// CCINIClass::Put_Integers (0x0046C950).
// Layout and behaviour: shared/ccini_class.hpp.

bool CCINIClass::Put_Integers(char const *section, char const *entry, int first, int second, int third)
{
    char buffer[512] = "";
    sprintf(buffer, "%d,%d,%d", first, second, third);
    return Put_String(section, entry, buffer);
}


// CCINIClass::Get_Level (0x0046C9D0).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

LevelType CCINIClass::Get_Level(char const *section, char const *entry, LevelType defvalue)
{
    char buffer[128];
    Get_String(section, entry, LevelNames[defvalue], buffer, sizeof(buffer));
    char *token = strtok(buffer, ",");
    if (_stricmp(token, "None") == 0) {
        return LEVEL_NONE;
    }
    if (_stricmp(token, "Normal") == 0) {
        return LEVEL_NORMAL;
    }
    if (_stricmp(token, "Strong") == 0) {
        return LEVEL_STRONG;
    }
    return defvalue;
}


// CCINIClass::Get_Abilities (0x0046CA80).
// Layout and behaviour: shared/ccini_class.hpp, shared/ccini_extra.hpp.

AbilitiesStruct CCINIClass::Get_Abilities(char const *section, char const *entry, AbilitiesStruct const &defvalue)
{
    char buffer[128];
    if (Get_String(section, entry, "", buffer, sizeof(buffer))) {
        AbilitiesStruct abilities;
        for (char *token = strtok(buffer, ","); token != 0 && *token != '\0'; token = strtok(0, ",")) {
            int ability = Ability_From_Name(token);
            if (ability != -1) {
                abilities.Ability[ability] = true;
            }
        }
        return abilities;
    }
    return defvalue;
}


// SHAPipe scalar deleting destructor (0x0046CB90): emitted in the CCINI unit
// with SHAPipe's vtable by CCINIClass::Calculate_Message_Digest; no source of
// its own (shared/sha_pipe.hpp).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int>::TypeList, copy constructor (0x0046CBF0; vtable 0x0079D9F4).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<int>::Clear (0x0046CC80; TypeList<int> vtable 0x0079d9f4 slot 3).
// DynamicVectorClass<int>::ID_pointer (0x0046CCB0; TypeList<int> vtable 0x0079d9f4 slot 5).
// VectorClass<int>::Fetch (0x0046CCD0; VectorClass<int> vtable 0x0079d9d4 slot 6).
// DynamicVectorClass<TechnoTypeClass *>::Clear (0x0046CCE0; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 3).
// DynamicVectorClass<TechnoTypeClass *>::ID_pointer (0x0046CD10; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 5).
// VectorClass<TechnoTypeClass *>::Fetch (0x0046CD30; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 6).


// TypeList members this unit emits by its own use of its type lists (bodies: shared/type_list.hpp):
// TypeList<int> scalar deleting destructor (0x0046CDD0; vtable 0x0079D9F4 slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x0079D9D4. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x0050B0F0.


extern "C" unsigned char vtable_46ceb0[]; extern "C" void __cdecl release_buffer_46ceb0(void*); extern "C" void __cdecl release_object_46ceb0(void*);
class Leaf_46ceb0 { public: void* get(unsigned int flags); };
void* Leaf_46ceb0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46ceb0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46ceb0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46ceb0(this); return this; }


extern "C" unsigned char vtable_46cf00[]; extern "C" void __cdecl release_buffer_46cf00(void*); extern "C" void __cdecl release_object_46cf00(void*);
class Leaf_46cf00 { public: void* get(unsigned int flags); };
void* Leaf_46cf00::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46cf00; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46cf00((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46cf00(this); return this; }


extern "C" unsigned char vtable_46cf50[]; extern "C" void __cdecl release_buffer_46cf50(void*); extern "C" void __cdecl release_object_46cf50(void*);
class Leaf_46cf50 { public: void* get(unsigned int flags); };
void* Leaf_46cf50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46cf50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46cf50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46cf50(this); return this; }


// VectorClass<int> copy constructor (0x0046CFA0): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<int>::Add (0x0046D300): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<int>::VectorClass(unsigned size, int const *array) (0x0046D020; stores the VectorClass<int> vtable 0x0079d9d4).
// VectorClass<int>::operator equal (0x0046D070; VectorClass<int> vtable 0x0079d9d4 slot 1).
// VectorClass<int>::Resize (0x0046D0B0; VectorClass<int> vtable 0x0079d9d4 slot 2).
// VectorClass<int>::Clear (0x0046D160; VectorClass<int> vtable 0x0079d9d4 slot 3).
// VectorClass<int>::ID_object (0x0046D190; VectorClass<int> vtable 0x0079d9d4 slot 4).
// VectorClass<int>::ID_pointer (0x0046D1D0; VectorClass<int> vtable 0x0079d9d4 slot 5).
// DynamicVectorClass<int>::Resize (0x0046D250; TypeList<int> vtable 0x0079d9f4 slot 2).
// DynamicVectorClass<int>::ID_object (0x0046D350; TypeList<int> vtable 0x0079d9f4 slot 4).


// VectorClass<TechnoTypeClass *> copy constructor (0x0046D380): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<TechnoTypeClass *>::operator equal (0x0046D400; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 1).
// VectorClass<TechnoTypeClass *>::Resize (0x0046D440; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 2).
// VectorClass<TechnoTypeClass *>::Clear (0x0046D4F0; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 3).
// VectorClass<TechnoTypeClass *>::ID_object (0x0046D520; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 4).
// VectorClass<TechnoTypeClass *>::ID_pointer (0x0046D560; VectorClass<TechnoTypeClass *> vtable 0x0079da14 slot 5).
// DynamicVectorClass<TechnoTypeClass *>::Resize (0x0046D580; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 2).
// DynamicVectorClass<TechnoTypeClass *>::ID_object (0x0046D630; TypeList<TechnoTypeClass *> vtable 0x0079da34 slot 4).


// VectorClass<RGBClass> copy constructor (0x0046D660): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// VectorClass<RGBClass> operator equal (0x0046D720): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// VectorClass<RGBClass> Resize (0x0046D780): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<RGBClass>::ID_object (0x0046D8B0; VectorClass<RGBClass> vtable 0x0079da54 slot 4).
// Layout and behaviour: shared/vector_class.hpp; RGBClass::operator== compares the three bytes (shared/rgb_class.hpp).


// DynamicVectorClass<RGBClass> Resize (0x0046D930): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<RGBClass>::ID_object (0x0046DA30; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; RGBClass::operator== compares the three bytes (shared/rgb_class.hpp).


extern "C" unsigned char vtable_46da70[]; extern "C" void __cdecl release_buffer_46da70(void*); extern "C" void __cdecl release_object_46da70(void*);
class Leaf_46da70 { public: void* get(unsigned int flags); };
void* Leaf_46da70::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46da70; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46da70((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46da70(this); return this; }


extern "C" unsigned char vtable_46dac0[]; extern "C" void __cdecl release_buffer_46dac0(void*); extern "C" void __cdecl release_object_46dac0(void*);
class Leaf_46dac0 { public: void* get(unsigned int flags); };
void* Leaf_46dac0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46dac0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46dac0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46dac0(this); return this; }


extern "C" unsigned char vtable_46db10[]; extern "C" void __cdecl release_buffer_46db10(void*); extern "C" void __cdecl release_object_46db10(void*);
class Leaf_46db10 { public: void* get(unsigned int flags); };
void* Leaf_46db10::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_46db10; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_46db10((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_46db10(this); return this; }


// VectorClass<int> operator assign (0x0046DB60): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


extern "C" unsigned char table_46dbd0[]; extern "C" void* __cdecl storage_46dbd0(unsigned int);
class Storage_46dbd0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_46dbd0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_46dbd0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_46dbd0(requested << 2); owns=1; } return this; }


// VectorClass<TechnoTypeClass *> operator assign (0x0046DC20): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.


// VectorClass<RGBClass> operator assign (0x0046DD20): instantiated in the CCINI unit by the CCINIClass list readers
// from shared/vector_class.hpp; no source of its own.
