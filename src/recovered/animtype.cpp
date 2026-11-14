// animtype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/animtype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/anim_type.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/unknown_static_rect.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/shape_file.hpp"
#include "shared/ccini_class.hpp"
#include "shared/overlay_type.hpp"
#include "shared/warhead_type.hpp"


// AnimTypeClass::AnimTypeClass(NoInitClass const &noinit) (0x00425140)
// Load path: installs the vtables only.

AnimTypeClass::AnimTypeClass(NoInitClass const &noinit) :
    ObjectTypeClass(noinit)
{
}


// AnimTypeClass::~AnimTypeClass() (0x00425170)
// Removes the type from the 0x00AC0CB0 list and AnimTypes.

AnimTypeClass::~AnimTypeClass()
{
    unknown_objects_ac0cb0.Delete(this);
    AnimTypes.Delete(this);
}


// static void __fastcall AnimTypeClass::Init(int theater) (0x00425230)
// Loads every non-demand-loaded animation image for a theater. When the
// theater is unchanged (and no reload is forced) only the missing
// "NewTheater" images are loaded.
// Assumptions: unknown_1fc is "Theater=" and unknown_207 "NewTheater=";
// fullname size 512 (_MAX_FNAME + _MAX_EXT) from the frame size.

void __fastcall AnimTypeClass::Init(int theater)
{
    if (!Debug_Map && theater == unknown_loaded_theater_7d88dc) {
        for (int index = 0; index < AnimTypes.Count(); index++) {
            AnimTypeClass *anim = AnimTypes[index];
            if (anim->unknown_207 && anim->unknown_a4 == 0) {
                anim->unknown_slot40(theater);
            }
        }
        return;
    }

    for (int index = 0; index < AnimTypes.Count(); index++) {
        AnimTypeClass *anim = AnimTypes[index];
        if (!anim->DemandLoad) {
            if (anim->unknown_1fc) {
                if (anim->unknown_a4 != 0 && anim->unknown_a8) {
                    operator delete(anim->unknown_a4);
                    anim->unknown_a8 = false;
                }
                anim->unknown_a4 = 0;
                char fullname[512];
                _makepath(fullname, 0, 0, anim->ID, Theaters[theater].Suffix);
                anim->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
                if (anim->unknown_a4 == 0) {
                    _makepath(fullname, 0, 0, anim->ID, ".SHP");
                    anim->unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
                }
            } else if (anim->unknown_207) {
                anim->unknown_slot40(theater);
            }
        } else if (anim->unknown_1fc || anim->unknown_207) {
            if (anim->unknown_a4 != 0 && anim->unknown_a8) {
                operator delete(anim->unknown_a4);
                anim->unknown_a8 = false;
            }
            anim->unknown_a4 = 0;
        }
    }
}


// void AnimTypeClass::unknown_slot24(int theater) (0x004253C0)
// Reloads this type's theater-specific image when the theater changes.

void AnimTypeClass::unknown_slot24(int theater)
{
    if (!Debug_Map && theater == unknown_loaded_theater_7d88dc) {
        return;
    }
    if (unknown_1fc && !DemandLoad) {
        if (unknown_a4 != 0 && unknown_a8) {
            operator delete(unknown_a4);
            unknown_a8 = false;
        }
        unknown_a4 = 0;
        char fullname[512];
        _makepath(fullname, 0, 0, ID, Theaters[theater].Suffix);
        unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
        return;
    }
    if (unknown_207) {
        unknown_slot40(theater);
    }
}


// void AnimTypeClass::unknown_slot40(int theater) (0x00425490)
// Loads the image (TibSun AnimTypeClass::Load_Image): the theater-named SHP
// of the Image= name (else the ID), falling back to the generic name; then
// defaults End/LoopEnd from the frame count and records the largest frame.

