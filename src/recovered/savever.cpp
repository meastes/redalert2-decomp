// savever.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/savever.json carries the addresses and the rows that
// stay in their own files.


#define COM_PTR_IMPLEMENT_RELEASE
#define COM_PTR_RELEASE_IN_DESTRUCTOR
#include "shared/savegame_info.hpp"
#include "shared/com_ptr.hpp"


// SaveVersionInfo::SaveVersionInfo() (0x00659240)
// The first function of the SaveVersionInfo unit (TibSun savever.cpp, a separate file from
// saveload.cpp: SaveLoad's Save_Game/Load_Game call its accessors out of line). The unit's smart
// pointers release through comip's _Release, expanded inline (shared/com_ptr.hpp).

SaveVersionInfo::SaveVersionInfo()
{
    InternalVersion = 0;
    Version = 0;
    CampaignNumber = -1;
    ScenarioNumber = 0;
    GameType = 0;
    ScenarioDescription[0] = 0;
    PlayerHouse[0] = 0;
    UnknownString[0] = 0;
    PlayerName[0] = 0;
    ExecutableName[0] = 0;
    StartTime.dwLowDateTime = 0;
    StartTime.dwHighDateTime = 0;
    PlayTime.dwLowDateTime = 0;
    PlayTime.dwHighDateTime = 0;
    LastSaveTime.dwLowDateTime = 0;
    LastSaveTime.dwHighDateTime = 0;
}


// void SaveVersionInfo::Set_Version(int value) (0x006592B0)

void SaveVersionInfo::Set_Version(int value)
{
    Version = value;
}


// int SaveVersionInfo::Get_Version() const (0x006592C0)

int SaveVersionInfo::Get_Version() const
{
    return Version;
}


// void SaveVersionInfo::Set_Internal_Version(int value) (0x006592D0)

void SaveVersionInfo::Set_Internal_Version(int value)
{
    InternalVersion = value;
}


// int SaveVersionInfo::Get_Internal_Version() const (0x006592E0)

int SaveVersionInfo::Get_Internal_Version() const
{
    return InternalVersion;
}


// void SaveVersionInfo::Set_Scenario_Description(wchar_t const *text) (0x006592F0)

void SaveVersionInfo::Set_Scenario_Description(wchar_t const *text)
{
    ScenarioDescription[0x7F] = 0;
    wcsncpy(ScenarioDescription, text, 0x7F);
}


// wchar_t const *SaveVersionInfo::Get_Scenario_Description() const (0x00659310)

wchar_t const *SaveVersionInfo::Get_Scenario_Description() const
{
    return ScenarioDescription;
}


// void SaveVersionInfo::Set_Player_House(wchar_t const *text) (0x00659320)

void SaveVersionInfo::Set_Player_House(wchar_t const *text)
{
    PlayerHouse[0x3F] = 0;
    wcsncpy(PlayerHouse, text, 0x3F);
}


// wchar_t const *SaveVersionInfo::Get_Player_House() const (0x00659350)

wchar_t const *SaveVersionInfo::Get_Player_House() const
{
    return PlayerHouse;
}


// void SaveVersionInfo::Set_Campaign_Number(int value) (0x00659360)

void SaveVersionInfo::Set_Campaign_Number(int value)
{
    CampaignNumber = value;
}


// int SaveVersionInfo::Get_Campaign_Number() const (0x00659370)

int SaveVersionInfo::Get_Campaign_Number() const
{
    return CampaignNumber;
}


// void SaveVersionInfo::Set_Scenario_Number(int value) (0x00659380)

void SaveVersionInfo::Set_Scenario_Number(int value)
{
    ScenarioNumber = value;
}


// int SaveVersionInfo::Get_Scenario_Number() const (0x00659390)

int SaveVersionInfo::Get_Scenario_Number() const
{
    return ScenarioNumber;
}


// void SaveVersionInfo::Set_Unknown_String(char const *text) (0x006593A0)

void SaveVersionInfo::Set_Unknown_String(char const *text)
{
    UnknownString[0x103] = 0;
    strncpy(UnknownString, text, 0x103);
}


// char const *SaveVersionInfo::Get_Unknown_String() const (0x006593D0)

char const *SaveVersionInfo::Get_Unknown_String() const
{
    return UnknownString;
}


// void SaveVersionInfo::Set_Player_Name(char const *text) (0x006593E0)

