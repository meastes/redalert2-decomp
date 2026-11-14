// warhead.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/warhead.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/warhead_type.hpp"
#include "shared/ccini_class.hpp"
#include "shared/anim_type.hpp"


// Header-static dynamic initializers of the unit at 0x0071FEF0:  the
// shared/unknown_geometry.hpp constants (0x0071FEF0-0x007200B1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0071FF20, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x0071FF40, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x0071FF60, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x0071FF80, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x0071FFA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x0071FFC0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x00720000, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x00720030, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x00720060, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x00720090, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// Dynamic initializer (0x007200A0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0071fef0.cpp; no source of its own.


// float __fastcall Percent_Value(char const *text) (0x007200C0)

// The abstract list every warhead type joins; declared here because
// object.hpp declares it with element type ObjectClass * and WarheadType.cpp
// does not include object.hpp (0x00AC0CB0; RTTI element type
// AbstractClass *, declared as ObjectClass * in object.hpp).
extern DynamicVectorClass<AbstractClass *> unknown_objects_ac0cb0;


float __fastcall Percent_Value(char const *text)
{
    if (strchr(text, '%')) {
        return atoi(text) * 0.01;
    }
    return atof(text);
}


// WarheadTypeClass::WarheadTypeClass(char const *ini_name) (0x00720100)

WarheadTypeClass::WarheadTypeClass(char const *ini_name) :
    AbstractTypeClass(ini_name),
    Deform(0),
    ProneDamage(1.0),
    DeformThreshhold(0),
    InfDeath(0),
    CellSpread(0),
    CellInset(0),
    PercentAtMax(1.0f),
    CausesDelayKill(false),
    DelayKillFrames(5),
    DelayKillAtMax(1.0f),
    CombatLightSize(0),
    Particle(0),
    Wall(false),
    WallAbsoluteDestroyer(false),
    Wood(false),
    Tiberium(false),
    unknown_148(false),
    Sparky(false),
    Sonic(false),
    Fire(false),
    Conventional(false),
    Rocker(false),
    Bright(false),
    CLDisableRed(false),
    CLDisableGreen(false),
    CLDisableBlue(false),
    EMEffect(false),
    MindControl(false),
    IvanBomb(false),
    ElectricAssault(false),
    Parasite(false),
    Temporal(false),
    BombDisarm(false),
    Paralyzes(0),
    Culling(false),
    MakesDisguise(false),
    NukeMaker(false),
    Radiation(false),
    PsychicDamage(false),
    Bullets(false),
    Veinhole(false),
    ShakeXlo(0),
    ShakeXhi(0),
    ShakeYlo(0),
    ShakeYhi(0)
{
    Create_ID();
    unknown_WarheadTypeClass_list_00839b00.Add(this);
    unknown_objects_ac0cb0.Add(this);
    double *verses = Verses;
    verses[0] = 1.0;
    memcpy(verses + 1, verses, sizeof(Verses) - sizeof(Verses[0]));
}


// WarheadTypeClass::WarheadTypeClass(NoInitClass const &noinit) (0x00720360)
// Load path: only AnimList is constructed (empty).

WarheadTypeClass::WarheadTypeClass(NoInitClass const &noinit) :
    AbstractTypeClass(noinit)
{
}


// WarheadTypeClass::~WarheadTypeClass() (0x007203D0): defined with the scalar
// deleting destructor (WarheadTypeClass_scalar_deleting_destructor_00721090.cpp),
// which the unit also emits out of line here.
// Extent: the inventory stops at 0x0072043B; a linear sweep reaches the ret at
// 0x007204D3, followed by nop padding to Read_INI at 0x007204E0.


// bool WarheadTypeClass::Read_INI(CCINIClass &ini) (0x007204E0)
// The warhead's keys when its section exists. Verses= lists eleven armor
// modifiers, each a percentage ("50%") or a plain factor; a warhead that
// does nothing against armor types 4 and 6 is flagged (unknown_148).

extern "C" char *__cdecl strtok(char *string, char const *delimiters);
extern "C" char *__cdecl strchr(char const *string, int character);
extern "C" double __cdecl atof(char const *string);

static inline bool Is_Zero(double value)
{
    return value == 0.0;
}

static inline ParticleSystemTypeClass *Get_Particle_System(CCINIClass &ini, char const *section, char const *entry, ParticleSystemTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return Find_Or_Make_ParticleSystemType_00620df0(name);
    }
    return defvalue;
}

