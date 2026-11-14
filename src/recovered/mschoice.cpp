// mschoice.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mschoice.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ms_choice.hpp"
#include "shared/cc_file_class.hpp"


// Dynamic initializer of unknown_empty_rect (0x005AFEF0, _$E), the file-local
// empty Rect from shared/shape_file.hpp (0x00A71828 in this unit).


// MapSelectData::MapSelectData() (0x005AFF10)
// Empty lists and the default text rectangle.

MapSelectData::MapSelectData()
{
    Stages.Clear();
    Palette.Release();
    Anims.Clear();
}


// MapSelectData::~MapSelectData() (0x005AFFA0)

MapSelectData::~MapSelectData()
{
    Clear();
}


// bool MapSelectData::Load(Wstring const &section) (0x005B0030)
// Reads MAPSEL.INI: the stages listed as 1..100 under the section, then
// the section's Anims list (with TextRect and Palette), then its Sounds.
// Assumption: CCFileClass::Open(char const *, int) is an inline member of
// the original header (below; evidence: the Set_Name/Open calls on the local
// file are virtual, and its kept copy 0x00401980 sits among the inline
// header members of the first object file). Expanding it here is also what
// leaves GenericList::GenericList (0x00490E80) out of line in this function.

// CCFileClass::Open(char const *, int) (0x00401980): an inline header member
// in the original; the unqualified Set_Name/Open calls inside it stay virtual
// when it is expanded. Defined here until shared/cc_file_class.hpp carries it.
inline int CCFileClass::Open(char const *filename, int rights)
{
    Set_Name(filename);
    return Open(rights);
}

bool MapSelectData::Load(Wstring const &section)
{
    CCFileClass file;
    INIClass ini;
    Wstring anims;
    Wstring value;

    Clear();
    file.Open("MAPSEL.INI", 1);
    if (ini.Load(file, false) == 0) {
        Debug_Printf("Failed to load MAPSEL.INI\n");
        return false;
    }

    int index;
    for (index = 1; index <= 100; index++) {
        if (ini.Get_String(section, Int_To_Wstring(index), (char const *)0, value) == 0) {
            break;
        }
        MapStage *stage = new MapStage(ini, value);
        if (stage == 0) {
            Debug_Printf("MapSelect: Failed to create stage %s\n", value);
            return false;
        }
        Stages.Add(stage);
    }
    if (Stages.Count() == 0) {
        Debug_Printf("MapSelect: There isn't any stages!\n");
        return false;
    }

    if (ini.Get_String(section, "Anims", (char const *)0, anims) > 0) {
        if (ini.Get_String(anims, "TextRect", (char const *)0, value) > 0) {
            Wstring token;
            int position = value.Get_Token(0, ",", token);
            if (token.Length() > 0) {
                TextRect.X = token.As_Integer();
            }
            position = value.Get_Token(position, ",", token);
            if (token.Length() > 0) {
                TextRect.Y = token.As_Integer();
            }
            position = value.Get_Token(position, ",", token);
            if (token.Length() > 0) {
                TextRect.Width = token.As_Integer();
            }
            value.Get_Token(position, ",", token);
            if (token.Length() > 0) {
                TextRect.Height = token.As_Integer();
            }
        }
        if (ini.Get_String(anims, "Palette", (char const *)0, value) > 0) {
            Palette = value;
        }
        for (index = 1; index <= 100; index++) {
            if (ini.Get_String(anims, Int_To_Wstring(index), (char const *)0, value) == 0) {
                break;
            }
            MSAnimEntry *anim = new MSAnimEntry(value);
            if (anim == 0) {
                Debug_Printf("Failed to create anim %s\n", value);
                return false;
            }
            Anims.Add(anim);
        }
    }

    if (unknown_check_406f30()) {
        Wstring entry;
        if (ini.Get_String(section, "Sounds", (char const *)0, anims) > 0) {
            for (index = 0; index < ini.Entry_Count(anims); index++) {
                entry = ini.Get_Entry(anims, index);
                if (entry != 0 && ini.Get_String(anims, entry, (char const *)0, value) > 0) {
                    MSSfxEntry *sound = new MSSfxEntry(entry, value);
                    if (sound->Name == 0) {
                        delete sound;
                    } else if (sound != 0) {
                        Sounds.Add(sound);
                    }
                }
            }
        }
    }
    return true;
}


// void MapSelectData::Clear() (0x005B0730)
// Deletes the sound effects, animations and stages; resets the text
// rectangle to 640x400.

