/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/psystype.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/psystype.cpp where both define the function (names only).

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


/// <summary>
/// Constructs a particle system type with default values.
/// The new type is given a heap ID and added to the master particle system type list, so
/// that the rules and the game code can refer to it by name from this point on.
/// </summary>
/// <param name="ininame">The INI section name that describes this particle system.</param>
// (0x006205E0, _$E)
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


/// <summary>
/// Constructs a particle system type without initializing it.
/// This constructor is used by the load process, which is about to overwrite the object
/// with the data it reads from the save file.
/// </summary>
ParticleSystemTypeClass::ParticleSystemTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit), SpawnDirection(noinit), LaserColor(noinit) {}


/// <summary>
/// Destroys the particle system type.
/// Every object that still refers to this type is detached from it first, so that no
/// dangling reference survives the removal from the master list.
/// </summary>
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

/// <summary>
/// Fetches this particle system type's data from the INI database.
/// This routine is called while the rules are being parsed. It lays the particle system
/// specific entries over whatever the base class has already picked up.
/// </summary>
/// <returns>bool; Was the data read?</returns>
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


/// <summary>
/// Converts a particle system type into its name.
/// </summary>
/// <returns>Returns with the name of the particle system type. An invalid type yields the
/// placeholder name used for "no system".</returns>
// (0x00620C40)
// The ID of the indexed type, "<none>" when the index is out of range.

char const *__fastcall ParticleSystemTypeClass::Name_From(unsigned int partsys)
{
    if (partsys < (unsigned int)ParticleSystemTypes.Count()) {
        return ParticleSystemTypes[partsys]->ID;
    }
    return "<none>";
}


/// <summary>
/// Submits this particle system type to the CRC calculation.
/// This routine is used by the multiplayer sync check so that players running with
/// different rules are caught rather than left to drift apart.
/// </summary>
/// <param name="crc">The CRC engine to submit this object's data to.</param>
// (0x00620C60, slot 13)

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


/// <summary>
/// Fetches the class identifier of this object.
/// This routine is part of the persistence interface. The save system records the class
/// ID so that the right kind of object can be manufactured when the game is reloaded.
/// </summary>
/// <param name="retval">Pointer to the class ID to be filled in.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00620D00, slot 3)

extern "C" GUID const CLSID_ParticleSystemTypeClass;

HRESULT __stdcall ParticleSystemTypeClass::GetClassID(GUID *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = CLSID_ParticleSystemTypeClass;
    return S_OK;
}


/// <summary>
/// Loads this particle system type from the stream specified.
/// The object is re-constructed in place once the persisted data has been read, so that
/// its virtual table pointer is valid for the run it has been loaded into.
/// </summary>
/// <returns>Returns with the result code of the load operation.</returns>
// (0x00620D40)
// Loads the object type and restores the type in place; always reports
// success.

HRESULT __stdcall ParticleSystemTypeClass::Load(IStream *stream)
{
    if (SUCCEEDED(ObjectTypeClass::Load(stream))) {
        new (this) ParticleSystemTypeClass(NoInitClass());
    }
    return S_OK;
}


/// <summary>
/// Saves this particle system type to the stream specified.
/// </summary>
/// <param name="cleardirty">Should the object be marked as clean once it has been saved?</param>
/// <returns>Returns with the result code of the save operation.</returns>
// (0x00620D90, slot 6)
// The object type's save; always S_OK.

HRESULT __stdcall ParticleSystemTypeClass::Save(IStream *stream, BOOL cleardirty)
{
    ObjectTypeClass::Save(stream, cleardirty);
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


/// <summary>
/// Fetches the particle system type of the name specified, creating it if need be.
/// This routine is used while the rules are being parsed so that one section may refer
/// to a particle system whose own section has not been reached yet.
/// </summary>
/// <param name="name">The INI name of the particle system type to find.</param>
/// <returns>Returns with a pointer to the type found or created.</returns>
// (0x00620DF0)
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


/// <summary>
/// Creates and places a particle system upon the map.
/// Particle systems are never placed by the scenario or by the map editor's placement
/// logic, so this routine always refuses the request.
/// </summary>
/// <returns>bool; Was the object placed?</returns>
// (0x00620EA0, slot 32)
// Particle system types are never placed as objects.

bool ParticleSystemTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}


/// <summary>
/// Creates a particle system object of this type.
/// Particle systems are spawned by whatever owns them rather than through the generic
/// object type interface, so this routine never manufactures one.
/// </summary>
/// <returns>Returns with a pointer to the object created, which is always NULL.</returns>
// (0x00620EB0, slot 35)
// Particle system types make no object here.

ObjectClass *ParticleSystemTypeClass::Create_One_Of(HouseClass *house) const
{
    return 0;
}


// Compiler-emitted from the real ParticleSystemTypeClass destructor in this class unit.
