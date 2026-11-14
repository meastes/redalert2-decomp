// infantry.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/infantry.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_AS_CELL
#define RA2_SHIP_MOVE_TO_VALUE  // ILocomotion::Move_To takes the COM coordinate (shared/ilocomotion.hpp): JumpJet_To_Walk 0x508AB0
#define RA2_INFANTRY_COMIP_PERSIST  // comip levels of UnknownPersistPtr (shared/infantry.hpp)
#define RA2_CELL_FROM_COORD_AS_CELL  // the unit converts through CoordStruct::As_Cell (shared/coord.hpp)
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_COORD_TPOINT3D
#define RA2_COORD_CELL_BASE_INIT
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_TPOINT3D_PLUS_POINT2D
#define RA2_COORD_LENGTH
#define RA2_BUILDING_LIGHT_DISTANCE
#define RA2_COORD_COM_VALUE
#define RA2_TPOINT3D_DIFFERENCE
#define RA2_MAP_CLOSE_OBJECT
#endif

#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/building.hpp"
#include "shared/infantry_locomotion_ptr.hpp"
#include "shared/infantry.hpp"
#include "shared/house_type.hpp"
#include "shared/warhead_type.hpp"
#include "shared/anim.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/techno_map.hpp"
#include "shared/tactical.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/palette.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/id_source.hpp"
#include "shared/sidebar_building.hpp"
#include "shared/infantry_piggyback_ptr.hpp"
#include "shared/building_type.hpp"
#include "shared/tube.hpp"
#include "shared/parasite.hpp"
#include "shared/overlay_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/ground_type.hpp"
#include "shared/random2.hpp"
#include "shared/fast_math.hpp"
#include "shared/team.hpp"
#include "shared/team_type.hpp"
#include "shared/keyboard.hpp"
#include "shared/string_table.hpp"
#include "shared/fogged_object.hpp"
#include "shared/ccini_class.hpp"
#include "shared/tag.hpp"
#include "shared/tag_type.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/crt_memory.hpp"
#include "shared/radsite.hpp"
#include "shared/com_ptr.hpp"
#include "shared/foot.hpp"


// Header-static dynamic initializers of the unit at 0x004FF450:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004FF450-0x004FF636); the other
// drafts of this plan are emitted by these includes.

// The recovered COM instance belongs to Infantry.cpp, before Foot instantiates it.


// Dynamic initializer (0x004FF480, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF4A0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF4C0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF4E0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF500, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF520, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF560, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF590, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF5C0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF5F0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF600, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// Dynamic initializer (0x004FF620, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004ff450.cpp; no source of its own.


// InfantryClass::InfantryClass(InfantryTypeClass *type, HouseClass *house) (0x004FF640)
// Takes an ID, joins the infantry list, initializes from the type, creates
// the type's locomotor and links it, sets the body facing's rate (0x7F)
// and joins the techno index.

InfantryClass::InfantryClass(InfantryTypeClass *type, HouseClass *house) :
    FootClass(house),
    Type(type),
    Sequence(-1),
    unknown_5bc(0),
    unknown_5c0(false),
    unknown_5c1(false),
    unknown_5c2(false),
    unknown_5c3(false),
    unknown_5c4(false),
    unknown_5c5(false),
    unknown_5c8(0),
    unknown_5cc(false),
    unknown_5d0(2)
{
    Create_ID();
    Infantry.Add(this);
    Init();
    if (Type != 0) {
        Locomotor.CreateInstance(Type->Locomotor, 0, CLSCTX_ALL);
        Locomotor.Checked()->Link_To_Object(this);
    }
    unknown_2e0.Set_ROT(0x7F);
    unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
}

// Inline base copies: bindings from the original InfantryClass vtable slots.


// void InfantryClass::Init() (0x004FF8B0, slot 9)
// TechnoClass::Init and the house's notice; then from the type: flag
// +0x118 when the house type lists it (+0x14C) or for Trainable types of
// houses with +0x206, strength, ammunition and Cloakable (+0x32A).

void InfantryClass::Init()
{
    TechnoClass::Init();
    if (House != 0) {
        House->unknown_4ea970(this);
    }
    if (Type != 0) {
        if (House != 0) {
            TypeList<InfantryTypeClass *> &types = House->Type->VeteranInfantry;
            for (int index = 0; index < types.Count(); index++) {
                if (types[index] == Type) {
                    unknown_118.Set_Veteran(true);
                    break;
                }
            }
        }
        if (House != 0 && House->unknown_206 && Type->Trainable) {
            unknown_118.Set_Veteran(true);
        }
        Strength = Type->MaxStrength;
        unknown_70 = Type->MaxStrength;
        if (Type->InitialAmmo != -1) {
            unknown_254 = Type->InitialAmmo;
        } else {
            unknown_254 = Type->Ammo;
        }
        unknown_32a = Type->Cloakable;
    }
}


// InfantryClass::~InfantryClass() (0x004FF980)
// While the game runs (0x00A40958) and the type is set: tells the house
// (flag +0x144 when it has none of the type left), leaves the team and the
// house, stops the locomotor's animation and resets the infantry state.
// Always leaves the infantry list and the techno index.

InfantryClass::~InfantryClass()
{
    if (unknown_flag_a40958 && Type != 0) {
        if (House->Can_Build(Type, 0, 0) == -1) {
            House->unknown_144 = true;
        }
        if (Team != 0) {
            Team->Unknown_006B8FB0(this, -1, 0);
            Team = 0;
        }
        House->unknown_4ea800(this);
        InfantryClass::Limbo();
    }
    Unknown_006ECB90(this, true);
    Infantry.Delete(this);
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    unknown_7d = false;
}


// Damage, death animation, and fear response of an infantry unit.

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


// Animation frame including disguise sequences and jumpjet target facing (0x00500350).
extern int const unknown_sequence_facings_7a33f4[32];
extern "C" GUID const CLSID_JumpjetLocomotionClass;

int InfantryClass::unknown_500350()
{
    int sequence = Sequence;
    if (sequence == -1) {
        if (Map_Cell_At(unknown_slot104())->unknown_ec == 2 && !unknown_79) {
            sequence = 16;
        } else {
            sequence = 0;
        }
    }
    InfantryTypeClass *type = Type;
    if (!unknown_slot245(unknown_player_a35db4)) {
        type = (InfantryTypeClass *)unknown_slot50(true);
    }
    int count = type->DoControls[sequence].Count;
    int frame = StageClass::Stage % (count > 1 ? count : 1);
    bool is_jumpjet = false;
    if (Type->JumpJet) {
        UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Locomotor);
        GUID clsid;
        persist.Checked()->GetClassID(&clsid);
        is_jumpjet = clsid == CLSID_JumpjetLocomotionClass;
    }
    if (is_jumpjet && unknown_228 != 0) {
        if (type->DoControls[sequence].Jump > 0) {
            frame += unknown_sequence_facings_7a33f4[Dir_To_32(Direction_To(unknown_228))]
                * type->DoControls[sequence].Jump;
        }
    } else if (type->DoControls[sequence].Jump > 0) {
        frame += unknown_sequence_facings_7a33f4[Dir_To_32(unknown_2e0.Current())]
            * type->DoControls[sequence].Jump;
    }
    return type->DoControls[sequence].Frame + frame;
}


// Draw infantry, airborne pod imagery, and the ground shadow.
// Infantry.cpp already owns the shared empty rectangle initialized by the canonical header-static rows.
// Reuse the shape header's existing fallback-selection API.
extern "C" GUID const CLSID_BallisticLocomotionClass;
extern "C" ConvertClass *unknown_infantry_drawer_008323c4;
extern "C" ShapeFileStruct const *unknown_infantry_shadow_00a41134;
extern "C" int unknown_infantry_zadjust_007daeb8;
// Existing free declarations from lightning_storm.hpp; no class statics are needed here.
bool Unknown_0051FC50();
bool Unknown_0051FC60();
// Existing int formals carry Point2D and Rect pointers; their ABI stays unchanged.
void InfantryClass::unknown_slot67(Point2D const &input, Rect const &window)
{
    Point2D point=input;
    Cell cell=Get_Target_Cell();
    if (unknown_574==-1) {
        UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist(Locomotor);
        GUID class_id;
        persist.Checked()->GetClassID(&class_id);
        if (Get_Height_AGL()>0 && memcmp(&class_id,&CLSID_BallisticLocomotionClass,sizeof(GUID))==0) {
            ShapeFileStruct const *shape=static_cast<ShapeFileStruct const *>(MixFileClass::Retrieve("POD.SHP"));
            // COM returns a separate two-int point layout. The existing interface
            // spells that result Point2D; this storage view preserves its ABI and
            // applies the evidenced converting constructor without retyping it.
            Point2D shadow=input+Point2D(*reinterpret_cast<RectPoint2D const *>(&Locomotor.Checked()->Shadow_Point()));
            CC_Draw_Shape(unknown_surface_83995c,unknown_infantry_drawer_008323c4,shape,
                Locomotor.Checked()->Drawing_Code(),shadow,window,0x2e01,0,unknown_slot176()-2,0,1000,0,0,Point2D(0,0));
            Map[cell];
            Map[cell];
            unknown_slot293(shape,Locomotor.Checked()->Drawing_Code(),&input,&window,0,256,0,2,0,1000,0,0,Point2D(0,0),0);
        } else {
            ShapeFileStruct const *shape=static_cast<ShapeFileStruct const *>(Get_Image_Data());
            if (!shape) return;
            Cell target_cell=Get_Target_Cell();
            unknown_tactical_83996c->Add_To_Selectables(this, point);
            int level_light;
            if (Unknown_0051FC50()) level_light=unknown_id_source_a3d290->IonLevel;
            else if (Unknown_0051FC60()) level_light=unknown_id_source_a3d290->unknown_22f4;
            else level_light=unknown_id_source_a3d290->AmbientLevel;
            int brightness;
            if (unknown_79 || ((Map[Position_Coord_004f3f40()].unknown_140&0x10000) &&
                Position_Coord_004f3f40().Z>Map.Get_Height_GL(Position_Coord_004f3f40())+unknown_geometry_j/2)) {
                int light=level_light*(Get_Height_AGL()/(unknown_geometry_g*2));
                brightness=Map[target_cell].unknown_10a+light;
            } else {
                brightness=Map[target_cell].unknown_10a+((Map[Position_Coord_004f3f40()].unknown_140&0x10000)?-500:0);
            }
            brightness+=Rules->ExtraInfantryLight;
            int height=Get_Height_AGL();
            CellClass *current_cell=&Map[Position_Coord_004f3f40()];
            if ((current_cell->unknown_140&0x100) && height>=unknown_geometry_j) {
                if (((current_cell->unknown_140&0x800) && (current_cell->Adjacent_Cell(0).unknown_140&0x100)) ||
                    (!(current_cell->unknown_140&0x800) && (current_cell->Adjacent_Cell(6).unknown_140&0x100))) height-=unknown_geometry_j;
            }
            if (height>0 && (unknown_infantry_shadow_00a41134 ||
                (unknown_infantry_shadow_00a41134=static_cast<ShapeFileStruct const *>(MixFileClass::Retrieve("INFSHDW.SHP"))))) {
                CC_Draw_Shape(unknown_surface_83995c,unknown_infantry_drawer_008323c4,unknown_infantry_shadow_00a41134,
                    1,point+Point2D(0,-*reinterpret_cast<signed char const *>(&unknown_59e))+Point2D(0,Unknown_006A2970(height)),window,0x2601,0,-5-Unknown_006A2970(Position_Coord_004f3f40().Z-height),0,1000,0,0,Point2D(0,0));
            }
            // The original reads this storage byte with MOVSX; the current shared bool name remains a hypothesis.
            signed char draw_offset=*reinterpret_cast<signed char const *>(&unknown_59e);
            unknown_slot293(shape,unknown_500350(),&(point+Point2D(0,-draw_offset)),&window,0,256,unknown_infantry_zadjust_007daeb8,2,0,brightness,0,0,Point2D(0,0),0);
        }
    } else unknown_tactical_83996c->Add_To_Selectables(this, point);
    FootClass::unknown_slot67(point,window);
}


// Not exact (RA2_NON_MATCHING): InfantryClass::unknown_slot93 at 0x00500AA0, best score 86.18, 2026-11-13; differs: 1441 instructions in the original, 1446 in the draft (shape 99.34)
#ifdef RA2_NON_MATCHING
// Infantry arrival: garrison, capture, transport, sabotage, and cell effects.
#include "shared/session_state.hpp"
#include "shared/sound_play.hpp"
#include "shared/vox_class.hpp"
#include "shared/radar_event.hpp"
#include "shared/tileset_index.hpp"
#include "shared/cell_class.hpp"


#define MIN(a,b) (((a) < (b)) ? (a) : (b))

inline CoordStruct Adjacent_Cell(CoordStruct coord, int facing)
{
    TPoint2D<int> const &offset=unknown_facing_offsets_851ac0[facing & 7];
    return CoordStruct(coord.X+offset.X,coord.Y+offset.Y,coord.Z);
}