void MapSelectData::Clear()
{
    int index;
    for (index = 0; index < Sounds.Count(); index++) {
        if (Sounds[index] != 0) {
            delete Sounds[index];
        }
    }
    Sounds.Clear();
    TextRect.X = 0;
    TextRect.Y = 0;
    TextRect.Width = 640;
    TextRect.Height = 400;
    for (index = 0; index < Anims.Count(); index++) {
        if (Anims[index] != 0) {
            delete Anims[index];
        }
    }
    Anims.Clear();
    for (index = 0; index < Stages.Count(); index++) {
        if (Stages[index] != 0) {
            delete Stages[index];
        }
    }
    Stages.Clear();
}


// MapStage *MapSelectData::Find_Stage(Wstring const &name) const (0x005B07F0)
// Stage whose Name matches, ignoring case; null when absent.

MapStage *MapSelectData::Find_Stage(Wstring const &name) const
{
    if (name != 0) {
        for (int index = 0; index < Stages.Count(); index++) {
            Wstring wanted(name);
            wanted.To_Lower();
            Wstring stage(Stages[index]->Name);
            stage.To_Lower();
            if (wanted == stage) {
                return Stages[index];
            }
        }
    }
    return 0;
}


// MapStage *MapSelectData::Find_Stage_04(Wstring const &name) const (0x005B08A0)
// As Find_Stage, matching the stage's scenario name (Scenario).

MapStage *MapSelectData::Find_Stage_04(Wstring const &name) const
{
    if (name != 0) {
        for (int index = 0; index < Stages.Count(); index++) {
            Wstring wanted(name);
            wanted.To_Lower();
            Wstring stage(Stages[index]->Scenario);
            stage.To_Lower();
            if (wanted == stage) {
                return Stages[index];
            }
        }
    }
    return 0;
}


// MapStage *MapSelectData::Get_Stage(unsigned short index) const (0x005B0950)

MapStage *MapSelectData::Get_Stage(unsigned short index) const
{
    if (Stages.Count() <= index) {
        return 0;
    }
    return Stages[index];
}


// int MapSelectData::Stage_Index(MapStage *stage) (0x005B0970)

int MapSelectData::Stage_Index(MapStage *stage)
{
    return Stages.ID(stage);
}


// MSSfxEntry *MapSelectData::Find_Sound(Wstring const &name) const (0x005B0980)
// Sound entry of that name, ignoring case; null without sound.

MSSfxEntry *MapSelectData::Find_Sound(Wstring const &name) const
{
    if (unknown_check_406f30()) {
        for (int index = 0; index < Sounds.Count(); index++) {
            MSSfxEntry *sound = Sounds[index];
            Wstring entry(sound->Name);
            entry.To_Lower();
            Wstring wanted(name);
            wanted.To_Lower();
            if (entry == wanted) {
                return sound;
            }
        }
    }
    return 0;
}


// MapStage::MapStage(INIClass &ini, Wstring const &section) (0x005B0A20)
// Loads one stage from its MAPSEL.INI section: the scenario, description
// (a string-table label), voice-over, map movie, up to two overlays, the
// click map, the "Text1".."Text7" lines, the "Targets" points and the
// selections keyed "0".."255".
// Assumptions: `unused` is a local the original constructs and destroys but
// never reads; the Description line number 0x1E2 is the original __LINE__.

MapStage::MapStage(INIClass &ini, Wstring const &section)
{
    Wstring value;
    WideString text;
    Wstring unused;

    Name = section;

    Scenario = (char const *)0;
    if (ini.Get_String(section, "Scenario", (char const *)0, value) > 0) {
        Scenario = value;
    }

    Description = (unsigned short const *)0;
    if (ini.Get_String(section, "Description", (char const *)0, value) > 0) {
        Description = StringTable::Load_String(value.Get(), 0, "D:\\Ra2\\MSChoice.cpp", 0x1E2);
    }

    VoiceOver = (char const *)0;
    if (ini.Get_String(section, "VoiceOver", (char const *)0, value) > 0) {
        VoiceOver = value;
    }

    MapVQ = (char const *)0;
    if (ini.Get_String(section, "MapVQ", (char const *)0, value) > 0) {
        MapVQ = value;
    }

    OverlayCount = 0;
    int index;
    for (index = 0; index < 2; index++) {
        Overlays[index] = (char const *)0;
    }
    if (ini.Get_String(section, "Overlays", (char const *)0, value) > 0) {
        index = 0;
        Wstring token;
        int position = value.Get_Token(0, ",", token);
        while (token.Length() > 0 && index < 2) {
            Overlays[index] = token;
            position = value.Get_Token(position, ",", token);
            index++;
        }
        OverlayCount = index;
    }

    ClickMap = (char const *)0;
    if (ini.Get_String(section, "ClickMap", (char const *)0, value) > 0) {
        ClickMap = value;
    }

    for (index = 1; index < 8; index++) {
        if (ini.Get_String(section, "Text" + Int_To_Wstring(index), (char const *)0, value) > 0) {
            text = Wstring_To_Wide(value);
            MSTextEntry *entry = new MSTextEntry(text);
            if (entry != 0) {
                Texts.Add(entry);
            }
        }
    }

    if (ini.Get_String(section, "Targets", (char const *)0, value) > 0) {
        int count = 0;
        Wstring token;
        int position = value.Get_Token(0, ",", token);
        if (token.Length() > 0) {
            count = token.As_Integer();
        }
        for (index = 0; index < count; index++) {
            Point2D point(-1, -1);
            int next = value.Get_Token(position, ",", token);
            if (token.Length() > 0) {
                point.X = token.As_Integer();
            }
            position = value.Get_Token(next, ",", token);
            if (token.Length() > 0) {
                point.Y = token.As_Integer();
            }
            Targets.Add(point);
        }
    }

    for (index = 0; index < 256; index++) {
        if (ini.Get_String(section, Int_To_Wstring(index), (char const *)0, value) > 0) {
            MapSelection *selection = new MapSelection(value, (char)index);
            if (selection != 0) {
                Selections.Add(selection);
            }
        }
    }
}