static inline TypeList<AnimTypeClass *> Get_Anim_List(CCINIClass &ini, char const *section, char const *entry,
                                                      TypeList<AnimTypeClass *> const &defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        TypeList<AnimTypeClass *> list;
        for (char *name = strtok(buffer, ","); name != 0 && *name != 0; name = strtok(0, ",")) {
            AnimTypeClass *anim = AnimTypeClass::Find_Or_Make(name);
            if (anim != 0) {
                list.Add(anim);
            }
        }
        return list;
    }
    return defvalue;
}

bool WarheadTypeClass::Read_INI(CCINIClass &ini)
{
    if (ini.Is_Present(ID)) {
        CellSpread = ini.Get_Double(ID, "CellSpread", CellSpread);
        CellInset = ini.Get_Double(ID, "CellInset", CellInset);
        PercentAtMax = ini.Get_Double(ID, "PercentAtMax", PercentAtMax);
        CausesDelayKill = ini.Get_Bool(ID, "CausesDelayKill", CausesDelayKill);
        DelayKillFrames = ini.Get_Int(ID, "DelayKillFrames", DelayKillFrames);
        DelayKillAtMax = ini.Get_Double(ID, "DelayKillAtMax", DelayKillAtMax);
        CombatLightSize = ini.Get_Double(ID, "CombatLightSize", CombatLightSize);
        Particle = Get_Particle_System(ini, ID, "Particle", Particle);
        Conventional = ini.Get_Bool(ID, "Conventional", Conventional);
        Wall = ini.Get_Bool(ID, "Wall", Wall);
        WallAbsoluteDestroyer = ini.Get_Bool(ID, "WallAbsoluteDestroyer", WallAbsoluteDestroyer);
        Wood = ini.Get_Bool(ID, "Wood", Wood);
        Tiberium = ini.Get_Bool(ID, "Tiberium", Tiberium);
        Sparky = ini.Get_Bool(ID, "Sparky", Sparky);
        Sonic = ini.Get_Bool(ID, "Sonic", Sonic);
        Rocker = ini.Get_Bool(ID, "Rocker", Rocker);
        Fire = ini.Get_Bool(ID, "Fire", Fire);
        Bright = ini.Get_Bool(ID, "Bright", Bright);
        CLDisableRed = ini.Get_Bool(ID, "CLDisableRed", CLDisableRed);
        CLDisableGreen = ini.Get_Bool(ID, "CLDisableGreen", CLDisableGreen);
        CLDisableBlue = ini.Get_Bool(ID, "CLDisableBlue", CLDisableBlue);
        AnimList = Get_Anim_List(ini, ID, "AnimList", AnimList);
        InfDeath = ini.Get_Int(ID, "InfDeath", InfDeath);
        Deform = ini.Get_Double(ID, "Deform", Deform);
        DeformThreshhold = ini.Get_Int(ID, "DeformThreshhold", DeformThreshhold);
        EMEffect = ini.Get_Bool(ID, "EMEffect", EMEffect);
        MindControl = ini.Get_Bool(ID, "MindControl", MindControl);
        IvanBomb = ini.Get_Bool(ID, "IvanBomb", IvanBomb);
        ElectricAssault = ini.Get_Bool(ID, "ElectricAssault", ElectricAssault);
        Parasite = ini.Get_Bool(ID, "Parasite", Parasite);
        Temporal = ini.Get_Bool(ID, "Temporal", Temporal);
        BombDisarm = ini.Get_Bool(ID, "BombDisarm", BombDisarm);
        Paralyzes = ini.Get_Int(ID, "Paralyzes", Paralyzes);
        Culling = ini.Get_Bool(ID, "Culling", Culling);
        MakesDisguise = ini.Get_Bool(ID, "MakesDisguise", MakesDisguise);
        NukeMaker = ini.Get_Bool(ID, "NukeMaker", NukeMaker);
        ProneDamage = ini.Get_Double(ID, "ProneDamage", ProneDamage);
        Radiation = ini.Get_Bool(ID, "Radiation", Radiation);
        PsychicDamage = ini.Get_Bool(ID, "PsychicDamage", PsychicDamage);
        Bullets = ini.Get_Bool(ID, "Bullets", Bullets);
        Veinhole = ini.Get_Bool(ID, "Veinhole", Veinhole);
        ShakeXlo = ini.Get_Int(ID, "ShakeXlo", ShakeXlo);
        ShakeXhi = ini.Get_Int(ID, "ShakeXhi", ShakeXhi);
        ShakeYlo = ini.Get_Int(ID, "ShakeYlo", ShakeYlo);
        ShakeYhi = ini.Get_Int(ID, "ShakeYhi", ShakeYhi);
        char buffer[128];
        if (ini.Get_String(ID, "Verses", "100%%,100%%,100%%,100%%,100%%,100%%,100%%,100%%,100%%,100%%,100%%", buffer, sizeof(buffer))) {
            char *token = strtok(buffer, ",");
            for (int index = 0; index < 11; index++) {
                double percent = Percent_Value(token);
                Verses[index] = percent;
                token = strtok(0, ",");
            }
        }
        unknown_148 = Is_Zero(Verses[4]) && Is_Zero(Verses[6]);
        return true;
    }
    return false;
}