void InfantryClass::unknown_slot93(int why)
{
    CellClass *cell=&Map[CoordStruct(Coord)];
    if (why==2) {
        if (Get_Mission()==8 && (Type->Assaulter || Type->Occupier) && unknown_494 && unknown_494->Kind_Of()==6) {
            BuildingClass *building=static_cast<BuildingClass *>(unknown_494);
            if (building==cell->Cell_Building()) {
                if (!building->Unknown_00452EB0(this)) {
                    Assign_Destination(0,true);
                    Scatter(building->Center_Coord(),true,true);
                    return;
                }
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                if (Type->Occupier) {
                    Limbo();
                    building->unknown_554.Add(this);
                    building->Unknown_006D9610(building->Get_Cell_Ptr());
                    return;
                } else if (Type->Assaulter) {
                    building->Unknown_004536D0(this);
                    Assign_Destination(0,true);
                    Scatter(building->Center_Coord(),true,true);
                    return;
                }
                return;
            }
        }
        if (Get_Mission()==9) {
            BuildingClass *building=static_cast<BuildingClass *>(cell->Cell_Building());
            if (building && (building==unknown_494 || building==unknown_228)) {
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                if (Techno_Type_Class()->VoiceDie.Count()>0 && House->Is_Player_Control()) {
                    CoordStruct location=Coord;
                    Voc_Play_At(Techno_Type_Class()->VoiceDie.Pick(unknown_random_839218()),location,0);
                }
                if (Techno_Type_Class()->DieSound.Count()>0) {
                    CoordStruct location=Coord;
                    Voc_Play_At(Techno_Type_Class()->DieSound.Pick(unknown_random_839218()),location,0);
                }
                building->House->Refund_Money(unknown_slot167());
                if (unknown_34) unknown_34->Spring(48,this,unknown_geometry_k,false,0);
                Delete_Me();
                return;
            }
        }
        if (Get_Mission()==8 || Get_Mission()==11 || Get_Mission()==25) {
            AbstractClass *navigation=unknown_494;
            bool is_techno;
            if (navigation) is_techno=navigation->AbstractFlags.IsTechno;
            if (navigation && is_techno && static_cast<ObjectClass *>(navigation)->unknown_slot31()) {
                TechnoClass *destination=static_cast<TechnoClass *>(unknown_494);
                if (destination && destination->Kind_Of()==6) {
                    if (unknown_slot104()==destination->unknown_slot104()) {
                        if (destination->unknown_34) destination->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                        destination->unknown_slot54(false);
                        destination->unknown_slot222(House,1);
                        destination->unknown_290=Type->ArrayIndex;
                        destination->unknown_slot173();
                        if (unknown_34 && unknown_34->Is_To_Inherit()) destination->Attach_Tag(unknown_34);
                        if (unknown_34) unknown_34->Spring(48,this,unknown_geometry_k,false,0);
                        Delete_Me();
                        return;
                    }
                    UnknownCoord434820 locomotor_destination(Locomotor.Checked()->Destination());
                    if (locomotor_destination==unknown_geometry_l || unknown_slot104()==Coord_Cell(locomotor_destination)) {
                        Assign_Destination(unknown_494,true);
                        return;
                    }
                }
            }
            BuildingClass *building=static_cast<BuildingClass *>(cell->Cell_Building());
            if (building && (building==unknown_494 || building==unknown_228)) {
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                if (Type->Engineer) {
                    if (building->Kind_Of()==6 && building->Type->BridgeRepairHut) {
                        if (House->Is_Player_Control() && Unknown_0063B5B0(14,unknown_slot104())) Speech_Play_Name("EVA_BridgeRepaired",-1,-1);
                        bool train=false;
                        for (int y=-2;y<3;y++) {
                            for (int x=-2;x<3;x++) {
                                Cell spot=Cell(unknown_slot104()+Cell(x,y));
                                int tile=Map[spot].IsoTileIndex;
                                Cell overlay_spot=Cell(Cell(x,y)+unknown_slot104());
                                int overlay=Map[overlay_spot].OverlayIndex;
                                if ((tile>=unknown_tileset_a6cb68 && tile<unknown_tileset_a6cb68+16) || (overlay>=74 && overlay<=101)) train=true;
                            }
                        }
                        if (train) Map.Unknown_00552570(unknown_slot104());
                        else Map.Unknown_00555A60(unknown_slot104());
                        for (int index=Infantry.Count()-1;index>=0;--index) Infantry[index]->Detach(building,false);
                        building->unknown_slot173();
                    } else if (House->Is_Ally(building) || (building->House->Type->MultiplayPassive && building->Type->CanBeOccupied)) {
                        if (building->Strength==building->Techno_Type_Class()->MaxStrength) {
                            Assign_Destination(0,true);
                            Scatter(building->Center_Coord(),true,true);
                            return;
                        }
                        building->unknown_slot233();
                    } else {
                        bool capturable=false;
                        if (building->Kind_Of()==6) capturable=building->Type->Capturable;
                        if (unknown_game_mode_a3d298!=0 && unknown_session_a3d298.unknown_34 && building->Health_Ratio()>Rules->unknown_1370) {
                            int max_damage=building->Strength-int(building->Techno_Type_Class()->MaxStrength*Rules->unknown_1370*0.5);
                            int damage=MIN(building->Techno_Type_Class()->MaxStrength*((1-Rules->unknown_1370/2)/2),max_damage);
                            building->Take_Damage(&damage,0,Rules->unknown_c74,this,true,0);
                        } else if (capturable) {
                            if (building->Mission==19) {
                                Assign_Destination(0,true);
                                Scatter(building->Center_Coord(),true,true);
                                return;
                            }
                            if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                            building->House->unknown_18b=true;
                            if (unknown_34 && unknown_34->Is_To_Inherit()) building->Attach_Tag(unknown_34);
                            building->unknown_slot222(House,1);
                            building->unknown_290=Type->ArrayIndex;
                            building->unknown_slot173();
                        }
                    }
                } else if (Type->Agent) building->Unknown_00452390(House);
                if (unknown_34) unknown_34->Spring(48,this,unknown_geometry_k,false,0);
                Delete_Me();
                return;
            }
            if (!unknown_494) {
                if (Mission!=11) unknown_slot261(false,true);
                if (Map[CoordStruct(Coord)].Cell_Building()) Scatter(unknown_geometry_l,true,false);
            }
        }
        TechnoClass *transport;
        if (Type->VehicleThief) {
            transport=static_cast<TechnoClass *>(static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Unit(unknown_79));
            if (!transport) transport=static_cast<TechnoClass *>(static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Aircraft(unknown_79));
            if (!transport) transport=static_cast<TechnoClass *>(static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Building());
        } else {
            transport=static_cast<TechnoClass *>(static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Building());
            if (!transport) transport=static_cast<TechnoClass *>(static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Aircraft(unknown_79));
            if (!transport) transport=static_cast<TechnoClass *>(static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Unit(unknown_79));
        }
        TechnoClass *at_destination=0;
        if (unknown_494 && unknown_494->Kind_Of()==11) {
            CellClass *destination_cell=&Map[unknown_494->Center_Coord()];
            if (Type->VehicleThief) {
                at_destination=static_cast<TechnoClass *>(destination_cell->Cell_Unit(true));
                if (!at_destination) at_destination=static_cast<TechnoClass *>(destination_cell->Cell_Aircraft(true));
                if (!at_destination) at_destination=static_cast<TechnoClass *>(destination_cell->Cell_Building());
            } else {
                at_destination=static_cast<TechnoClass *>(destination_cell->Cell_Building());
                if (!at_destination) at_destination=static_cast<TechnoClass *>(destination_cell->Cell_Aircraft(true));
                if (!at_destination) at_destination=static_cast<TechnoClass *>(destination_cell->Cell_Unit(true));
            }
        }
        if (Get_Mission()==7 && transport && (unknown_494==transport || unknown_228==transport || (at_destination && transport==at_destination))) {
            if (transport->unknown_34) transport->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
            if (transport->Kind_Of()==6) {
                if (transport==static_cast<CellClass *>(Get_Cell_Ptr())->Cell_Building() && unknown_slot149(21)==1) {
                    Limbo();
                    unknown_slot150(2,transport);
                    transport->Cargo.Attach(this);
                    unknown_4cd9b0();
                    return;
                }
            } else if (unknown_slot104()==transport->unknown_slot104() && transport==unknown_494) {
                if (unknown_slot150(15,transport)==1) {
                    unknown_6d6e50(0);
                    unknown_79=false;
                    Limbo();
                    static_cast<FootClass *>(transport)->Unknown_004CCE80(this);
                    Hidden();
                    return;
                }
                Assign_Destination(0,true);
                QueueMission(5,false);
                Scatter(unknown_geometry_l,true,true);
                return;
            }
        }
        if (Get_Mission()==17 && Type->C4) {
            BuildingClass *building=static_cast<BuildingClass *>(cell->Cell_Building());
            if (building && building==unknown_494) {
                if (building->unknown_34) building->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
                bool &going_to_blow=*reinterpret_cast<bool *>(reinterpret_cast<char *>(building)+0x5af);
                if (building->Get_Mission()!=19 && !building->unknown_slot203()) {
                    if (going_to_blow) {
                        Assign_Destination(0,true);
                        unknown_244=unknown_slot187(1);
                        Scatter(building->Center_Coord(),true,true);
                        return;
                    }
                    going_to_blow=true;
                    double warning_delay=Rules->unknown_13b8*900.0;
                    warning_delay*=0.5;
                    building->Clicked_As_Target(int(warning_delay));
                    *reinterpret_cast<CDTimerClass<FrameTimerClass> *>(reinterpret_cast<char *>(building)+0x420)=int(Rules->unknown_13b8*900.0);
                    *reinterpret_cast<AbstractClass **>(reinterpret_cast<char *>(building)+0x438)=this;
                }
                unknown_4cd9b0();
                unknown_slot252(false);
                unknown_244=unknown_slot187(1);
                Scatter(building->Center_Coord(),true,true);
                return;
            }
            if (&Map[Center_Coord()]==unknown_494) {
                Unknown_0047E2C0(CoordStruct(Coord),Rules->unknown_13a8,this,Rules->unknown_c74,true);
                unknown_slot291();
                Scatter(Adjacent_Cell(CoordStruct(Coord),Dir_To_8(unknown_2e0.Current())),true,true);
                QueueMission(2,false);
                if (!unknown_494 || Map[unknown_494->Center_Coord()].unknown_ec==2) Mark(1);
                Unknown_0047E2C0(CoordStruct(Coord),Rules->unknown_13a8,static_cast<TechnoClass *>(0),Rules->unknown_c74,true);
                Unknown_0047E2C0(CoordStruct(Coord),Rules->unknown_13a8,static_cast<TechnoClass *>(0),Rules->unknown_c74,true);
                if (!unknown_7d) return;
                Mark(1);
            }
        }
        if (unknown_370) {
            unknown_slot149(8);
            if (Type->VoiceComment.Count()>1) Voc_Play_At(Type->VoiceComment[1],CoordStruct(Coord),0);
            if (unknown_79) {
                if ((cell->unknown_128 & 0x1c)==0x1c) reinterpret_cast<UnknownCellClass *>(cell)->Unknown_00476920(&unknown_geometry_l,true,true,true);
            } else if ((cell->unknown_124 & 0x1c)==0x1c) reinterpret_cast<UnknownCellClass *>(cell)->Unknown_00476920(&unknown_geometry_l,true,true,false);
        }
        if (MissionQueue==-1 && !unknown_494 && !unknown_228 && !HasAnyLink()) unknown_slot261(false,true);
        Commence();
        unknown_slot263(false,false,false,0);
        unknown_slot262(false,false,false,0);
        Map.Unknown_0054A550(CoordStruct(Coord),0,unknown_1f4+3,0);
        CellClass *current=&Map[CoordStruct(Coord)];
        int land=current->unknown_ec;
        if (!Locomotor.Checked()->Is_Moving() && !Type->C4 && !unknown_6d7910(14) && (land==3 || land==2) && (!unknown_79 || !current->IsUnderBridge)) {
            int damage=Strength;
            Take_Damage(&damage,0,Rules->unknown_c74,0,true,0);
            return;
        }
    }
    if (unknown_7d) FootClass::unknown_slot93(why);
}
#endif

// void InfantryClass::Detach(AbstractClass *target, bool all) (0x00501CB0, slot 10)

void InfantryClass::Detach(AbstractClass *target, bool all)
{
    FootClass::Detach(target, all);
    if (Type == target) {
        Type = 0;
    }
}


// Assign an infantry destination, including transport contact and locomotor transitions.
extern "C" GUID const unknown_Walk_clsid_007a1fa0;
extern "C" GUID const CLSID_JumpjetLocomotionClass;
// RTTI-checked unit view, like the shared As_Building/As_Infantry operations.
class UnitClass;
inline UnitClass *As_Unit(AbstractClass *object)
{
    if (object==0) return 0;
    return object->Kind_Of()==1 ? (UnitClass *)object : 0;
}
void InfantryClass::Assign_Destination(AbstractClass *target, bool immediate)
{
    if (Owning_House()->Is_Human_Player()) {
        if (Sequence==27 || Sequence==28 || Sequence==29 || Sequence==30) return;
    }
    if (Type->JumpJet && target) {
        if (Cell(target->Center_Coord())==Cell(Center_Coord())) return;
    }
    if (Locomotor.Checked()->Is_Moving() && target &&
        Map[Center_Coord()].Is_Clear_To_Move(Type->SpeedType,true,false,-1,0,-1,true)) {
        if (Type->JumpJet || Mission!=1 || unknown_494!=target) unknown_slot291();
    }
    if (House->Is_Human_Player() && target && unknown_494==target &&
        unknown_5c3 && !Type->Fraidycat && !Type->Cyborg) unknown_slot311(7,false,false);
    if (Get_Mission()==7 || MissionQueue==7) {
        if (!HasAnyLink()) {
            TechnoClass *transport=target->As_Techno();
            if (transport) {
                if (transport->HasAnyLink()) {
                    if (transport->Kind_Of()==1) {
                        unknown_3f8=(AbstractClass *)As_Unit(target);
                        unknown_6d6e50(0);
                        target=0;
                    } else if (transport->Kind_Of()==6) {
                        unknown_3f8=As_Building((ObjectClass *)target);
                        unknown_6d6e50(0);
                        target=0;
                    } else unknown_6d6e50(target);
                } else {
                    if (unknown_slot150(2,transport)==1) {
                        int reply=unknown_slot149(14);
                        if (reply==14) return;
                        if (reply!=1) unknown_slot149(3);
                    }
                }
            }
        } else unknown_4d0=-1;
    } else unknown_4d0=-1;
    if (target && Type->JumpJet && Locomotor.Checked()->Is_Moving()) {
        UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist(Locomotor);
        GUID class_id;
        persist.Checked()->GetClassID(&class_id);
        if (memcmp(&class_id,&unknown_Walk_clsid_007a1fa0,sizeof(GUID))==0) {
            unknown_49c.Add_Head(target);
            target=Get_Target_Cell_Ptr();
            if (target && (static_cast<CellClass *>(target)->unknown_140&0x100)) target=0;
        }
    }
    if (Type->JumpJet && target && !Locomotor.Checked()->Is_Moving()) {
        bool should_fly=unknown_508dd0(&unknown_slot19(0).As_Cell(),&target->Center_Coord().As_Cell());
        bool is_jumpjet=unknown_508d00();
        if (Type->HoverAttack) should_fly=true;
        if (is_jumpjet) {
            if (!should_fly) {
                UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
                if (piggy && piggy.Checked()->Is_Piggybacking() && piggy.Checked()->Is_Ok_To_End()) { piggy.Checked()->End_Piggyback(&Locomotor); }
                UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> walk(unknown_Walk_clsid_007a1fa0);
                walk.Checked()->Link_To_Object(this);
                piggy=UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30>(walk);
                if (piggy) {
                    piggy.Checked()->Begin_Piggyback(Locomotor);
                    Locomotor=walk;
                }
            }
        } else if (should_fly) {
            UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
            if (piggy && piggy.Checked()->Is_Piggybacking() && piggy.Checked()->Is_Ok_To_End()) { piggy.Checked()->End_Piggyback(&Locomotor); }
        }
    }
    FootClass::Assign_Destination(target,immediate);
}


// void InfantryClass::unknown_slot220(AbstractClass *target) (0x00502470, slot 220)
// A new target while alive clears +0x57D and replays the idle sequence
// (28 deploying, 2 prone, else 0). Unless deploying without DeployFire:
// clears +0x4D0, TechnoClass's slot 220, and Infiltrate infantry without
// +0x494 aimed at a building whose type allows it (+0x1216 for non-Agents,
// +0x1219 for Agents) call slot 260 (target, 1); clears +0x57E.

void InfantryClass::unknown_slot220(AbstractClass *target)
{
    if (target != unknown_228 && Strength > 0) {
        unknown_57d = false;
        if (unknown_5090d0()) {
            unknown_slot311(28, 0, 0);
        } else if (unknown_5c3) {
            unknown_slot311(2, 0, 0);
        } else {
            unknown_slot311(0, 0, 0);
        }
    }
    if (!unknown_5090d0() || Type->DeployFire) {
        unknown_4d0 = -1;
        TechnoClass::unknown_slot220(target);
        if (unknown_494 == 0 && Type->Infiltrate) {
            AbstractClass *object = target;
            BuildingClass *building = (object != 0 && object->Kind_Of() == 6) ? (BuildingClass *)object : 0;
            if (!Type->Agent && building != 0 && building->Type->Capturable) {
                Assign_Destination(object, true);
            } else if (Type->Agent && building != 0 && building->Type->Spyable) {
                Assign_Destination(object, true);
            }
        }
        unknown_57e = false;
    }
}


// Runs infantry status, missions, reactions and animation updates (0x00502D20).
inline CoordStruct Infantry_Damage_Animation_Coord(CoordStruct const &coord)
{
    CoordStruct where = coord;
    where.X += 120;
    where.Y += 120;
    return where;
}
void InfantryClass::AI()
{
    if (unknown_574 >= 0) {
        unknown_5025c0();
        unknown_slot267(false);
        return;
    }
    if ((unknown_slot111() || unknown_slot112()) && Frame % 24 == 0 && Rules->unknown_2a8 != 0) {
        new AnimClass(Rules->unknown_2a8, Infantry_Damage_Animation_Coord(Coord), 0, 1, 0x600, 0);
    }
    if (unknown_20c != 0) {
        unknown_20c->AI();
    }
    if (unknown_slot112() || (unknown_slot111() && unknown_210)) {
        Locomotor.Checked()->Process();
    }
    if (unknown_slot111()) {
        if (unknown_228 != 0) {
            unknown_slot220(0);
        }
        if (unknown_494 != 0) {
            Assign_Destination(0, true);
        }
        return;
    }
    if (unknown_slot122()) {
        if (Get_Mission() == -1 && MissionQueue == -1) {
            unknown_slot261(false, true);
        }
        Commence();
    }
    if (Strength <= 0 && !Is_Busy_Sequence_00509710(Sequence)) {
        Strength = 1;
    }
    FootClass::AI();
    if (!unknown_7d) {
        return;
    }
    if (Get_Mission() == 5 || Get_Mission() == 11) {
        CoordStruct coord = Coord;
        BuildingClass *building = Map_Cell(coord)->Unknown_004717B0();
        if (building != 0 && !building->Type->InvisibleInGame) {
            if (building->Type->LaserFence) {
                if (building->unknown_4ec != 12 && building->unknown_4ec != 8) {
                    goto scatter;
                }
            } else if (building->Type->FirestormWall) {
                if (!building->House->unknown_142) {
                    goto scatter_done;
                }
                goto scatter;
            } else if (!building->Type->Gate || !building->unknown_44d880()) {
            scatter:
                Scatter(unknown_geometry_l, true, true);
            }
        }
    }
scatter_done:
    if (In_Which_Layer() != 2) {
        Mark(2);
    }
    if (unknown_57d && StageClass::Rate == 0) {
        unknown_57d = false;
        if (unknown_5090d0()) {
            unknown_slot311(28, false, false);
        } else {
            unknown_slot311(0, false, false);
        }
    }
    if (unknown_5070e0()) {
        return;
    }
    if (unknown_507170()) {
        return;
    }
    if (unknown_slot122()) {
        if (Get_Mission() == -1 && MissionQueue == -1) {
            unknown_slot261(false, true);
        }
        Commence();
    }
    unknown_506f30();
    if (unknown_494 == 0 && !unknown_5c3 && unknown_5c2 && unknown_5b0.Value() == 0) {
        unknown_5c2 = false;
    }
    Update_Fire_00507530();
    if (unknown_7d) {
        Advance_Animation_005078b0();
        if (unknown_7d) {
            Update_Movement_00507b90();
        }
    }
}