void SaveVersionInfo::Set_Player_Name(char const *text)
{
    PlayerName[0x3F] = 0;
    strncpy(PlayerName, text, 0x3F);
}


// char const *SaveVersionInfo::Get_Player_Name() const (0x00659400)
// mischa85/RedAlert2 commit 5a79accc names this SaveVersionInfo::Get_Player_House (src/savever.cpp),
// the char[64] at +0x294 (name hypothesis; recorded for the endgame renames).

char const *SaveVersionInfo::Get_Player_Name() const
{
    return PlayerName;
}


// void SaveVersionInfo::Set_Executable_Name(char const *text) (0x00659410)

void SaveVersionInfo::Set_Executable_Name(char const *text)
{
    ExecutableName[0x103] = 0;
    strncpy(ExecutableName, text, 0x103);
}


// char const *SaveVersionInfo::Get_Executable_Name() const (0x00659440)

char const *SaveVersionInfo::Get_Executable_Name() const
{
    return ExecutableName;
}


// void SaveVersionInfo::Set_Start_Time(FILETIME const &time) (0x00659450)

void SaveVersionInfo::Set_Start_Time(FILETIME const &time)
{
    StartTime = time;
}


// void SaveVersionInfo::Get_Start_Time(FILETIME &time) const (0x00659470)

void SaveVersionInfo::Get_Start_Time(FILETIME &time) const
{
    time = StartTime;
}


// void SaveVersionInfo::Set_Play_Time(FILETIME const &time) (0x00659490)

void SaveVersionInfo::Set_Play_Time(FILETIME const &time)
{
    PlayTime = time;
}


// void SaveVersionInfo::Get_Play_Time(FILETIME &time) const (0x006594B0)

void SaveVersionInfo::Get_Play_Time(FILETIME &time) const
{
    time = PlayTime;
}


// void SaveVersionInfo::Set_Last_Time(FILETIME const &time) (0x006594D0)

void SaveVersionInfo::Set_Last_Time(FILETIME const &time)
{
    LastSaveTime = time;
}


// void SaveVersionInfo::Get_Last_Time(FILETIME &time) const (0x006594F0)

void SaveVersionInfo::Get_Last_Time(FILETIME &time) const
{
    time = LastSaveTime;
}


// void SaveVersionInfo::Set_Game_Type(int type) (0x00659510)

void SaveVersionInfo::Set_Game_Type(int type)
{
    GameType = type;
}


// int SaveVersionInfo::Get_Game_Type() const (0x00659520)

int SaveVersionInfo::Get_Game_Type() const
{
    return GameType;
}


// long SaveVersionInfo::Save(IStorage *storage) (0x00659530)
// Writes the summary twice: as properties of the summary property set (when
// the storage has one) and as streams of the storage. Both "Player Name"
// entries (4 and 8) take +0x294.


long SaveVersionInfo::Save(IStorage *storage)
{
    if (storage == 0) {
        return E_POINTER;
    }
    UnknownComPtr<IPropertySetStorage, &unknown_uuid_IPropertySetStorage> set;
    HRESULT result;
    if (SUCCEEDED(storage->QueryInterface(IID_IPropertySetStorage, (void **)&set.Pointer))) {
        result = Write_Property_Wide(set, 2, ScenarioDescription);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Wide(set, 3, PlayerHouse);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Int(set, 9, Version);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Int(set, 16, InternalVersion);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Time(set, 12, &StartTime);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Time(set, 13, &LastSaveTime);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Time(set, 10, &PlayTime);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_String(set, 18, ExecutableName);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_String(set, 4, PlayerName);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_String(set, 8, PlayerName);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Int(set, 100, ScenarioNumber);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Int(set, 101, CampaignNumber);
        if (FAILED(result)) {
            return result;
        }
        result = Write_Property_Int(set, 102, GameType);
        if (FAILED(result)) {
            return result;
        }
    }
    result = Write_Stream_Wide(storage, 2, ScenarioDescription);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Wide(storage, 3, PlayerHouse);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Int(storage, 9, Version);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Int(storage, 16, InternalVersion);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Time(storage, 12, &StartTime);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Time(storage, 13, &LastSaveTime);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Time(storage, 10, &PlayTime);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_String(storage, 18, ExecutableName);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_String(storage, 4, PlayerName);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_String(storage, 8, PlayerName);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Int(storage, 100, ScenarioNumber);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Int(storage, 101, CampaignNumber);
    if (FAILED(result)) {
        return result;
    }
    result = Write_Stream_Int(storage, 102, GameType);
    if (FAILED(result)) {
        return result;
    }
    return S_OK;
}