void AnimTypeClass::unknown_slot40(int theater)
{
    if (!DemandLoad) {
        if (unknown_1fc) {
            Fetch_Normal_Image();
        } else {
            if (unknown_a4 != 0 && unknown_a8) {
                operator delete(unknown_a4);
                unknown_a8 = false;
            }
            unknown_a4 = 0;
            char fullname[512];
            _makepath(fullname, 0, 0, unknown_1c8[0] != '\0' ? (char const *)unknown_1c8 : ID, ".SHP");
            Theater_Naming_Convention(fullname, theater);
            unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
            if (unknown_a4 == 0) {
                Generic_Naming_Convention(fullname);
                unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
            }
        }
    }

    ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_a4;
    if (shape != 0) {
        if (End == 0) {
            End = shape->FrameCount;
            if (Shadow) {
                End /= 2;
            }
        }
        if (LoopEnd == 0) {
            LoopEnd = End;
        }

        int biggest_size = 0;
        int biggest_frame = 0;
        for (int frame = 0; frame < shape->FrameCount; frame++) {
            Rect rect = shape->Get_Frame_Rect(frame);
            if (rect.Width * rect.Height > biggest_size) {
                biggest_frame = frame;
                biggest_size = rect.Width * rect.Height;
            }
        }
        unknown_268 = biggest_frame;
    }
}


// static char const *__fastcall AnimTypeClass::Name_From(int index) (0x00425620)
// The ID of the type at an index (the original's bound check admits Count).

char const *__fastcall AnimTypeClass::Name_From(int index)
{
    if ((unsigned)index > (unsigned)AnimTypes.Count()) {
        return unknown_string_83c55c;
    }
    return AnimTypes[index]->ID;
}


// static int __fastcall AnimTypeClass::From_Name(char const *name) (0x00425640)
// Index of the type with this ID (case-insensitive), or -1.