// Infantry cell-entry constraints and occupancy priority.
extern "C" int __cdecl abs(int);
TubeClass *__fastcall Cell_Tube_0047a130(CellClass const *cell);
// Four-byte occupancy storage; only the evidenced vehicle bit is interpreted.
struct CellOccupancyFlags_00503190 { unsigned int unknown_0:5; unsigned int vehicle:1; unsigned int unknown_6:26; };
int InfantryClass::Can_Enter_Cell(CellClass *cell,int direction,int height,CellClass *adjacent,bool unused)
{
    bool bridge=(cell->unknown_140&0x100) && (height==-1 || abs(height-cell->unknown_11b)>1);
    unsigned char flags=cell->unknown_124;
    int infantry_owner=cell->unknown_54;
    bool vehicle=reinterpret_cast<CellOccupancyFlags_00503190 const &>(cell->unknown_124).vehicle!=0;
    TubeClass *tube=Cell_Tube_0047a130(cell);
    if (direction==8) {
        if (tube && Cell(tube->ExitCell)!=Cell(tube->EnterCell)) return 0;
        return 7;
    }
    if (tube) {
        int angle=abs(direction-tube->Facing);
        if (angle>2 && angle<6 && direction!=-1) return 7;
    }
    unsigned opposite=(direction-4)&7;
    TubeClass *other_tube=Cell_Tube_0047a130(&cell->Adjacent_Cell(opposite));
    if (other_tube) {
        int angle=abs(opposite-other_tube->Facing);
        if (angle>2 && angle<6 && direction!=-1) return 7;
    }
    if (height-cell->unknown_11b>4) return 0;
    if (unknown_slot102(reinterpret_cast<UnknownCellClass*>(cell),direction,&height,&bridge,reinterpret_cast<UnknownCellClass*>(adjacent))==7) return 7;
    if (height!=-1 && (cell->unknown_140&0x100) && height==cell->unknown_11b+4) {
        flags=cell->unknown_128;
        infantry_owner=cell->unknown_58;
        vehicle=reinterpret_cast<CellOccupancyFlags_00503190 const &>(cell->unknown_128).vehicle!=0;
    }
    if (unknown_32d && !unknown_a40774 && !Map.Unknown_0055A740(cell,true) && !unknown_slot189()) return 7;
    int result=0;
    if (cell->OverlayIndex!=-1) {
        OverlayTypeClass *overlay=OverlayTypes[cell->OverlayIndex];
        if (overlay->Crate && !House->Is_Human_Player()) return 7;
        if (overlay->Wall && (cell->OverlayData>>4)!=overlay->DamageLevels) {
            if (!unknown_slot163() || !unknown_slot231(0)->WeaponType->Is_Wall_Destroyer()) return 7;
            result=House->Is_Ally(cell->unknown_50) ? 4 : 5;
        }
    }
    int stationary_infantry=0;
    ObjectClass *occupant=bridge ? cell->AltOccupierPtr : cell->OccupierPtr;
    while (occupant) {
        if (occupant!=this) {
            if (unknown_588 && unknown_588->unknown_28) {
                if (occupant->unknown_slot104()==unknown_588->unknown_28->unknown_slot104()) return 0;
            }
            if (Get_Mission()==7 || Get_Mission()==8 || Get_Mission()==9 ||
                (Get_Mission()==17 && Type->C4) ||
                ((Get_Mission()==11 || Get_Mission()==25 || Get_Mission()==5) && Type->Engineer)) {
                if (occupant==unknown_494 || reinterpret_cast<AbstractClass*>(&Map[CoordStruct(occupant->Coord)])==unknown_494 || occupant==unknown_228) {
                    if (!occupant->unknown_slot83()) {
                        float cost=Ground[cell->unknown_ec].Cost[Type->SpeedType];
                        bool unrestricted=unknown_370 || bridge;
                        if (!unrestricted && cost==0) return 7;
                        return 0;
                    }
                    return 7;
                }
                ObjectClass *building=cell->Cell_Building();
                if (building && occupant!=building) {
                    if (building==unknown_494 || reinterpret_cast<AbstractClass*>(&Map[building->Center_Coord()])==unknown_494 || building==unknown_228) {
                        occupant=occupant->unknown_30;
                        continue;
                    }
                }
            }
            if (Get_Mission()==11 && unknown_1b0==occupant && unknown_1b0->Kind_Of()==1) return 7;
            if (Type->VehicleThief && occupant->unknown_slot31() && unknown_494==occupant && !occupant->Techno_Type_Class()->IsTrain) return 0;
            if (occupant->Kind_Of()==6) {
                BuildingClass *building=static_cast<BuildingClass*>(occupant);
                if (building->Type->InvisibleInGame || (building->Type->LaserFence && (building->unknown_4ec==12 || building->unknown_4ec==8))) {
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (building->Type->FirestormWall) {
                    if (building->House->unknown_142) return 7;
                    occupant=occupant->unknown_30;
                    continue;
                }
                if (building->Type->Gate) {
                    if (!building->unknown_44d880()) {
                        if (building->House->Is_Ally(House)) { if (result<3) result=3; }
                        else {
                            if (!unknown_slot163()) return 7;
                            if (result<5) result=5;
                        }
                    }
                    occupant=occupant->unknown_30;
                    continue;
                }
            }
            if (Get_Mission()==7 && occupant==unknown_494 && unknown_370) return 0;
            if (House->Is_Ally(static_cast<TechnoClass*>(occupant)) || unknown_a40774) {
                switch (occupant->Kind_Of()) {
                case 1: {
                    FootClass *unit=static_cast<FootClass*>(occupant);
                    if (unit->Locomotor.Checked()->Is_Moving() || unit->unknown_494) {
                        if (unit->unknown_5a0 || unit->Locomotor.Checked()->Will_Jump_Tracks()) { if (result<2) result=2; }
                    } else if (result<6) result=6;
                    break;
                }
                case 2:
                case 6: return 7;
                case 15:
                    if (!static_cast<FootClass*>(occupant)->Locomotor.Checked()->Is_Moving()) ++stationary_infantry;
                    break;
                default: break;
                }
            } else {
                TechnoClass *techno=occupant->As_Techno();
                if (!techno || techno->unknown_1b8!=2) {
                    int kind=occupant->Kind_Of();
                    if (unknown_6c16f0(-1)<=0 && kind!=36) return 7;
                    switch (kind) {
                    case 36: break;
                    case 15:
                        if (occupant->unknown_slot49(House)) result=6;
                        break;
                    case 6:
                        if (static_cast<BuildingClass*>(occupant)->Type->BridgeRepairHut) return 7;
                        if (result<5) result=5;
                        break;
                    default:
                        if (result<5) result=5;
                        break;
                    }
                } else if (result<1) result=1;
            }
        }
        occupant=occupant->unknown_30;
    }
    if (!unknown_370 && !bridge && Ground[cell->unknown_ec].Cost[Type->SpeedType]==0) return 7;
    if (!result && vehicle) return 2;
    if (infantry_owner!=-1) {
        if (House->Is_Ally(infantry_owner)) {
            if ((flags&0x1c)==0x1c && result<2) {
                if (stationary_infantry==3) result=6;
                else result=2;
            }
        } else {
            if (unknown_6c16f0(-1)>0) {
                if (result<5) result=5;
            } else return 7;
        }
    }
    if (!result && (flags&0x1c)==0x1c) return 7;
    return result;
}


// Not exact (RA2_NON_MATCHING): InfantryClass::unknown_5025c0 at 0x005025C0, best score 45.42, 2026-11-13; differs: 560 instructions in the original, 554 in the draft (shape 87.07)
#ifdef RA2_NON_MATCHING
// Advances underground Infantry through a tube and releases it at the exit (0x005025C0).
#include "shared/selection_coord.hpp"
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);
// Already accepted kept copies; full73-byte bodies and7NOP padding independently rechecked in r363. Zero new credit.

extern CoordStruct unknown_coord_a410f8;   // 0x00A410F8
extern double const unknown_tube_angle_scale_79b620;
extern double const unknown_tube_diagonal_7a3a20;
extern double const unknown_tube_width_79a700;

// The coordinate moved distance leptons toward direction (inline, defined further down in this unit).
inline CoordStruct Move_Coord(CoordStruct const &start, UnknownFacingDir const &direction, int distance);
// A coordinate moved by a planar offset (TibSun point.h TPoint3D + TPoint2D).
inline CoordStruct operator+(CoordStruct const &coord, TPoint2D<int> const &offset)
{
    return CoordStruct(coord.X + offset.X, coord.Y + offset.Y, coord.Z);
}

// TibSun InfantryClass::Tunnel_AI: walks the soldier along its tube and sets it down at the exit.
void InfantryClass::unknown_5025c0()
{
    bool finished = unknown_TubeClass_list_008661c8[unknown_574]->Directions[unknown_575] == -1;
    if (!finished) {
        int distance = Distance(unknown_45c);
        TubeClass *tube = unknown_TubeClass_list_008661c8[unknown_574];
        UnknownFacingDir facing;
        UnknownFacingDir direction = facing.Direction(Center_Coord(), unknown_45c);
        int speed = Type->Speed;
        int height_step;
        {
            Cell tube_cell = tube->ExitCell;
            CoordStruct exit_coord(tube_cell);
            tube_cell = tube->EnterCell;
            CoordStruct enter_coord(tube_cell);
            height_step = tube->unknown_1c0;
            height_step = (Map.Get_Height_GL(exit_coord) - Map.Get_Height_GL(enter_coord)) / height_step;
        }
        if (distance <= speed) {
            unknown_575++;
            int tube_dir = tube->Directions[unknown_575];
            finished = tube_dir == -1;
            Set_Coord(unknown_45c);
            if (!finished) {
                CoordStruct coord = Adjacent_Cell(unknown_45c, tube->Directions[unknown_575]);
                unknown_45c = unknown_45c + (reinterpret_cast<TPoint2D<int> const &>(Map[coord].unknown_475d60()) - reinterpret_cast<TPoint2D<int> const &>(Map[unknown_45c].unknown_475d60()));
                unknown_45c.Z += height_step;
                UnknownFacingDir dir = UnknownFacingDir().Direction(Center_Coord(), unknown_45c);
                CoordStruct new_coord = Move_Coord(Object_Coord(this), dir, speed - distance);
                double divisor;
                if (tube_dir % 2) {
                    divisor = unknown_tube_diagonal_7a3a20;
                } else {
                    divisor = unknown_tube_width_79a700;
                }
                new_coord.Z += (speed - distance) / divisor * height_step;
                Set_Coord(new_coord);
                return;
            }
        } else {
            int tube_dir = tube->Directions[unknown_575];
            CoordStruct new_coord = Move_Coord(Object_Coord(this), direction, speed);
            double divisor;
            if (tube_dir % 2) {
                divisor = unknown_tube_diagonal_7a3a20;
            } else {
                divisor = unknown_tube_width_79a700;
            }
            new_coord.Z += speed / divisor * height_step;
            Set_Coord(new_coord);
            return;
        }
    }
    if (finished) {
        CellClass *cell = &Map[Object_Coord(this)];
        ObjectClass *occupier = cell->OccupierPtr;
        // Existing slot101 uses a 32-bit int placeholder for its cell pointer.
        if (Can_Enter_Cell(cell, -1, 0, 0, 1)) {
            DynamicVectorClass<ObjectClass *> objects;
            for (; occupier != 0; occupier = occupier->unknown_30) {
                objects.Add(occupier);
            }
            for (int i = 0; i < objects.Count(); ++i) {
                ObjectClass *object = objects[i];
                if (object->Kind_Of() == 1 || object->Kind_Of() == 15) {
                    FootClass *unit = static_cast<FootClass *>(object);
                    if (!unit->Locomotor.Checked()->Is_Moving()) {
                        unit->Scatter(CoordStruct(0, 0, 0), true, true);
                    }
                }
            }
            unknown_slot306(0.0);
        } else {
            CoordStruct spot = reinterpret_cast<UnknownCellClass *>(cell)->Unknown_004764B0(Object_Coord(this), 0, false);
            spot.Z = Map.Get_Height_GL(spot);
            Set_Coord(spot);
            Locomotor.Checked()->Force_Immediate_Destination(unknown_coord_a410f8);
            Locomotor.Checked()->Stop_Movement_Animation();
            Mark(1);
            Set_Occupy_Bit(Object_Coord(this));
            if (unknown_494 == cell) {
                Scatter(CoordStruct(0, 0, 0), true, true);
            } else {
                unknown_slot306(1.0);
            }
            unknown_574 = -1;
            unknown_57b = true;
            unknown_slot93(2);
        }
    }
}
#endif

// int InfantryClass::unknown_slot218(AbstractClass *target, int weapon, bool check_range) (0x00503A60, slot 218)
// Fire check: 6 during sequences 11-15, 20, 21 and 34-36; when 0x006C16F0
// (-1) is negative only infantry targets below the rules health ratio at
// +0x1360 are allowed (else 5). Then TechnoClass's answer; when that allows
// firing: 7 while +0x468 exceeds 0.1, or with +0x494 in an uninterruptible
// sequence or with a weapon flagged +0x105; 5 for weapons flagged +0x126 at
// anything but slot 105's object, and for IvanBomb warheads at objects
// already carrying a bomb; otherwise the locomotor's slot 35 when non-zero.

int InfantryClass::unknown_slot218(AbstractClass *target, int weapon, bool check_range)
{
    if (Is_Busy_Sequence(Sequence)) {
        return 6;
    }
    if (unknown_6c16f0(-1) < 0) {
        if (target == 0 || target->Kind_Of() != 15
            || !(((ObjectClass *)target)->Health_Ratio() < Rules->unknown_1360)) {
            return 5;
        }
    }
    int result = TechnoClass::unknown_slot218(target, weapon, check_range);
    if (result == 0) {
        if (unknown_468 > 0.1
            || (unknown_494 != 0 && Sequence != -1 && !unknown_do_controls_7a3474[Sequence].Interrupt)) {
            return 7;
        }
        WeaponTypeClass *weapon_type = unknown_slot231(weapon)->WeaponType;
        if (weapon_type != 0) {
            if (weapon_type->unknown_105 && unknown_494 != 0) {
                return 7;
            }
            if (weapon_type->unknown_126 && target != Get_Cell_Ptr()) {
                return 5;
            }
            WarheadTypeClass *warhead = weapon_type->Warhead;
            if (warhead != 0 && warhead->IvanBomb && target != 0 && ((ObjectClass *)target)->AbstractFlags.IsObject
                && ((ObjectClass *)target)->unknown_38 != 0) {
                return 5;
            }
        }
        int state = Locomotor.Checked()->State();
        if (state != 0) {
            return state;
        }
    }
    return result;
}


// bool InfantryClass::unknown_slot261(bool arg1, bool arg2) (0x00503C00, slot 261)
// FootClass's answer; unless slot 270 holds or the mission is 28, chooses
// a follow-up mission: with a target 1 (keeping 17 or 8), with +0x494 2
// (keeping 8 or 17); otherwise (not under missions 5 or 11 or a mission
// whose control has +0x05/+0x07; false then) 11 or 5 by owner, team, house
// +0x194 against the rules' +0x10A8, slot 163, Engineer, VehicleThief,
// 0x006D7910 (16) and DefaultToGuardArea. Queues it unless the current
// mission is 25 or 11.

bool InfantryClass::unknown_slot261(bool arg1, bool arg2)
{
    bool result = FootClass::unknown_slot261(arg1, arg2);
    if (unknown_slot270()) {
        return result;
    }
    if (Get_Mission() == 28) {
        return result;
    }
    int mission;
    if (unknown_228 != 0) {
        mission = 1;
        if (Get_Mission() == 17) {
            mission = 17;
        }
        if (Get_Mission() == 8) {
            mission = 8;
        }
    } else {
        unknown_4c8b00();
        if (unknown_494 != 0) {
            mission = 2;
            if (Get_Mission() == 8) {
                mission = 8;
            }
            if (Get_Mission() == 17) {
                mission = 17;
            }
        } else {
            if (Get_Mission() == 5 || Get_Mission() == 11) {
                return false;
            }
            if (Get_Mission() != -1 && (Control()->unknown_05 || Control()->unknown_07)) {
                return false;
            }
            if (!House->Unknown_004F52F0() && Team == 0) {
                if (House->unknown_194 < Rules->unknown_10a8 && Mission != 11) {
                    mission = 5;
                } else if (!unknown_slot163() && !Type->Engineer && !Type->VehicleThief) {
                    mission = 5;
                } else {
                    mission = 11;
                }
            } else if (Mission != 11 && ((!unknown_6d7910(16) && !Type->DefaultToGuardArea) || Team != 0)) {
                mission = 5;
            } else {
                mission = 11;
            }
        }
    }
    if (Mission != 25 && Mission != 11 && mission != -1) {
        QueueMission(mission, false);
    }
    return result;
}


