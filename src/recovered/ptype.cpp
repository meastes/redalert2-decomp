// ptype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ptype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#define TYPELIST_IMPLEMENT_DEFAULT_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_CLEAR
#include "shared/particle_system.hpp"
#include "shared/ccini_class.hpp"
#include "shared/warhead_type.hpp"


// Header-static dynamic initializers of the unit at 0x00620F50:  the
// shared/unknown_geometry.hpp constants (0x00620F50-0x00621111); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00620F80, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00620FA0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00620FC0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00620FE0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00621000, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00621020, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00621060, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00621090, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x006210C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x006210F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


// Dynamic initializer (0x00621100, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00620f50.cpp; no source of its own.


ParticleTypeClass::ParticleTypeClass(char const *ini_name) :
    ObjectTypeClass(ini_name), NextParticleOffset(0, 0, 0), XVelocity(1), YVelocity(1),
    MinZVelocity(0), ZVelocityRange(1), ColorSpeed(0), MaxDC(0), MaxEC(1), Warhead(0),
    Damage(0), StartFrame(0), NumLoopFrames(1), Translucency(0), WindEffect(0),
    Velocity(0), Deacc(0), Radius(0), DeleteOnStateLimit(false), EndStateAI(0),
    StartStateAI(0), StateAIAdvance(4), FinalDamageState(0), Translucent25State(255),
    Translucent50State(255), Normalized(false), NextParticle(-1), BehavesLike(-1)
{
    ParticleTypes.Add(this);
    unknown_objects_ac0cb0.Add(this);
    unknown_204 = false;
}


ParticleTypeClass::ParticleTypeClass(NoInitClass const &noinit) : ObjectTypeClass(noinit), StartColor1(noinit), StartColor2(noinit) {}


ParticleTypeClass::~ParticleTypeClass()
{
    Unknown_006ECB90(this, true);
    unknown_objects_ac0cb0.Delete(this);
    ParticleTypes.Delete(this);
}


// bool ParticleTypeClass::Read_INI(CCINIClass &ini) (0x00621490)
// The object keys, then the particle keys: the colour list (empty when
// absent), damage and warhead, animation and state timing, velocities and
// colours, the next particle type (made when missing) and BehavesLike= by
// name (Gas, Smoke, Fire, Spark, Railgun; -1 when unknown).

// The behaviour names in BehavesLike order (0x007EC1B4).
extern char const *unknown_particle_behaves_like_names_7ec1b4[5];

static inline WarheadTypeClass *Get_Warhead(CCINIClass &ini, char const *section, char const *entry, WarheadTypeClass *defvalue)
{
    char name[128];
    if (ini.Get_String(section, entry, unknown_string_83c55c, name, sizeof(name))) {
        return WarheadTypeClass::Find_Or_Make(name);
    }
    return defvalue;
}

// The behaviour with this name, or -1.
static inline int Behaves_From_Name(char const *name)
{
    for (int index = 0; index < 5; index++) {
        if (_stricmp(name, unknown_particle_behaves_like_names_7ec1b4[index]) == 0) {
            return index;
        }
    }
    return -1;
}