int __fastcall AnimTypeClass::From_Name(char const *name)
{
    if (name != 0) {
        for (int index = 0; index < AnimTypes.Count(); index++) {
            if (_stricmp(AnimTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// AnimTypeClass::Read_INI (0x00425690, slot 25)
// The object type keys, then the animation keys from the type's own section;
// "Theater"/"NewTheater" come from the art INI when the type has an image
// name and no image yet.

// The animation named by an entry (Find_Or_Make_Type over AnimTypes), else the default.
static inline AnimTypeClass *Get_Anim_Type(CCINIClass &ini, char const *section, char const *entry, AnimTypeClass *defvalue)
{
    char buffer[128];
    if (ini.Get_String(section, entry, "", buffer, sizeof(buffer))) {
        return Find_Or_Make_Type(buffer, AnimTypes);
    }
    return defvalue;
}

bool AnimTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
    char const *section = ID;
    bool shadow = ini.Get_Bool(section, "Shadow", Shadow);
    if (shadow != Shadow) {
        Shadow = shadow;
        if (Shadow) {
            End /= 2;
        } else {
            End *= 2;
        }
        if (LoopEnd > End) {
            LoopEnd = End;
        }
    }

    if (unknown_1c8[0] != '\0') {
        if (unknown_a4 == 0) {
            unknown_1fc = ArtINI.Get_Bool(section, "Theater", unknown_1fc);
            unknown_207 = ArtINI.Get_Bool(section, "NewTheater", unknown_207);
        }
        unknown_slot40(-1);
    }

    Layer = ini.Get_LayerType(section, "Layer", (LayerType)Layer);
    AltPalette = ArtINI.Get_Bool(section, "AltPalette", AltPalette);
    Flat = ini.Get_Bool(section, "Flat", Flat);
    Flamer = ini.Get_Bool(section, "Flamer", Flamer);
    Normalized = ini.Get_Bool(section, "Normalized", Normalized);
    Translucent = ini.Get_Bool(section, "Translucent", Translucent);
    Scorch = ini.Get_Bool(section, "Scorch", Scorch);
    Crater = ini.Get_Bool(section, "Crater", Crater);
    ForceBigCraters = ini.Get_Bool(section, "ForceBigCraters", ForceBigCraters);
    Sticky = ini.Get_Bool(section, "Sticky", Sticky);
    PingPong = ini.Get_Bool(section, "PingPong", PingPong);
    Reverse = ini.Get_Bool(section, "Reverse", Reverse);
    PsiWarning = ini.Get_Bool(section, "PsiWarning", PsiWarning);
    TiberiumChainReaction = ini.Get_Bool(section, "TiberiumChainReaction", TiberiumChainReaction);

    int rate = ini.Get_Int(section, "Rate", -1);
    if (rate != -1) {
        Rate = rate > 0 ? 900 / rate : 0;
    }

    Damage = ini.Get_Double(section, "Damage", Damage);
    Start = ini.Get_Int(section, "Start", Start);
    End = ini.Get_Int(section, "End", End);
    LoopStart = ini.Get_Int(section, "LoopStart", LoopStart);
    LoopEnd = ini.Get_Int(section, "LoopEnd", LoopEnd);
    LoopCount = ini.Get_Int(section, "LoopCount", LoopCount);

    Next = Get_Anim_Type(ini, section, "Next", Next);

    DetailLevel = ini.Get_Int(section, "DetailLevel", DetailLevel);
    TranslucencyDetailLevel = ini.Get_Int(section, "TranslucencyDetailLevel", TranslucencyDetailLevel);
    Point2D delay = ini.Get_Point(section, "RandomLoopDelay", Point2D(RandomLoopDelayMin, RandomLoopDelayMax));
    RandomLoopDelayMin = delay.X;
    RandomLoopDelayMax = delay.Y;
    Translucency = ini.Get_Int(section, "Translucency", Translucency);
    IsTiberium = ini.Get_Bool(section, "IsTiberium", IsTiberium);
    HideIfNoOre = ini.Get_Bool(section, "HideIfNoOre", HideIfNoOre);
    YSortAdjust = ini.Get_Int(section, "YSortAdjust", YSortAdjust);
    DemandLoad = ini.Get_Bool(section, "DemandLoad", DemandLoad);
    FreeAfterPlaying = ini.Get_Bool(section, "FreeAfterPlaying", FreeAfterPlaying);
    Elasticity = ini.Get_Double(section, "Elasticity", Elasticity);
    MaxXYVel = ini.Get_Double(section, "MaxXYVel", MaxXYVel);
    MinZVel = ini.Get_Double(section, "MinZVel", MinZVel);

    Spawns = Get_Anim_Type(ini, section, "Spawns", Spawns);

    SpawnCount = ini.Get_Int(section, "SpawnCount", SpawnCount);
    IsMeteor = ini.Get_Bool(section, "IsMeteor", IsMeteor);
    IsVeins = ini.Get_Bool(section, "IsVeins", IsVeins);
    TiberiumSpreadRadius = ini.Get_Int(section, "TiberiumSpreadRadius", TiberiumSpreadRadius);
    TiberiumSpawnType = Get_Type(ini, section, "TiberiumSpawnType", TiberiumSpawnType);
    IsAnimatedTiberium = ini.Get_Bool(section, "IsAnimatedTiberium", IsAnimatedTiberium);
    ShouldFogRemove = ini.Get_Bool(section, "ShouldFogRemove", ShouldFogRemove);
    IsFlamingGuy = ini.Get_Bool(section, "IsFlamingGuy", IsFlamingGuy);
    RunningFrames = ini.Get_Int(section, "RunningFrames", RunningFrames);
    YDrawOffset = ini.Get_Int(section, "YDrawOffset", YDrawOffset);
    ZAdjust = ini.Get_Int(section, "ZAdjust", ZAdjust);

    StartSound = Get_Sound(ini, section, "StartSound", StartSound);
    if (StartSound == -1) {
        StartSound = Get_Sound(ini, section, "Report", -1);
    }
    StopSound = Get_Sound(ini, section, "StopSound", StopSound);

    BounceAnim = Get_Type(ini, section, "BounceAnim", BounceAnim);
    ExpireAnim = Get_Type(ini, section, "ExpireAnim", ExpireAnim);
    TrailerAnim = Get_Type(ini, section, "TrailerAnim", TrailerAnim);
    TrailerSeperation = ini.Get_Int(section, "TrailerSeperation", TrailerSeperation);
    DamageRadius = ini.Get_Int(section, "DamageRadius", DamageRadius);
    Warhead = Get_Type(ini, section, "Warhead", Warhead);
    Bouncer = ini.Get_Bool(section, "Bouncer", Bouncer);
    Tiled = ini.Get_Bool(section, "Tiled", Tiled);
    ShouldUseCellDrawer = ini.Get_Bool(section, "ShouldUseCellDrawer", ShouldUseCellDrawer);
    UseNormalLight = ini.Get_Bool(section, "UseNormalLight", UseNormalLight);

    Point2D rates = ini.Get_Point(section, "RandomRate", Point2D(-1, -1));
    if (rates.X != -1) {
        RandomRateMin = rates.X > 0 ? 900 / rates.X : 0;
    }
    if (rates.Y != -1) {
        RandomRateMax = rates.Y > 0 ? 900 / rates.Y : 0;
    }
    if (RandomRateMax < 0) {
        RandomRateMax = 0;
    }
    if (RandomRateMin > RandomRateMax) {
        RandomRateMin = RandomRateMax;
    }
    return true;
    }
    return false;
}


// HRESULT __stdcall AnimTypeClass::Load(IStream *stream) (0x00426140)
// Restores the type in place, reloads its voxel and image, swizzles the
// type pointers and reloads a non-demand-loaded image for the scenario's
// theater.

HRESULT __stdcall AnimTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) AnimTypeClass(NoInitClass());
    Fetch_Voxel_Image();
    Fetch_Normal_Image();
    Swizzler.Swizzle((void **)&Next);
    Swizzler.Swizzle((void **)&ExpireAnim);
    Swizzler.Swizzle((void **)&TrailerAnim);
    Swizzler.Swizzle((void **)&BounceAnim);
    Swizzler.Swizzle((void **)&Spawns);
    Swizzler.Swizzle((void **)&Warhead);
    Swizzler.Swizzle((void **)&TiberiumSpawnType);
    if (!DemandLoad) {
        if (unknown_1fc) {
            char fullname[512];
            _makepath(fullname, 0, 0, ID, Theaters[unknown_id_source_a3d290->Theater].Suffix);
            unknown_a4 = (void *)MixFileClass::Retrieve(fullname);
            return S_OK;
        }
        if (unknown_207) {
            unknown_slot40(unknown_id_source_a3d290->Theater);
        }
    }
    return S_OK;
}


// HRESULT __stdcall AnimTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x004262A0)

HRESULT __stdcall AnimTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall AnimTypeClass::GetClassID(GUID *clsid) (0x004262C0)

HRESULT __stdcall AnimTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_AnimTypeClass;
    return S_OK;
}