// long SaveVersionInfo::Load(IStorage *storage) (0x00659A90)
// Reads the summary from the summary property set when the storage has one
// and its description property reads; otherwise from the storage's streams
// (Read_Stream_Int/Time expand inline here until the inline budget runs out:
// the stream-name lookup, Checked and _Release then go out of line, the last
// Read_Stream_Int is called). The `result < 0` tests (not FAILED's cast) and
// `(void **)&set` (comip operator&) are what give the original's budget.
extern "C" wchar_t *__cdecl wcscpy(wchar_t *dest, wchar_t const *source);   // 0x00783689
extern "C" char *__cdecl strcpy(char *destination, char const *source);
#pragma intrinsic(strcpy)


long SaveVersionInfo::Load(IStorage *storage)
{
    if (storage == 0) {
        return E_POINTER;
    }
    UnknownComPtr<IPropertySetStorage, &unknown_uuid_IPropertySetStorage> set;
    wchar_t wide[256];
    char text[256];
    HRESULT result;
    if (SUCCEEDED(storage->QueryInterface(IID_IPropertySetStorage, (void **)&set))
        && SUCCEEDED(result = Read_Property_Wide(set, 2, wide))) {
        wcscpy(ScenarioDescription, wide);
        result = Read_Property_Wide(set, 3, wide);
        if (result < 0) {
            return result;
        }
        wcscpy(PlayerHouse, wide);
        result = Read_Property_Int(set, 9, &Version);
        if (result < 0) {
            return result;
        }
        result = Read_Property_Int(set, 16, &InternalVersion);
        if (result < 0) {
            return result;
        }
        result = Read_Property_Time(set, 12, &StartTime);
        if (result < 0) {
            return result;
        }
        result = Read_Property_Time(set, 13, &LastSaveTime);
        if (result < 0) {
            return result;
        }
        result = Read_Property_Time(set, 10, &PlayTime);
        if (result < 0) {
            return result;
        }
        result = Read_Property_String(set, 18, text);
        if (result < 0) {
            return result;
        }
        strcpy(ExecutableName, text);
        result = Read_Property_String(set, 4, text);
        if (result < 0) {
            return result;
        }
        strcpy(PlayerName, text);
        result = Read_Property_Int(set, 100, &ScenarioNumber);
        if (result < 0) {
            return result;
        }
        result = Read_Property_Int(set, 101, &CampaignNumber);
        if (result < 0) {
            return result;
        }
        result = Read_Property_Int(set, 102, &GameType);
        if (result < 0) {
            return result;
        }
    } else {
    result = Read_Stream_Wide(storage, 2, wide);
    if (result < 0) {
        return result;
    }
    wcscpy(ScenarioDescription, wide);
    result = Read_Stream_Wide(storage, 3, wide);
    if (result < 0) {
        return result;
    }
    wcscpy(PlayerHouse, wide);
    result = Read_Stream_Int(storage, 9, &Version);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_Int(storage, 16, &InternalVersion);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_Time(storage, 12, &StartTime);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_Time(storage, 13, &LastSaveTime);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_Time(storage, 10, &PlayTime);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_String(storage, 18, text);
    if (result < 0) {
        return result;
    }
    strcpy(ExecutableName, text);
    result = Read_Stream_String(storage, 4, text);
    if (result < 0) {
        return result;
    }
    strcpy(PlayerName, text);
    result = Read_Stream_Int(storage, 100, &ScenarioNumber);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_Int(storage, 101, &CampaignNumber);
    if (result < 0) {
        return result;
    }
    result = Read_Stream_Int(storage, 102, &GameType);
    if (result < 0) {
        return result;
    }
    }
    return S_OK;
}


// HRESULT SaveVersionInfo::Read_Stream_String(IStorage *storage, int id, char *buffer) (0x0065A3A0; name hypothesis)
// Reads a summary value stored as a wide-string stream (127 characters).