bool ParticleTypeClass::Read_INI(CCINIClass &ini)
{
    if (ObjectTypeClass::Read_INI(ini)) {
        ColorList = ini.Get_Color_List(ID, "ColorList", TypeList<RGBClass>());
        MaxDC = ini.Get_Int(ID, "MaxDC", MaxDC);
        MaxEC = ini.Get_Int(ID, "MaxEC", MaxEC);
        Damage = ini.Get_Int(ID, "Damage", Damage);
        Warhead = Get_Warhead(ini, ID, "Warhead", Warhead);
        StartFrame = ini.Get_Int(ID, "StartFrame", StartFrame);
        NumLoopFrames = ini.Get_Int(ID, "NumLoopFrames", NumLoopFrames);
        Translucency = ini.Get_Int(ID, "Translucency", Translucency);
        WindEffect = ini.Get_Int(ID, "WindEffect", WindEffect);
        Velocity = ini.Get_Double(ID, "Velocity", Velocity);
        Deacc = ini.Get_Double(ID, "Deacc", Deacc);
        Radius = ini.Get_Int(ID, "Radius", Radius);
        DeleteOnStateLimit = ini.Get_Bool(ID, "DeleteOnStateLimit", DeleteOnStateLimit);
        EndStateAI = ini.Get_Int(ID, "EndStateAI", EndStateAI);
        StartStateAI = ini.Get_Int(ID, "StartStateAI", StartStateAI);
        StateAIAdvance = ini.Get_Int(ID, "StateAIAdvance", StateAIAdvance);
        Translucent50State = ini.Get_Int(ID, "Translucent50State", Translucent50State);
        Translucent25State = ini.Get_Int(ID, "Translucent25State", Translucent25State);
        Normalized = ini.Get_Bool(ID, "Normalized", Normalized);
        ColorSpeed = ini.Get_Double(ID, "ColorSpeed", ColorSpeed);
        XVelocity = ini.Get_Int(ID, "XVelocity", XVelocity);
        YVelocity = ini.Get_Int(ID, "YVelocity", YVelocity);
        MinZVelocity = ini.Get_Int(ID, "MinZVelocity", MinZVelocity);
        ZVelocityRange = ini.Get_Int(ID, "ZVelocityRange", ZVelocityRange);
        (INIPoint3D &)NextParticleOffset = ini.Get_Integer_Triple(ID, "NextParticleOffset", (INIPoint3D &)NextParticleOffset);
        StartColor1 = ini.Get_RGB(ID, "StartColor1", StartColor1);
        StartColor2 = ini.Get_RGB(ID, "StartColor2", StartColor2);
        FinalDamageState = ini.Get_Int(ID, "FinalDamageState", EndStateAI);
        char buffer[32];
        if (ini.Get_String(ID, "NextParticle", 0, buffer, sizeof(buffer))) {
            NextParticle = Index_Or_Make(buffer);
        }
        buffer[0] = '\0';
        ini.Get_String(ID, "BehavesLike", 0, buffer, sizeof(buffer));
        BehavesLike = Behaves_From_Name(buffer);
        return true;
    }
    return false;
}


// int __fastcall ParticleTypeClass::Index_Or_Make(char const *name) (0x00621960)
// The index of the named particle type, made (and so appended) when
// missing; -1 for "<none>".


int __fastcall ParticleTypeClass::Index_Or_Make(char const *name)
{
    if (_stricmp(name, "<none>") != 0) {
        for (int index = 0; index < ParticleTypes.Count(); index++) {
            if (_stricmp(name, ParticleTypes[index]->ID) == 0) {
                return index;
            }
        }
        ParticleTypeClass *type = new ParticleTypeClass(name);
        return ParticleTypes.ID(type);
    }
    return -1;
}


// char const *__fastcall ParticleTypeClass::Name_From(unsigned int index) (0x006219F0)
// The ID of the indexed particle type, 0 when the index is out of range.

char const *__fastcall ParticleTypeClass::Name_From(unsigned int index)
{
    if (index < (unsigned int)ParticleTypes.Count()) {
        return ParticleTypes[index]->ID;
    }
    return 0;
}


// void ParticleTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x00621A10, slot 13)

void ParticleTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractTypeClass::Compute_CRC(crc);
    crc((long)MaxDC);
    crc((long)MaxEC);
    crc((long)Damage);
    crc((long)StartFrame);
    crc((long)NumLoopFrames);
    crc((long)Translucency);
    crc((long)WindEffect);
    crc(Velocity);
    crc(Deacc);
    crc((long)Radius);
    crc(DeleteOnStateLimit);
    crc(EndStateAI);
    crc(StartStateAI);
    crc(StateAIAdvance);
    crc((long)Translucent25State);
    crc((long)Translucent50State);
    crc(Normalized);
    crc((long)NextParticle);
    crc((long)BehavesLike);
    crc(ColorSpeed);
}


