// Damage, death animation, and fear response of an infantry unit.
#include "shared/infantry.hpp"
#include "shared/warhead_type.hpp"
#include "shared/anim.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_geometry.hpp"
// @ref ??0AnimClass@@QAE@PBVAnimTypeClass@@ABUCoordStruct@@HHHH@Z=00420110
// @ref ??2@YAPAXI@Z=00782017
// @ref ?Take_Damage@FootClass@@=004c6000
// @ref ?unknown_6d3040@TechnoClass@@=006d3040

int InfantryClass::Take_Damage(int *damage,int distance,WarheadTypeClass *warhead,TechnoClass *source,bool forced,int no_crew)
{
    if (unknown_5c3 && *damage>0 && !forced) {
        *damage=int(*damage*warhead->ProneDamage);
        if (*damage<1) *damage=1;
    }
    int result=FootClass::Take_Damage(damage,distance,warhead,source,forced,no_crew);
    if (result==5) return result;
    if (result==0) return result;
    if (result==4) {
        // The existing integer slot216 declaration carries the original source-pointer bits.
        unknown_slot216(reinterpret_cast<int>(source));
        unknown_slot291();
        unknown_slot210();
        QueueMission(-1,false);
        QueueMission(5,false);
        Commence();
        unknown_6d3040(source);
        bool delete_after_sequence=false;
        if (forced && Type->Cyborg) {
            delete_after_sequence=true;
            if (unknown_7c) new AnimClass(Rules->InfantryExplode,CoordStruct(Coord),0,1,0x600,0);
        }
        if (Get_Height_AGL()<=10 && Map[CoordStruct(Coord)].unknown_ec==2 && unknown_7c) {
            new AnimClass(Rules->unknown_09c,CoordStruct(Coord),0,1,0x600,0);
            new AnimClass(Rules->SplashList[0],CoordStruct(Coord)+CoordStruct(0,0,3),0,1,0x600,0);
            delete_after_sequence=true;
        } else if (Type->Cyborg && unknown_5c3) {
            new AnimClass(Rules->InfantryExplode,CoordStruct(Coord),0,1,0x600,0);
            delete_after_sequence=true;
        } else if (Type->JumpJet) {
            new AnimClass(Rules->InfantryExplode,CoordStruct(Coord),0,1,0x600,0);
            delete_after_sequence=true;
        } else {
            int death=warhead->InfDeath;
            if (Sequence==33) death=3;
            if (source && source->Kind_Of()==6 && static_cast<BuildingClass *>(source)->Type->LaserFence) death=5;
            if (Type->DeathAnims.Count()>0) {
                if (death<0 || death>=Type->DeathAnims.Count() || !Type->DeathAnims[death]) death=0;
                if (Type->DeathAnims[death]) new AnimClass(Type->DeathAnims[death],CoordStruct(Coord),0,1,0x600,0);
                delete_after_sequence=true;
            } else if (Type->NotHuman) {
                unknown_slot311(11,true,false);
            } else {
                switch (death) {
                default:
                    delete_after_sequence=true;
                    break;
                case 1:
                    unknown_slot311(11,true,false);
                        break;
                case 2:
                    unknown_slot311(12,true,false);
                        break;
                case 3:
                    new AnimClass(Rules->InfantryExplode,CoordStruct(Coord),0,1,0x600,0);
                    delete_after_sequence=true;
                    break;
                case 4:
                    new AnimClass(Rules->FlamingInfantry,CoordStruct(Coord),0,1,0x600,0);
                    delete_after_sequence=true;
                    break;
                case 5:
                    new AnimClass(AnimTypes[1],CoordStruct(Coord),0,1,0x600,0);
                    delete_after_sequence=true;
                    break;
                case 6:
                    new AnimClass(Rules->InfantryHeadPop,CoordStruct(Coord),0,1,0x600,0);
                    delete_after_sequence=true;
                    break;
                case 7:
                    new AnimClass(Rules->InfantryNuked,CoordStruct(Coord),0,1,0x600,0);
                    delete_after_sequence=true;
                    break;
                }
            }
        }
        if (delete_after_sequence) {
            if (Type->Crashable) {
                if (!unknown_slot224(0)) {
                    Delete_Me();
                    return 4;
                }
            } else Delete_Me();
        }
        return 4;
    }
    if (result!=4) {
        CoordStruct source_position=source ? CoordStruct(source->Coord) : unknown_geometry_l;
        if (!House->Is_Human_Player() && Type->Engineer && (Get_Mission()==5 || Get_Mission()==11)) QueueMission(15,false);
        if (source) Scatter(source_position,false,false);
        if (source && unknown_5bc<100) {
            if (Type->Fraidycat) unknown_5bc=300;
            else if (!Type->Fearless && !unknown_6d7910(13)) unknown_5bc=100;
        } else if (!Type->Fearless && !unknown_6d7910(13)) {
            int more_fear=50;
            if (Health_Ratio()>Rules->unknown_1370) more_fear/=2;
            if (Health_Ratio()>Rules->unknown_1368) more_fear/=2;
            unknown_5bc=unknown_5bc+more_fear<300 ? unknown_5bc+more_fear : 300;
        }
    }
    return result;
}