// Schedules and plays idle infantry/cow actions (0x00503E00).
// Full independent extent includes the ten-entry switch table: 800 bytes.
inline double Infantry_Idle_Interval(double low, double high)
{
    int draw = unknown_id_source_a3d290->Random()(0, 0x7ffffffe);
    return (high - low) * ((double)draw * (1.0 / 2147483646.0)) + low;
}
inline bool Infantry_Play_Idle_Action(InfantryClass *infantry)
{
    int action = unknown_id_source_a3d290->Random()(0, 10);
    bool is_cow = infantry->unknown_func_5d27d0("COW");
    if (is_cow && unknown_id_source_a3d290->Random()(0, 10) < 5) {
        action = 8;
    }
    switch (action) {
    case 3: case 4: case 5:
        infantry->unknown_slot311(9, false, false);
        return true;
    case 6: {
        UnknownFacingDir facing((unsigned short)(unknown_id_source_a3d290->Random()(0,7)<<13));
        infantry->unknown_2e0.Set(UnknownFacingDir((unsigned short)(Dir_To_256(facing) << 8)));
        return true;
    }
    case 1: case 2: case 7:
        infantry->unknown_slot311(10, false, false);
        if (!infantry->unknown_77 && infantry->House->Is_Player_Control() && infantry->Type->VoiceComment.Count() > 0
            && unknown_random_839218(0, 2) == 0) {
            CoordStruct coord = infantry->Coord;
            Voc_Play_At(infantry->Type->VoiceComment[0], coord, 0);
            return true;
        }
        break;
    case 8: {
        UnknownFacingDir facing((unsigned short)(unknown_id_source_a3d290->Random()(0,7)<<13));
        infantry->unknown_2e0.Set(UnknownFacingDir((unsigned short)(Dir_To_256(facing) << 8)));
        if (is_cow || (!infantry->House->Unknown_004F52F0() && infantry->Type->Fraidycat)) {
            infantry->Scatter(unknown_geometry_l, true, false);
            return true;
        }
        break;
    }
    case 9: case 10: {
        UnknownFacingDir facing((unsigned short)(unknown_id_source_a3d290->Random()(0,7)<<13));
        infantry->unknown_2e0.Set(UnknownFacingDir((unsigned short)(Dir_To_256(facing) << 8)));
        break;
    }
    }
    return true;
}
bool InfantryClass::unknown_slot258()
{
    if (unknown_slot257()) {
        // Rules +0x1378 is read as a double; the current shared header pads it.
        unknown_130 = CDTimerClass<FrameTimerClass>((long)Infantry_Idle_Interval(
            Rules->IdleActionFrequency * 450.0,
            Rules->IdleActionFrequency * 1800.0));
        if (Type->Fraidycat && !House->Unknown_004F52F0() && unknown_5bc > 50) {
            Scatter(unknown_geometry_l, true, false);
            return true;
        }
        return Infantry_Play_Idle_Action(this);
    }
    return false;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void InfantryClass::Scatter(CoordStruct const &threat, bool forced, bool nokidding) -- 0x00504120
// TibSun/YR InfantryClass::Scatter: scatters the infantry away from a threat, or out of its cell when there is none.
// Head from ra2-decomp (YR 0x0051D0D0): the threat test through operator!=, the direction through the file-local
// Coord_Direction on a copy of Coord, and the cell fraction through Cell_Fraction. Direct position.X/.Y reads there
// let VC6 fold the height product into its operand register (imul edi,[g]) and flip the Adjacent_Cell sum.

// The direction from one point to another (Westwood's Direction(Point2D, Point2D), by value).
inline UnknownFacingDir Direction_00504120(Point2D const &from, Point2D const &to)
{
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to.X)-from.X)-1.5707963267948966)*-10430.060040584269));
}

// Infantry.CPP file-local helpers (ra2-decomp src/infantry.cpp, the YR twin 0x0051D0D0): the lepton
// fraction of a coordinate inside its cell and the direction between two coordinates.
static inline Point2D Cell_Fraction(CoordStruct const &x, CoordStruct const &y)
{
    return Point2D(x.X & 255, y.Y & 255);
}

static inline UnknownFacingDir Coord_Direction(CoordStruct const &coord1, CoordStruct const &coord2)
{
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(coord1.Y)-coord2.Y,double(coord2.X)-coord1.X)-1.5707963267948966)*-10430.060040584269));
}

void InfantryClass::Scatter(CoordStruct const &threat, bool forced, bool nokidding)
{
    if ((Sequence==27 || Sequence==28 || Sequence==29 || Sequence==30) && forced && nokidding) {
        unknown_slot311(31,false,false);
    } else if (House->Is_Human_Player()) {
        if (Sequence==27 || Sequence==28 || Sequence==29 || Sequence==30) return;
    }
    if (Locomotor.Checked()->Is_Moving()) forced=false;
    if (!Control()->unknown_09 && !forced) return;
    if (!Type->Fraidycat && unknown_228 && !forced) return;
    if (Sequence!=-1 && Sequence!=31 && !unknown_do_controls_7a3474[Sequence].Interrupt) return;
    if (!Rules->unknown_1431 && !unknown_6d7910(3) && !nokidding && House->Is_Human_Player() && !forced && !Team) return;
    if (forced || Type->Fraidycat) {
        int facing;
        if (threat!=unknown_geometry_l) {
            CoordStruct position=Coord;
            facing=Dir_To_8(Coord_Direction(threat,position));
            facing=facing+unknown_id_source_a3d290->Random()(0,4)-2;
        } else {
            Point2D position=Cell_Fraction(Center_Coord(),Center_Coord());
            if (position!=Point2D(128,128)) facing=Dir_To_8(Direction_00504120(Point2D(128,128),position));
            else facing=Dir_To_8(unknown_2e0.Current());
            facing=facing+unknown_id_source_a3d290->Random()(0,4)-2;
            Cell nearby=Map.Nearby_Location(Cell(unknown_slot19(0)),Type->SpeedType,-1,0,unknown_79,Point2D(1,1),0,1,0,1,Cell(0,0),0);
            if (nearby!=unknown_geometry_k) {
                Assign_Destination(&Map[nearby],true);
                Locomotor.Checked()->Process();
                return;
            }
        }
        Cell alternate(0,0);
        Cell best(0,0);
        Cell origin=Cell(unknown_slot19(0));
        CellClass *origin_cell=&Map[origin];
        int height=origin_cell->unknown_11b+Is_Moving_Onto_Bridge(0) ? 4 : 0;
        CoordStruct destination=unknown_slot19(0);
        destination.Z=height*unknown_geometry_g;
        for (int index=0;index<8;++index) {
            int direction=(facing+index)&7;
            Cell check=Adjacent_Cell(origin,direction);
            CellClass *candidate=&Map[check];
            if (Map.Unknown_0055A660(check,true) && Can_Enter_Cell(candidate,direction,Unknown_005D47D0(),0,1)==0) {
                if (alternate==unknown_geometry_k) alternate=check;
                if (unknown_geometry_k==Cell(0,0)) {
                    CoordStruct projected=Cell_Coord(check);
                    projected.Z=destination.Z;
                    if (check==unknown_tactical_83996c->Coord_To_Cell(projected) && !Map[check].IsUnderBridge) {
                        best=check;
                        break;
                    }
                }
            }
        }
        Cell chosen=best;
        if (chosen==unknown_geometry_k) chosen=alternate;
        if (chosen!=unknown_geometry_k) {
            QueueMission(2,false);
            Assign_Destination(&Map[chosen],true);
        }
    }
}


// Selects an infantry animation sequence, sound, rate and start frame (0x00504740).
bool InfantryClass::unknown_slot311(int sequence, bool force, bool random_start)
{
    if (sequence == -1 || Type->DoControls[sequence].Count == 0) {
        return false;
    }
    if (Sequence == 33 && unknown_7a) {
        return false;
    }
    if (Type->MovementZone == 3) {
        UnknownCellClass *cell = Map_Cell_At(unknown_slot104());
        int on_land = 1;
        if ((cell->unknown_ec == 6 || cell->unknown_ec == 2) && !unknown_79) {
            on_land = 0;
            if (sequence == 3 || sequence == 6) sequence = 17;
            else if (sequence == 2 || sequence == 0) sequence = 16;
            else if (sequence == 9) sequence = 18;
            else if (sequence == 10) sequence = 19;
            else if (sequence == 11) sequence = 20;
            else if (sequence == 12) sequence = 21;
            else if (sequence == 4 || sequence == 8) sequence = 22;
        }
        if (unknown_5d0 == 0 && on_land == 1) {
            CoordStruct coord = Coord;
            Voc_Play_At(Type->LeaveWaterSound, coord, 0);
        } else if (unknown_5d0 == 1 && on_land == 0) {
            CoordStruct coord = Coord;
            Voc_Play_At(Type->EnterWaterSound, coord, 0);
        }
        unknown_5d0 = on_land;
    }
    if (In_Air() && !unknown_79 && Type->DoControls[23].Frame > 0 && sequence == 0) {
        sequence = 23;
    } else if (sequence == 3 && unknown_5bc >= 200) {
        sequence = 37;
    }
    if (sequence == Sequence || (Sequence != -1 && !force && !unknown_do_controls_7a3474[Sequence].Interrupt)) {
        return false;
    }
    if (sequence == 27) {
        if (Techno_Type_Class()->DeploySound != -1) {
            CoordStruct coord = Coord;
            Voc_Play_At(Techno_Type_Class()->DeploySound, coord, 0);
        }
    } else if (sequence == 31 && Techno_Type_Class()->UndeploySound != -1) {
        CoordStruct coord = Coord;
        Voc_Play_At(Techno_Type_Class()->UndeploySound, coord, 0);
    }
    Sequence = sequence;
    if (Is_Prone_Sequence_005096e0(sequence)) {
        int rate = Options.Normalize_Delay(unknown_do_controls_7a3474[sequence].Rate);
        StageClass::Set_Rate(rate);
    } else {
        int rate = unknown_do_controls_7a3474[sequence].Rate;
        StageClass::Set_Rate(rate);
    }
    if (random_start) {
        int count = Type->DoControls[sequence].Count;
        if (count <= 1) count = 1;
        StageClass::Stage = unknown_id_source_a3d290->Random()(0, count - 1);
    } else {
        StageClass::Stage = 0;
    }
    if (Strength == 0) {
        unknown_slot291();
    }
    switch (sequence) {
    case 5:
        unknown_5c3 = true;
        break;
    case 7:
    case 27:
        unknown_5c3 = false;
        return true;
    }
    return true;
}


// bool InfantryClass::unknown_slot291() (0x00504AE0, slot 291)
// Plays slot 311 (28 while deploying (sequences 27-30), 2 when prone,
// else 0), records in +0x5C4 whether slot 101 reports the current cell
// occupied in the eight-way facing, then FootClass's slot 291.

bool InfantryClass::unknown_slot291()
{
    if (unknown_5090d0()) {
        unknown_slot311(28, 0, 0);
    } else if (unknown_5c3) {
        unknown_slot311(2, 0, 0);
    } else {
        unknown_slot311(0, 0, 0);
    }
    int facing = Dir_To_8(unknown_2e0.Current());
    CoordStruct coord = Coord;
    if (Can_Enter_Cell(&Map[coord], facing, Unknown_005D47D0(), 0, 1) == 0) {
        unknown_5c4 = false;
    } else {
        unknown_5c4 = true;
    }
    return FootClass::unknown_slot291();
}


// Start an infantry move at a free subcell and transfer its occupancy reservation.
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
extern double const unknown_facing_radians_79b620;
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);
inline double UnknownFacingDir::As_Radian() const
{
    return (short(Raw)-0x3fff)*unknown_facing_radians_79b620;
}
// Inline direction operations; member-versus-free spelling is unproved for RA2.
inline UnknownFacingDir Direction_00504bc0(CoordStruct const &from, CoordStruct const &to)
{
    UnknownFacingDir direction;
    if (from.Equal_XY_004bfb60(to)) direction.Raw=0;
    direction=UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,double(to.X)-from.X)-unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
    return direction;
}
inline UnknownFacingDir operator+(UnknownFacingDir const &left, UnknownFacingDir const &right)
{
    return UnknownFacingDir(left.Raw+right.Raw);
}
// A view of the evidenced four-byte cell flags; bridge is bit 8.
struct CellBridgeFlags_00504bc0 {
    unsigned unknown_low : 8;
    unsigned under_bridge : 1;
    unsigned unknown_high : 23;
};
inline CoordStruct Move_Coord(CoordStruct const &start, UnknownFacingDir const &direction, int distance)
{
    double angle=direction.As_Radian();
    return CoordStruct(int(start.X+unknown_cos_4ba4d0(angle)*distance),int(start.Y-unknown_sin_4ba480(angle)*distance),start.Z);
}
bool InfantryClass::unknown_slot290(CoordStruct &headto)
{
    CoordStruct old=headto;
    headto=reinterpret_cast<UnknownCellClass &>(Map[headto]).Unknown_004764B0(Move_Coord(headto,Direction_00504bc0(Center_Coord(),headto)+UnknownFacingDir(0x7fff),124),false,reinterpret_cast<CellBridgeFlags_00504bc0 const &>(Map[headto].unknown_140).under_bridge);
    if (headto==unknown_geometry_l && Can_Enter_Cell(&Map[old],-1,-1,0,1)==0) {
        headto=reinterpret_cast<UnknownCellClass &>(Map[old]).Unknown_004764B0(Move_Coord(old,Direction_00504bc0(Center_Coord(),headto)+UnknownFacingDir(0x7fff),128),true,reinterpret_cast<CellBridgeFlags_00504bc0 const &>(Map[old].unknown_140).under_bridge);
    }
    if (headto!=unknown_geometry_l && FootClass::unknown_slot290(headto)) {
        if (!unknown_7d) return false;
        Clear_Occupy_Bit(CoordStruct(Coord));
        Set_Occupy_Bit(headto);
        return true;
    }
    return false;
}


// bool InfantryClass::Limbo() (0x00504F00, slot 52)
// Stops the locomotor's animation, resets the infantry state (+0x5D0 2,
// +0x5C3, Sequence 0) and continues with FootClass's slot 52.

bool InfantryClass::Limbo()
{
    Locomotor.Checked()->Stop_Movement_Animation();
    unknown_5d0 = 2;
    unknown_5c3 = false;
    Sequence = 0;
    return FootClass::Limbo();
}


// BulletClass *InfantryClass::unknown_slot221(AbstractClass *target, int arg2) (0x00504F50, slot 221)
// TechnoClass's (after clearing +0x57D); when that is set, a Fraidycat
// without ammunition and not +0x76 starts fearing (+0x5BC 300) and leaves
// missions 1 and 15 for 5.

BulletClass *InfantryClass::unknown_slot221(AbstractClass *target, int arg2)
{
    unknown_57d = false;
    BulletClass *result = TechnoClass::unknown_slot221(target, arg2);
    if (result != 0 && !unknown_76 && Type->Fraidycat && unknown_254 == 0) {
        unknown_5bc = 300;
        if (Get_Mission() == 1 || Get_Mission() == 15) {
            QueueMission(5, false);
        }
    }
    return result;
}


// bool InfantryClass::Unlimbo(CoordStruct const *coord, int dir) (0x00504FD0, slot 53)
// Puts the infantry on the map: on the ground it moves to a free spot of
// the cell (alternate spots when 0x00A40774 is set or 0x0055A660 refuses the
// cell) and fails when there is none. After FootClass's slot 53, types
// without Sight clear +0x373, the spot below bridge height is occupied
// (slot 58) and +0x5D0 becomes 2.

bool InfantryClass::Unlimbo(CoordStruct const *coord, int dir)
{
    int ground = Map.Get_Height_GL(*coord);
    CoordStruct where = *coord;
    if (where.Z == ground) {
        int alternate = unknown_a40774 != 0 || !Map.Unknown_0055A660(Coord_Cell(*coord), true);
        where = Map_Cell(*coord)->Unknown_004764B0(*coord, alternate, false);
        if (where.X == unknown_geometry_l.X && where.Y == unknown_geometry_l.Y
            && where.Z == unknown_geometry_l.Z) {
            return false;
        }
        where.Z = ground;
    }
    if (FootClass::Unlimbo(&where, dir)) {
        if (Type->Sight == 0) {
            unknown_373 = false;
        }
        if (where.Z <= unknown_geometry_j + ground) {
            Set_Occupy_Bit(where);
        }
        unknown_5d0 = 2;
        return true;
    }
    return false;
}


// AbstractClass *InfantryClass::unknown_slot219(int threat, CoordStruct const *coord, bool arg3) (0x00505120, slot 219)
// Threat search: engineers of owners failing HouseClass 0x004F52F0 take the
// house's +0x5410 object within 0xF00 leptons (else add 0x200); vehicle
// thieves take an active +0x494 (slot 31, not IsTrain) within 0xF00. Then
// adjusts the threat flags (Infiltrate/VehicleThief, the weapons'
// projectile flags, warhead +0x148, C4, Thief) for FootClass's slot 219.