HRESULT SaveVersionInfo::Read_Stream_String(IStorage *storage, int id, char *buffer)
{
    UnknownComPtr<IStream, &unknown_uuid_IStream> stream;
    wchar_t text[128];
    buffer[0] = 0;
    HRESULT result = storage->OpenStream(Savegame_Stream_Name(id), 0, 0x10, 0, &stream);
    if (FAILED(result)) {
        return result;
    }
    wchar_t *character = text;
    for (;;) {
        result = stream.Checked()->Read(character, sizeof(wchar_t), 0);
        if (FAILED(result)) {
            return result;
        }
        if (*character == 0) {
            break;
        }
        character++;
    }
    WideCharToMultiByte(0, 0, text, -1, buffer, 0x7F, 0, 0);
    return result;
}


// HRESULT SaveVersionInfo::Read_Stream_Wide(IStorage *storage, int id, wchar_t *buffer) (0x0065A490; name hypothesis)
// Reads a summary value stored as a wide-string stream.

HRESULT SaveVersionInfo::Read_Stream_Wide(IStorage *storage, int id, wchar_t *buffer)
{
    buffer[0] = 0;
    UnknownComPtr<IStream, &unknown_uuid_IStream> stream;
    HRESULT result = storage->OpenStream(Savegame_Stream_Name(id), 0, 0x10, 0, &stream);
    if (FAILED(result)) {
        return result;
    }
    for (;;) {
        result = stream.Checked()->Read(buffer, sizeof(wchar_t), 0);
        if (FAILED(result)) {
            return result;
        }
        if (*buffer == 0) {
            break;
        }
        buffer++;
    }
    return result;
}


// HRESULT SaveVersionInfo::Read_Property_String(IPropertySetStorage *storage, int id, char *buffer) (0x0065A540; name hypothesis)
// Reads a wide-string property of the summary property set.

HRESULT SaveVersionInfo::Read_Property_String(IPropertySetStorage *storage, int id, char *buffer)
{
    buffer[0] = 0;
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    HRESULT result = storage->Open(unknown_fmtid_7afd60, 0x12, &property);
    if (FAILED(result)) {
        return result;
    }
    PROPSPEC spec;
    spec.ulKind = 1;
    spec.propid = id;
    PROPVARIANT value;
    result = property.Checked()->ReadMultiple(1, &spec, &value);
    if (FAILED(result)) {
        return result;
    }
    if (value.vt == 0x1F) {
        WideCharToMultiByte(0, 0, value.pwszVal, -1, buffer, 0x80, 0, 0);
    }
    return result;
}


// HRESULT SaveVersionInfo::Read_Property_Wide(IPropertySetStorage *storage, int id, wchar_t *buffer) (0x0065A620; name hypothesis)
// Reads a wide-string property of the summary property set.

HRESULT SaveVersionInfo::Read_Property_Wide(IPropertySetStorage *storage, int id, wchar_t *buffer)
{
    buffer[0] = 0;
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    HRESULT result = storage->Open(unknown_fmtid_7afd60, 0x12, &property);
    if (FAILED(result)) {
        return result;
    }
    PROPSPEC spec;
    spec.ulKind = 1;
    spec.propid = id;
    PROPVARIANT value;
    result = property.Checked()->ReadMultiple(1, &spec, &value);
    if (FAILED(result)) {
        return result;
    }
    if (value.vt == 0x1F) {
        wcsncpy(buffer, value.pwszVal, 0x80);
    }
    return result;
}


// HRESULT SaveVersionInfo::Read_Stream_Int(IStorage *storage, int id, int *value) (0x0065A700; name hypothesis)
// Reads a summary value stored as a 4-byte stream.