// void WarheadTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x00720BD0)

void WarheadTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc(Wall);
    crc(Wood);
    crc(Tiberium);
    crc(unknown_148);
    crc(Sparky);
    crc(Sonic);
    crc(Fire);
    crc(Conventional);
    crc(Rocker);
    crc(Bright);
    crc(CLDisableRed);
    crc(CLDisableGreen);
    crc(CLDisableBlue);
    crc(Deform);
    crc((long)DeformThreshhold);
    crc(ProneDamage);
    crc(Veinhole);
    crc((long)ShakeXlo);
    crc((long)ShakeXhi);
    crc((long)ShakeYlo);
    crc((long)ShakeYhi);
    for (int i = 0; i < 11; i++) {
        crc(Verses[i]);
    }
    crc((long)AnimList.Count());
    crc((long)InfDeath);
}


// HRESULT __stdcall WarheadTypeClass::GetClassID(GUID *clsid) (0x00720D70)

HRESULT __stdcall WarheadTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_WarheadTypeClass;
    return S_OK;
}


// HRESULT __stdcall WarheadTypeClass::Load(IStream *stream) (0x00720DB0)
// Restores the warhead in place, rebuilds its animation list from the saved
// count and entries, and swizzles the list and the particle system.

// Rebuilds a list in place from its saved count and elements.
template <class T>
static inline void Load_List(IStream *stream, TypeList<T> &list)
{
    new (&list) TypeList<T>;
    int count;
    stream->Read(&count, sizeof(count), 0);
    for (int index = 0; index < count; index++) {
        T value;
        stream->Read(&value, sizeof(value), 0);
        list.Add(value);
    }
}

HRESULT __stdcall WarheadTypeClass::Load(IStream *stream)
{
    HRESULT result = AbstractTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) WarheadTypeClass(NoInitClass());
    Load_List(stream, AnimList);
    for (int index = 0; index < AnimList.Count(); index++) {
        Swizzle_Pointer(AnimList[index]);
    }
    Swizzle_Pointer(Particle);
    return S_OK;
}


// HRESULT __stdcall WarheadTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x00720EC0)
// The base object, then the animation list as a count and pointers.

HRESULT __stdcall WarheadTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractTypeClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = AnimList.Count();
    stream->Write(&count, sizeof(count), 0);
    for (int i = 0; i < count; i++) {
        stream->Write(&AnimList[i], sizeof(AnimTypeClass *), 0);
    }
    return S_OK;
}


// WarheadTypeClass *__fastcall WarheadTypeClass::Find_Or_Make(char const *name) (0x00720F30)
// The named warhead type, created when missing; 0 for "<none>" and "none".

WarheadTypeClass *__fastcall WarheadTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
        if (!_stricmp(unknown_WarheadTypeClass_list_00839b00[index]->ID, name)) {
            return unknown_WarheadTypeClass_list_00839b00[index];
        }
    }
    return new WarheadTypeClass(name);
}


// void WarheadTypeClass::Detach(AbstractClass *target, bool all) (0x00720FC0)

void WarheadTypeClass::Detach(AbstractClass *target, bool all)
{
    if (target == (AbstractClass *)Particle) {
        Particle = 0;
    }
    AnimTypeClass *anim = (AnimTypeClass *)target;
    AnimList.Delete(anim);
}


// WarheadTypeClass *__fastcall WarheadTypeClass::From_Name(char const *name) (0x00721020)

WarheadTypeClass *__fastcall WarheadTypeClass::From_Name(char const *name)
{
    for (int index = 0; index < unknown_WarheadTypeClass_list_00839b00.Count(); index++) {
        if (!_stricmp(unknown_WarheadTypeClass_list_00839b00[index]->ID, name)) {
            return unknown_WarheadTypeClass_list_00839b00[index];
        }
    }
    return 0;
}


// int WarheadTypeClass::Size_Of() const (0x00721070)

int WarheadTypeClass::Size_Of() const
{
    return sizeof(WarheadTypeClass);
}


// int WarheadTypeClass::Kind_Of() const (0x00721080)

int WarheadTypeClass::Kind_Of() const
{
    return 50;
}