// The length of a coordinate offset, squared in X, Y, Z order (this shape;
// foot.hpp's Coord_Length converts Z first).
inline int Coord_Length_XYZ(CoordStruct const &coord)
{
    return (int)Fast_Sqrt((double)coord.X * coord.X + (double)coord.Y * coord.Y + (double)coord.Z * coord.Z);
}

AbstractClass *InfantryClass::unknown_slot219(int threat, CoordStruct const *coord, bool arg3)
{
    if (!House->Unknown_004F52F0() && Type->Engineer) {
        ObjectClass *object = (ObjectClass *)House->unknown_5410;
        if (object != 0) {
            if (Coord_Length_XYZ(Center_Coord() - object->Center_Coord()) < 0xF00) {
                return (AbstractClass *)House->unknown_5410;
            }
        }
        threat |= 0x200;
    }
    if (Type->VehicleThief) {
        AbstractClass *thief_target = unknown_494;
        bool active;
        if (thief_target != 0) {
            active = thief_target->AbstractFlags.IsTechno;
        }
        if (thief_target != 0 && active && ((TechnoClass *)unknown_494)->unknown_slot31()
        && !((TechnoClass *)unknown_494)->Techno_Type_Class()->IsTrain && unknown_5d4c00(unknown_494) < 0xF00) {
            return unknown_494;
        }
    }
    if (!unknown_slot163()) {
        if (!Type->Infiltrate && !Type->VehicleThief) {
            return 0;
        }
        if (Type->VehicleThief) {
            threat = (threat & ~0xA4) | 0x10;
        }
    }
    if (!(threat & 0x1B978)) {
        if (unknown_slot231(0)->WeaponType != 0) {
            threat |= unknown_slot231(0)->WeaponType->Allowed_Threats();
        }
        if (unknown_slot231(1)->WeaponType != 0) {
            threat |= unknown_slot231(1)->WeaponType->Allowed_Threats();
        }
    }
    if (unknown_slot163() && unknown_slot231(0)->WeaponType->Warhead->unknown_148) {
        threat &= ~0xB4;
    }
    if ((Type->C4 || unknown_6d7910(14)) && !House->Unknown_004F52F0()) {
        threat |= 0x20;
    }
    if (Type->Thief) {
        threat |= 0x240;
    }
    return FootClass::unknown_slot219(threat, coord, arg3);
}


// Infantry object action selection (0x00505390, virtual slot 28).
// Action numbers are retained until their complete mapping is evidenced.
// Original RTTI-kind checked conversion: kinds 1, 2, 6 and 15, all TechnoClass descendants.
TechnoClass *__fastcall As_Techno_0040dbd0(AbstractClass *object);
unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action);

inline Cell Infantry_Add_Cells(Cell const & a, Cell const & b)
{
    Cell sum;
    sum.X = a.X + b.X;
    sum.Y = a.Y + b.Y;
    return sum;
}

inline bool Infantry_Key_Down(int key)
{
    return WWKeyboard->Down(key);
}
int InfantryClass::unknown_slot28(ObjectClass *object, bool ignore_modifiers)
{
    bool force_move;
    bool force_fire;
    if (!(unsigned char)ignore_modifiers && (Infantry_Key_Down(KeyForceMove1) || Infantry_Key_Down(KeyForceMove2))) {
        force_move = true;
read_force_fire:
        if (!Infantry_Key_Down(KeyForceFire1) && !Infantry_Key_Down(KeyForceFire2)) goto clear_force_fire;
        force_fire = true;
    } else {
        force_move = false;
        if (!(unsigned char)ignore_modifiers) goto read_force_fire;
clear_force_fire:
        force_fire = false;
    }
    if (force_move && force_fire) force_fire = false;
    int action = FootClass::unknown_slot28(object, ignore_modifiers);
    if (action == 8) return 8;
    TechnoClass *target = static_cast<TechnoClass *>(object);
    BuildingClass *building = static_cast<BuildingClass *>(object);
    {
        if (Type->Engineer && House->Is_Player_Control() && object->unknown_38 && object->unknown_68) return 0x39;
        if (Type->Engineer && object->Kind_Of() == 6 && House->Is_Player_Control() &&
            !object->unknown_slot31() && building->Type->Repairable) {
            if (building->Type->BridgeRepairHut) {
                Cell cell = Coord_Cell(object->Center_Coord());
                return Map.Unknown_00569320(cell) ? 0x1d : 0x20;
            }
            if (House->Is_Ally(target) || (building->House->Type->MultiplayPassive && building->Type->CanBeOccupied)) {
                if (building->Type->Hospital && Health_Ratio() < Rules->unknown_1360) return 3;
                if (object->Health_Ratio() == Rules->unknown_1360) return 0x20;
                return 0x1d;
            }
            if (building->Type->Capturable) {
                if (object->Health_Ratio() <= Rules->unknown_143c) goto return_capture_action;
                return 0x1c;
            }
        }
        if (object->Kind_Of() == 6 && House->Is_Player_Control() && building->Unknown_00452EB0(this) && !force_fire) {
return_capture_action:
            return 9;
        }
        if (Type->Engineer && action == 5) return 2;
        if (unknown_6c16f0(-1) < 0 && House->Is_Player_Control()) {
            if (House->Is_Ally(target)) {
                if (object->Kind_Of() == 15) {
                    if (object != this) {
                        if (object->Health_Ratio() < Rules->unknown_1360) return 0x1b;
                    } else return 0x1a;
                }
                TechnoClass *transport = As_Techno_0040dbd0(object);
                if (!transport || transport->Techno_Type_Class()->Passengers == 0) {
                    if (action == 0x1a || action == 1) goto return_selected_action;
                    return 7;
                }
            } else return 0x3b;
        }
        if (House->Is_Player_Control() && Type->VehicleThief && object->unknown_slot31()) {
            if (target->Techno_Type_Class()->IsTrain) return 7;
            if (target->House != House) {
                if ((unknown_id_source_a3d290->unknown_000 & 0x800) != 0) {
                    for (int i = 0; i < Rules->unknown_9ec.Count(); ++i) {
                        if (Rules->unknown_9ec[i] == (UnitTypeClass *)object) return 7;
                    }
                }
                return 9;
            }
        }
        if (object->Kind_Of() == 6 && House->Is_Ally(target) && House->Is_Player_Control() && building->Type->Hospital && !force_fire) {
            if (Strength < Type->MaxStrength) {
                if (!Type->Deployer || (Sequence != 27 && Sequence != 28 && Sequence != 29 && Sequence != 30)) {
                    switch (unknown_slot150(15,target)) {
                    case 1: action = 3; break;
                    case 10: action = 0x1f; break;
                    }
                }
            } else action = 0x1f;
        }
        if (object->Kind_Of() == 6 && House->Is_Ally(target) && House->Is_Player_Control() && building->Type->Armory) {
            if (!unknown_118.Is_Elite()) {
                if (!Type->Deployer || (Sequence != 27 && Sequence != 28 && Sequence != 29 && Sequence != 30)) {
                    switch (unknown_slot150(15,target)) {
                    case 1: action = 3; break;
                    case 10: action = 0x1f; break;
                    }
                }
            } else action = 0x1f;
        }
        if (House->Is_Player_Control() && (Type->C4 || unknown_6d7910(14)) && action == 5 && object->Kind_Of() == 6 && !object->unknown_slot31()) {
            if (building->Type->CanC4 && !building->Type->InvisibleInGame) return 0x10;
            return 5;
        }
        WeaponTypeClass *weapon = unknown_slot231(unknown_slot174(object))->WeaponType;
        if (action == 5 && weapon && weapon->unknown_115) return 0x40;
        if (House->Is_Player_Control() && action == 5 && Type->Ivan) {
            if (object->Class_Of()->unknown_1fe && !object->unknown_38) return 0x35;
            return 0x36;
        }
        if (House->Is_Player_Control() && Type->Deployer && (Sequence == 27 || Sequence == 28 || Sequence == 29 || Sequence == 30)) {
            if (action == 1) return 2;
            if (action == 5) {
                if (!Type->DeployFire) goto stop_deployed_fire;
                {
                    int weapon_index = unknown_slot174(0);
                    if (!unknown_slot212(object,weapon_index)) goto stop_deployed_fire;
                }
                goto deployed_fire_allowed;
stop_deployed_fire:
                return 2;
deployed_fire_allowed:;
            } else if (action == 4) {
                if (!Infantry_Action_Allowed_00614b50(4)) return 0x1e;
                if (Type->UndeployDelay > -1) return 0x1e;
            }
        }
        if (House->Is_Player_Control()) {
            if (action == 4) {
                if (Type->Deployer && Type->UndeployDelay > -1 && Sequence == 31) return 0x1e;
                goto check_action_four;
            }
            goto check_action_31;
        }
        if (action == 4) {
check_action_four:
            if (Infantry_Action_Allowed_00614b50(4)) {
                if (!House->Is_Player_Control()) action = 0;
                else {
                    if (unknown_slot229()->WeaponType && unknown_slot229()->WeaponType->unknown_126) action = 0x34;
                    else if (!Type->Deployer) action = 0;
                }
            } else action = 0x1e;
        } else {
check_action_31:
            if (action == 0x1f) goto infiltration;
        }
        TechnoTypeClass *transport_type;
        if (House->Is_Ally(target) && House->Is_Player_Control() && As_Techno_0040dbd0(object) &&
            action != 5 && action != 0x1a && (transport_type = target->Techno_Type_Class()) != 0 && transport_type->Passengers > 0) {
            if (Type->BalloonHover) action = 0x1f;
            else {
                FootClass *foot = static_cast<FootClass *>(object);
                bool transport_ready;
                if (object) transport_ready = object->AbstractFlags.IsFoot != 0;
                if (object && transport_ready &&
                    ((!foot->Team || foot->Team->Class->Loadable) ? foot->Locomotor.Checked()->Is_Moving() : true)) action = 0x1f;
                else {
                    switch (unknown_slot150(15,target)) {
                    case 1: action = 3; break;
                    case 10: action = 0x1f; break;
                    }
                }
            }
        }
infiltration:
        if (House->Is_Player_Control() && Type->Infiltrate) {
            if (action != 5) goto cloning;
            if ((!Type->Agent && !House->Is_Ally(target) && object->Kind_Of() == 6 && building->Type->Capturable) ||
                (Type->Agent && !House->Is_Ally(target) && object->Kind_Of() == 6 && building->Type->Spyable)) {
                action = 9;
                if (object->Kind_Of() == 6) {
                    Cell base = Coord_Cell(object->Center_Coord());
                    int zone = Map.Zone(Get_Target_Cell(),Type->MovementZone,unknown_79);
                    Cell const *offset = building->Type->Occupy_List(0);
                    bool found = false;
                    while (!(offset->X == 0x7fff && offset->Y == 0x7fff) && !found) {
                        Cell foundation = Infantry_Add_Cells(base, *offset++);
                        for (int facing = 0; facing < 8; ++facing) {
                            if (Map.Zone(Infantry_Add_Cells(foundation,unknown_adjacent_851a70[facing & 7]),Type->MovementZone,false) == zone) {
                                found = true;
                                break;
                            }
                        }
                    }
                }
                goto finish;
            }
            if (!unknown_slot163()) {
                action = 0;
                goto finish;
            }
        } else if (action != 5) goto cloning;
        if (action == 5) {
            if (!unknown_slot163()) action = 0x3b;
        } else {
cloning:
            if (action == 7 && object->Kind_Of() == 6 && House->Is_Player_Control() && !object->unknown_slot31() && building->Type->Cloning)
                return object->Owning_House() != unknown_player_a35db4 ? 0x1f : 0xb;
            goto finish;
        }
finish:
        if (House->Is_Player_Control() && action == 0) {
            if (unknown_slot111() || unknown_slot112()) goto return_selected_action;
            action = 2;
        }
        if (action == 2) {
            if (object->unknown_slot48() && !object->unknown_slot51(true) && object->unknown_slot50(true)->Kind_Of() == 21) action = 1;
        } else if (action == 5 && object->Class_Of()->unknown_203) action = 2;
        goto return_selected_action;
    }
return_selected_action:
    return action;
}


// bool InfantryClass::unknown_slot78(int arg1, ObjectClass *arg2, bool arg3) (0x00506050, slot 78)
// Slot 28's action against arg2, with 9, 28 and 29 handled as 9 and 27 and
// 57 as 5; acting on itself with 5 is refused; FootClass's slot 78 with the
// action in place of arg1.

bool InfantryClass::unknown_slot78(int arg1, ObjectClass *arg2, bool arg3)
{
    int action = unknown_slot28(arg2, arg3);
    switch (action) {
    case 1:
    case 5:
    case 16:
    case 26:
        action = action;
        break;

    case 9:
    case 28:
    case 29:
        action = 9;
        break;

    case 27:
    case 57:
        action = 5;
        break;
    }
    if (arg2 == this && action == 5) {
        return false;
    }
    return FootClass::unknown_slot78(action, arg2, arg3);
}


// bool InfantryClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4) (0x00506100, slot 77)
// While deploying, refuses when slot 27 reports cell arg2 as blocked (2);
// otherwise FootClass's slot 77.

bool InfantryClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4)
{
    if (unknown_5090d0() && unknown_slot27(arg2, false, arg4) == 2) {
        return false;
    }
    return FootClass::unknown_slot77(arg1, arg2, arg3, arg4);
}


// wchar_t const *InfantryClass::unknown_slot35() (0x00506160, slot 35)
// The name shown: "Technician" while +0x5C1 is set; the disguise type's
// name to other houses than the player's allies; else the type's UIName.

wchar_t const *InfantryClass::unknown_slot35()
{
    if (unknown_5c1) {
        return StringTable::Load_String("TXT_TECHNICIAN", 0, "D:\\Ra2\\Infantry.CPP", 4221);
    }
    if (unknown_slot48() && !House->Is_Ally(unknown_player_a35db4)) {
        return unknown_slot50(true)->UIName;
    }
    return Type->UIName;
}


// void InfantryClass::unknown_slot240() (0x005061D0, slot 240)
// Unless slot 212 accepts +0x228 with slot 174's weapon: without +0x234
// asks slot 219 (1, the current coordinate, 0) for one, hands the answer to
// slot 220, and when there is none (and the mission is not 5 and the type is
// not JumpJet) calls slot 261 (0, 1).

void InfantryClass::unknown_slot240()
{
    int weapon = unknown_slot174(0);
    if (unknown_228 != 0 && unknown_slot212(unknown_228, weapon)) {
        return;
    }
    AbstractClass *found = 0;
    if (unknown_234 == 0) {
        CoordStruct coord = Coord;
        found = unknown_slot219(1, &coord, 0);
    }
    unknown_slot220(found);
    if (found == 0 && Mission != 5 && !Techno_Type_Class()->JumpJet) {
        unknown_slot261(0, 1);
    }
}


// int InfantryClass::Do_MISSION_ATTACK() (0x00506280, slot 126)
// Mission handler for buildings as targets (+0x228): C4 infantry (or those
// passing 0x006D7910 (14)) plant on a building whose type has +0x121A and not
// +0x1385 (slot 260, then queue mission 17); owners failing HouseClass
// 0x004F52F0 send Infiltrate infantry, or Occupier/Assaulter infantry the
// building admits, in (slot 260, mission 8). Owners passing it while
// deploying run slot 240 and wait the mission rate plus 0-2 frames;
// otherwise FootClass's slot 126.

int InfantryClass::Do_MISSION_ATTACK()
{
    if (Type->C4 || unknown_6d7910(14)) {
        if (unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
            BuildingTypeClass *type = ((BuildingClass *)unknown_228)->Type;
            if (type->CanC4 && !type->InvisibleInGame) {
                Assign_Destination(unknown_228, true);
                QueueMission(17, false);
                return 1;
            }
        }
    }
    if (!House->Unknown_004F52F0() && unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
        if (Type->Infiltrate
            || ((Type->Occupier || Type->Assaulter) && ((BuildingClass *)unknown_228)->Unknown_00452EB0(this))) {
            Assign_Destination(unknown_228, true);
            Set_Mission(8);
            return 1;
        }
    }
    if (House->Unknown_004F52F0() && unknown_5090d0()) {
        unknown_slot240();
        int delay = (int)(Control()->unknown_10 * 900.0);
        return delay + unknown_id_source_a3d290->Random()(0, 2);   // the scenario's Random()
    }
    return FootClass::Do_MISSION_ATTACK();
}


// int InfantryClass::Do_MISSION_HUNT() (0x005063E0, slot 132)
// Against a building target (unless the house check 0x004F52F0 holds):
// Infiltrate types, or Occupier/Assaulter types the building accepts,
// target it and set mission 8 (returns 1); otherwise FootClass's.

int InfantryClass::Do_MISSION_HUNT()
{
    if (!House->Unknown_004F52F0() && unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
        if (Type->Infiltrate
            || ((Type->Occupier || Type->Assaulter) && ((BuildingClass *)unknown_228)->Unknown_00452EB0(this))) {
            Assign_Destination(unknown_228, true);
            Set_Mission(8);
            return 1;
        }
    }
    return FootClass::Do_MISSION_HUNT();
}


