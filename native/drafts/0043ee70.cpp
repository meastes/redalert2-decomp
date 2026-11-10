// NON-EXACT DRAFT 0x43EE70 FUN_0043ee70
// entry 0x43EE70; exact-match score 99.74 (misses_index best), this text 88.74; plan "draft as-is"
// origin lane: Scout862 (build/class-work/Scout862/w/tdJ_g1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): unit-copy helpers already in the host dropped; Delete_All as a local helper; 16-bit facing word; each changed line is marked NATIVE.
// Building damage, original-only extent 0043EE70..0043F835 plus11 NOP bytes.
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/building_map.hpp"
#include "shared/rules.hpp"
#include "shared/warhead_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/light_source.hpp"
#include "shared/weapon_type.hpp"
#include "shared/bullet_type.hpp"
#include "shared/anim.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/coord_distance.hpp"
extern bool Building_Evict_Contacts_007cf670;
// @ref ?Building_Evict_Contacts_007cf670@@=007cf670
// @ref ??_7?$DynamicVectorClass@PAVTechnoClass@@@@6B@=0079a79c
// @ref ??_7?$VectorClass@PAVTechnoClass@@@@6B@=0079a7fc
// @ref ??0?$VectorClass@PAVTechnoClass@@@@QAE@IPBQAVTechnoClass@@@Z=0040bc10
// @ref __real@4@3fffc000000000000000=0079d0e8
inline CoordStruct Position_Coord_00450950(ObjectClass const &object);
CoordStruct __fastcall Coord_Scatter(CoordStruct const &coord,int distance,bool lock);
// Coordinate difference, retaining the original out-of-line three-int construction.
// Source-local hypothesis for the reference Coord/TPoint3D difference operation.
// The difference of two coordinates (an inline helper).
inline int Length_0043ee70(UnknownVector3 const &vector)
{
    double y=vector.Y;
    double z=vector.Z;
    double x=vector.X;
    return (int)Fast_Sqrt(x*x+z*z+y*y);
}
// NATIVE: the lane's overlay member DynamicVectorClass::Delete_All as a local helper.
template<class T> inline void Delete_All_0043ee70(DynamicVectorClass<T> &vector)
{
    int capacity=vector.Length();   // NATIVE: VectorMax through its accessor
    vector.Clear();
    vector.Resize(capacity);
}
// Reference Random_Dir contract: the four-byte direction wire preserves its upper word.
inline UnknownFacingDir Random_Dir(int minimum,int maximum)
{
    union RandomDirectionBits {
        unsigned int Bits;
        struct { signed short Lower : 16; signed short Upper : 16; } Words;
        struct { unsigned int Lower : 8; unsigned int Angle : 8; } Bytes;
    } random_direction;
    random_direction.Words.Lower=0;
    random_direction.Bytes.Angle=unknown_id_source_a3d290->Random()();
    UnknownFacingDir direction;
    direction.Raw=(unsigned short)random_direction.Bits;   // NATIVE: the tree's UnknownFacingDir is the 16-bit word (the lane's overlay had a 32-bit Whole; the low word is the same)
    return direction;
}
int BuildingClass::unknown_slot86(int *damage,int distance,WarheadTypeClass *warhead,TechnoClass *source,bool forced,int arg6)
{
    int result=0;
    int index;
    if (this!=source) {
    Health_Ratio();
    int old_shape=unknown_43be20();
    if (source && !unknown_slot31()) {
        House->unknown_5408=Frame;
        House->unknown_540c=source->Owning_House()->ArrayIndex;
        unknown_6d3400(source);
    }
    Cell const *offset=unknown_slot64(0);
    DynamicVectorClass<TechnoClass *> contacts;
    for (index=0;index<RadioLinks.Length();++index) {
        TechnoClass *contact=Contact(index);
        if (contact) contacts.Add(contact);
    }
    if (Type->LaserFence && !forced) return 0;
    if (Type->BridgeRepairHut && Type->unknown_203) return 0;
    if (Strength!=0) {
        result=TechnoClass::unknown_slot86(damage,distance,warhead,source,forced,arg6);
        if (!unknown_7d) return result;
        switch(result) {
        case 5:return result;
        case 4: {
            for (index=0;index<contacts.Count();++index) {
                TechnoClass *contact=contacts[index];
                if (Distance_To(contact)>=256 && !Type->Helipad) {
                    unknown_slot150(23,contact);
                    contact->unknown_3f8=0;
                } else {
                    int strength=contact->unknown_slot32()->MaxStrength*10;
                    contact->unknown_slot86(&strength,0,Rules->unknown_c74,0,true,1);
                }
            }
            Delete_All_0043ee70(contacts);
            if (Type->CanBeOccupied) unknown_452f50(0);
            if (unknown_4e8) unknown_4e8->Deactivate(0);
            unknown_slot286(0,source,forced,offset);
            if (unknown_420>0) { unknown_slot60();Leave_Rubble_0043eba0(); }
            break; }
        case 2:
            if (unknown_25c[2]) static_cast<ParticleSystemClass *>(unknown_25c[2])->unknown_d4*=1.5f;
        case 3:
            if (Building_Evict_Contacts_007cf670) {
                for (index=0;index<RadioLinks.Length();++index) {
                    TechnoClass *contact=Contact(index);
                    if (contact) {
                        if (Distance_To(contact)<512 && !Type->Helipad) {unknown_slot150(23,contact);contact->unknown_3f8=0;}
                    }
                }
            }
            if (unknown_slot32()->DamageSound==-1) Voc_Static_Add(Rules->BuildingDamageSound,Position_Coord_00450950(*this),0);
            while (*offset!=Cell(32767,32767)) {
                Cell cell=Cell(*offset++) + unknown_slot104();
                CoordStruct coord(cell);
                coord.Z=Map.Get_Height_GL(coord);
                AnimClass *anim=0;
                if (warhead->Sparky) {
                    switch(Random_Pick(0,5+Type->Width()+Type->Foundation_Height(false))) {
                    case 1:case 2:case 3:case 4:case 5:
                        anim=new AnimClass(Rules->OnFire[0],Coord_Scatter(coord,96,false),0,Random_Pick(1,3),0x600,0);break;
                    case 6:case 7:case 8:
                        anim=new AnimClass(Rules->OnFire[1],Coord_Scatter(coord,96,false),0,Random_Pick(1,3),0x600,0);break;
                    case 9:
                        anim=new AnimClass(Rules->OnFire[2],Coord_Scatter(coord,96,false),0,1,0x600,0);break;
                    }
                }
                if (anim) anim->Attach_To(this);
            }
            break;
        }
    }
    if (!unknown_7d) return 4;
    if (source && result!=0) {
        if (!Type->unknown_202 && !unknown_slot31()) House->Attacked(this);
        unknown_434=source->Owning_House()->ArrayIndex;
        if (Mission!=19 && !House->Is_Ally(source) && unknown_slot231(0)->WeaponType &&
            !unknown_slot231(0)->WeaponType->Projectile->AA && (!unknown_228 || !unknown_slot213(unknown_228))) {
            if (source->Kind_Of()!=2 && (!House->unknown_func_4f52f0() || Rules->PlayerReturnFire)) unknown_slot220(source);
            else if (!unknown_2e0.Is_Rotating()) {
                unknown_2e0.Set_Desired(Random_Dir(0,255));
            }
        }
    }
    if (result!=0) unknown_44d070(Health_Ratio()<=Rules->unknown_1368);
    if (old_shape!=unknown_43be20()) unknown_75=true;
    }
    return result;
}