// MapStage::~MapStage() (0x005B11B0)
// Deletes the stage's text lines and selections.

MapStage::~MapStage()
{
    int index;
    for (index = 0; index < Texts.Count(); index++) {
        if (Texts[index] != 0) {
            delete Texts[index];
        }
    }
    for (index = 0; index < Selections.Count(); index++) {
        if (Selections[index] != 0) {
            delete Selections[index];
        }
    }
}


// MapSelection *MapStage::Find_Selection(char const *name) const (0x005B12D0)
// Selection of that name, ignoring case; null when absent.

MapSelection *MapStage::Find_Selection(char const *name) const
{
    Wstring wanted(name);
    wanted.To_Lower();
    for (int index = 0; index < Selections.Count(); index++) {
        if (Selections[index]->Name != 0) {
            Wstring selection(Selections[index]->Name);
            selection.To_Lower();
            if (wanted == selection) {
                return Selections[index];
            }
        }
    }
    return 0;
}


// Wstring MapStage::Selection_Name(char key) const (0x005B1380)
// Name of the selection bound to a key, or a null string.

Wstring MapStage::Selection_Name(char key) const
{
    for (int index = 0; index < Selections.Count(); index++) {
        if (key == Selections[index]->Key) {
            return Selections[index]->Name;
        }
    }
    return Wstring((char const *)0);
}


// MSTextEntry::MSTextEntry(WideString const &line) (0x005B13D0)
// Parses "x,y,unknown,text".

MSTextEntry::MSTextEntry(WideString const &line) :
    X(0),
    Y(0),
    unknown_08(0),
    Text((unsigned short const *)0)
{
    WideString token;
    int position = line.Get_Token(0, L",", token);
    if (token.Length() > 0) {
        X = token.As_Integer();
    }
    position = line.Get_Token(position, L",", token);
    if (token.Length() > 0) {
        Y = token.As_Integer();
    }
    position = line.Get_Token(position, L",", token);
    if (token.Length() > 0) {
        unknown_08 = token.As_Integer();
    }
    line.Get_Token(position, L",", token);
    if (token.Length() > 0) {
        Text = token;
    }
}


// MSAnimEntry::MSAnimEntry(Wstring const &line) (0x005B14C0)
// Parses "name,a,b,c".

MSAnimEntry::MSAnimEntry(Wstring const &line) :
    Name((char const *)0),
    unknown_04(0),
    unknown_08(0),
    unknown_0C(0)
{
    Wstring token;
    int position = line.Get_Token(0, ",", token);
    if (token.Length() > 0) {
        Name = token;
    }
    position = line.Get_Token(position, ",", token);
    if (token.Length() > 0) {
        unknown_04 = token.As_Integer();
    }
    position = line.Get_Token(position, ",", token);
    if (token.Length() > 0) {
        unknown_08 = token.As_Integer();
    }
    line.Get_Token(position, ",", token);
    if (token.Length() > 0) {
        unknown_0C = token.As_Integer();
    }
}


// MSSfxEntry::MSSfxEntry(Wstring const &name, Wstring const &line) (0x005B15B0)
// With sound available: line is "file,volume" (volume in percent, clamped
// to 0..100 and scaled to 0..255); keeps the name when audible.

