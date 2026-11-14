// psystype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/psystype.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/particle_system.hpp"
#include "shared/ccini_class.hpp"


// Header-static dynamic initializers of the unit at 0x00620430:  the
// shared/unknown_geometry.hpp constants (0x00620430-0x006205F1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00620460, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x00620480, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x006204A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x006204C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x006204E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x00620500, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x00620540, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x00620570, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x006205A0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x006205D0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


// Dynamic initializer (0x006205E0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00620430.cpp; no source of its own.


ParticleSystemTypeClass::ParticleSystemTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name), HoldsWhat(-1), Spawns(false), SpawnFrames(1),
    Slowdown(0), ParticleCap(50), SpawnRadius(0), SpawnCutoff(0), SpawnTranslucencyCutoff(0),
    BehavesLike(-1), Lifetime(-1), SpawnDirection(0, 0, 0), ParticlesPerCoord(0.1),
    SpiralDeltaPerCoord(0.025), SpiralRadius(25.0), PositionPerturbationCoefficient(0),
    MovementPerturbationCoefficient(0), VelocityPerturbationCoefficient(0), SpawnSparkPercentage(0),
    SparkSpawnFrames(0), LightSize(0), Laser(false), OneFrameLight(false)
{
    AbstractClass::Create_ID();
    ParticleSystemTypes.Add(this);
    unknown_204 = true;
}


ParticleSystemTypeClass::ParticleSystemTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit), SpawnDirection(noinit), LaserColor(noinit) {}


ParticleSystemTypeClass::~ParticleSystemTypeClass()
{
    Unknown_006ECB90(this, true);
    ParticleSystemTypes.Delete(this);
}


// bool ParticleSystemTypeClass::Read_INI(CCINIClass &ini) (0x00620830)
// The object keys, then the particle system keys: the particle type it
// holds (made when missing), spawn timing and limits, BehavesLike= by
// name (Smoke, Gas, Fire, Spark, Railgun; -1 when unknown), the spawn
// direction and perturbation coefficients, and the laser and light keys.

// The index of the named particle type, made when missing; -1 for
// "<none>" (0x00621960, __fastcall; ParticleType.cpp).
int __fastcall ParticleTypeClass_Index_Or_Make_621960(char const *name);

// The behaviour names in BehavesLike order (0x007EBFD8).
extern char const *unknown_behaves_like_names_7ebfd8[5];

// The behaviour with this name, or -1.
static inline int Behaves_From_Name(char const *name)
{
    for (int index = 0; index < 5; index++) {
        if (_stricmp(name, unknown_behaves_like_names_7ebfd8[index]) == 0) {
            return index;
        }
    }
    return -1;
}

bool ParticleSystemTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        char buffer[64];
        ini.Get_String(ID, "HoldsWhat", unknown_string_83c55c, buffer, sizeof(buffer));
        HoldsWhat = ParticleTypeClass_Index_Or_Make_621960(buffer);
        Spawns = ini.Get_Bool(ID, "Spawns", Spawns);
        SpawnFrames = ini.Get_Int(ID, "SpawnFrames", SpawnFrames);
        ParticleCap = ini.Get_Int(ID, "ParticleCap", ParticleCap);
        SpawnRadius = ini.Get_Int(ID, "SpawnRadius", SpawnRadius);
        Slowdown = ini.Get_Double(ID, "Slowdown", Slowdown);
        SpawnCutoff = ini.Get_Double(ID, "SpawnCutoff", SpawnCutoff);
        SpawnTranslucencyCutoff = ini.Get_Double(ID, "SpawnTranslucencyCutoff", SpawnTranslucencyCutoff);
        Lifetime = ini.Get_Int(ID, "Lifetime", Lifetime);
        ini.Get_String(ID, "BehavesLike", unknown_string_83c55c, buffer, sizeof(buffer));
        BehavesLike = Behaves_From_Name(buffer);
        SpawnDirection = ini.Get_Float_Triple(ID, "SpawnDirection", SpawnDirection);
        ParticlesPerCoord = ini.Get_Double(ID, "ParticlesPerCoord", ParticlesPerCoord);
        SpiralDeltaPerCoord = ini.Get_Double(ID, "SpiralDeltaPerCoord", SpiralDeltaPerCoord);
        SpiralRadius = ini.Get_Double(ID, "SpiralRadius", SpiralRadius);
        PositionPerturbationCoefficient = ini.Get_Double(ID, "PositionPerturbationCoefficient", PositionPerturbationCoefficient);
        MovementPerturbationCoefficient = ini.Get_Double(ID, "MovementPerturbationCoefficient", MovementPerturbationCoefficient);
        VelocityPerturbationCoefficient = ini.Get_Double(ID, "VelocityPerturbationCoefficient", VelocityPerturbationCoefficient);
        Laser = ini.Get_Bool(ID, "Laser", Laser);
        LaserColor = ini.Get_RGB(ID, "LaserColor", LaserColor);
        SparkSpawnFrames = ini.Get_Int(ID, "SparkSpawnFrames", SparkSpawnFrames);
        LightSize = ini.Get_Int(ID, "LightSize", LightSize);
        OneFrameLight = ini.Get_Bool(ID, "OneFrameLight", OneFrameLight);
        SpawnSparkPercentage = ini.Get_Double(ID, "SpawnSparkPercentage", SpawnSparkPercentage);
        return true;
    }
    return false;
}


