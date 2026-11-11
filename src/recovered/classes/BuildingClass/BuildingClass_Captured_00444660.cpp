// bool BuildingClass::unknown_slot222(HouseClass *newowner, int announce) (0x00444660): TibSun
// BuildingClass::Captured (building.cpp:4212), independently bounded 00444660..0044537e. YR twin gamemd
// 0x00448260 (same contact-loop layout and out-of-line Set_Anim_Drawer/Set_Anim_Translucency calls).
// Original Reestablish vector slots, read after exact-target SHA256 verification.
// @ref ??_G?$VectorClass@PAVReestablish@@@@UAEPAXI@Z=00454a30
// @ref ??8?$VectorClass@PAVReestablish@@@@UBE_NABV0@@Z=004544b0
// @ref ?Resize@?$VectorClass@PAVReestablish@@@@UAE_NHPBQAVReestablish@@@Z=004544f0
// @ref ?Clear@?$VectorClass@PAVReestablish@@@@UAEXXZ=004545a0
// @ref ?ID@?$VectorClass@PAVReestablish@@@@UAEHABQAVReestablish@@@Z=004545d0
// @ref ?ID@?$VectorClass@PAVReestablish@@@@UAEHPBQAVReestablish@@@Z=00454610
// @ref ?Fetch@?$VectorClass@PAVReestablish@@@@UBEPAVReestablish@@H@Z=00453e30
// @ref ??_G?$DynamicVectorClass@PAVReestablish@@@@UAEPAXI@Z=00454a80
// @ref ?Resize@?$DynamicVectorClass@PAVReestablish@@@@UAE_NHPBQAVReestablish@@@Z=00454630
// @ref ?Clear@?$DynamicVectorClass@PAVReestablish@@@@UAEXXZ=00453de0
// @ref ?ID@?$DynamicVectorClass@PAVReestablish@@@@UAEHABQAVReestablish@@@Z=004546e0
// @ref ?ID@?$DynamicVectorClass@PAVReestablish@@@@UAEHPBQAVReestablish@@@Z=00453e10
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/target.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_vector3.hpp"
inline UnknownVector3 Coord_Difference(CoordStruct const &,CoordStruct const &);
void __fastcall Building_Refund_Upgrades_00452710(BuildingClass *building);
void __fastcall Building_Reset_Fence_Progress_0044e570(BuildingClass *building);
extern bool unknown_flag_8339e8;
extern "C" char const capture_eva_building_007cf924[];
extern "C" char const capture_eva_tech_lost_007cf958[];
extern "C" char const capture_eva_tech_007cf93c[];
// @ref _capture_eva_building_007cf924=007cf924
// @ref _capture_eva_tech_lost_007cf958=007cf958
// @ref _capture_eva_tech_007cf93c=007cf93c
// @ref ?unknown_flag_8339e8@@3_NA=008339e8
class Reestablish {
public:
    Reestablish(TechnoClass *contact, bool tethered) : Contact(contact), WasTethered(tethered) {}
    TechnoClass *Contact;
    bool WasTethered;
};
// The independently observed Reestablish base constructor is called at 0044499f.
// Keep this real member out of line; no inlined copy occurs in Building.CPP.
template<> VectorClass<Reestablish*>::VectorClass(unsigned size,Reestablish *const *array)
    : Vector(0),VectorMax(size),IsValid(true),IsAllocated(false)
{
    if(size) {
        if(array) Vector=new ((void*)array) Reestablish*[size];
        else {Vector=new Reestablish*[size];IsAllocated=true;}
    }
}
// @ref ??0?$VectorClass@PAVReestablish@@@@QAE@IPBQAVReestablish@@@Z=00454ba0
// @ref ??_7?$VectorClass@PAVReestablish@@@@6B@=0079d0f0
// @ref ??_7?$DynamicVectorClass@PAVReestablish@@@@6B@=0079d110
// @ref ?unknown_4444b0@BuildingClass@@QAE_N_N@Z=004444b0
bool BuildingClass::unknown_slot222(HouseClass *newowner, int announce)
{
    if(newowner!=House) {
    if(unknown_38 && !Type->CanBeOccupied) { unknown_38->Unknown_00435FB0(); }
    if(Type->CloakGenerator) { newowner->unknown_55e8=true; }
    if(House->Type->MultiplayPassive && Type->ProduceCashStartup) {
        newowner->Refund_Money(Type->ProduceCashStartup);
        unknown_5a0=CDTimerClass<FrameTimerClass>(Type->ProduceCashDelay);
    }
    if(unknown_1a8 & (1<<newowner->Type->House)) {
        unknown_1a8-=1<<newowner->Type->House;
        if(Type->Radar) { unknown_44ab00(); }
    }
    unknown_32b=false;
    if(Type->LaserFencePost) { unknown_44dc40(0); }
    if(House->Unknown_004F52B0() || newowner->Unknown_004F52B0()) {
        if(!newowner->Type->MultiplayPassive && (char)announce) {
            if(Type->NeedsEngineer) {
                if(House!=newowner) {
                    if(House->Unknown_004F52B0()) { Speech_Play_Name(capture_eva_tech_lost_007cf958,-1,-1); }
                    if(newowner->Unknown_004F52B0()) { Speech_Play_Name(capture_eva_tech_007cf93c,-1,-1); }
                }
            } else {
                if(Unknown_0063B5B0(10,unknown_slot104())) { Speech_Play_Name(capture_eva_building_007cf924,-1,-1); }
            }
        }
        unknown_flag_8339e8=true;
        Map.Flag_To_Redraw(0);
    }
    unknown_75=true;
    unknown_59c=true;
    if(Type->NeedsEngineer) { unknown_44d600(); }
    if(unknown_game_mode_a3d298==4) { newowner->unknown_42a8.Unknown_0070C490(Type->ArrayIndex); }
    TechnoClass *passenger=(TechnoClass*)Cargo.Attached_Object();
    if(passenger) { passenger->unknown_slot222(newowner,1); }
    DynamicVectorClass<Reestablish*> reestablish;
    int i;
    // TibSun building.cpp:4212 Captured handles one Contact_With_Whom(); RA2 walks every radio link and
    // nulls a non-foot contact before the run-away message (the original pushes the nulled register).
    // TibSun's order: the tether flag is read and the contact captured inside the condition's arm, and
    // the radio-contact flag is tested after it (the run-away block precedes the capture arm, as in the
    // original and the YR twin 0x00448260).
    for(i=0;i<RadioLinks.Length();++i) {
        TechnoClass *contact=Contact(i);
        if(contact) {
            bool captured=false;
            bool tethered=false;
            if(!contact->AbstractFlags.IsFoot) contact=0;
            else if(unknown_slot150(19,contact)==1 && (Type->WeaponsFactory || Coord_Difference(contact->Center_Coord(),unknown_slot41((int)contact)).Length()<64)) {
                tethered=contact->unknown_370;
                contact->unknown_slot222(newowner,1);
                captured=true;
            }
            if(!captured) {
                unknown_slot150(23,contact);
                unknown_slot150(3,contact);
            } else {
                Reestablish *record=new Reestablish(contact,tethered);
                reestablish.Add(record);
            }
        }
    }
    if(unknown_41c) {
        ((FactoryClass*)unknown_41c)->Abandon_Production_004B97C0();
        delete unknown_41c;
        unknown_41c=0;
    }
    House->unknown_func_4eaba0(this);
    HouseClass *oldowner=House;
    TargetClass to_capture(this);
    unknown_5b3=true;
    if(Rules->unknown_7c8.Is_Present(Type)) { oldowner->unknown_50.Delete(this); }
    if(Type->UnitRepair) { oldowner->unknown_80.Delete(this); }
    if(Type->Cloning) { oldowner->unknown_98.Delete(this); }
    if(Type->PsychicDetectionRadius>0) { oldowner->unknown_b0.Delete(this); }
    oldowner->Buildings.Delete(this);
    if(Type->OrePurifier) { --oldowner->unknown_52cc; }
    if(Type->Helipad) { oldowner->unknown_218-=Type->NumberOfDocks; }
    if(Type->CloakGenerator) {
        Disable_Cloak_Generator_0044fad0();
        unknown_5bc=1;
        Cloaking_AI(1);
    }
    if(Type->GapGenerator) { unknown_slot236(); }
    unknown_slot263(false,false,false,0);
    TechnoClass::unknown_slot222(newowner,1);
    if(Type->CloakGenerator && unknown_slot195()) { Enable_Cloak_Generator_0044fa90(); }
    if(Type->GapGenerator && unknown_slot195()) { unknown_slot235(); }
    unknown_4444b0(false);
    oldowner->unknown_5410=this;
    oldowner->unknown_func_4e8640();
    House->unknown_func_4e8640();
    if(House->unknown_5410==this) { House->unknown_5410=0; }
    if(oldowner->Unknown_004F52B0() && Type->ConstructionYard) {
        Map.unknown_11a0=0;Map.unknown_11a4=0;Map.unknown_11a8=-1;
        Map.Unknown_00498F10(0);
    }
    House->unknown_func_4eac70(this);
    unknown_5b8=false;
    unknown_slot282(true);
    if(Type->LaserFencePost) {
        unknown_44dbe0();
        Building_Reset_Fence_Progress_0044e570(this);
    }
    if(House->Unknown_004F52B0()) {unknown_slot262(false,false,false,0);Map.Flag_To_Redraw(1);}
    if(unknown_1a8 & (1<<newowner->Type->House)) { unknown_1a8 &= ~(1<<newowner->Type->House); }
    Update_Anim_Appearance_0044d190();
    if(Rules->unknown_7c8.Is_Present(Type)) { newowner->unknown_50.Add(this); }
    if(Type->UnitRepair) { newowner->unknown_80.Add(this); }
    if(Type->Cloning) { newowner->unknown_98.Add(this); }
    if(Type->PsychicDetectionRadius>0) { newowner->unknown_b0.Add(this); }
    newowner->Buildings.Add(this);
    if(Type->OrePurifier) { ++House->unknown_52cc; }
    if(Type->Helipad) { House->unknown_218+=Type->NumberOfDocks; }
    if(Type->Factory) {oldowner->unknown_func_4f3170(Type->Factory,Type->Naval,0);newowner->unknown_func_4f3170(Type->Factory,Type->Naval,0);}
    for(i=0;i<reestablish.Count();++i) {
        Reestablish *record=reestablish[i];
        unknown_slot150(2,record->Contact);
        record=reestablish[i];
        if(record->WasTethered) {unknown_370=true;record=reestablish[i];record->Contact->unknown_370=true;}
    }
    if(unknown_game_mode_a3d298!=0 && !newowner->unknown_func_4f52f0()) {
        Building_Refund_Upgrades_00452710(this);
    }
    oldowner->unknown_144=true;newowner->unknown_144=true;
    return true;
    }
    return false;
}