// int InfantryClass::Do_MISSION_GUARD() (0x00506470)
// Defers to FootClass's slot 129 unless 0x00507F70 handles it (not -1).

int InfantryClass::Do_MISSION_GUARD()
{
    int result = Update_Deployed_Attack_00507f70();
    if (result == -1) {
        return FootClass::Do_MISSION_GUARD();
    }
    return result;
}


// int InfantryClass::Do_MISSION_GUARD_AREA() (0x00506490)
// Defers to FootClass's slot 130 unless 0x00507F70 handles it (not -1).

int InfantryClass::Do_MISSION_GUARD_AREA()
{
    int result = Update_Deployed_Attack_00507f70();
    if (result == -1) {
        return FootClass::Do_MISSION_GUARD_AREA();
    }
    return result;
}


// int InfantryClass::Do_MISSION_MOVE() (0x005064B0, slot 133)
// While deploying (27-30): clear the target when the owner's house check
// (0x004F52F0) holds (1), or without UndeployDelay play action 31 and wait
// its frame count; otherwise FootClass's.

int InfantryClass::Do_MISSION_MOVE()
{
    if (Sequence == 27 || Sequence == 28 || Sequence == 29 || Sequence == 30) {
        if (Owning_House()->Unknown_004F52F0()) {
            Assign_Destination(0, true);
            return 1;
        }
        if (Type->UndeployDelay <= -1) {
            unknown_slot311(31, 0, 0);
            return Type->DoControls[31].Count;
        }
    }
    return FootClass::Do_MISSION_MOVE();
}


// Deploy / undeploy action and weapon-target choice (0x00506530).
int InfantryClass::Do_MISSION_UNLOAD()
{
    if (Type->Deployer) {
        int delay = -1;
        if (unknown_5090d0()) {
            if (Type->UndeployDelay <= -1) {
                unknown_slot311(31, true, false);
            }
        } else {
            unknown_slot311(27, true, false);
            WeaponStruct *weapon = unknown_slot229();
            if (Unknown_006D8640(weapon) && weapon->WeaponType->unknown_126) {
                if (unknown_func_5d27d0("DESO")) {
                    delay = Type->DoControls[27].Count + 1;
                } else {
                    unknown_slot220((AbstractClass *)&Map[unknown_slot104()]);
                }
            }
            if (Type->UndeployDelay > -1) {
                delay = Type->UndeployDelay;
            }
        }
        Set_Mission(5);
        Assign_Destination(0, true);
        if (delay > -1) {
            return delay;
        }
    }
    return FootClass::Do_MISSION_UNLOAD();
}


// Refines movement/interaction results for infantry at a cell (0x00506650).
inline BuildingTypeClass *Fogged_Building_Type(FoggedObjectClass const *object)
{
    return object->unknown_30 == 6 ? (BuildingTypeClass *)object->DrawRecords[0].Type : 0;
}
int InfantryClass::unknown_slot27(Cell const *cell, bool arg2, bool arg3)
{
    if (!House->Is_Player_Control()) {
        return 0;
    }
    int result = FootClass::unknown_slot27(cell, arg2, arg3);
    if (unknown_6c16f0(-1) < 0 && House->Is_Player_Control() && result == 5) {
        result = 26;
    }
    if (Type->JumpJet && (result == 1 || result == 2)) {
        if (Map[*cell].Is_Near_Tunnel_NW() || Map[*cell].Is_Near_Tunnel_ES()) {
            result = 0;
        }
    }
    if (House->Is_Player_Control() && Type->Deployer && unknown_5090d0()) {
        if (result == 1) {
            result = 2;
        } else if (result == 5) {
            if (Type->DeployFire) {
                int weapon_index = unknown_slot174(0);
                CoordStruct fire_coord(cell->X * 256 + 128, cell->Y * 256 + 128, 0);
                if (!In_Range(fire_coord, weapon_index)) result = 2;
            } else result = 2;
        } else {
            goto ordinary_cell;
        }
    } else {
    ordinary_cell:
        if (result == 1 && Map[*cell].Has_Tunnel()) {
            return Map[*cell].unknown_47a120(this) ? 35 : 36;
        }
    }

    CellClass &target_cell = Map[*cell];
    BuildingTypeClass *remembered_type = 0;
    FoggedObjectClass *remembered = 0;
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && arg2 && target_cell.FoggedObjects != 0) {
        for (int index = 0; index < target_cell.FoggedObjects->Count(); ++index) {
            remembered = (*target_cell.FoggedObjects)[index];
            if (remembered->unknown_74 && remembered->unknown_30 == 6) {
                remembered_type = Fogged_Building_Type(remembered);
                break;
            }
        }
    }
    if (Type->Engineer && remembered_type != 0 && House->Is_Player_Control()) {
        if (remembered_type->BridgeRepairHut) {
            Cell hut_cell = Coord_Cell(remembered->Center_Coord());
            result = Map.Unknown_00569320(hut_cell) ? 29 : 32;
            goto result_ready;
        }
        if (remembered_type->Repairable) {
            result = 9;
            goto result_ready;
        }
    }
    if (result == 5 && !unknown_slot163()) {
        result = 59;
    }
result_ready:
    if (House->Is_Player_Control() && result == 0 && !Type->JumpJet && !unknown_slot111() && !unknown_slot112()) {
        result = 2;
    }
    return result;
}


// ObjectTypeClass *InfantryClass::Class_Of() const (0x00506950, slot 33)

ObjectTypeClass *InfantryClass::Class_Of() const
{
    return Type;
}


// Creates scenario infantry from the [Infantry] records (0x00506960).
extern char const *unknown_infantry_section_7a33f0;
extern int unknown_ini_format_a40d24;
extern CoordStruct const unknown_infantry_spots_850e50[];
extern CellClass unknown_dummy_cell_a6f9f8;
inline CoordStruct Infantry_Subcell_Position(CoordStruct center, int spot)
{
    CoordStruct aligned((center.X / 256) * 256, (center.Y / 256) * 256, center.Z);
    return aligned + unknown_infantry_spots_850e50[spot];
}
void __fastcall InfantryClass::Read_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(unknown_infantry_section_7a33f0);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_infantry_section_7a33f0, index);
        char buffer[128];
        ini.Get_String(unknown_infantry_section_7a33f0, entry, 0, buffer, sizeof(buffer));
        int house_index = HouseClass::Find_Index(strtok(buffer, ","));
        if (house_index != -1) {
            HouseClass *house = Houses[house_index];
            int type_index = InfantryTypeClass::From_Name(strtok(0, ","));
            if (type_index != -1) {
                InfantryClass *infantry = new InfantryClass(InfantryTypes[type_index], house);
                if (infantry != 0) {
                    int health = atoi(strtok(0, ","));
                    CoordStruct coord;
                    if (unknown_ini_format_a40d24 >= 4) {
                        int x = atoi(strtok(0, ","));
                        short y = (short)atoi(strtok(0, ","));
                        coord = Cell_Coord(Cell((short)x, y));
                    } else {
                        int packed = atoi(strtok(0, ","));
                        coord = Cell_Coord(Cell((short)(packed % 128), (short)(packed / 128)));
                    }
                    int spot = atoi(strtok(0, ","));
                    coord = Infantry_Subcell_Position(coord, spot);
                    int mission = MissionClass::Mission_From_Name(strtok(0, ","));
                    char *token = strtok(0, ",");
                    int direction = token != 0 ? atoi(token) : 0;
                    token = strtok(0, ",");
                    if (token != 0) {
                        TagTypeClass *tag_type = TagTypeClass::From_Name(token);
                        if (tag_type != 0) {
                            TagClass *tag = TagClass::Find_Or_Make(tag_type);
                            if (tag != 0) {
                                infantry->Attach_Tag(tag);
                            }
                        }
                    }
                    token = strtok(0, ",");
                    if (token != 0) {
                        infantry->unknown_118.From_Integer(atoi(token));
                    }
                    token = strtok(0, ",");
                    if (token != 0) {
                        infantry->unknown_1ac = atoi(token);
                    }
                    token = strtok(0, ",");
                    if (token != 0) {
                        infantry->unknown_79 = atoi(token) != 0;
                        if (infantry->unknown_79) {
                            coord.Z = Map.Get_Height_GL(coord) + unknown_geometry_j;
                        } else {
                            coord.Z = Map.Get_Height_GL(coord);
                        }
                    }
                    token = strtok(0, ",");
                    if (token != 0) {
                        infantry->unknown_379 = atoi(token) != 0;
                    }
                    token = strtok(0, ",");
                    if (token != 0) {
                        infantry->unknown_37a = atoi(token) != 0;
                    }
                    if (&Map[coord] == &unknown_dummy_cell_a6f9f8 || !infantry->Unlimbo(&coord, direction)) {
                        delete infantry;
                    } else {
                        InfantryTypeClass *type = infantry->Type;
                        double scaled_health = (double)health * (1.0 / 256.0);
                        scaled_health *= type->MaxStrength;
                        infantry->Strength = (int)scaled_health;
                        if (infantry->Strength > type->MaxStrength - 3) {
                            infantry->Strength = type->MaxStrength;
                        }
                        if (infantry->Strength < 1) {
                            infantry->Strength = 1;
                        }
                        infantry->unknown_70 = infantry->Strength;
                        if (unknown_game_mode_a3d298 == 0 || infantry->House->Unknown_004F52F0()) {
                            infantry->QueueMission(mission, false);
                            infantry->Commence();
                        } else {
                            infantry->unknown_slot261(false, true);
                        }
                    }
                }
            }
        }
    }
}


// Writes the [Infantry] section from the global infantry list (0x00506D70).
extern char const *unknown_infantry_section_7a33f0;

void __fastcall InfantryClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(unknown_infantry_section_7a33f0, 0);
    for (int index = 0; index < Infantry.Count(); index++) {
        InfantryClass *infantry = Infantry[index];
        if (!infantry->unknown_76) {
            char id[12];
            sprintf(id, "%d", index);
            char const *tag;
            if (infantry->unknown_34 != 0) {
                tag = (char const *)infantry->unknown_34->Class->ID;
            } else {
                tag = "None";
            }
            int mission = infantry->Get_Mission() == -1 ? infantry->MissionQueue : infantry->Get_Mission();
            unsigned raw = *(unsigned const *)&infantry->unknown_2e0.Current();
            unsigned direction = (((raw >> 7) + 1) >> 1) & 0xff;
            CoordStruct coord = infantry->Coord;
            char line[128];
            sprintf(line, "%s,%s,%d,%d,%d,%d,%s,%d,%s,%d,%d,%d,%d,%d",
                    (char const *)infantry->House->IniName, (char const *)infantry->Type->ID,
                    (int)(infantry->Health_Ratio() * 256.0),
                    infantry->unknown_slot104().X, infantry->unknown_slot104().Y,
                    Unknown_004763D0(coord),
                    MissionClass::Name_From_Mission(mission), direction, tag,
                    infantry->unknown_118.To_Integer(), infantry->unknown_1ac,
                    infantry->unknown_79, infantry->unknown_379, infantry->unknown_37a);
            ini.Put_String(unknown_infantry_section_7a33f0, id, line);
        }
    }
}


// Fear decays, changes prone state and can make a Fraidycat flee (0x00506F30).
inline void Infantry_Decay_Fear(int &fear)
{
    --fear;
}
void InfantryClass::unknown_506f30()
{
    if (unknown_5bc > 0) {
        if (!Type->Fearless) {
            Infantry_Decay_Fear(unknown_5bc);
        }
        if (unknown_5bc == 0 && unknown_254 == 0 && unknown_slot163()) {
            unknown_254 = Type->Ammo;
        }
        if (unknown_5c3) {
            if (unknown_5bc < 50 && !unknown_5090d0()) {
                unknown_slot311(7, false, false);
            }
        } else if (unknown_5bc >= 50 && !unknown_5090d0()) {
            if (!(House->Unknown_004F52F0() && (unknown_494 != 0 || Locomotor.Checked()->Is_Moving()))
                && !Type->Fraidycat) {
                unknown_slot311(5, false, false);
            }
        }
    }
    if (Type->Fraidycat && unknown_5bc > 50 && !unknown_5090d0() && !unknown_7a) {
        if (!Locomotor.Checked()->Is_Moving() && unknown_494 == 0) {
            Scatter(unknown_geometry_l, true, false);
        }
    }
}


// Marks the team target state, or handles a teamless guard outside the map predicate (0x005070E0).
// Complete independently decoded body through RET 0x0050716C: 141 bytes.
// Inventory 0x0050711C is inside a displacement, not a separate entry.
bool InfantryClass::unknown_5070e0()
{
    if (Team != 0 && unknown_32d) {
        Team->TargetNotAssigned = true;
    }
    if (Team == 0 && Get_Mission() == 5) {
        CoordStruct coord = Coord;
        if (!Map.Unknown_0054AB00(coord)) {
            unknown_slot210();
            Delete_Me();
            return true;
        }
    }
    return false;
}


// Thief capture and pursuit of an enemy vehicle; +0xC13 is the shared Thief flag.

bool InfantryClass::unknown_507170()
{
    if (!Type->Thief || !unknown_494 || unknown_494->Kind_Of()!=1) return false;
    TechnoClass *vehicle=static_cast<TechnoClass *>(unknown_494);
    if (House->Is_Ally(vehicle)) return false;
    // The destination query receives the requesting infantry.
    CoordStruct coord=vehicle->unknown_slot19(this);
    int vehicle_z=coord.Z;
    coord=Center_Coord();
    if (abs(vehicle_z-coord.Z)<unknown_geometry_g) {
        if ((reinterpret_cast<UnknownVector3 const &>(Center_Coord())-reinterpret_cast<UnknownVector3 const &>(vehicle->Center_Coord())).Length()<128) {
            if (vehicle->unknown_34) vehicle->unknown_34->Spring(1,this,unknown_geometry_k,false,0);
            vehicle->unknown_slot149(3);
            vehicle->unknown_slot54(false);
            if (unknown_34 && unknown_34->Is_To_Inherit()) vehicle->Attach_Tag(unknown_34);
            vehicle->unknown_slot222(House,1);
            vehicle->unknown_290=Type->ArrayIndex;
            Delete_Me();
            return true;
        }
    }
    if ((reinterpret_cast<UnknownVector3 const &>(Center_Coord())-reinterpret_cast<UnknownVector3 const &>(vehicle->Center_Coord())).Length()<512) {
        if (UnknownCoord434820(Locomotor.Checked()->Destination())!=unknown_geometry_l) {
            if (reinterpret_cast<UnknownVector3 const &>(vehicle->Center_Coord()).Distance_To(reinterpret_cast<UnknownVector3 const &>(UnknownCoord434820(Locomotor.Checked()->Destination())))<=128) return false;
        }
        Assign_Destination(unknown_494,true);
    } else {
        if (UnknownCoord434820(Locomotor.Checked()->Destination())!=unknown_geometry_l) {
            if (Coord_Cell(vehicle->unknown_slot19(this))==Coord_Cell(UnknownCoord434820(Locomotor.Checked()->Destination()))) return false;
        }
    }
    // The original calls this again after the near-target branch.
    Assign_Destination(unknown_494,true);
    return false;
}


// Starts a firing sequence and fires on its configured frame (0x00507530).
extern "C" GUID const CLSID_JumpjetLocomotionClass;
void InfantryClass::Update_Fire_00507530()
{
    if (unknown_228 != 0) {
    int weapon = unknown_slot174(unknown_228);
    if (!unknown_57d) {
        int error = unknown_slot218(unknown_228, weapon, 1);
        switch (error) {
        case 0: {

            if (Type->JumpJet) {
                bool is_jumpjet;
                {
                    UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Locomotor);
                    GUID clsid;
                    persist.Checked()->GetClassID(&clsid);
                    is_jumpjet = memcmp(&clsid, &CLSID_JumpjetLocomotionClass, sizeof(GUID)) == 0;
                }
                if (is_jumpjet) {
                    unknown_slot311(26, false, false);
                    goto sequence_started;
                }
            }
            if (unknown_5090d0()) {
                unknown_slot311(29, false, false);
            } else if (unknown_5c3) {
                unknown_slot311(8, false, false);
            } else {
                unknown_slot311(4, false, false);
            }
        sequence_started:
            unknown_57d = true;
            unknown_2e0.Set(Direction_To(unknown_228));
            if (unknown_228 == unknown_494) {
                unknown_4cd9b0();
                unknown_slot291();
            }
                } break;
        case 5: {

            if (unknown_6c16f0(weapon) < 0) {
                AbstractClass *target = unknown_228;
                if (target != 0 && target->AbstractFlags.IsObject && target->Kind_Of() == 15) {
                    if (((ObjectClass *)target)->Health_Ratio() >= Rules->unknown_1360) {
                        unknown_slot220(0);
                    }
                } else {
                    unknown_slot220(0);
                }
            }
                } break;
        case 9:

            unknown_slot252(false);
                    break;
        }

    }
    int fire_frame = Type->FireUp;
    if (unknown_5c3) {
        fire_frame = Type->FireProne;
    }
    if (unknown_57d && StageClass::Stage == fire_frame) {
        weapon = unknown_slot174(unknown_228);
        if (unknown_slot218(unknown_228, weapon, 1) == 0) {
            unknown_slot221(unknown_228, weapon);
        } else {
            unknown_57d = false;
            if (unknown_5c3) {
                unknown_slot311(2, false, false);
            } else if (unknown_5090d0()) {
                unknown_slot311(28, false, false);
            } else {
                unknown_slot311(0, false, false);
            }
        }
        if (unknown_228 != 0 && unknown_slot231(0)->WeaponType->unknown_a8 < Rules->unknown_1328) {
            ((UnknownCellClass *)&Map[unknown_228->Center_Coord()])->Unknown_00476920(&CoordStruct(Coord), true, false, false);
        }
    }
    } else {
        unknown_57d = false;
    }
}