int __fastcall ParticleSystemTypeClass::Index_Or_Make(char const *name)
{
    if (_stricmp(name, "<none>") != 0 && strlen(name) != 0) {
        for (int index = 0; index < ParticleSystemTypes.Count(); ++index) {
            if (_stricmp(name, ParticleSystemTypes[index]->ID) == 0) return index;
        }
        ParticleSystemTypeClass *type = new ParticleSystemTypeClass(name);
        return ParticleSystemTypes.ID(type);
    }
    return -1;
}


// char const *__fastcall ParticleSystemTypeClass::Name_From(unsigned int index) (0x00620C40)
// The ID of the indexed type, "<none>" when the index is out of range.

char const *__fastcall ParticleSystemTypeClass::Name_From(unsigned int index)
{
    if (index < (unsigned int)ParticleSystemTypes.Count()) {
        return ParticleSystemTypes[index]->ID;
    }
    return "<none>";
}


// void ParticleSystemTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x00620C60, slot 13)

void ParticleSystemTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)HoldsWhat);
    crc(Spawns);
    crc((long)SpawnFrames);
    crc(Slowdown);
    crc((long)ParticleCap);
    crc((long)SpawnRadius);
    crc(SpawnCutoff);
    crc(SpawnTranslucencyCutoff);
    crc((long)BehavesLike);
    crc((long)Lifetime);
}


// HRESULT __stdcall ParticleSystemTypeClass::GetClassID(GUID *clsid) (0x00620D00, slot 3)

extern "C" GUID const CLSID_ParticleSystemTypeClass;

HRESULT __stdcall ParticleSystemTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_ParticleSystemTypeClass;
    return S_OK;
}


// HRESULT __stdcall ParticleSystemTypeClass::Load(IStream *stream) (0x00620D40)
// Loads the object type and restores the type in place; always reports
// success.

HRESULT __stdcall ParticleSystemTypeClass::Load(IStream *stream)
{
    if (SUCCEEDED(ObjectTypeClass::Load(stream))) {
        new (this) ParticleSystemTypeClass(NoInitClass());
    }
    return S_OK;
}


// HRESULT __stdcall ParticleSystemTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x00620D90, slot 6)
// The object type's save; always S_OK.

HRESULT __stdcall ParticleSystemTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    ObjectTypeClass::Save(stream, clear_dirty);
    return S_OK;
}


// int __fastcall ParticleSystemTypeClass::Behaviour_From_Name(char const *name) (0x00620DB0)
// The BehavesLike= index of a name, -1 when unknown.

int __fastcall ParticleSystemTypeClass::Behaviour_From_Name(char const *name)
{
    for (int index = 0; index < 5; index++) {
        if (!_stricmp(name, unknown_behaviour_names_7ebfd8[index])) {
            return index;
        }
    }
    return -1;
}


// ParticleSystemTypeClass *__fastcall ParticleSystemTypeClass::Find_Or_Make(char const *name) (0x00620DF0)
// The named type, created when missing; 0 for "<none>" and "none".

ParticleSystemTypeClass *__fastcall ParticleSystemTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < ParticleSystemTypes.Count(); index++) {
        if (!_stricmp(ParticleSystemTypes[index]->ID, name)) {
            return ParticleSystemTypes[index];
        }
    }
    return new ParticleSystemTypeClass(name);
}


// int ParticleSystemTypeClass::Size_Of() const (0x00620E80, slot 12)

int ParticleSystemTypeClass::Size_Of() const
{
    return sizeof(ParticleSystemTypeClass);
}


// int ParticleSystemTypeClass::Kind_Of() const (0x00620E90, slot 11)

int ParticleSystemTypeClass::Kind_Of() const
{
    return 0x19;
}


// bool ParticleSystemTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00620EA0, slot 32)
// Particle system types are never placed as objects.

bool ParticleSystemTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}


// ObjectClass *ParticleSystemTypeClass::Create_One_Of(HouseClass *house) const (0x00620EB0, slot 35)
// Particle system types make no object here.

ObjectClass *ParticleSystemTypeClass::Create_One_Of(HouseClass *house) const
{
    return 0;
}


// Compiler-emitted from the real ParticleSystemTypeClass destructor in this class unit.
