// campaign.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/campaign.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/campaign.hpp"


// Header-static dynamic initializers of the unit at 0x00465280:  the
// shared/unknown_geometry.hpp constants (0x00465280-0x00465441); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004652B0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x004652D0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x004652F0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x00465310, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x00465330, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x00465350, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x00465390, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x004653C0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x004653F0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x00465420, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// Dynamic initializer (0x00465430, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00465280.cpp; no source of its own.


// ??0CampaignClass@@QAE@PBD@Z (0x00465450)
// A campaign named ini_name, listed in the campaign list, with an empty description.

CampaignClass::CampaignClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    unknown_98(-1),
    unknown_29c(-1)
{
    Campaigns.Add(this);
    Scenario[0] = '\0';
    wcscpy(Description, unknown_empty_wide_839d4c);
}


// ??1CampaignClass@@UAE@XZ (0x00465500)
// Detaches the campaign and unlists it.

CampaignClass::~CampaignClass()
{
    Detach_This_From_All(this, true);
    Campaigns.Delete(this);
}


// static int __fastcall CampaignClass::Find_Index(char const *name) (0x00465580)
// The index in Campaigns of the campaign with this ID (case-insensitive), or -1.

int __fastcall CampaignClass::Find_Index(char const *name)
{
    for (int index = 0; index < Campaigns.Count(); index++) {
        CampaignClass *campaign = Campaigns[index];
        if (_stricmp(campaign->ID, name) == 0) {
            return index;
        }
    }
    return -1;
}


// bool CampaignClass::Read_INI(CCINIClass &ini) (0x004655C0, slot 25)
// The campaign's CD, final movie and scenario (upper case), and its
// description; debug-only campaigns get " - for debug/testing" appended.

bool CampaignClass::Read_INI(CCINIClass &ini)
{
    if (AbstractTypeClass::Read_INI(ini)) {
        unknown_98 = ini.Get_Int(ID, "CD", unknown_98);
        unknown_29c = ini.Get_Listed_Name(ID, "FinalMovie", unknown_29c);
        ini.Get_String(ID, "Scenario", Scenario, Scenario, sizeof(Scenario));
        _strupr(Scenario);
        if (!ini.Get_Bool(ID, "DebugOnly", false)) {
            ((CampaignINI &)ini).Get_Wide_String(ID, "Description", Description, sizeof(Description) / sizeof(Description[0]));
        } else {
            char buffer[1024];
            ini.Get_String(ID, "Description", 0, buffer, sizeof(buffer));
            strcat(buffer, " - for debug/testing");
            Char_To_Wide_006fa0c0(Description, buffer, -1);
        }
        return true;
    }
    return false;
}


// static void __fastcall CampaignClass::Read_Battles(CCINIClass &ini) (0x004656F0)
// Every [Battles] entry names a campaign: an existing one is found by ID,
// otherwise a new one is made; each then reads its own section.

void __fastcall CampaignClass::Read_Battles(CCINIClass &ini)
{
    int count = ini.Entry_Count("Battles");
    for (int index = 0; index < count; index++) {
        char name[32];
        ini.Get_String("Battles", ini.Get_Entry("Battles", index), "", name, sizeof(name));
        int found = Find_Index(name);
        CampaignClass *campaign;
        if (found == -1) {
            campaign = new CampaignClass(name);
        } else {
            campaign = Campaigns[found];
        }
        campaign->Read_INI(ini);
    }
}


// ?GetClassID@CampaignClass@@UAGJPAUGUID@@@Z (0x00465860)
// The class identifier.

HRESULT __stdcall CampaignClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_CampaignClass;
    return S_OK;
}


// ?Compute_CRC@CampaignClass@@UBEXAAVWWCRCEngine@@@Z (0x004658A0)
// AbstractTypeClass part and the campaign fields.

void CampaignClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)unknown_98);
    crc(Scenario);
    crc((long)unknown_29c);
}


// ?Load@CampaignClass@@UAGJPAUIStream@@@Z (0x004658E0)
// AbstractClass load, then the vtables.

HRESULT __stdcall CampaignClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) CampaignClass(NoInitClass());
    return S_OK;
}


// ?Save@CampaignClass@@UAGJPAUIStream@@H@Z (0x00465930)
// Only the AbstractClass part.

HRESULT __stdcall CampaignClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// ?Size_Of@CampaignClass@@UBEHXZ (0x00465950)
// Object size.

int CampaignClass::Size_Of() const
{
    return sizeof(CampaignClass);
}


// ?Kind_Of@CampaignClass@@UBEHXZ (0x00465960)
// RTTI kind 0x0A.

int CampaignClass::Kind_Of() const
{
    return 0x0A;
}


// ??_GCampaignClass@@UAEPAXI@Z (0x00465970)
// Compiler generated (the destructor is expanded inline).