// Completes animation sequences and returns to locomotion/idle (0x005078B0).
// Sequence 27 continues with 28 and 34 with 35; 11-15 leave a corpse animation
// (the type's DeadBodies, else Rules' for humans) and remove the object; 31 and
// unlisted sequences pick the next idle/move sequence. Case 31 and the default's
// last arm compile to one block (cross-jumped).
inline AnimTypeClass *Pick_Animation(TypeList<AnimTypeClass *> const &list, unsigned draw)
{
    return list[draw % list.Count()];
}
// A corpse animation is chosen after allocation, with one scenario draw.
inline AnimTypeClass *InfantryClass::Pick_Dead_Body(bool use_type) const
{
    unsigned draw = (unsigned)unknown_id_source_a3d290->Random()();
    if (use_type) {
        return Pick_Animation(Type->DeadBodies, draw);
    }
    return Pick_Animation(Rules->DeadBodies, draw);
}
void InfantryClass::Advance_Animation_005078b0()
{
    if (Sequence == -1 || StageClass::Stage >= Type->DoControls[Sequence].Count) {
        switch (Sequence) {
        case 27:
            unknown_slot311(28, true, false);
            unknown_6d96a0();
            break;
        case 31:
            unknown_slot311(0, true, false);
            break;
        default:
            if (Sequence != -1) {
                int facing = Type->DoControls[Sequence].Facing;
                if (facing != -1) {
                    UnknownFacingDir direction((unsigned short)(facing << 13));
                    unknown_2e0.Set(direction);
                }
            }
            if (Locomotor.Checked()->Is_Moving() && unknown_468 > 0.1) {
                if (unknown_5c3) {
                    unknown_slot311(6, true, false);
                } else {
                    unknown_slot311(3, true, false);
                }
            } else {
                if (unknown_5090d0()) {
                    unknown_slot311(28, true, false);
                } else if (unknown_5c3) {
                    unknown_slot311(2, true, false);
                } else {
                    unknown_slot311(0, true, false);
                }
            }
            break;
        case 34:
            unknown_slot311(35, true, false);
            break;
        case 11: case 12: case 13: case 14: case 15:
            if (StageClass::Stage >= Type->DoControls[Sequence].Count) {
                if (Type->DeadBodies.Count() > 0) {
                    new AnimClass(Pick_Dead_Body(true),
                        Center_Coord(), 0, 1, 0x600, 0);
                } else if (!Type->NotHuman) {
                    new AnimClass(Pick_Dead_Body(false),
                        Center_Coord(), 0, 1, 0x600, 0);
                }
                Delete_Me();
            }
            break;
        case 20: case 21: case 36:
            if (StageClass::Stage >= Type->DoControls[Sequence].Count) {
                Delete_Me();
            }
            break;
        case 33:
            break;
        }
    }
    if (Sequence == 33 && !unknown_7a) {
        unknown_slot311(0, true, false);
    }
}


// Selects the standing, walking or jumpjet sequence (0x00507B90).
extern "C" GUID const CLSID_JumpjetLocomotionClass;
void InfantryClass::Update_Movement_00507b90()
{
    if (Get_Mission() == 2 && !Locomotor.Checked()->Is_Moving()) {
        if (unknown_494 == 0) {
            unknown_slot261(false, true);
        } else {
            Assign_Destination(unknown_494, true);
            unknown_slot306(1.0);
        }
    }
    if (unknown_494 != 0) {
        Cell target_cell = Coord_Cell(unknown_494->unknown_slot19(this));
        if (unknown_5c4 && Can_Enter_Cell(Get_Target_Cell_Ptr(), -1, -1, 0, 1) == 7) {
            goto select_sequence;
        }
        if (!Locomotor.Checked()->Is_Moving() && !unknown_370 && unknown_494 != 0 && unknown_32d) {
            Cell current_cell = Coord_Cell(unknown_slot19(0));
            if (!Map.Unknown_0054F7C0(current_cell, target_cell, Type->MovementZone,
            Is_Moving_Onto_Bridge(0), (unsigned char)((Map[target_cell].unknown_140 >> 8) & 1), unknown_slot189())
            && !Type->Infiltrate && Get_Mission() != 7) {
                Assign_Destination(0, true);
            }
        }
    }
select_sequence:
    if (Locomotor.Checked()->Is_Really_Moving_Now()) {
        if (unknown_508d00()) {
            if (unknown_57d) return;
            if (unknown_468 > 0.8) {
                unknown_slot311(24, false, false);
            } else {
                unknown_slot311(23, false, false);
            }
            return;
        }
        if (unknown_5c3) {
            unknown_slot311(6, false, false);
        } else {
            unknown_slot311(3, false, false);
        }
    } else {
        if (Sequence == 3 || Sequence == 24 || Sequence == 23) {
            unknown_slot311(0, false, false);
        } else if (Sequence == 6) {
            unknown_slot311(2, false, false);
        } else if (Sequence == 17) {
            unknown_slot311(16, false, false);
        }
        return;
    }
}


// Deployed attack and radiation update (0x00507F70).
inline int Infantry_Deployment_Radiation_Threshold(WeaponTypeClass const &weapon)
{
    return weapon.unknown_12c / 3;
}
int InfantryClass::Update_Deployed_Attack_00507f70()
{
    if (unknown_5090d0()) {
        if (Type->UndeployDelay > -1) {
            unknown_slot311(31, false, false);
            return Type->DoControls[31].Count;
        }
        if (Type->DeployFire) {
            if (Type->ImmuneToRadiation) {
                RadSiteClass *site = reinterpret_cast<RadSiteCell *>(&Map[unknown_slot104()])->Unknown_0047CE40();
                int threshold = Infantry_Deployment_Radiation_Threshold(*unknown_slot231(1)->WeaponType);
                if (site == 0 || site->Get_Current_Level() < threshold) {
                    unknown_slot220(&Map[unknown_slot104()]);
                    if (unknown_slot218(unknown_228, 1, 1) == 0) {
                        // Slot221's existing int argument carries the same 32-bit target pointer.
                        unknown_slot221(unknown_228, 1);
                        unknown_slot220(0);
                        unknown_slot311(29, false, false);
                        return Type->DoControls[29].Count;
                    }
                    unknown_slot220(0);
                }
                int delay = int(Control()->unknown_10 * 900.0);
                return delay + unknown_id_source_a3d290->Random()(10, 20);
            }
            unknown_slot240();
            int delay = int(Control()->unknown_10 * 900.0);
            return delay + unknown_id_source_a3d290->Random()(0, 2);
        }
    } else {
        if (!Owning_House()->Unknown_004F52F0() && Type->Deployer && Type->DeployFire &&
            Type->UndeployDelay <= -1 && unknown_494 == 0 &&
            Rules->AIAutoDeployFrameDelay[House->unknown_c8] + unknown_ac < Frame) {
            if (unknown_1b0 != 0) {
                if (Coord_Cell(unknown_1b0->Center_Coord()) != Coord_Cell(Center_Coord())) {
                    return -1;
                }
            }
            if (!Type->ImmuneToRadiation) {
                if (!Locomotor.Checked()->Is_Moving()) {
                    unknown_slot311(27, false, false);
                    return Type->DoControls[27].Count;
                }
                Locomotor.Checked()->Stop_Moving();
                unknown_5cc = true;
                int delay = int(Control()->unknown_10 * 900.0);
                return delay + unknown_id_source_a3d290->Random()(0, 2);
            }
        }
    }
    return -1;
}


// void *InfantryClass::Get_Image_Data() (0x00508310, slot 26)
// FootClass's (a tail jump).

void *InfantryClass::Get_Image_Data()
{
    return FootClass::Get_Image_Data();
}


// bool InfantryClass::unknown_slot257() (0x00508320, slot 257)
// TechnoClass's answer, but false while moving, prone (+0x5C3), +0x57D or
// deploying (27-30), and only for sequences 0, 1 and 16.

bool InfantryClass::unknown_slot257()
{
    if (!TechnoClass::unknown_slot257()) {
        return false;
    }
    if (Locomotor.Checked()->Is_Moving()) {
        return false;
    }
    if (unknown_5c3) {
        return false;
    }
    if (unknown_57d) {
        return false;
    }
    if (unknown_5090d0()) {
        return false;
    }
    if (Sequence == 1 || Sequence == 0 || Sequence == 16) {
        return true;
    }
    return false;
}


// bool InfantryClass::unknown_slot56(CoordStruct const &coord) (0x005083B0, slot 56)
// ObjectClass's slot 56; on success queue mission 5 (15 when the house
// check 0x004F52F0 fails) and play action 33.

bool InfantryClass::unknown_slot56(CoordStruct const &coord)
{
    if (ObjectClass::unknown_slot56(coord)) {
        if (House->Unknown_004F52F0()) {
            QueueMission(5, false);
        } else {
            QueueMission(15, false);
        }
        unknown_slot311(33, 1, 0);
        return true;
    }
    return false;
}


// void InfantryClass::Set_Occupy_Bit(CoordStruct const &coord) (0x00508410, slot 58)
// Occupies the coordinate's spot: on a bridge cell at or above bridge
// height the bridge-level bit and owner, otherwise the ground ones.

void InfantryClass::Set_Occupy_Bit(CoordStruct const &coord)
{
    int spot = Unknown_004763D0(coord);
    UnknownCellClass *cell = Map_Cell(coord);
    if (Map.Get_Height_GL(coord) + unknown_geometry_j <= coord.Z && (cell->Flags & 0x100)) {
        cell->unknown_128.Composite |= 1 << spot;
        cell->unknown_58 = Owner();
    } else {
        cell->unknown_124.Composite |= 1 << spot;
        cell->unknown_54 = Owner();
    }
}


// Clears the infantry occupancy bit at a coordinate (0x005084A0).
void InfantryClass::Clear_Occupy_Bit(CoordStruct const &coord)
{
    int spot = Unknown_004763D0(coord);
    UnknownCellClass *cell = Map_Cell(coord);
    int ground_height = Map.Get_Height_GL(coord);
    if (unknown_geometry_j + ground_height <= coord.Z) {
        cell->unknown_128.Composite &= ~(1 << spot);
        if ((cell->unknown_128.Composite & 0x1c) == 0) {
            cell->unknown_58 = -1;
        }
    } else {
        cell->unknown_124.Composite &= ~(1 << spot);
        if ((cell->unknown_124.Composite & 0x1c) == 0) {
            cell->unknown_54 = -1;
        }
    }
}


// int InfantryClass::unknown_slot174(AbstractClass *target) (0x00508530, slot 174)
// During the deploy sequences (27-30), the type's DeployFireWeapon when DeployFire
// is set; otherwise TechnoClass's.

int InfantryClass::unknown_slot174(AbstractClass *target)
{
    if ((Sequence == 27 || Sequence == 28 || Sequence == 29 || Sequence == 30) && Type->DeployFire) {
        return Type->DeployFireWeapon;
    }
    return TechnoClass::unknown_slot174(target);
}


// HRESULT __stdcall InfantryClass::Load(IStream *stream) (0x00508570, slot 5)
// Leaves the techno index, loads the foot, rebuilds itself in place while
// keeping the locomotor, rejoins the index and fixes up pointers.