// void AnimTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x00426300)

void AnimTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectTypeClass::Compute_CRC(crc);
    crc((long)Layer);
    crc(Normalized);
    crc(Flat);
    crc(Translucent);
    crc(Scorch);
    crc(Flamer);
    crc(Crater);
    crc(Sticky);
    crc(PingPong);
    crc(Reverse);
    crc(Shadow);
    crc((long)unknown_268);
    crc(Damage);
    crc(TiberiumChainReaction);
    crc((long)Rate);
    crc((long)Start);
    crc((long)LoopStart);
    crc((long)LoopEnd);
    crc((long)End);
    crc((long)StartSound);
    crc((long)StopSound);
    crc((long)DetailLevel);
    crc((long)TranslucencyDetailLevel);
    crc((long)RandomLoopDelayMin);
    crc((long)RandomLoopDelayMax);
    crc((long)Translucency);
    crc(Tiled);
}


// static AnimTypeClass *__fastcall AnimTypeClass::Find_Or_Make(char const *name) (0x004264A0)
// The type with this ID, else a new one; null for "<none>" and "none".

AnimTypeClass *__fastcall AnimTypeClass::Find_Or_Make(char const *name)
{
    if (_stricmp("<none>", name) == 0 || _stricmp("none", name) == 0) {
        return 0;
    }
    for (int index = 0; index < AnimTypes.Count(); index++) {
        if (_stricmp(AnimTypes[index]->ID, name) == 0) {
            return AnimTypes[index];
        }
    }
    return new AnimTypeClass(name);
}


