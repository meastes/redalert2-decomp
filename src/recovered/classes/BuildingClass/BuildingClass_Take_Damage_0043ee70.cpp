// Apply building damage, notify docked contacts, and update damage animations.
#include "shared/vector_class_decl.hpp"
class TechnoClass;
// Real original out-of-line base constructor copy; no new body credit.
// Generic complete constructor definition from the real shared vector header remains available.
// @ref ??0?$VectorClass@PAVTechnoClass@@@@QAE@IPBQAVTechnoClass@@@Z=0040bc10
// @ref ??_7?$DynamicVectorClass@PAVTechnoClass@@@@6B@=0079a79c
// @ref ??_7?$VectorClass@PAVTechnoClass@@@@6B@=0079a7fc
#include "shared/building.hpp"
#include "shared/building_map.hpp"
#include "shared/house.hpp"
#include "shared/anim.hpp"
#include "shared/bullet_type.hpp"
#include "shared/particle_system.hpp"
#include "shared/warhead_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/sound_play.hpp"
CoordStruct __fastcall Coord_Scatter(CoordStruct const &coord, int distance, bool lock);
inline CoordStruct Position_Coord_00450950(ObjectClass const &object);
extern bool Damage_Radio_Runaway_007cf670;
extern float const Damage_Spawn_Multiplier_0079d0e8;
// @ref ?Damage_Radio_Runaway_007cf670@@=007cf670
// @ref ?Damage_Spawn_Multiplier_0079d0e8@@=0079d0e8
// @ref ?unknown_slot86@TechnoClass@@=006cdc40
// @ref ??0AnimClass@@=00420110
// @ref ??0UnknownVector3@@=0041b630
// @ref ?Length@UnknownVector3@@=0041b780
// @ref ?Voc_Static_Add@@=00714290
inline UnknownVector3 Damage_Coord_Difference_0043ee70(CoordStruct const &left, CoordStruct const &right)
{
    return UnknownVector3(left.X-right.X,left.Y-right.Y,left.Z-right.Z);
}
// Hypothesis: object-distance operations composed from the coordinate difference.
// Both virtual coordinates finish before any component is subtracted.
inline int Damage_Object_Distance_0043ee70(ObjectClass const &origin,AbstractClass const &target)
{
    UnknownVector3 difference=Damage_Coord_Difference_0043ee70(origin.Center_Coord(),target.Center_Coord());
    return difference.Length();
}
inline double Damage_Object_Distance_Squared_0043ee70(ObjectClass const &origin,AbstractClass const &target)
{
    UnknownVector3 difference=Damage_Coord_Difference_0043ee70(origin.Center_Coord(),target.Center_Coord());
    return (double)difference.Z*difference.Z+(double)difference.Y*difference.Y+(double)difference.X*difference.X;
}
// The original random direction stores the RNG low byte in direction bits8..15.
// The upper half of the four-byte direction representation remains unspecified.
inline UnknownFacingDir Damage_Random_Direction_0043ee70()
{
    union DirectionBits {
        unsigned packed;
        unsigned short halves[2];
        struct { unsigned low_byte:8; unsigned direction_byte:8; };
    } bits;
    bits.halves[0]=0;
    bits.direction_byte=unknown_id_source_a3d290->Random()();
    UnknownFacingDir result;
    *reinterpret_cast<unsigned *>(&result)=bits.packed;
    return result;
}