HRESULT SaveVersionInfo::Read_Stream_Int(IStorage *storage, int id, int *value)
{
    *value = 0;
    UnknownComPtr<IStream, &unknown_uuid_IStream> stream;
    HRESULT result = storage->OpenStream(Savegame_Stream_Name(id), 0, 0x10, 0, &stream);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Read(value, sizeof(int), 0);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Read_Property_Int(IPropertySetStorage *storage, int id, int *value) (0x0065A7C0; name hypothesis)
// Reads a 4-byte integer property (VT_I4) of the summary property set.

HRESULT SaveVersionInfo::Read_Property_Int(IPropertySetStorage *storage, int id, int *value)
{
    *value = 0;
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    HRESULT result = storage->Open(unknown_fmtid_7afd60, 0x12, &property);
    if (FAILED(result)) {
        return result;
    }
    PROPSPEC spec;
    spec.ulKind = 1;
    spec.propid = id;
    PROPVARIANT variant;
    result = property.Checked()->ReadMultiple(1, &spec, &variant);
    if (FAILED(result)) {
        return result;
    }
    if (variant.vt == 3) {
        *value = variant.lVal;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Stream_String(IStorage *storage, int id, char const *text) (0x0065A890; name hypothesis)
// Writes a summary value as its own stream and commits it.

HRESULT SaveVersionInfo::Write_Stream_String(IStorage *storage, int id, char const *text)
{
    wchar_t wide[128];
    MultiByteToWideChar(0, 0, text, -1, wide, 0x80);
    UnknownComPtr<IFullStream, &unknown_uuid_IFullStream> stream;
    HRESULT result = storage->CreateStream(Savegame_Stream_Name(id), 0x12, 0, 0, (IStream **)&stream);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Write(wide, (wcslen(wide) + 1) * sizeof(wchar_t), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Commit(0);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Stream_Wide(IStorage *storage, int id, wchar_t const *text) (0x0065A9D0; name hypothesis)
// Writes a summary value as its own stream and commits it.

HRESULT SaveVersionInfo::Write_Stream_Wide(IStorage *storage, int id, wchar_t const *text)
{
    UnknownComPtr<IFullStream, &unknown_uuid_IFullStream> stream;
    HRESULT result = storage->CreateStream(Savegame_Stream_Name(id), 0x12, 0, 0, (IStream **)&stream);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Write(text, (wcslen(text) + 1) * sizeof(wchar_t), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Commit(0);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Property_String(IPropertySetStorage *storage, int id, char const *text) (0x0065AAD0; name hypothesis)
// Writes a property of the summary property set (creating the set when
// it cannot be opened).

HRESULT SaveVersionInfo::Write_Property_String(IPropertySetStorage *storage, int id, char const *text)
{
    wchar_t wide[128];
    MultiByteToWideChar(0, 0, text, -1, wide, 0x80);
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    if (FAILED(storage->Open(unknown_fmtid_7afd60, 0x12, &property))) {
        HRESULT result = storage->Create(unknown_fmtid_7afd60, 0, 0, 0x1012, &property);
        if (FAILED(result)) {
            return result;
        }
    }
    PROPSPEC spec;
    PROPVARIANT variant;
    spec.ulKind = 1;
    spec.propid = id;
    variant.vt = 0x1F;
    variant.pwszVal = wide;
    HRESULT result = property.Checked()->WriteMultiple(1, &spec, &variant, 2);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Property_Wide(IPropertySetStorage *storage, int id, wchar_t const *text) (0x0065ABF0; name hypothesis)
// Writes a property of the summary property set (creating the set when
// it cannot be opened).

HRESULT SaveVersionInfo::Write_Property_Wide(IPropertySetStorage *storage, int id, wchar_t const *text)
{
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    if (FAILED(storage->Open(unknown_fmtid_7afd60, 0x12, &property))) {
        HRESULT result = storage->Create(unknown_fmtid_7afd60, 0, 0, 0x1012, &property);
        if (FAILED(result)) {
            return result;
        }
    }
    PROPSPEC spec;
    PROPVARIANT variant;
    spec.ulKind = 1;
    spec.propid = id;
    variant.vt = 0x1F;
    variant.pwszVal = (wchar_t *)text;
    HRESULT result = property.Checked()->WriteMultiple(1, &spec, &variant, 2);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Stream_Int(IStorage *storage, int id, int value) (0x0065ACE0; name hypothesis)
// Writes a summary value as its own stream and commits it.

HRESULT SaveVersionInfo::Write_Stream_Int(IStorage *storage, int id, int value)
{
    UnknownComPtr<IFullStream, &unknown_uuid_IFullStream> stream;
    HRESULT result = storage->CreateStream(Savegame_Stream_Name(id), 0x12, 0, 0, (IStream **)&stream);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Write(&value, sizeof(value), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Commit(0);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Property_Int(IPropertySetStorage *storage, int id, int value) (0x0065ADD0; name hypothesis)
// Writes a property of the summary property set (creating the set when
// it cannot be opened).

HRESULT SaveVersionInfo::Write_Property_Int(IPropertySetStorage *storage, int id, int value)
{
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    if (FAILED(storage->Open(unknown_fmtid_7afd60, 0x12, &property))) {
        HRESULT result = storage->Create(unknown_fmtid_7afd60, 0, 0, 0x1012, &property);
        if (FAILED(result)) {
            return result;
        }
    }
    PROPSPEC spec;
    PROPVARIANT variant;
    spec.ulKind = 1;
    spec.propid = id;
    variant.vt = 3;
    variant.lVal = value;
    HRESULT result = property.Checked()->WriteMultiple(1, &spec, &variant, 2);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Read_Stream_Time(IStorage *storage, int id, FILETIME *time) (0x0065AEC0; name hypothesis)
// Reads a summary value stored as an 8-byte FILETIME stream.

HRESULT SaveVersionInfo::Read_Stream_Time(IStorage *storage, int id, FILETIME *time)
{
    *(__int64 *)time = 0;
    UnknownComPtr<IStream, &unknown_uuid_IStream> stream;
    HRESULT result = storage->OpenStream(Savegame_Stream_Name(id), 0, 0x10, 0, &stream);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Read(time, sizeof(FILETIME), 0);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Read_Property_Time(IPropertySetStorage *storage, int id, FILETIME *time) (0x0065AF80; name hypothesis)
// Reads a FILETIME property (VT_FILETIME) of the summary property set.

HRESULT SaveVersionInfo::Read_Property_Time(IPropertySetStorage *storage, int id, FILETIME *time)
{
    time->dwLowDateTime = 0;
    time->dwHighDateTime = 0;
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    HRESULT result = storage->Open(unknown_fmtid_7afd60, 0x12, &property);
    if (FAILED(result)) {
        return result;
    }
    PROPSPEC spec;
    spec.ulKind = 1;
    spec.propid = id;
    PROPVARIANT variant;
    result = property.Checked()->ReadMultiple(1, &spec, &variant);
    if (FAILED(result)) {
        return result;
    }
    if (variant.vt == 0x40) {
        *time = variant.filetime;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Stream_Time(IStorage *storage, int id, FILETIME const *time) (0x0065B060; name hypothesis)
// Writes a summary value as its own stream and commits it.

HRESULT SaveVersionInfo::Write_Stream_Time(IStorage *storage, int id, FILETIME const *time)
{
    UnknownComPtr<IFullStream, &unknown_uuid_IFullStream> stream;
    HRESULT result = storage->CreateStream(Savegame_Stream_Name(id), 0x12, 0, 0, (IStream **)&stream);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Write(time, sizeof(FILETIME), 0);
    if (FAILED(result)) {
        return result;
    }
    result = stream.Checked()->Commit(0);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// HRESULT SaveVersionInfo::Write_Property_Time(IPropertySetStorage *storage, int id, FILETIME const *time) (0x0065B150; name hypothesis)
// Writes a property of the summary property set (creating the set when
// it cannot be opened).

HRESULT SaveVersionInfo::Write_Property_Time(IPropertySetStorage *storage, int id, FILETIME const *time)
{
    UnknownComPtr<IPropertyStorage, &unknown_uuid_IPropertyStorage> property;
    if (FAILED(storage->Open(unknown_fmtid_7afd60, 0x12, &property))) {
        HRESULT result = storage->Create(unknown_fmtid_7afd60, 0, 0, 0x1012, &property);
        if (FAILED(result)) {
            return result;
        }
    }
    PROPSPEC spec;
    PROPVARIANT variant;
    spec.ulKind = 1;
    spec.propid = id;
    variant.vt = 0x40;
    variant.filetime = *time;
    HRESULT result = property.Checked()->WriteMultiple(1, &spec, &variant, 2);
    if (FAILED(result)) {
        return result;
    }
    return result;
}


// wchar_t const *__fastcall Savegame_Stream_Name(int id) (0x0065B250)
// The stream name of a summary property ID (0 when unknown): the inline
// helper of shared/savegame_info.hpp, emitted out of line where
// SaveVersionInfo::Load 0x00659A90 runs out of inline budget.


// IStream *UnknownComPtr<IStream>::Checked() const (0x0065B280)
// comip's checked access (E_POINTER when empty), emitted out of line where
// SaveVersionInfo::Load 0x00659A90 runs out of inline budget
// (shared/com_ptr.hpp).


// void UnknownComPtr<IPropertySetStorage>::Release_Held() (0x0065B2A0)
// comip's _Release, emitted out of line by SaveVersionInfo::Load
// 0x00659A90 (shared/com_ptr.hpp).


// void UnknownComPtr<IStream>::Release_Held() (0x0065B2B0)
// comip's _Release, emitted out of line by SaveVersionInfo::Load
// 0x00659A90 (shared/com_ptr.hpp).