// HRESULT __stdcall ParticleTypeClass::GetClassID(GUID *clsid) (0x00621B50, slot 3)

extern "C" GUID const CLSID_ParticleTypeClass;

HRESULT __stdcall ParticleTypeClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_ParticleTypeClass;
    return S_OK;
}


// HRESULT __stdcall ParticleTypeClass::Load(IStream *stream) (0x00621B90)
// Loads the object type, restores the particle type in place, swizzles its
// warhead, rebuilds the colour list from the saved count and colours and
// reloads its image.

HRESULT __stdcall ParticleTypeClass::Load(IStream *stream)
{
    HRESULT result = ObjectTypeClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) ParticleTypeClass(NoInitClass());
    Swizzler.Swizzle((void **)&Warhead);
    int count = 0;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        RGBClass color;
        result = stream->Read(&color, sizeof(color), 0);
        if (FAILED(result)) {
            return result;
        }
        ColorList.Add(color);
    }
    Fetch_Normal_Image();
    return S_OK;
}


// HRESULT __stdcall ParticleTypeClass::Save(IStream *stream, BOOL clear_dirty) (0x00621CD0)
// Saves the object type, then the colour list as a count and its colours
// (a failed colour write still reports success).

HRESULT __stdcall ParticleTypeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = ObjectTypeClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = ColorList.Count();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        if (FAILED(stream->Write(&ColorList[index], sizeof(RGBClass), 0))) {
            return S_OK;
        }
    }
    return S_OK;
}


// ParticleTypeClass *__fastcall ParticleTypeClass::Find_Or_Make(char const *name) (0x00621D50)
// The named particle type, created when missing; 0 for "<none>" and "none".

ParticleTypeClass *__fastcall ParticleTypeClass::Find_Or_Make(char const *name)
{
    if (!_stricmp("<none>", name) || !_stricmp("none", name)) {
        return 0;
    }
    for (int index = 0; index < ParticleTypes.Count(); index++) {
        if (!_stricmp(ParticleTypes[index]->ID, name)) {
            return ParticleTypes[index];
        }
    }
    return new ParticleTypeClass(name);
}


// void ParticleTypeClass::Detach(AbstractClass *target, bool all) (0x00621DE0, slot 10)
// Forgets the warhead when it is the target.

void ParticleTypeClass::Detach(AbstractClass *target, bool all)
{
    if (Warhead == target) {
        Warhead = 0;
    }
}


// int __fastcall ParticleTypeClass::Behaviour_From_Name(char const *name) (0x00621E00)
// The BehavesLike= index of a name, -1 when unknown.

int __fastcall ParticleTypeClass::Behaviour_From_Name(char const *name)
{
    for (int index = 0; index < 5; index++) {
        if (!_stricmp(name, unknown_particle_behaviour_names_7ec1b4[index])) {
            return index;
        }
    }
    return -1;
}


// int ParticleTypeClass::Size_Of() const (0x00621E40, slot 12)

int ParticleTypeClass::Size_Of() const
{
    return sizeof(ParticleTypeClass);
}


// int ParticleTypeClass::Kind_Of() const (0x00621E50, slot 11)

int ParticleTypeClass::Kind_Of() const
{
    return 0x17;
}


// bool ParticleTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00621E60, slot 32)
// Particle types are never placed as objects.

bool ParticleTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}


// ObjectClass *ParticleTypeClass::Create_One_Of(HouseClass *house) const (0x00621E70, slot 35)
// Particle types make no object here.

ObjectClass *ParticleTypeClass::Create_One_Of(HouseClass *house) const
{
    return 0;
}


// Compiler-emitted from real ParticleTypeClass destructor in this class unit.


extern "C" unsigned char vtable_622040[]; extern "C" void __cdecl release_buffer_622040(void*); extern "C" void __cdecl release_object_622040(void*);
class Leaf_622040 { public: void* get(unsigned int flags); };
void* Leaf_622040::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_622040; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_622040((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_622040(this); return this; }