MSSfxEntry::MSSfxEntry(Wstring const &name, Wstring const &line) :
    Name((char const *)0),
    unknown_04(false),
    unknown_08(0),
    Volume(255)
{
    if (name != 0 && unknown_check_406f30()) {
        Wstring file;
        int position = line.Get_Token(0, ",", file);
        Wstring volume;
        line.Get_Token(position, ",", volume);
        if (volume != 0) {
            Volume = volume.As_Integer();
            if (Volume > 100) {
                Volume = 100;
            } else if (Volume < 0) {
                Volume = 0;
            }
            Volume = Volume * 255 / 100;
        }
        if (Volume > 0 && file != 0) {
            Name = name;
        }
    }
}


// MSSfxEntry::~MSSfxEntry() (0x005B16C0)
// Out-of-line for the screens that delete their entries (destroys Name).

MSSfxEntry::~MSSfxEntry()
{
}


// void MSSfxEntry::Play() (0x005B16D0)
// Empty in this build (callers find the entry, then call this).

void MSSfxEntry::Play()
{
}


// DynamicVectorClass_Point2D_Clear (0x005B16E0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_Point2D_ID_pointer (0x005B1710): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_Fetch (0x005B1730): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSTextEntry_ptr_Clear (0x005B1750): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSTextEntry_ptr_ID_pointer (0x005B1780): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_Fetch (0x005B17A0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapSelection_ptr_Clear (0x005B17B0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapSelection_ptr_ID_pointer (0x005B17E0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_Fetch (0x005B1800): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapStage_ptr_Clear (0x005B1810): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapStage_ptr_ID_pointer (0x005B1840): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_Fetch (0x005B1860): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSAnimEntry_ptr_Clear (0x005B1870): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSAnimEntry_ptr_ID_pointer (0x005B18A0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_Fetch (0x005B18C0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSSfxEntry_ptr_Clear (0x005B18D0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSSfxEntry_ptr_ID_pointer (0x005B1900): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_Fetch (0x005B1920): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_operator_equal (0x005B1930): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_Resize (0x005B1990): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_Clear (0x005B1A60): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_ID_object (0x005B1A90): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_ID_pointer (0x005B1AE0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_Point2D_Resize (0x005B1B00): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_Point2D_ID_object (0x005B1BE0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_operator_equal (0x005B1C20): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_Resize (0x005B1C60): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_Clear (0x005B1D10): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_ID_object (0x005B1D40): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_ID_pointer (0x005B1D80): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSTextEntry_ptr_Resize (0x005B1DA0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSTextEntry_ptr_ID_object (0x005B1E50): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_operator_equal (0x005B1E80): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_Resize (0x005B1EC0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_Clear (0x005B1F70): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_ID_object (0x005B1FA0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_ID_pointer (0x005B1FE0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapSelection_ptr_Resize (0x005B2000): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapSelection_ptr_ID_object (0x005B20B0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_operator_equal (0x005B20E0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_Resize (0x005B2120): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_Clear (0x005B21D0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_ID_object (0x005B2200): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_ID_pointer (0x005B2240): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapStage_ptr_Resize (0x005B2260): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapStage_ptr_ID_object (0x005B2310): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_operator_equal (0x005B2340): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_Resize (0x005B2380): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_Clear (0x005B2430): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_ID_object (0x005B2460): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_ID_pointer (0x005B24A0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSAnimEntry_ptr_Resize (0x005B24C0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSAnimEntry_ptr_ID_object (0x005B2570): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_operator_equal (0x005B25A0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_Resize (0x005B25E0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_Clear (0x005B2690): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_ID_object (0x005B26C0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_ID_pointer (0x005B2700): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSSfxEntry_ptr_Resize (0x005B2720): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSSfxEntry_ptr_ID_object (0x005B27D0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_Point2D_scalar_deleting_destructor (0x005B2800): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_Point2D_scalar_deleting_destructor (0x005B2850): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSTextEntry_ptr_scalar_deleting_destructor (0x005B28A0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSTextEntry_ptr_scalar_deleting_destructor (0x005B28F0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapSelection_ptr_scalar_deleting_destructor (0x005B2940): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapSelection_ptr_scalar_deleting_destructor (0x005B2990): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MapStage_ptr_scalar_deleting_destructor (0x005B29E0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MapStage_ptr_scalar_deleting_destructor (0x005B2A30): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSAnimEntry_ptr_scalar_deleting_destructor (0x005B2A80): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSAnimEntry_ptr_scalar_deleting_destructor (0x005B2AD0): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// VectorClass_MSSfxEntry_ptr_scalar_deleting_destructor (0x005B2B20): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.


// DynamicVectorClass_MSSfxEntry_ptr_scalar_deleting_destructor (0x005B2B70): instantiated in MSChoice.cpp from shared/vector_class.hpp
// (the map selection lists); no source of its own.