HRESULT __stdcall InfantryClass::Load(IStream *stream)
{
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    HRESULT result = FootClass::Load(stream);
    if (SUCCEEDED(result)) {
        ILocomotion *locomotor = Locomotor;
        new (this) InfantryClass(NoInitClass());
        Locomotor = locomotor;
        locomotor->Release();
        unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
        Swizzle_Pointer(Type);
        Swizzle_Pointer(unknown_40c);
        Swizzle_Pointer(unknown_410);
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall InfantryClass::Save(IStream *stream, BOOL clear_dirty) (0x00508710, slot 6)

HRESULT __stdcall InfantryClass::Save(IStream *stream, BOOL clear_dirty)
{
    return FootClass::Save(stream, clear_dirty);
}


// void InfantryClass::unknown_slot307() (0x00508730)
// Ends sequences 6, 3 and 0x11.

void InfantryClass::unknown_slot307()
{
    if (Sequence == 6 || Sequence == 3 || Sequence == 0x11) {
        Sequence = -1;
    }
}


// void InfantryClass::unknown_slot308() (0x00508750)
// Clears +0x5CC and starts sequence 0x1B (slot 311).

void InfantryClass::unknown_slot308()
{
    if (unknown_5cc) {
        unknown_5cc = false;
        unknown_slot311(0x1B, 0, 0);
    }
}


// bool InfantryClass::unknown_slot122() (0x00508770, slot 122)
// False under missions 6 and 21, +0x57D or +0x7A; otherwise true when the
// locomotor is not moving now (or the mission is 5, 15, or 1 with no
// +0x228) and the current sequence is unset or interruptible.

bool InfantryClass::unknown_slot122()
{
    if (Mission == 6 || Mission == 21 || unknown_57d || unknown_7a) {
        return false;
    }
    if (!Locomotor.Checked()->Is_Moving_Now() || Get_Mission() == 5 || Get_Mission() == 15
        || (Get_Mission() == 1 && unknown_228 == 0)) {
        if (Sequence == -1 || unknown_do_controls_7a3474[Sequence].Interrupt) {
            return true;
        }
    }
    return false;
}


// void InfantryClass::unknown_slot295() (0x00508820, slot 295)
// Unless the type is Fearless or check 13 (0x006D7910) holds: +0x5BC = 300
// (hypothesis: a fear counter).

void InfantryClass::unknown_slot295()
{
    if (!Type->Fearless && !unknown_6d7910(13)) {
        unknown_5bc = 300;
    }
}


// void InfantryClass::unknown_slot296() (0x00508850, slot 296)
// Clears +0x5BC unless the type is Fearless.

void InfantryClass::unknown_slot296()
{
    if (!Type->Fearless) {
        unknown_5bc = 0;
    }
}


// void InfantryClass::unknown_slot297(int arg1) (0x00508870, slot 297)
// Slot 311 with action 10 for arg1 1, else 9.

void InfantryClass::unknown_slot297(int arg1)
{
    switch (arg1) {
    case 1:
        unknown_slot311(10, 0, 0);
        break;

    default:
        unknown_slot311(9, 0, 0);
        break;
    }
}


// void InfantryClass::Compute_CRC(WWCRCEngine &crc) const (0x005088A0, slot 13)

void InfantryClass::Compute_CRC(WWCRCEngine &crc) const
{
    FootClass::Compute_CRC(crc);
    crc((long)Sequence);
    crc(unknown_5b0.Value());
    crc((long)unknown_5bc);
    crc(unknown_5c1);
    crc(unknown_5c2);
    crc(unknown_5c3);
    crc(unknown_5c4);
    crc(unknown_5c5);
}


// CoordStruct InfantryClass::unknown_slot181(int arg1) (0x00508940, slot 181)
// Slot 43's coordinate for arg1.

CoordStruct InfantryClass::unknown_slot181(int arg1)
{
    return unknown_slot43(arg1);
}


// Apparent speed while prone: two thirds for crawlers, 150% otherwise (0x00508980).
int InfantryClass::unknown_slot303()
{
    int speed = FootClass::unknown_slot303();
    if (unknown_5c3) {
        if (Type->Crawls) {
            speed += speed / -3;
        } else {
            speed += speed / 2;
        }
    }
    return speed;
}


// Foot update plus a nearby jumpjet locomotor's IPersist class-ID query (0x005089D0).
void InfantryClass::unknown_slot288()
{
    FootClass::unknown_slot288();
    if (unknown_494 != 0 && unknown_5d4c00(unknown_494) < 1024 && unknown_508d00()) {
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool InfantryClass::unknown_slot289() -- 0x00508AB0
// TibSun/YR InfantryClass::JumpJet_To_Walk: a grounded jumpjet infantry with a short path left gets a walking
// locomotor that carries the jumpjet one as its piggyback. From ra2-decomp (YR 0x00521EB0): the comip
// `piggy != NULL` tests and Move_To taking the Center_Coord result directly (through CoordStruct's COM
// conversion under RA2_SHIP_MOVE_TO_VALUE). Those comip levels spend the inline budget that sends the
// temporary's release out of line (0x00509860) and keep `this` in ebx, as in the original.

extern "C" GUID const unknown_Walk_clsid_007a1fa0;     // CLSID_WalkLocomotion
extern "C" GUID const CLSID_JumpjetLocomotionClass;

bool InfantryClass::unknown_slot289()
{
    int path_length = 0;
    if (unknown_494 != 0) {
        for (int i = 0; i < 24; i++) {
            if (Path[i] == -1) break;
            if (Path[i] == 8) break;
            path_length++;
        }
    }
    if (path_length >= 4) return false;

    if (unknown_508d00()) {
        UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
        if (piggy != 0 && !piggy.Checked()->Is_Piggybacking()) {
            UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> walk(unknown_Walk_clsid_007a1fa0);
            walk.Checked()->Link_To_Object(this);
            piggy = UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>(walk);
            if (piggy != 0) {
                Path[0] = -1;
                piggy.Checked()->Begin_Piggyback(Locomotor);
                Locomotor = walk;
                Locomotor.Checked()->Move_To(unknown_494->Center_Coord());
                return true;
            }
        }
    }

    return false;
}


// void InfantryClass::unknown_slot294() (0x00508CF0, slot 294)

void InfantryClass::unknown_slot294()
{
    unknown_5c0 = true;
}


// bool InfantryClass::unknown_508d00() (0x00508D00)
// True for a JumpJet infantry type whose locomotor's class ID is
// JumpjetLocomotionClass (asked through IPersist::GetClassID).

extern "C" GUID const CLSID_JumpjetLocomotionClass;    // 0x007A2000

// An IPersist pointer queried from the locomotor (the compiler's
// _com_ptr_t<IPersist> as comip.h expands it: Attach(NULL) on failure,
// Attach(pointer) on success, the release out of line, 0x004A4D00).


bool InfantryClass::unknown_508d00()
{
    if (!Type->JumpJet) {
        return false;
    }
    UnknownPersistPtr persist(Locomotor);
    GUID clsid;
    persist->GetClassID(&clsid);
    return clsid == CLSID_JumpjetLocomotionClass;
}


// bool InfantryClass::unknown_508dd0(Cell const *from, Cell const *to) (0x00508DD0)
// False under the owner's 0x0051FC80 condition, true at once when slot 108
// is positive; false for a 0x004F52F0 owner whose start cell passes
// 0x00479CF0. Otherwise true when the two cells are not connected
// (0x0054F7C0, zone 7) or at least four cells apart; false for the same or
// an adjacent cell; for two or three cells true unless both cells pass
// 0x0055A660 and the path estimate (0x0042AA30) is within 7.

bool InfantryClass::unknown_508dd0(Cell const *from, Cell const *to)
{
    Get_Height_AGL();
    if (House->Unknown_0051FC80()) {
        return false;
    }
    if (Get_Height_AGL() > 0) {
        return true;
    }
    if (House->Unknown_004F52F0()) {
        UnknownCellClass *cell = Map_Cell_At(*from);
        if (cell != 0 && cell->Unknown_00479CF0()) {
            return false;
        }
    }
    if (!Map.Unknown_0054F7C0(*from, *to, 7, unknown_79, (Map_Cell_At(*to)->Flags >> 8) & 1, 0)) {
        return true;
    }
    if (*from == *to) {
        return false;
    }
    int dy = abs(to->Y - from->Y);
    int dx = abs(to->X - from->X);
    int distance = (dx > dy) ? dx : dy;
    if (distance == 1) {
        return false;
    }
    if (distance >= 4) {
        return true;
    }
    if (!Map.Unknown_0055A660(*from, true) || !Map.Unknown_0055A660(*to, true)) {
        return true;
    }
    return Search.Test_Cell_Walk(*from, *to, this, unknown_79, (Map_Cell_At(*to)->Flags >> 8) & 1, 7) > 7;
}


// void InfantryClass::unknown_slot304() (0x00508F30, slot 304)
// With a target (+0x228): FootClass 0x004C4C30 unless deploying; then, for
// owners failing HouseClass 0x004F52F0, Deployer + DeployFire types whose
// DeployFireWeapon has a type without +0x126: while deploying, sequence 31
// when slot 212 refuses the target; otherwise, once slot 212 accepts it,
// sequence 27 when standing, or stop the locomotor, set +0x5CC and draw
// from the scenario's random generator.

void InfantryClass::unknown_slot304()
{
    if (unknown_228 == 0) {
        return;
    }
    if (!unknown_5090d0()) {
        unknown_4c4c30();
    }
    if (Owning_House()->Unknown_004F52F0()) {
        return;
    }
    if (!Type->Deployer || !Type->DeployFire) {
        return;
    }
    int weapon = Type->DeployFireWeapon;
    bool in_range = unknown_slot212(unknown_228, weapon);
    WeaponTypeClass *weapon_type = unknown_slot231(weapon)->WeaponType;
    if (weapon_type == 0 || weapon_type->unknown_126) {
        return;
    }
    if (unknown_5090d0()) {
        if (!in_range) {
            unknown_slot311(31, 0, 0);
        }
        return;
    }
    if (!in_range) {
        return;
    }
    if (!Locomotor.Checked()->Is_Moving()) {
        unknown_slot311(27, 0, 0);
        return;
    }
    Locomotor.Checked()->Stop_Moving();
    unknown_5cc = true;
    Control();
    unknown_id_source_a3d290->Random()(0, 2);   // the scenario's Random() (src/shared/abstract.hpp)
}


// bool InfantryClass::unknown_slot193() (0x00509090)
// The type's Engineer flag.

bool InfantryClass::unknown_slot193()
{
    return Type->Engineer;
}


// bool InfantryClass::unknown_5090a0() const (0x005090A0)
// The type's Deployer flag.

bool InfantryClass::unknown_5090a0() const
{
    return Type->Deployer;
}


// bool InfantryClass::unknown_5090b0() const (0x005090B0)
// True when the type's UndeployDelay is set (above -1).

bool InfantryClass::unknown_5090b0() const
{
    return Type->UndeployDelay > -1;
}


// bool InfantryClass::unknown_5090d0() const (0x005090D0)
// Whether the infantry is playing a deploy sequence (27-30); expanded
// inline in slots 137 and 291.

bool InfantryClass::unknown_5090d0() const
{
    return Sequence == 27 || Sequence == 28 || Sequence == 29 || Sequence == 30;
}


// bool InfantryClass::unknown_509100() const (0x00509100)
// The type's DeployFire flag.

bool InfantryClass::unknown_509100() const
{
    return Type->DeployFire;
}


// int InfantryClass::unknown_509110() (0x00509110; no callers in the
// executable: InfantryClass slot 304 expands the same steps inline)
// A standing soldier plays sequence 27 (deploy-fire) and the delay is that
// sequence's frame count; a moving one stops its locomotor, sets +0x5CC and
// waits the mission control's rate in frames plus 0..2.

int InfantryClass::unknown_509110()
{
    if (!Locomotor.Checked()->Is_Moving()) {
        unknown_slot311(27, 0, 0);
        return Type->DoControls[27].Count;
    }
    Locomotor.Checked()->Stop_Moving();
    unknown_5cc = true;
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// void InfantryClass::unknown_slot82(int arg1) (0x005091C0, slot 82)
// Kills the infantry: full-strength damage with the rules warhead (+0xC74),
// forced.

void InfantryClass::unknown_slot82(int arg1)
{
    int damage = Type->MaxStrength;
    Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
}


// ObjectTypeClass *InfantryClass::unknown_slot50(bool arg1) (0x00509200, slot 50)
// The type shown: the real type when allied to the player and arg1 is
// clear; otherwise the disguise (+0x40C), or the rules default for the
// player's side (+0xB6C, +0xB70 when the player's +0x130 is set or there is
// no player).

ObjectTypeClass *InfantryClass::unknown_slot50(bool arg1)
{
    if (House->Is_Ally(unknown_player_a35db4) && !arg1) {
        return Type;
    }
    if (unknown_40c != 0) {
        return unknown_40c;
    }
    if (unknown_player_a35db4 != 0 && unknown_player_a35db4->unknown_130 == 0) {
        return Rules->unknown_b6c;
    }
    return Rules->unknown_b70;
}


// HouseClass *InfantryClass::unknown_slot51(bool arg1) (0x00509270, slot 51)
// The infantry's house when allied to the player and arg1 is clear;
// otherwise +0x410, or the player's house when that is not set.

HouseClass *InfantryClass::unknown_slot51(bool arg1)
{
    if (House->Is_Ally(unknown_player_a35db4) && !arg1) {
        return House;
    }
    return unknown_410 != 0 ? unknown_410 : unknown_player_a35db4;
}


// void InfantryClass::unknown_slot255(TechnoClass *techno) (0x005092B0, slot 255)
// Disguises as another infantry: TechnoClass's slot 255, then copies its
// disguise when it is disguised itself, else its type and house.

void InfantryClass::unknown_slot255(TechnoClass *techno)
{
    if (techno != 0 && techno->Kind_Of() == 15) {
        TechnoClass::unknown_slot255(techno);
        if (techno->unknown_slot48()) {
            unknown_40c = techno->unknown_slot50(true);
            unknown_410 = techno->unknown_slot51(true);
        } else {
            unknown_40c = techno->Class_Of();
            unknown_410 = techno->Owning_House();
        }
    }
}


// void InfantryClass::unknown_slot256() (0x00509330, slot 256)
// For PermaDisguise types: sets +0x170, records the house (+0x410) and the
// rules object for it (+0x40C; by the house's +0x130); otherwise clears
// +0x170.

void InfantryClass::unknown_slot256()
{
    if (Type->PermaDisguise) {
        unknown_170 = true;
        unknown_410 = House;
        if (House->unknown_130 == 0) {
            unknown_40c = Rules->unknown_b6c;
        } else {
            unknown_40c = Rules->unknown_b70;
        }
    } else {
        unknown_170 = false;
    }
}


// bool InfantryClass::unknown_slot49(HouseClass *house) (0x00509390, slot 49)
// True when slot 48 holds, house is not allied to the owner, the cell under
// the infantry fails the cell test 0x0047C300 for house, and +0x410 is
// unset, house itself, or allied to house.

bool InfantryClass::unknown_slot49(HouseClass *house)
{
    if (unknown_slot48()) {
        Cell cell = Coord_Cell(Center_Coord());
        UnknownCellClass *object = (UnknownCellClass *)&Map[cell];
        if (!House->Is_Ally(house) && !object->unknown_47c300(house->ArrayIndex)) {
            if (unknown_410 == house || unknown_410 == 0 || house->Is_Ally(unknown_410)) {
                return true;
            }
        }
    }
    return false;
}


// bool InfantryClass::unknown_slot274() (0x00509450, slot 274)
// TechnoClass's: a jump to its out-of-line copy (0x006D8F20).

bool InfantryClass::unknown_slot274()
{
    return TechnoClass::unknown_slot274();
}


// bool InfantryClass::unknown_slot275() (0x00509460, slot 275)
// TechnoClass's: a jump to its out-of-line copy (0x006D8F40).

bool InfantryClass::unknown_slot275()
{
    return TechnoClass::unknown_slot275();
}


// bool InfantryClass::unknown_slot310() (0x00509470, slot 310)
// Whether a Deployer type is playing one of the deploy sequences (27-30).

bool InfantryClass::unknown_slot310()
{
    return Type->Deployer && (Sequence == 27 || Sequence == 28 || Sequence == 29 || Sequence == 30);
}


// void InfantryClass::unknown_5094b0(BuildingClass *building) (0x005094B0;
// no callers in the executable: InfantryClass slot 93 expands the same steps
// inline for an infantry arriving in the building's cell)
// An Occupier leaves the map (slot 52), joins the building's occupants
// (+0x554) and the building refreshes its cell; an Assaulter is handed to the
// building (0x004536D0), stops (slot 260) and moves to the building's centre.

void InfantryClass::unknown_5094b0(BuildingClass *building)
{
    if (Type->Occupier) {
        Limbo();
        building->unknown_554.Add(this);
        building->Unknown_006D9610(building->Get_Cell_Ptr());
        return;
    }
    if (Type->Assaulter) {
        building->Unknown_004536D0(this);
        Assign_Destination(0, true);
        Scatter(building->Center_Coord(), true, true);
    }
}


// bool InfantryClass::INoticeSink_Unknown(int code, int arg) (0x00509590;
// InfantryClass's INoticeSink vtable 0x007A351C, `this` adjusted from the
// +8 subobject)
// Notice 0x117B for a BalloonHover or JumpJet type in radio contact: tell
// the contact message 8, then attack the target at +0x1B0 unless it is the
// navigation target (+0x494), otherwise clear the navigation and move to the
// invalid coordinate. Notice 0x117C for a Crashable type: slot 109 (0) and
// sequence 0x24. True when handled.

bool InfantryClass::INoticeSink_Unknown(int code, int arg)
{
    if ((Type->BalloonHover || Type->JumpJet) && code == 0x117B && HasAnyLink()) {
        unknown_slot149(8);
        if (unknown_1b0 != 0 && unknown_1b0 != unknown_494) {
            Assign_Destination(unknown_1b0, true);
            return true;
        }
        Unknown_004CD9B0();
        Scatter(unknown_geometry_l, true, false);
        return true;
    }
    if (Type->Crashable && code == 0x117C) {
        Set_Height_AGL(0);
        unknown_slot311(0x24, 1, 0);
        return true;
    }
    return false;
}


// bool InfantryClass::unknown_slot204() (0x00509670, slot 204)
// Whether on mission 5 or 11 with sequence 0, 9, 10 or 32.

bool InfantryClass::unknown_slot204()
{
    if ((Get_Mission() == 5 || Get_Mission() == 11)
        && (Sequence == 0 || Sequence == 9 || Sequence == 10 || Sequence == 32)) {
        return true;
    }
    return false;
}


// void InfantryClass::unknown_slot205() (0x005096B0, slot 205)
// Slot 311 with action 32 while slot 204 holds.

void InfantryClass::unknown_slot205()
{
    if (unknown_slot204()) {
        unknown_slot311(32, 0, 0);
    }
}


// Prone/crawling animation states (receiver unused; member ABI hypothesis).
bool InfantryClass::Is_Prone_Sequence_005096e0(int sequence) const
{
    if (sequence != 9 && sequence != 10 && sequence != 18 && sequence != 19
        && sequence != 23 && sequence != 32) {
        return false;
    }
    return true;
}


// Sequence check (receiver unused; member ABI hypothesis).
bool InfantryClass::Is_Busy_Sequence_00509710(int sequence) const
{
    return sequence == 11 || sequence == 12 || sequence == 13 || sequence == 14 || sequence == 15
        || sequence == 34 || sequence == 35 || sequence == 36 || sequence == 20 || sequence == 21;
}


// Whether the current sequence blocks firing; called by TechnoClass at 0x006C9B87.
bool InfantryClass::Is_Busy_00509760() const
{
    return Is_Busy_Sequence_00509710(Sequence);
}


// int InfantryClass::Size_Of() const (0x005097B0)
// Compiler-emitted from shared/infantry.hpp in this unit; no source of its own.


// HRESULT __stdcall InfantryClass::GetClassID(GUID *clsid) (0x005097C0)
// Compiler-emitted from shared/infantry.hpp in this unit; no source of its own.


// int InfantryClass::Kind_Of() const (0x00509800)
// Compiler-emitted from shared/infantry.hpp in this unit; no source of its own.


// void *InfantryClass::`scalar deleting destructor'(unsigned int) (0x00509810)
// Compiler-emitted from shared/infantry.hpp in this unit; no source of its own.


// Assigns a held locomotor and transfers its COM references (0x00509830).
// Compiler COM support, reconstructed from its observed pointer operations.
UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> &
UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::operator=(ILocomotion *pointer)
{
    if (Pointer != pointer) {
        ILocomotion *previous = Pointer;
        Pointer = pointer;
        if (Pointer != 0) {
            Pointer->AddRef();
        }
        if (previous != 0) {
            previous->Release();
        }
    }
    return *this;
}


// void UnknownComPtr<IPiggyback>::Release_Held() -- 0x00509860
// comip's _Release for the IPiggyback pointer: releases what is held, if anything. The destructor is the inline
// level above it; JumpJet_To_Walk 0x00508AB0 calls this copy for its converted temporary.
void UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}


// Converts the held locomotor to IPersist (0x00509870): emitted from the inline
// constructor in shared/infantry.hpp, called by 0x005021DB and 0x00508B0B.


// Binary_Search<IndexClass<long, AbstractClass *>::NodeElement> (0x005098E0)
// Template function of shared/ini_class.hpp (fastcall), instanced by the
// object index's Search_For_Node (shared/object_index.hpp); this unit keeps
// the copy. No source of its own.
