// NON-EXACT DRAFT 0x6C5570 FUN_006c5570
// entry 0x6C5570; exact-match score 91.81 (misses_index best), this text 91.7; plan "draft, verify"
// origin lane: Scout668 (build/class-work/Scout668/w/keep/eo_9170.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// TechnoClass::unknown_6c5570 (TibSun Evaluate_Object).
#include "shared/techno.hpp"
#include "shared/infantry.hpp"
#include "shared/building.hpp"
#include "shared/foot.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/warhead_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/fast_math.hpp"
static inline int Coord_Size(CoordStruct const &c)
{
    double y=c.Y;
    return (int)Fast_Sqrt((double)c.X*(double)c.X+y*y+(double)c.Z*(double)c.Z);
}
bool TechnoClass::unknown_6c5570(int method,int mask,int range,ObjectClass *object,int *value,int zone,CoordStruct const *coord)
{
    TechnoClass *tech=static_cast<TechnoClass *>(object);
    bool engineer=unknown_slot193();
    int which=unknown_slot174(object);
    WeaponTypeClass *weapon=unknown_slot231(which)->WeaponType;
    if(!(method&0x18200) && unknown_slot217(object,which)==5) return false;
    if(object->unknown_70<=0 && unknown_slot32()->VHPScan==2) return false;
    if(weapon && weapon->Warhead) {
        if((int)(weapon->Warhead->Verses[object->unknown_slot32()->unknown_9c]*100.0)==1) return false;
    }
    if(object==0 || object->unknown_76 || object->Strength==0) return false;
    if(tech->unknown_1b8==2) {
        if(!Map[object->Center_Coord()].Is_Sensed(House->ArrayIndex) && House!=tech->House) return false;
    }
    if(!tech->unknown_32d) return false;
    if(tech->Control()->unknown_04) return false;
    if(object->unknown_slot108()<-20) return false;
    CoordStruct objectcoord=object->Center_Coord();
    if(zone!=-1 && Map.Zone(Coord_Cell(objectcoord),unknown_slot32()->MovementZone,object->unknown_79)!=zone) return false;
    if((Kind_Of()!=15 || !static_cast<InfantryClass *>(this)->unknown_5c0) && !unknown_slot32()->AttackFriendlies && House->Is_Ally(tech)) {
        if(unknown_6c16f0(-1)>=0 && !engineer) return false;
        if(object->Health_Ratio()==Rules->unknown_1360) return false;
        if(object->Kind_Of()==2 && Kind_Of()==1) {
            if(object->unknown_slot108()>0) return false;
            if(Map[object->Center_Coord()].Cell_Building()) return false;
        } else if(Kind_Of()==1 && !object->unknown_slot31()) return false;
    }
    if(unknown_id_source_a3d290->unknown_000&0x800) {
        ObjectTypeClass *type=object->Class_Of();
        for(int i=0;i<Rules->unknown_9ec.Count();i++) {
            if(Rules->unknown_9ec[i]==type) return false;
        }
    }
    int dist;
    if(object->unknown_slot21()) {
        double x=object->Center_Coord().X-Center_Coord().X;
        double y=object->Center_Coord().Y-Center_Coord().Y;
        dist=(int)Fast_Sqrt(y*y+x*x);
        if(range>0 && dist>range) return false;
    } else {
        dist=Coord_Size(Center_Coord()-object->Center_Coord());
        if(range>0 && dist>range) return false;
    }
    if(range==0) {
        if(!unknown_slot163()) {
            if(dist>unknown_slot32()->GuardRange.Value) return false;
        } else {
            if(!unknown_slot212(object,which)) return false;
        }
    }
    if(House->Is_Player_Control() && !tech->unknown_372 && !tech->unknown_373 && unknown_game_mode_a3d298==0 && object->Kind_Of()!=2) return false;
    if(object->Kind_Of()==6 && static_cast<BuildingClass *>(object)->Type->InvisibleInGame) return false;
    int otype=object->Kind_Of();
    if(!((1<<otype)&mask) && (!(mask&2) || !object->unknown_slot31())) return false;
    TechnoTypeClass *tclass=object->unknown_slot32();
    if(!tclass->unknown_201) return false;
    if(unknown_game_mode_a3d298!=0 && tech->House->Type->MultiplayPassive) {
        if(House->Is_Human_Player() || object->Kind_Of()!=6 || Kind_Of()!=15) return false;
        InfantryClass *inf=static_cast<InfantryClass *>(this);
        bool defense=false;
        if(inf->Team && inf->Team->Class->IsBaseDefense) defense=true;
        bool capture;
        if(!static_cast<BuildingClass *>(object)->Type->NeedsEngineer || !inf->Type->Engineer || tech->House==House) capture=false; else capture=true;
        bool occupy;
        if(!inf->Type->Occupier || !static_cast<BuildingClass *>(object)->Unknown_00452EB0(this)) occupy=false; else occupy=true;
        if(unknown_game_mode_a3d298==0 || defense || (!capture && !occupy)) return false;
    }
    if(tclass->unknown_202 && (object->Kind_Of()!=6 || tech->House->Type->MultiplayPassive)) {
        if(House->Is_Human_Player() || object->Kind_Of()!=6 || Kind_Of()!=15) return false;
        InfantryClass *inf=static_cast<InfantryClass *>(this);
        bool defense=false;
        if(inf->Team && inf->Team->Class->IsBaseDefense) defense=true;
        bool capture;
        if(!static_cast<BuildingClass *>(object)->Type->NeedsEngineer || !inf->Type->Engineer || tech->House==House) capture=false; else capture=true;
        bool occupy;
        if(!inf->Type->Occupier || !static_cast<BuildingClass *>(object)->Unknown_00452EB0(this)) occupy=false; else occupy=true;
        if(unknown_game_mode_a3d298==0 || defense || (!capture && !occupy)) return false;
    }
    if(tclass->IsTrain && Kind_Of()==15 && static_cast<InfantryClass *>(this)->Type->VehicleThief) return false;
    if(object->unknown_slot49(House) && !unknown_slot32()->DetectDisguise) {
        if(!tech->unknown_184.Value()) return false;
        if(House->Is_Human_Player()) return false;
        int roll=unknown_id_source_a3d290->Random()(0,99);
        if(roll>Rules->DisabledDisguiseDetectionPercent[House->unknown_c8]) return false;
    }
    if(method&0x100) {
        object->Owning_House();
        return false;
    }
    if((method&0x200) && (otype!=6 || !static_cast<BuildingTypeClass *>(tclass)->Capturable)) return false;
    bool foot=false;
    if(this) foot=AbstractFlags.IsFoot;
    if((!foot || !static_cast<FootClass *>(this)->Team) && House->Is_Human_Player() && !object->unknown_slot31() && otype==6) {
        WeaponStruct *armed=tech->unknown_slot230();
        if((!armed || !armed->WeaponType || !object->unknown_slot32()->ThreatPosed) && !engineer) return false;
    }
    if(engineer) {
        if(object->Kind_Of()!=6) return false;
        if(House->Is_Ally(tech->House)) {
            if(object->Health_Ratio()>Rules->unknown_1370) return false;
            if(!static_cast<BuildingClass *>(object)->Type->unknown_slot33(House)) return false;
        }
    }
    if((method&0x40) && !tclass->Storage) return false;
    CellClass *here=&Map[Position_Coord_004f3f40()];
    CellClass *there=&Map[object->Position_Coord_004f3f40()];
    if((here->unknown_140&0x100) && (there->unknown_140&0x100) && unknown_79!=object->unknown_79) return false;
    *value=(int)unknown_6d7550(object,*coord);
    if(unknown_slot32()->VHPScan==1) {
        if(object->unknown_70<=0) *value=*value/2;
        else if(object->unknown_70<=object->unknown_slot32()->MaxStrength/2) *value=*value*2;
    }
    if(House->unknown_190 && House->unknown_54f0!=-1 && tech->House!=Houses[House->unknown_54f0]) *value=1;
    BuildingTypeClass *btype=static_cast<BuildingTypeClass *>(tclass);
    if((method&0x800) && otype==6) {
        if(btype->Power>0) *value=btype->Power*1000+*value;
        else *value=0;
    }
    if(method&0x8000) {
        if(otype!=6 || btype->MaxNumberOccupants<=0) return false;
        *value+=btype->MaxNumberOccupants*1000;
    }
    if(method&0x10000) {
        if(otype!=6 || !btype->NeedsEngineer) return false;
        *value+=1000;
    }
    if((method&0x1000) && otype==6 && btype->Factory==0) *value=0;
    if(method&0x2000) {
        WeaponStruct *w=unknown_slot230();
        bool keep=false;
        if(w && w->WeaponType && otype==6) {
            BuildingClass *b=object->Kind_Of()==6?static_cast<BuildingClass *>(object):0;
            // behaviour fix (codex4 r505): a building that cannot be occupied keeps its score.
            if(!b->Type->CanBeOccupied || b->unknown_4532f0()!=0) keep=true;
        }
        if(!keep) *value=0;
    }
    double areamod=unknown_6c5270(Coord_Cell(object->Center_Coord()));
    if(areamod!=1.0) *value=(int)(areamod*(double)*value);
    if(*value!=0) {
        *value=*value<1?1:*value;
        return true;
    }
    *value=0;
    return false;
}
