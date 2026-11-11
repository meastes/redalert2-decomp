// Original smoke-vector RTTI79D0CC proves const ParticleSystemTypeClass pointees.
// Existing mutable ParticleSystem constructor contract is bridged explicitly, not retyped.
// @ref ??_7?$VectorClass@PBVParticleSystemTypeClass@@@@6B@=0079d0ac
// @ref ??_7?$DynamicVectorClass@PBVParticleSystemTypeClass@@@@6B@=0079d0cc
// Whole TechnoClass damage response, including the debris, particles and retaliation tail.
// Case 4 as one guarded block and separate `case 0: break; case 3: break;` labels follow the
// ra2-decomp Take_Damage shape (THIRD_PARTY.md); the separate labels give the original's case-1 table entry.
#include "shared/cell.hpp"
#include "shared/building_type_class.hpp"
#include "shared/building_class.hpp"
#include "shared/foot.hpp"
#include "shared/team.hpp"
#include "shared/warhead_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/anim.hpp"
#include "shared/voxel_anim.hpp"
#include "shared/particle_system.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"
#include "shared/id_source.hpp"
#include "shared/sidebar_building.hpp"
#include "shared/sound_play.hpp"

inline FootClass *As_Foot_L(TechnoClass *t) { return (t && t->AbstractFlags.IsFoot) ? static_cast<FootClass *>(t) : 0; }
inline bool Has_Ability_L(TechnoClass *t, int ability)
{
    if (t->unknown_118.Is_Veteran() || t->unknown_118.Is_Elite()) {
        TechnoTypeClass *type=t->unknown_slot32();
        if ((t->unknown_118.Is_Veteran() && type->VeteranAbilities.Ability[ability]) ||
            (t->unknown_118.Is_Elite() && (type->VeteranAbilities.Ability[ability] || type->EliteAbilities.Ability[ability]))) return true;
    }
    return false;
}
// @ref ?Damage_Smoke_Offset@TechnoTypeClass@@QBE?AUCoordStruct@@XZ=006df6c0
// @ref ??0ParticleSystemClass@@QAE@PAVParticleSystemTypeClass@@ABV?$TPoint3D@H@@PAVAbstractClass@@PAVObjectClass@@ABUCoordStruct@@@Z=0060aa70
int TechnoClass::unknown_slot86(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *attacker, bool forced, int no_crew)
{
    bool healing=*damage<0;
    if (!forced && !healing) {
        double incoming=*damage;
        *damage=(int)(incoming/(House->unknown_func_4f58a0(unknown_slot32())*unknown_120));
        if (Has_Ability_L(this,1)) {
            *damage=(int)(*damage/Rules->VeteranArmor);
        }
        if (*damage<1) *damage=1;
        if (attacker && unknown_slot32()->TypeImmune) {
            TechnoTypeClass *attacker_type=attacker->unknown_slot32();
            if (unknown_slot32()==attacker_type && House==attacker->House) return 0;
        }
    }
    if (unknown_slot203() && !forced && !healing) {
        Unknown_0047F600(Coord,*damage*2,warhead,1,1);
        *damage=0;
        return 0;
    }
    if (unknown_slot111()) {
        *damage=0;
        return 0;
    }
    if (unknown_slot32()->DamageReducesReadiness) {
        double fraction=(double)*damage/unknown_slot32()->MaxStrength;
        double reduction=unknown_slot32()->ReadinessReductionMultiplier*fraction;
        double readiness=unknown_254;
        unknown_254=(int)(readiness-unknown_slot32()->Ammo*reduction);
        if (unknown_254<0) unknown_254=0;
        unknown_6c8300();
    }
    if (warhead) {
        if (warhead->Radiation && unknown_slot32()->ImmuneToRadiation) { *damage=0; return 0; }
        if (warhead->PsychicDamage && unknown_slot32()->ImmuneToPsionics) { *damage=0; return 0; }
    }
    int result=ObjectClass::unknown_slot86(damage,distance,warhead,attacker,forced,no_crew);
    if (attacker) {
        TechnoTypeClass *type=unknown_slot32();
        double incoming=*damage;
        House->unknown_func_4eeba0((int)(type->Raw_Cost()*(incoming/unknown_slot32()->MaxStrength)),attacker->House);
    }
    if (result==4) {
        if (Kind_Of()==6 && warhead && warhead->CausesDelayKill) {
            BuildingClass *building=As_Building(this);
            if (building->Type->EligibleForDelayKill) {
                int delay=warhead->DelayKillFrames;
                int frames=(int)((double)delay+
                    ((warhead->DelayKillAtMax*(double)delay-delay)/(double)((int)warhead->CellSpread*256))*distance);
                if (!building->unknown_5af || frames<building->unknown_420.Value()) {
                    building->unknown_5af=true;
                    building->unknown_420=CDTimerClass<FrameTimerClass>(frames);
                }
                unknown_7d=true;
                Strength=1;
                result=5;
            }
        }
    }
    if (result==5) return 5;
    if (result!=0) {
        unknown_13c=CDTimerClass<FrameTimerClass>(Rules->RadarCombatFlashTime);
    }
    if (result!=0 && result!=4 && unknown_slot32()->CanDisguise && !unknown_slot32()->PermaDisguise) {
        if (unknown_slot48()) unknown_slot256();
        unknown_178=CDTimerClass<FrameTimerClass>(*damage*2);
    }
    if (Strength==0) result=4;
    switch (result) {
    case 4:
        {
            if (unknown_slot32()->VoiceDie.Count()>0 && House->Is_Player_Control()) {
                Voc_Play_At(unknown_slot32()->VoiceDie.Pick(unknown_random_839218()),Position_Coord_004f3f40(),0);
            }
            if (unknown_slot32()->DieSound.Count()>0) {
                Voc_Play_At(unknown_slot32()->DieSound.Pick(unknown_random_839218()),Position_Coord_004f3f40(),0);
            }
            SendToEachLink(3);
            unknown_slot210();
            if (unknown_25c[0]) { unknown_25c[0]->unknown_slot60(); unknown_25c[0]=0; }
            if (!(unknown_slot108()<=10 && unknown_7c && Map[Position_Coord_004f3f40()].unknown_ec==2)) {
                if (unknown_slot32()->MaxDebris>0) {
                    int maximum=unknown_slot32()->MaxDebris-1;
                    int debris=Random_Pick(unknown_slot32()->MinDebris,maximum);
                    if (unknown_slot32()->DebrisTypes.Count()>0) {
                        int index=0;
                        while (debris>0) {
                            int pieces=abs(unknown_id_source_a3d290->RandomNumber)%(unknown_slot32()->DebrisMaximums[index]+1);
                            if (pieces>=debris) pieces=debris;
                            for (int n=pieces;n>0;--n) {
                                new VoxelAnimClass(unknown_slot32()->DebrisTypes[index],Center_Coord(),House);
                            }
                            debris-=pieces;
                            if (++index>=unknown_slot32()->DebrisTypes.Count()) index=0;
                        }
                    }
                    if (unknown_slot32()->DebrisAnims.Count()>0) {
                        for (int n=debris;n>0;--n) {
                            new AnimClass(unknown_slot32()->DebrisAnims[Random_Pick(0,unknown_slot32()->DebrisAnims.Count()-1)],Center_Coord()+CoordStruct(0,0,20),0,1,0x600,0);
                        }
                    } else if (unknown_slot32()->DebrisTypes.Count()==0) {
                        for (;debris>0;--debris) {
                            new AnimClass(Rules->MetallicDebris[Random_Pick(0,Rules->MetallicDebris.Count()-1)],Center_Coord()+CoordStruct(0,0,20),0,1,0x600,0);
                        }
                    }
                }
                WeaponTypeClass *weapon=unknown_slot231(unknown_10c)->WeaponType;
                if (unknown_slot32()->Explodes || Has_Ability_L(this,10) || (weapon && weapon->unknown_11c)) {
                    unknown_6d3040(attacker);
                    unknown_6d7e20(0);
                }
            }
        }
        if (unknown_38) unknown_38->Unknown_00435D30();
        break;
    case 2:
        {
            TechnoTypeClass *type=unknown_slot32();
            if (type->VoiceFeedback.Count()>0 && unknown_random_839218(0,99)<30 && House->Is_Player_Control()) {
                int selection=unknown_random_839218();
                int voice=type->VoiceFeedback.Pick(selection);
                Voc_Play_At(voice,Center_Coord(),0);
            }
        }
        break;
    case 0:
        break;
    case 3:
        break;
    default:
        if (unknown_slot32()->DamageSound!=-1) {
            Voc_Static_Add(unknown_slot32()->DamageSound,Position_Coord_004f3f40(),0);
        }
        if (unknown_slot32()->DamageSound!=-1) {
            Voc_Static_Add(unknown_slot32()->DamageSound,Position_Coord_004f3f40(),0);
        }
        if ((unknown_slot32()->ToProtect || unknown_327) && !House->Is_Human_Player() && attacker) unknown_6d3400(attacker);
        break;
    }
    if (result!=4) {
    if (attacker && !House->Is_Ally(attacker)) unknown_329=true;
    unknown_slot61();
    if (Health_Ratio()>Rules->unknown_1368) {
        if (unknown_25c[3]) unknown_25c[3]->unknown_slot60();
    } else if (result==2 || result==3) {
            DynamicVectorClass<ParticleSystemTypeClass const *> smoke_types;
            for (int index=unknown_slot32()->DamageParticleSystems.Count()-1;index>=0;--index) {
                if (unknown_slot32()->DamageParticleSystems[index]->BehavesLike==0) smoke_types.Add(unknown_slot32()->DamageParticleSystems[index]);
            }
            if (!unknown_25c[3] && smoke_types.Count()>0 && unknown_slot108()>-10) {
                CoordStruct offset=unknown_slot32()->Damage_Smoke_Offset();
                unknown_25c[3]=new ParticleSystemClass(const_cast<ParticleSystemTypeClass *>(smoke_types[Random_Pick(0,smoke_types.Count()-1)]),Position_Coord_004f3f40()+offset,0,this,unknown_geometry_l);
            }
    }
    if (!healing) {
    TechnoClass *target=attacker;
    if (unknown_6d3b60(attacker,(int)warhead)) {
        if (attacker==0 && Rules->unknown_1431) target=0;
        if (target) {
            int which=unknown_slot174(target);
            bool retaliate=unknown_slot212(target,which);
            if (!retaliate) {
                if (House->Is_Human_Player()) {
                    TPoint3D<int> difference=Center_Coord()-target->Center_Coord();
                    double range=(int)unknown_sqrt_4ba410((double)difference.X*(double)difference.X+(double)difference.Z*(double)difference.Z+(double)difference.Y*(double)difference.Y);
                    retaliate=range<=(unknown_slot32()->Sight+0.5)*256.0;
                } else {
                    retaliate=true;
                }
            }
            if (retaliate) {
                Override_Mission(1,target,0);
            }
        }
        FootClass *foot=As_Foot_L(this);
        if (foot) {
            if (!unknown_228 && !foot->unknown_494) {
                if (Rules->unknown_1431 || Has_Ability_L(this,3)) {
                    unknown_slot87(unknown_geometry_l,true,false);
                }
            }
        }
    } else {
        FootClass *foot=As_Foot_L(this);
        if (foot) {
            if (Control()->unknown_09) {
                if (!unknown_370) {
                    if (!foot->Locomotor.Checked()->Is_Moving() && !unknown_228 && !foot->unknown_494 && Kind_Of()!=2) {
                        if (!House->Is_Human_Player() || Rules->unknown_1431 || Has_Ability_L(this,3)) {
                            unknown_slot87(unknown_geometry_l,true,false);
                        }
                    }
                }
            }
        }
    }
    }
    }
    return result;
}