int BuildingClass::unknown_slot86(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int no_crew)
{
    int result=0;
    if (this!=source) {
    Health_Ratio();
    int old_frame=unknown_43be20();
    if (source && !unknown_slot31()) {
        House->unknown_5408=Frame;
        House->unknown_540c=source->Owning_House()->ArrayIndex;
        unknown_6d3400(source);
    }
    Cell const *offset=unknown_slot64(0);
    DynamicVectorClass<TechnoClass *> contacts;
    for (int index=0;index<RadioLinks.Length();++index) {
        TechnoClass *contact=Contact(index);
        if (contact) contacts.Add(contact);
    }
    if (Type->LaserFence && !forced) return 0;
    if (Type->BridgeRepairHut && Type->unknown_203) return 0;
    if (Strength!=0) {
        result=TechnoClass::unknown_slot86(damage,distance,warhead,source,forced,no_crew);
        if (!unknown_7d) return result;
        switch (result) {
        case 5:
            return result;
        case 4: {
            for (int index=0;index<contacts.Count();++index) {
                TechnoClass *contact=contacts[index];
                if (Damage_Object_Distance_0043ee70(*this,*contact)<256 || Type->Helipad) {
                    int strength=10*contact->unknown_slot32()->MaxStrength;
                    contact->unknown_slot86(&strength,0,Rules->unknown_c74,0,true,1);
                } else {
                    unknown_slot150(23,contact);
                    contact->unknown_3f8=0;
                }
            }
            contacts.Delete_All();
            if (Type->CanBeOccupied) unknown_452f50(0);
            if (unknown_4e8) unknown_4e8->Deactivate(false);
            unknown_slot286(0,source,forced,offset);
            if (unknown_420.Value()>0) {
                unknown_slot60();
                Leave_Rubble_0043eba0();
            }
            break;
        }
        case 2:
            if (unknown_25c[2]) static_cast<ParticleSystemClass *>(unknown_25c[2])->unknown_d4 *= 1.5f;
        case 3:
            if (Damage_Radio_Runaway_007cf670) {
                for (int index=0;index<RadioLinks.Length();++index) {
                    TechnoClass *contact=Contact(index);
                    if (contact) {
                        int separation=(int)unknown_sqrt_4ba410(Damage_Object_Distance_Squared_0043ee70(*this,*contact));
                        if (separation<512 && !Type->Helipad) {
                            unknown_slot150(23,contact);
                            contact->unknown_3f8=0;
                        }
                    }
                }
            }
            if (unknown_slot32()->DamageSound==-1) Voc_Static_Add(Rules->BuildingDamageSound,Position_Coord_00450950(*this),0);
            while (*offset!=Cell(32767,32767)) {
                Cell relative(*offset++);
                Cell cell=unknown_slot104()+relative;
                CoordStruct location(cell);
                location.Z=Map.Unknown_0055A280(location);
                if (warhead->Sparky) {
                    AnimClass *fire=0;
                    switch (Random_Pick(0,5+Type->Foundation_Height(false)+Type->Width())) {
                    case 1: case 2: case 3: case 4: case 5:
                        fire=new AnimClass(Rules->OnFire[0],Coord_Scatter(location,96,false),0,Random_Pick(1,3),0x600,0);
                        break;
                    case 6: case 7: case 8:
                        fire=new AnimClass(Rules->OnFire[1],Coord_Scatter(location,96,false),0,Random_Pick(1,3),0x600,0);
                        break;
                    case 9:
                        fire=new AnimClass(Rules->OnFire[2],Coord_Scatter(location,96,false),0,1,0x600,0);
                        break;
                    }
                    if (fire) fire->Attach_To(this);
                }
            }
            break;
        }
    }
    if (!unknown_7d) return 4;
    if (source) {
        if (result==0) goto refresh_frame;
        if (!Type->unknown_202 && !unknown_slot31()) House->Attacked(this);
        unknown_434=source->Owning_House()->ArrayIndex;
        if (Mission!=19 && !House->Is_Ally(source) && unknown_slot231(0)->WeaponType && !unknown_slot231(0)->WeaponType->Projectile->AA && (!unknown_228 || !unknown_slot213(unknown_228))) {
            if (source->Kind_Of()==2 || (House->Unknown_004F52F0() && !Rules->PlayerReturnFire)) {
                if (!unknown_2e0.Is_Rotating()) unknown_2e0.Set_Desired(Damage_Random_Direction_0043ee70());
            } else unknown_slot220(source);
        }
    }
    if (result!=0) unknown_44d070(Health_Ratio()<=Rules->unknown_1368);
refresh_frame:
    if (old_frame!=unknown_43be20()) unknown_75=true;
    }
    return result;
}