// void AnimTypeClass::Detach(AbstractClass *target, bool all) (0x00426530)

void AnimTypeClass::Detach(AbstractClass *target, bool all)
{
    if (Next == target) {
        Next = 0;
    }
}


// void *AnimTypeClass::Get_Image_Data() const (0x00426550)
// The image, demand-loading it from its file (theater-named, else generic)
// on first use.

void *AnimTypeClass::Get_Image_Data() const
{
    AnimTypeClass *self = (AnimTypeClass *)this;
    if (unknown_a4 == 0 && DemandLoad) {
        Debug_Printf("Demand loading image for %s\n", (char const *)Name);
        char fullname[512];
        _makepath(fullname, 0, 0, unknown_1c8[0] != 0 ? (char const *)unknown_1c8 : (char const *)ID, ".SHP");
        if (unknown_1fc) {
            _makepath(fullname, 0, 0, ID, Theaters[unknown_id_source_a3d290->Theater].Suffix);
        } else if (unknown_207) {
            Theater_Naming_Convention(fullname, unknown_id_source_a3d290->Theater);
        }
        CCFileClass file(fullname);
        self->unknown_a4 = Load_Alloc_Data(file);
        if (unknown_a4 != 0) {
            self->unknown_a8 = true;
        } else {
            Generic_Naming_Convention(fullname);
            CCFileClass generic(fullname);
            self->unknown_a4 = Load_Alloc_Data(generic);
            if (unknown_a4 != 0) {
                self->unknown_a8 = true;
            }
        }
        self->unknown_slot40(unknown_id_source_a3d290->Theater);
        return unknown_a4;
    }
    return unknown_a4;
}


// void AnimTypeClass::unknown_func_426700() (0x00426700)
// Frees a demand-loaded image once played (FreeAfterPlaying).

void AnimTypeClass::unknown_func_426700()
{
    if (DemandLoad && unknown_a4 != 0 && FreeAfterPlaying) {
        Debug_Printf("Freeing loaded image for %s\n", (char const *)Name);
        operator delete(unknown_a4);
        unknown_a4 = 0;
        unknown_a8 = false;
    }
}


// 0x00426760: function not yet identified
//
// Owner: ObjectTypeClass, virtual slot 28 (vtable +0x70) (from RTTI/vtables).
// Body: 4 bytes (frozen inventory bound).
// Referenced from 10 sites (data).
// Ghidra prototype: undefined4 unknown_return_minus_one_00426760(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00426760 {

unsigned int  unknown_return_minus_one_00426760(void)
{
    return 0xffffffff;
}

}  // namespace seed_00426760


// int AnimTypeClass::Kind_Of() const (0x00426770)
// Compiler-emitted from shared/anim_type.hpp in this unit; no source of its own.


// int AnimTypeClass::Fetch_Heap_ID() const (0x00426780)
// Compiler-emitted from shared/anim_type.hpp in this unit; no source of its own.


// int AnimTypeClass::Size_Of() const (0x00426790)
// Compiler-emitted from shared/anim_type.hpp in this unit; no source of its own.


// bool AnimTypeClass::Create_And_Place(Cell const *cell, HouseClass *house) (0x004267A0)
// Compiler-emitted from shared/anim_type.hpp in this unit; no source of its own.


// ObjectClass *AnimTypeClass::Create_One_Of(HouseClass *house) (0x004267B0)
// Compiler-emitted from shared/anim_type.hpp in this unit; no source of its own.


// void *AnimTypeClass::`scalar deleting destructor'(unsigned int) (0x004267C0)
// Compiler-emitted from shared/anim_type.hpp in this unit; no source of its own.
