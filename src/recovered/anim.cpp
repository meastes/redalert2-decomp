// anim.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/anim.json carries the addresses and the rows that
// stay in their own files.


#define RA2_POINT_LENGTH
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_COORD_TPOINT3D
#define RA2_ANIM_INLINE_CENTER
#define RA2_BOUNCE_DECLARE_MEMBERS
#define RA2_STAGE_SETTERS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/random.hpp"
#include "shared/map_view.hpp"
#include "shared/options.hpp"
#include "shared/id_source.hpp"
#include "shared/color_scheme.hpp"
#include "shared/cell_class.hpp"
#include "shared/house.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/frame_rate.hpp"
#include "shared/combat.hpp"
#include "shared/palette.hpp"
#include "shared/terrain.hpp"
#include "shared/crt_memory.hpp"
#include "shared/cell.hpp"
#include "shared/overlay.hpp"
#include "shared/crt_string.hpp"
#include "shared/fast_math.hpp"
#include "shared/facing.hpp"
#include "shared/adjacent_cell.hpp"
#include "shared/building_light.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/rect.hpp"
#include "shared/object.hpp"


// (RA2_POINT_LENGTH, RA2_COORD_FROM_CELL_INLINE: AnimClass 0x004237B0.)
// Header-static dynamic initializers of the unit at 0x0041FF20: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0041FF20-0x00420101); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0041FF40, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x0041FF70, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x0041FF90, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x0041FFB0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x0041FFD0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x0041FFF0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x00420010, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x00420050, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x00420080, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x004200B0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x004200E0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// Dynamic initializer (0x004200F0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0041ff20.cpp; no source of its own.


// AnimClass::AnimClass(AnimTypeClass const *type, CoordStruct const &coord, int loop_delay, int loops, int flags, int zadjust) (0x00420110)
// Starts an animation of a type at a coordinate: takes the type's defaults
// (sort and z adjust, the frame count when the type has none, a random rate
// when the type asks for one), places it (a bouncing or meteor animation
// first picks a random launch velocity and start position above the
// coordinate), lights the map when it is the type that reveals ground, sets
// the loop count and, with no loop delay, starts it (0x004226D0).
// The existing Anim layout splits the contiguous 0x50-byte BounceClass at +0x108
// into its leading state and two quaternions. Calls use that evidenced full view.

AnimClass::AnimClass(AnimTypeClass const *type, CoordStruct const &coord, int loop_delay, int loops, int flags, int zadjust) :
    Class((AnimTypeClass *)type),
    unknown_b4(0),
    unknown_b8(-1),
    unknown_bc(0),
    unknown_c0(-1),
    unknown_e4(1000),
    ZAdjust(0),
    YSortAdjust(0),
    unknown_f0(unknown_geometry_l),
    unknown_fc(0),
    unknown_100(false),
    unknown_101(false),
    unknown_102(false),
    unknown_103(false),
    unknown_104(0),
    unknown_138(0.0f, 0.0f, 0.0f, 1.0f),
    unknown_148(0.0f, 0.0f, 0.0f, 1.0f),
    unknown_158(false),
    unknown_159(false),
    unknown_15c(0),
    LoopDelay(loop_delay),
    unknown_168(1.0),
    unknown_170(flags),
    unknown_174(false),
    Loops(1),
    unknown_176(false),
    unknown_177(false),
    unknown_178(false),
    unknown_179(false),
    unknown_17a(false),
    unknown_17b(false),
    unknown_17c(true),
    unknown_17d(false),
    unknown_17e(false)
{
    Palette.Buffer[0] = 0;
    AbstractClass::Create_ID();
    unknown_180.Init();
    unknown_194.Init();
    Anims.Add(this);
    unknown_7d = true;
    YSortAdjust = Class->YSortAdjust;
    if (Class->End == -1) {
        Class->End = ((ShapeFileStruct *)Class->Get_Image_Data())->FrameCount;
        if (Class->Shadow) {
            Class->End = Class->End / 2;
        }
    }
    if (Class->LoopEnd == -1) {
        Class->LoopEnd = Class->End;
    }
    if (zadjust != 0) {
        ZAdjust = zadjust;
    } else {
        ZAdjust = Class->ZAdjust;
    }
    int rate = Class->Rate;
    if ((Class->RandomRateMin != 0 || Class->RandomRateMax != 0) && Class->RandomRateMin <= Class->RandomRateMax) {
        rate = unknown_id_source_a3d290->Random()(Class->RandomRateMin, Class->RandomRateMax);
    }
    if (Class->Normalized) {
        rate = Options.Normalize_Delay(rate);
        Set_Rate(rate);
    } else {
        Set_Rate(rate);
    }
    Stage = 0;
    if (Class->Layer == 3 || Class->Layer == 4) {
        unknown_func_5d4920(Rules->unknown_6dc);
    } else {
        ObjectClass::Set_Height_AGL(0);
    }
    if (Class->Reverse) {
        Set_Stage(Class->LoopEnd);
        Set_Step(-Fetch_Step());
    }
    if (!Class->Bouncer && !Class->IsMeteor) {
        ObjectClass::Unlimbo(&coord, 0);
    } else {
        unknown_174 = true;
        if (Class->IsMeteor) {
            int rand_y = unknown_id_source_a3d290->Random()();
            int rand_x = unknown_id_source_a3d290->Random()();
            Vector3 velocity(
                abs(rand_x) % (int)(Class->MaxXYVel * 2) - Class->MaxXYVel,
                abs(rand_y) % (int)(Class->MaxXYVel * 2) - Class->MaxXYVel,
                Class->MinZVel);
            if (velocity.X < -velocity.Y) {
                velocity.X = -velocity.X;
                velocity.Y = -velocity.Y;
            }
            int height = 70 - abs(unknown_id_source_a3d290->Random()() % 20);
            CoordStruct start((int)(coord.X - height * velocity.X), (int)(coord.Y - height * velocity.Y), (int)(coord.Z - height * velocity.Z));
            ObjectClass::Unlimbo(&start, 0);
            ((BounceClass *)&unknown_108)->Init(Center_Coord(), Class->Elasticity, 1.4f, 0, velocity, 0);
        } else {
            ObjectClass::Unlimbo(&coord, 0);
            CoordStruct position;
            position = Center_Coord() + CoordStruct(0, 0, 10);
            int rand_z = unknown_id_source_a3d290->Random()();
            int rand_y = unknown_id_source_a3d290->Random()();
            int rand_x = unknown_id_source_a3d290->Random()();
            float vy = abs(rand_y) % (int)(Class->MaxXYVel * 2) - Class->MaxXYVel;
            Vector3 velocity(
                abs(rand_x) % (int)(Class->MaxXYVel * 2) - Class->MaxXYVel,
                vy,
                abs(rand_z) % (int)(Class->unknown_2e0 - Class->MinZVel + 1.0) + Class->MinZVel);
            ((BounceClass *)&unknown_108)->Init(position, Class->Elasticity, 1.4f, 0, velocity, 0);
        }
    }
    if (Class == Rules->DropZoneAnim) {
        Map.Sight_From(coord, Rules->DropZoneRadius / 256, unknown_player_a35db4, false, 0, 0, true, 0);
        Map.Sight_From(coord, Rules->DropZoneRadius / 256, unknown_player_a35db4, false, 0, 0, true, 1);
    }
    if (loops <= 1) {
        loops = 1;
    }
    Loops = Class->LoopCount * loops;
    Loops = (Loops > 1) ? Loops : 1;
    if (LoopDelay == 0) {
        unknown_func_4226d0();
    }
}


// AnimClass::AnimClass() (0x004208E0)
// An empty animation (no type) registered in Anims.

AnimClass::AnimClass() :
    Class(0),
    unknown_b4(0),
    unknown_b8(-1),
    unknown_bc(0),
    unknown_c0(-1),
    unknown_e4(1000),
    ZAdjust(0),
    YSortAdjust(0),
    unknown_100(false),
    unknown_101(false),
    unknown_102(false),
    unknown_103(false),
    unknown_104(0),
    unknown_138(0.0f, 0.0f, 0.0f, 1.0f),
    unknown_148(0.0f, 0.0f, 0.0f, 1.0f),
    unknown_159(false),
    LoopDelay(0),
    unknown_168(0),
    unknown_174(false),
    Loops(1),
    unknown_176(false),
    unknown_177(false),
    unknown_17b(false),
    unknown_17c(true),
    unknown_17d(false),
    unknown_17e(false)
{
    Anims.Add(this);
    unknown_180.Init();
    unknown_194.Init();
    unknown_7d = true;
}


// AnimClass::~AnimClass() (0x00420A90)
// Detaches itself everywhere; while unknown_flag_a40958 is set it also lets
// go of the object it follows (clearing that object's flag when no other
// animation follows it) and leaves the map (slot 52, expanded here). Then
// it leaves the object list, releases its type's image and audio, and
// leaves Anims (or the 0x00A35E60 list for an ID of -2).
// Needs ObjectClass::Limbo declared bool (see slot 52).

AnimClass::~AnimClass()
{
    Unknown_006ECB90(this, true);
    if (unknown_flag_a40958) {
        if (unknown_b4 != 0) {
            ObjectClass *object = unknown_b4;
            int index;
            for (index = 0; index < Anims.Count(); index++) {
                AnimClass *anim = Anims[index];
                if (anim != this && anim->unknown_b4 == object) {
                    break;
                }
            }
            if (index == Anims.Count()) {
                object->Fire_Out();
                object->unknown_78 = false;
            }
            unknown_b4 = 0;
        }
        Limbo();
    }
    unknown_objects_ac0cb0.Delete(this);
    Class->Free_Image();
    unknown_180.Unknown_00406070();
    unknown_194.Unknown_00406070();
    unknown_180.Unknown_00405C10();
    unknown_194.Unknown_00405C10();
    unknown_b4 = 0;
    Class = 0;
    if (Fetch_ID() == -2) {
        unknown_anims_a35e60.Delete(this);
    } else {
        Anims.Delete(this);
    }
}


// int __fastcall Anim_From_Name(char const *name) (0x00420CD0)
// Index in AnimTypes of the type with this ID; -1 for null or "<none>".

int __fastcall Anim_From_Name(char const *name)
{
    if (name != 0 && _stricmp(name, "<none>") != 0) {
        for (int index = 0; index < AnimTypes.Count(); index++) {
            if (_stricmp(AnimTypes[index]->ID, name) == 0) {
                return index;
            }
        }
    }
    return -1;
}


// void __fastcall Unknown_00420D30(ObjectClass *object) (0x00420D30)
// Ends the loops of the animations attached to an object.

void __fastcall Unknown_00420D30(ObjectClass *object)
{
    if (object != 0) {
        for (int index = 0; index < Anims.Count(); index++) {
            AnimClass *anim = Anims[index];
            if (anim->unknown_b4 == object) {
                anim->Loops = 0;
            }
        }
    }
}


// int AnimClass::Sort_Y() const (0x00420D70)
// The base value (ObjectClass slot 45, a sort height) plus YSortAdjust.

int AnimClass::Sort_Y() const
{
    return ObjectClass::Sort_Y() + YSortAdjust;
}


// Center_Coord defined inline in the AnimClass declaration.


// bool AnimClass::Render(Rect *bounds, bool arg2, bool arg3) (0x00420E20)
// Refused while the animation waits out its loop delay; otherwise the
// ObjectClass slot. Needs ObjectClass::Render declared bool
// (0x005D34E0 returns al).

bool AnimClass::Render(Rect *bounds, bool arg2, bool arg3)
{
    if (LoopDelay != 0) {
        return false;
    }
    return ObjectClass::Render(bounds, arg2, arg3);
}


// void AnimClass::unknown_slot67(Point2D const &point, Rect const &cliprect) (0x00420E50, slot 67)
// Draws the animation (TibSun AnimClass::Draw_It) at the screen point point
// within the window cliprect: skipped when the frame rate is too low for its
// detail level, when hidden or fogged; the type's translucency at enough
// detail, the drawer and brightness of its palette, its cell or its own,
// then the bouncing shadow, the tiled column, the flat or upright frame and
// the shadow frame.

void AnimClass::unknown_slot67(Point2D const &point, Rect const &cliprect)
{
    if (unknown_a6eb1c < Unknown_0053D840() && Class->DetailLevel > 1) {
        return;
    }
    if (!unknown_17d && Class->DetailLevel <= unknown_a40b2c && (!unknown_179 || !Class->ShouldFogRemove)) {
        ShapeFileStruct const *shapefile = (ShapeFileStruct const *)Get_Image_Data();
        if (shapefile != 0) {
            int flags = unknown_170;
            int shapenum = Class->Start + Stage;
            if (unknown_101) {
                flags |= 4;
            }

            if (Class->TranslucencyDetailLevel <= unknown_a40b2c) {
                if (Class->Translucent) {
                    if (unknown_158 >= 15) return;
                    if (Stage > Class->End * 0.6) {
                        flags |= 6;
                    } else if (Stage > Class->End * 0.4) {
                        flags |= 4;
                    } else if (Stage > Class->End * 0.2) {
                        flags |= 2;
                    }
                } else if (Class->Translucency > 0) {
                    if (unknown_158 >= 15) return;
                    if (Class->Translucency == 25) {
                        flags |= 2;
                    } else if (Class->Translucency == 50) {
                        flags |= 4;
                    } else if (Class->Translucency == 75) {
                        flags |= 6;
                    }
                } else if (unknown_158 != 0) {
                    if (unknown_158 > 15) return;
                    if (unknown_158 > 10) {
                        flags |= 4;
                    } else if (unknown_158 > 5) {
                        flags |= 4;
                    } else {
                        flags |= 2;
                    }
                }
            }

            if ((flags & 1) == 0) {
                flags |= 0x800;
            }

            int height = Get_Height_AGL();
            int brightness = 1000;
            ConvertClass *convert;

            if (Class->IsVeins) {
                convert = ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter;
                if (!Class->UseNormalLight) {
                    brightness = Map[unknown_slot42()].unknown_10c;
                }
            } else if (unknown_176) {
                CellClass *cellptr = &Map[Cell(unknown_slot42())];
                if (cellptr->unknown_34 == 0) {
                    cellptr->Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
                }
                convert = (ConvertClass *)cellptr->unknown_34;
                if (!Class->UseNormalLight) {
                    brightness = cellptr->unknown_10c;
                }
            } else if (unknown_bc != 0) {
                convert = unknown_bc;
                if (!Class->UseNormalLight) {
                    brightness = unknown_e4;
                }
            } else {
                convert = Unit_Draw_Convert_008323C0;
                if (Class->AltPalette) {
                    convert = ColorSchemes[0]->Converter;
                }
                if (!Class->UseNormalLight) {
                    brightness = Map[Cell(unknown_slot42())].unknown_10a;
                }
            }

            if (unknown_174) {
                CC_Draw_Shape(unknown_surface_83995c, convert, shapefile, shapenum, point + Point2D(0, Class->YDrawOffset + Unknown_006A2970(height)), cliprect, 0x2601, 0, Class->YDrawOffset - Unknown_006A2970(Get_Height()), 0, 1000, 0, 0, Point2D(0, 0));
            }

            if (Class->Tiled) {
                int frameheight = shapefile->Get_Frame_Rect(0).Height;
                Point2D origin = point;
                Point2D drawpoint = origin - Point2D(0, frameheight / 2);
                bool done = false;
                int height_offset = ZAdjust + Class->YDrawOffset - Unknown_006A2970(Get_Height()) - 50;
                while (!done) {
                    CC_Draw_Shape(unknown_surface_83995c, Unit_Draw_Convert_008323C0, shapefile, shapenum, Point2D(origin.X, drawpoint.Y + Class->YDrawOffset), unknown_rect_839630, flags | 0x2000, 0, height_offset, 2, brightness, 0, 0, Point2D(0, 0));
                    if (drawpoint.Y < 0) done = true;
                    drawpoint.Y -= frameheight;
                    height_offset -= frameheight + frameheight / 2;
                }
            } else if (Class->Flat) {
                CC_Draw_Shape(unknown_surface_83995c, convert, shapefile, shapenum, point + Point2D(0, Class->YDrawOffset), cliprect, flags | 0x2000, 0, ZAdjust + Class->YDrawOffset - Unknown_006A2970(Get_Height()) - 3, 0, brightness, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_83995c, convert, shapefile, shapenum, point + Point2D(0, Class->YDrawOffset), cliprect, flags | 0x2000, 0, ZAdjust + Class->YDrawOffset - Unknown_006A2970(Get_Height()) - 2, 2, brightness, 0, 0, Point2D(0, 0));
                if (Class->Shadow) {
                    CC_Draw_Shape(unknown_surface_83995c, convert, shapefile, shapenum + shapefile->Get_Count() / 2, point, cliprect, flags & ~6 | 0x601, 0, -2 - Unknown_006A2970(Get_Height()), 0, 1000, 0, 0, Point2D(0, 0));
                }
            }
        }
    }
}


// bool AnimClass::Mark(int arg1) (0x00421500)
// The ObjectClass result as a truth value.

bool AnimClass::Mark(int arg1)
{
    return ObjectClass::Mark(arg1) ? true : false;
}


// Cell const *AnimClass::Occupy_List(int arg1) (0x00421520)
// No cell: a function-local static (0x7FFF, 0x7FFF).

Cell const *AnimClass::Occupy_List(int arg1)
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    return &unknown_none.Cell;
}


// Exit thunk of the function-local static in AnimClass::Occupy_List
// (0x00421570)
// Compiler generated ($E) right after unknown_slot64: UnknownStaticCell's empty
// destructor, registered with atexit on first use.


// int AnimClass::unknown_slot116() (0x00421580)
// One bounce step (UnknownBounce 0x004370E0): a meteor speeds up; on a
// bounce (1) the type's BounceAnim is spawned and every object in the cell
// within DamageRadius (|dx| + |dy|) takes the type's Damage with its
// Warhead, scaled by distance; at rest (2) the animation ends (slot 60).
// The position follows the bounce state; returns the step's result.


int AnimClass::unknown_slot116()
{
    UnknownBounce &bounce = unknown_108;
    int state = bounce.Unknown_004370E0();
    if (Class->IsMeteor) {
        bounce.unknown_2c += bounce.unknown_08;
    }
    switch (state) {
    case 1:
        {
            if (Class->BounceAnim) {
                new AnimClass(Class->BounceAnim, Center_Coord(), 0, 1, 0x600, 0);
            }
            UnknownAnimCell *cell = (UnknownAnimCell *)&Map[unknown_108.Unknown_00436F80()];
            for (ObjectClass *object = cell->unknown_e4; object != 0; object = object->unknown_30) {
                CoordStruct delta = unknown_108.Unknown_00436F80() - object->Center_Coord();
                int distance = abs(delta.X) + abs(delta.Y);
                if (distance <= Class->DamageRadius) {
                    int damage = Class->Damage;
                    object->Take_Damage(&damage, Unknown_006A2970(distance), (WarheadTypeClass *)Class->Warhead, 0, 0, 0);
                }
            }
        }
        break;
    case 2:
        Delete_Me();
        break;
    }
    Set_Coord(unknown_108.Unknown_00436F80());
    return state;
}


// AnimClass AI 00421710: RA2 visibility, bounce effects, tiberium and frame lifecycle.
extern DynamicVectorClass<OverlayTypeClass *> OverlayTypes;
extern Rect unknown_rect_839630;
bool __fastcall Building_Player_Detector_Covers_Cell_00438a60(HouseClass const *house, CellClass const *cell);
void AnimClass::AI()
{
    if (!unknown_178 && Class->StartSound!=-1)
        Voc_Update_At(Center_Coord(),&unknown_180);
    if (Class->IsFlamingGuy) {
        Flaming_Guy_AI();
        ObjectClass::AI();
    }
    if (Class->PsiWarning) {
        CellClass *cell=&Map[Center_Coord()];
        HouseClass *house=unknown_160;
        bool covered=Building_Player_Detector_Covers_Cell_00438a60(house,cell);
        unknown_17d=!covered;
    }
    if (Class==Rules->Behind) {
        if (!Options.ShowHidden) unknown_17d=true;
        else unknown_17d=false;
    }
    if (Class->HideIfNoOre) {
        AbstractClass *object=Get_Cell_Ptr();
        CellClass *cell=(CellClass *)object;
        if (!cell || !cell->Get_Tiberium_Value()) unknown_17d=true;
        else unknown_17d=false;
    }
    if (unknown_103 && unknown_104==Stage) unknown_103=false;
    if (unknown_174) {
        int result=unknown_slot116();
        if (result==2 || result==1) {
            CellClass &cell=Map[Position_Coord_004f3f40()];
            int land=cell.unknown_ec;
            bool water=land==2;
            int z=Position_Coord_004f3f40().Z;
            int height=Map.Get_Height_GL(Position_Coord_004f3f40())+unknown_geometry_j;
            bool bridge=z>=height;
            if (water && !bridge) {
                if (Class->IsMeteor) {
                    new AnimClass(Rules->SplashList[Rules->SplashList.Count()-1],Position_Coord_004f3f40()+CoordStruct(0,0,3),0,1,0x600,0);
                } else {
                    new AnimClass(Rules->unknown_09c,Position_Coord_004f3f40(),0,1,0x600,0);
                    new AnimClass(Rules->SplashList[0],Position_Coord_004f3f40()+CoordStruct(0,0,3),0,1,0x600,0);
                }
            } else {
                if (Class->ExpireAnim) {
                    Vector3 position=unknown_108.Position;
                    new AnimClass(Class->ExpireAnim,CoordStruct((int)position.X,(int)position.Y,(int)position.Z),0,1,0x2600,-30);
                    Unknown_0047E2C0(unknown_108.Unknown_00436F80(),(int)Class->Damage,0,Class->Warhead,1);
                    Unknown_0047F600(unknown_108.Unknown_00436F80(),(int)Class->Damage,Class->Warhead,0,0);
                }
            }
            if (!water || bridge) {
                CoordStruct coord=unknown_108.Unknown_00436F80();
                if (Class->Spawns && Class->SpawnCount>0) {
                    int first=Random_Pick(0,Class->SpawnCount);
                    int count=first+Random_Pick(0,Class->SpawnCount);
                    for (int i=0;i<count;i++)
                        new AnimClass(Class->Spawns,coord,0,1,0x600,0);
                }
                Rect update(0,0,0,0);
                if (Class->IsTiberium && !bridge) {
                    for (int x=-Class->TiberiumSpreadRadius;x<=Class->TiberiumSpreadRadius;x++) {
                        for (int y=-Class->TiberiumSpreadRadius;y<=Class->TiberiumSpreadRadius;y++) {
                            double sq=(double)x*x+(double)y*y;
                            int distance=(int)Fast_Sqrt(sq);
                            if (distance<=Class->TiberiumSpreadRadius) {
                                // Original uses x's facing table slot for every y iteration.
                                CellClass *cell=&Map[Cell(unknown_adjacent_851a70[x&7]+Cell(coord))];
                                bool germinate=cell->Can_Tiberium_Germinate(0);
                                if (germinate && Class->TiberiumSpawnType) {
                                    new OverlayClass(OverlayTypes[Class->TiberiumSpawnType->ArrayIndex+Random_Pick(0,3)],cell->Fetch_CellID(),-1);
                                    unsigned char data=(unsigned char)Random_Pick(0,2);
                                    cell->OverlayData=data;
                                    Rect overlay=cell->Overlay_Render_Rect();
                                    overlay.Y-=unknown_rect_839630.Y;
                                    update=Union(update,overlay);
                                }
                            }
                        }
                    }
                    unknown_tactical_83996c->Register_Dirty_Area(update,false);
                }
            }
            Delete_Me();
            return;
        }
    }
    if (unknown_7d && !unknown_17b) {
        if (Class->TrailerAnim && (Class->TrailerSeperation==1 || Frame%Class->TrailerSeperation==0))
            new AnimClass(Class->TrailerAnim,Center_Coord(),1,1,0x600,0);
    }
    if (Class==Rules->DropZoneAnim && Map[Center_Coord()].Cell_Building()) unknown_17b=true;
    if (unknown_17b) { Delete_Me(); return; }
    if (unknown_17c) { unknown_17c=false; return; }
    if (LoopDelay) {
        --LoopDelay;
        if (!LoopDelay) unknown_func_4226d0();
    } else if (unknown_7d) {
        if (Class->IsVeins) unknown_func_422f50();
        if (Class->IsAnimatedTiberium) {
            int overlay=Map[Center_Coord()-CoordStruct(384,384,0)].OverlayIndex;
            if (overlay==-1 || OverlayTypes[overlay]->CellAnim!=Class) unknown_17b=true;
        }
        if (Class->End==-1) {
            Class->End=((ShapeFileStruct const *)Class->Get_Image_Data())->Get_Count();
            if (Class->Shadow) Class->End/=2;
        }
        if (Class->LoopEnd==-1) Class->LoopEnd=Class->End;
        Mark(2);
        if (!unknown_17e && !unknown_102 && StageClass::Graphic_Logic()) {
            int stage=Stage;
            if (Class->Damage>0.0 && !unknown_174) {
                if (unknown_b4 && unknown_b4->Kind_Of()==36) unknown_168+=Class->Damage*5.0;
                else unknown_168+=Class->Damage;
                if (unknown_168>=1.0 && !unknown_178) {
                    int damage=(int)unknown_168;
                    unknown_168-=damage;
                    if (strcmp(Class->ID,"INVISO")==0)
                        Unknown_0047E2C0(Center_Coord(),damage,0,Rules->unknown_c74,1);
                    else Unknown_0047E2C0(Center_Coord(),damage,0,Rules->FlameDamage2,1);
                    if (!unknown_7d) return;
                }
            }
            if (Class->unknown_268 && Class->Start+stage==Class->unknown_268 && !unknown_174)
                unknown_func_4228f0();
            if (Class->PingPong) {
                if ((Loops<=1 && (stage>=Class->End || stage==0)) ||
                    (Loops>1 && (stage>=Class->LoopEnd-Class->Start || stage==Class->Start))) {
                    Step=-Step;
                    return;
                }
            }
            if ((Loops<=1 && stage>=Class->End) || (Loops>1 && stage>=Class->LoopEnd-Class->Start)
                || (Class->Shadow && stage>=Class->LoopEnd-Class->Start) || (Class->Reverse && stage<=Class->Start)) {
                if (Loops && Loops!=255) --Loops;
                if (Loops) {
                    if (Class->Reverse) Stage=Class->LoopEnd;
                    else Stage=Class->LoopStart-Class->Start;
                    if (Class->RandomLoopDelayMin || Class->RandomLoopDelayMax)
                        LoopDelay=Random_Pick(Class->RandomLoopDelayMin,Class->RandomLoopDelayMax);
                } else {
                    if (Class->Next) {
                        Class=Class->Next;
                        if (Class->End==-1) {
                            Class->End=((ShapeFileStruct const *)Class->Get_Image_Data())->Get_Count();
                            if (Class->Shadow) Class->End/=2;
                        }
                        if (Class->LoopEnd==-1) Class->LoopEnd=Class->End;
                        unknown_17b=false;
                        Loops=(unsigned char)Class->LoopCount;
                        unknown_168=0.0;
                        int delay=Class->Rate;
                        if (Class->RandomRateMin || Class->RandomRateMax)
                            delay=Random_Pick(Class->RandomRateMin,Class->RandomRateMax);
                        if (Class->Normalized) StageClass::Set_Rate(Options.Normalize_Delay(delay));
                        else StageClass::Set_Rate(delay);
                        Stage=Class->Start;
                        unknown_func_4226d0();
                    } else {
                        unknown_159=true;
                        Delete_Me();
                    }
                }
            }
        }
    }
}


// void AnimClass::Attach_To(ObjectClass *object) (0x00422540; name hypothesis)
// Moves the animation from the object it follows (unknown_b4) to object:
// leaving clears the old object's flag (+0x78, through its slot 89) when no
// other animation still follows it and keeps the animation at its current
// center; joining stores the offset from the new object's center (slot
// 103). The map entry is removed and re-added around each change.


void AnimClass::Attach_To(ObjectClass *object)
{
    if (unknown_b4 != 0) {
        ObjectClass *old = unknown_b4;
        bool on_map = unknown_74;
        if (on_map) {
            Map.Remove(this);
        }
        int index;
        for (index = 0; index < Anims.Count(); index++) {
            AnimClass *anim = Anims[index];
            if (anim != this && anim->unknown_b4 == unknown_b4) {
                break;
            }
        }
        if (index == Anims.Count()) {
            old->Fire_Out();
            old->unknown_78 = false;
        }
        CoordStruct coord = Center_Coord();
        unknown_b4 = 0;
        Set_Coord(coord);
        if (on_map) {
            Map.Submit(this);
        }
    }
    if (object != 0) {
        CoordStruct coord = Center_Coord();
        Map.Remove(this);
        object->unknown_78 = true;
        unknown_b4 = object;
        CoordStruct offset = coord - object->Center_Coord();
        Set_Coord(offset);
        Map.Submit(this);
    }
}


// void AnimClass::unknown_func_422680(AbstractClass *value) (0x00422680)

void AnimClass::unknown_func_422680(AbstractClass *value)
{
    unknown_15c = value;
}


class Leaf_422690 { public: void get(unsigned int house); };
void Leaf_422690::get(unsigned int house) { unsigned int animation_address = (unsigned int)this; unsigned int house_address; unsigned int unused_value; house_address = house; *(unsigned int*)(animation_address + 0x160) = house_address; }


// int AnimClass::In_Which_Layer() const (0x004226A0)
// The layer: 2 when attached to an object, else the type's Layer (3 without
// a type).

int AnimClass::In_Which_Layer() const
{
    if (unknown_b4 != 0) {
        return 2;
    }
    if (Class != 0) {
        return Class->Layer;
    }
    return 3;
}


// void AnimClass::unknown_func_4226d0() (0x004226D0)
// Starts the animation: slot 71 (2), the type's StartSound through the
// first audio controller (or just restarts it), restarts the second one,
// marks the ground when the type asks for it (+0x268 == 0), and for a
// TiberiumChainReaction type on a tiberium cell: grows the cell's overlay,
// one time in three spawns one of the tiberium's animations 10 leptons
// up, damages the area and clears the cell.

void AnimClass::unknown_func_4226d0()
{
    Mark(2);
    if (!unknown_178 && Class->StartSound != -1) {
        Voc_Play_At(Class->StartSound, Center_Coord(), &unknown_180);
    } else {
        unknown_180.Unknown_00405D50();
    }
    unknown_194.Unknown_00405D50();
    if (Class->unknown_268 == 0) {
        unknown_func_4228f0();
    }
    if (!unknown_178 && Class->TiberiumChainReaction) {
        UnknownAnimCell *cell = (UnknownAnimCell *)&Map[Center_Coord()];
        int index = cell->Unknown_0047A220();
        if (index != -1) {
            UnknownAnimTiberium *tiberium = unknown_tiberiums_ac0b30[index];
            cell->Unknown_00475DB0(cell->unknown_11e + 1);
            if (tiberium->unknown_c4.Count() > 0 && abs(unknown_id_source_a3d290->Random()()) % 3 == 0) {
                AnimClass *anim = new AnimClass(
                    tiberium->unknown_c4[unknown_id_source_a3d290->Random()(0, tiberium->unknown_c4.Count() - 1)],
                    Center_Coord() + CoordStruct(0, 0, 10), 0, 1, 0x600, 0);
                anim->unknown_bc = ColorSchemes[tiberium->unknown_c0]->Converter;
                anim->unknown_e4 = cell->unknown_10a;
            }
            {
                CoordStruct coord = Coord;
                Unknown_0047E2C0(coord, Rules->unknown_488, 0, Rules->unknown_c74, 0);
            }
            cell->Recalc_Attributes(-1);
            Map.Update_Cell_Zone(cell->unknown_24);
            Map.Unknown_00566340(cell->unknown_24);
        }
    }
}


// void AnimClass::unknown_func_4228f0() (0x004228F0)
// Marks the ground under a low animation (slot 108 < 30) with the size of
// its type's first frame (30 x 30 without an image): a Scorch type leaves a
// scorch (with Crater too, only half of the time); otherwise a Crater type
// flags the cell (6) and leaves a crater (ForceBigCraters: 300, forced).

void AnimClass::unknown_func_4228f0()
{
    Cell where = Coord_Cell(Center_Coord());
    UnknownAnimCell *cell = (UnknownAnimCell *)&Map[where];
    int width = 30;
    int height = 30;
    ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
    if (shape != 0) {
        width = shape->Get_Frame_Rect(Class->unknown_268).Width;
        height = shape->Get_Frame_Rect(Class->unknown_268).Height;
    }
    if (Get_Height_AGL() < 30) {
        if (Class->Scorch &&
            (!Class->Crater || unknown_id_source_a3d290->Random()(0, 0x7FFFFFFE) * (1.0 / 0x7FFFFFFE) < 0.5)) {
            Unknown_00688460(Center_Coord(), width, height, false);
            return;
        }
        if (Class->Crater) {
            cell->Unknown_00475DB0(6);
            if (Class->ForceBigCraters) {
                Unknown_00688750(Center_Coord(), 300, 300, true);
                return;
            }
            Unknown_00688750(Center_Coord(), width, height, false);
        }
    }
}


// void AnimClass::Detach(AbstractClass *target, bool all) (0x00422B20)
// Drops references to a removed object; losing the attached object
// detaches the animation from the map and ends it (slot 71).

void AnimClass::Detach(AbstractClass *target, bool all)
{
    ObjectClass::Detach(target, all);
    if (unknown_b4 == target) {
        Map.Remove(this);
        unknown_b4 = 0;
        unknown_17b = true;
        Mark(0);
    }
    if (Class == target) {
        Class = 0;
    }
    if (unknown_15c == target) {
        unknown_15c = 0;
        Delete_Me();
    }
}


// void __fastcall Unknown_00422BA0(int arg1, Cell const &cell) (0x00422BA0)
// Damages the center of a cell with the rules' values (+0x1190, warhead
// +0xC60); in game mode 0 it also hands unknown_481fb0 to 0x004955D0 (30).
// Called from BulletClass's AI; arg1 is not read.

void __fastcall Unknown_00422BA0(int arg1, Cell const &cell)
{
    CoordStruct coord;
    coord.Z = 0;
    coord.X = cell.X * 256 + 128;
    coord.Y = cell.Y * 256 + 128;
    Unknown_0047E2C0(coord, Rules->unknown_1190, 0, Rules->unknown_c60, 0);
    if (unknown_game_mode_a3d298 == 0) {
        Unknown_004955D0(&unknown_palette_837e10, 30, unknown_481fb0);
    }
}


// void AnimClass::unknown_func_422c10() (0x00422C10)

void AnimClass::unknown_func_422c10()
{
    unknown_17e = true;
}


// void AnimClass::unknown_func_422c20() (0x00422C20)

void AnimClass::unknown_func_422c20()
{
    unknown_17e = false;
}


// HRESULT __stdcall AnimClass::Load(IStream *stream) (0x00422C30, slot 5)
// Loads the object, rebuilds itself in place, restarts both audio
// controllers, fixes up its pointers and turns the saved index unknown_c0
// back into unknown_bc (the 0x00AB6EF8 entry's +0x30C value, else the
// palette's entry, else 0).

HRESULT __stdcall AnimClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) AnimClass(NoInitClass());
        unknown_180.Init();
        unknown_194.Init();
        Swizzle_Pointer(Class);
        Swizzle_Pointer(unknown_b4);
        Swizzle_Pointer(unknown_15c);
        if (unknown_c0 > -1) {
            if (unknown_c0 < ColorSchemes.Count()) {
                unknown_bc = ColorSchemes[unknown_c0]->Converter;
                return result;
            }
        } else {
            if (Palette.Buffer[0] != '\0') {
                UnknownPaletteSource *source = (UnknownPaletteSource *)Palette.Load();
                if (source != 0) {
                    ColorScheme *entry = source->unknown_slot6(0);
                    if (entry != 0) {
                        unknown_bc = entry->Converter;
                        return result;
                    }
                }
            }
            unknown_bc = 0;
        }
    }
    return result;
}


// HRESULT __stdcall AnimClass::Save(IStream *stream, BOOL clear_dirty) (0x00422D50)
// Records unknown_bc as an index into the 0x00AB6EF8 list (-1 if absent),
// then saves through AbstractClass::Save.

HRESULT __stdcall AnimClass::Save(IStream *stream, BOOL clear_dirty)
{
    if (unknown_bc != 0) {
        for (int index = ColorSchemes.Count() - 1; index >= 0; index--) {
            if (unknown_bc == ColorSchemes[index]->Converter) {
                unknown_c0 = index;
                goto save;
            }
        }
    }
    unknown_c0 = -1;
save:
    return AbstractClass::Save(stream, clear_dirty);
}


// void AnimClass::Compute_CRC(WWCRCEngine &crc) const (0x00422DB0)

void AnimClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectClass::Compute_CRC(crc);
    if (unknown_b4 != 0) {
        crc(unknown_b4->Fetch_ID());
    }
    crc((long)unknown_b8);
    crc((long)unknown_e4);
    crc((long)ZAdjust);
    crc((long)YSortAdjust);
    crc((long)LoopDelay);
    crc(unknown_168);
    crc((long)unknown_170);
    crc(unknown_174);
    crc((long)Loops);
    crc(unknown_176);
    crc(unknown_177);
    crc(unknown_17b);
    crc(unknown_17c);
    crc(unknown_17e);
}


// int AnimClass::unknown_slot117() const (0x00422EB0)
// The type's last frame (End).

int AnimClass::unknown_slot117() const
{
    return Class->End;
}


// ObjectTypeClass *AnimClass::Class_Of() const (0x00422EC0)

ObjectTypeClass *AnimClass::Class_Of() const
{
    return Class;
}


// bool AnimClass::Limbo() (0x00422ED0)
// Leaves the map: a vein animation clears its cell's 0x20000 flag (+0x140)
// first; when the ObjectClass slot succeeds the map entry is removed too.
// Needs ObjectClass::Limbo declared bool (0x005D3700 returns al).

bool AnimClass::Limbo()
{
    if (Class->IsVeins) {
        CoordStruct coord = Coord;
        unsigned long &flags = ((UnknownAnimCell *)&Map[coord])->Flags;
        flags = flags & ~0x20000;
    }
    if (ObjectClass::Limbo()) {
        Map.Remove(this);
        return true;
    }
    return false;
}


// void AnimClass::unknown_func_422f50() (0x00422F50)
// Vein damage: the animation is done (unknown_17b) unless its cell is a
// grown vein cell (+0x44 == 0x7E, +0x11E >= 0x30, +0x11C clear) holding a
// harmless first object (slot 108 <= 0). On even frames every techno in the
// cell that is not immune to veins, not in state 8, alive, on the map
// (unknown_7d) and low (slot 108 <= 5) takes the rules' vein damage
// (+0xE4) with the veins' warhead (+0xCB0).

void AnimClass::unknown_func_422f50()
{
    CoordStruct coord = Coord;
    UnknownAnimCell *cell = (UnknownAnimCell *)&Map[coord];
    ObjectClass *object = cell->unknown_e4;
    if (object == 0 || object->Get_Height_AGL() > 0 || cell->unknown_44 != 0x7E ||
        cell->unknown_11e < 0x30 || cell->unknown_11c) {
        unknown_17b = true;
    }
    if (!(Frame & 1)) {
        while (object != 0) {
            ObjectClass *next = object->unknown_30;
            int damage = Rules->unknown_e4;
            UnknownAnimTechno *techno = (UnknownAnimTechno *)object->As_Techno();
            if (techno != 0 && !((UnknownAnimTechnoType *)techno->Techno_Type_Class())->ImmuneToVeins &&
                !techno->Unknown_006D7910(8) && techno->Strength > 0 && techno->unknown_7d &&
                techno->Get_Height_AGL() <= 5) {
                techno->Take_Damage(&damage, 0, (WarheadTypeClass *)Rules->unknown_cb0, 0, 0, 0);
            }
            object = next;
        }
    }
}


// void AnimClass::Delete_Me() (0x00423070)
// Stops the animation's sounds: Attach_To(0), resets the first
// audio controller and, unless unknown_178 is set, plays the type's
// StopSound at the center through the second controller; then the
// ObjectClass slot.

void AnimClass::Delete_Me()
{
    Attach_To(0);
    unknown_180.Unknown_00406070();
    if (!unknown_178 && Class->StopSound != -1) {
        Voc_Play_At(Class->StopSound, Center_Coord(), &unknown_194);
    }
    ObjectClass::Delete_Me();
}


// int AnimClass::Get_Height() const (0x004230E0)
// The height: the coordinate's Z, plus the attached object's Z.

int AnimClass::Get_Height() const
{
    CoordStruct coord = Coord;
    if (unknown_b4 != 0) {
        int z = coord.Z;
        coord = unknown_b4->Coord;
        return z + coord.Z;
    }
    return coord.Z;
}


// void AnimClass::Flaming_Guy_AI() (0x00423120)
// Per-frame logic of a burning infantry animation (TibSun anim.cpp
// Flaming_Guy_AI): runs toward the next water cell (unknown_f0, TibSun
// FlamingGuyCoords; retries in unknown_fc), keeps the running frames pointed
// the way it goes, falls off a bridge edge (unknown_7a, IsFalling), and ends in
// the death sequence (unknown_17a, IsFlamingGuyEnd).
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

// The point DISTANCE leptons from START toward DIR (TibSun inline.h Move_Coord; Z kept).
static inline CoordStruct Move_Coord(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = ((short)dir.Raw - 0x3fff) * unknown_facing_radians_79b620;
    return CoordStruct((int)(start.X + unknown_cos_4ba4d0(radians) * distance), (int)(start.Y - unknown_sin_4ba480(radians) * distance), start.Z);
}

// The direction from one coordinate to another (TibSun inline.h Direction).
static inline UnknownFacingDir Direction(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1)
        - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
}

// The offset between two coordinates (TibSun Coord::operator-).
static inline UnknownVector3 Flaming_Offset(CoordStruct const &from, CoordStruct const &to)
{
    return UnknownVector3(from.X - to.X, from.Y - to.Y, from.Z - to.Z);
}

// The cell next to CELL toward an 8-way facing, the cell as the left operand.
static inline Cell Flaming_Adjacent_Cell(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}

void AnimClass::Flaming_Guy_AI()
{
    if (!unknown_17a) {
        Set_Rate(0);
    }

    if (unknown_f0 != unknown_geometry_l && !unknown_17a) {
        CoordStruct coord = Coord;
        coord.Z = 0;
        CoordStruct target = unknown_f0;
        target.Z = 0;
        if (Flaming_Offset(coord, target).Length() <= 18) {
            if (!unknown_7a) {
                CoordStruct nextcoord = unknown_func_4237b0();
                bool sink = (Map[unknown_f0].unknown_ec == 2 || Map[unknown_f0].unknown_ec == 6) && Get_Height_AGL() <= unknown_geometry_g;
                if (nextcoord != unknown_geometry_l && unknown_fc < 7 && !sink) {
                    unknown_fc++;
                    int z = Position_Coord_004f3f40().Z;
                    CoordStruct oldcoord = unknown_f0;
                    if (z <= Map.Get_Height_GL(oldcoord) + 2 * unknown_geometry_g) {
                        oldcoord.Z = Map.Get_Height_GL(oldcoord);
                    } else {
                        oldcoord.Z = Map.Get_Height_GL(oldcoord) + unknown_geometry_j;
                    }
                    Set_Coord(oldcoord);
                    unknown_f0 = nextcoord;
                } else {
                    unknown_f0 = unknown_geometry_l;
                    unknown_17a = true;
                    Set_Rate(1);
                    Stage = Class->RunningFrames * 8 + 1;
                    return;
                }
            }
        } else {
            int z = Position_Coord_004f3f40().Z;
            UnknownFacingDir direction = UnknownFacingDir().Direction(Center_Coord(), unknown_f0);
            CoordStruct newcoord = Move_Coord(Position_Coord_004f3f40(), direction, 18);
            if (z <= Map.Get_Height_GL(newcoord) + 2 * unknown_geometry_g) {
                newcoord.Z = Map.Get_Height_GL(newcoord);
            } else {
                newcoord.Z = Map.Get_Height_GL(newcoord) + unknown_geometry_j;
            }
            Set_Coord(newcoord);
        }

        if (!unknown_7a) {
            Cell position = Position_Coord_004f3f40();
            if (Position_Coord_004f3f40().Z >= Map.Get_Height_GL(Position_Coord_004f3f40()) + unknown_geometry_j) {
                if (!(Map[Position_Coord_004f3f40()].unknown_140 & 0x100) && !(Map[Flaming_Adjacent_Cell(position, 6)].unknown_140 & 0x100) && !(Map[Flaming_Adjacent_Cell(position, 0)].unknown_140 & 0x100)) {
                    unknown_7a = true;
                }
            }
        }
    } else {
        if (!unknown_7a) {
            if (!unknown_17a) {
                CoordStruct nextcoord = unknown_func_4237b0();
                if (nextcoord != unknown_geometry_l) {
                    unknown_f0 = nextcoord;
                } else {
                    unknown_f0 = unknown_geometry_l;
                    unknown_17a = true;
                    Set_Rate(1);
                    Stage = Class->RunningFrames * 8 + 1;
                }
            }
        }
    }

    if (!unknown_17a) {
        int stage = 0;
        if (unknown_f0 != unknown_geometry_l) {
            stage = Class->RunningFrames * ((-1 - Dir_To_8(Direction(Center_Coord(), unknown_f0))) & 7);
        }
        Stage = stage + Frame / 3 % Class->RunningFrames;
        return;
    } else {
        int stage = ((ShapeFileStruct const *)Get_Image_Data())->FrameCount / 2 - 1;
        if (Stage >= stage) {
            Delete_Me();
        }
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Adopted from ra2-decomp src/anim.cpp AnimClass::Next_Flaming_Guy_Coord
// (YR 0x00425D10, ca9b8ad), on the declarations of the earlier drafts.
// CoordStruct AnimClass::unknown_func_4237b0() (0x004237B0)
// Finds a place for the animation to come down: the nearest water cell
// within five cells whose neighbours toward facings 6 and 0 are clear
// (cells to the north-west score 3 better), then a step from the
// animation's cell toward it, then a random neighbour; the zero coordinate
// for none (ra2-decomp Next_Flaming_Guy_Coord; the cell tests are water, not
// under a bridge, neighbours not under a bridge). The point length is
// TPoint2D::Length (shared/point.hpp RA2_POINT_LENGTH) and the cell centres
// are their Coord(Cell) (shared/coord.hpp RA2_COORD_FROM_CELL_INLINE); both
// macros are defined by the unit's first row 0x0041FF20.

CoordStruct AnimClass::unknown_func_4237b0()
{
    bool any = false;
    Cell start = Coord_Cell(Position_Coord_004f3f40());
    int mindist = 10000;
    Cell mincell = unknown_geometry_k;
    for (int x = start.X - 5; x < start.X + 5; x++) {
        any = true;
        for (int y = start.Y - 5; y < start.Y + 5; y++) {
            if (Map.Unknown_0055A660(Cell(x, y), true)) {
                CellClass &cell = Map[Cell(x, y)];
                if (cell.unknown_ec == 2 && !(cell.unknown_140 & 0x100)
                    && !(cell.Adjacent_Cell(6).unknown_140 & 0x100)
                    && !(cell.Adjacent_Cell(0).unknown_140 & 0x100)) {
                    int dist = cell.MapCoords != start ? TPoint2D<int>(x - start.X, y - start.Y).Length() : 0;
                    if (x > start.X || y > start.Y) {
                        dist -= 3;
                    }
                    if (dist < mindist) {
                        mindist = dist;
                        mincell = Cell(x, y);
                    }
                }
            }
        }
    }
    if (any && mincell != unknown_geometry_k) {
        int dx = mincell.X - start.X;
        int dy = mincell.Y - start.Y;
        int step_x = 0;
        int step_y = 0;
        if (dx < 0) {
            step_x = -1;
        } else if (dx > 0) {
            step_x = 1;
        }
        if (dy < 0) {
            step_y = -1;
        } else if (dy > 0) {
            step_y = 1;
        }
        if (unknown_func_423b90(start + Cell(step_x, step_y))) {
            return CoordStruct(Cell(start.X + step_x, start.Y + step_y));
        }
        if (unknown_func_423b90(start + Cell(step_x, 0))) {
            return CoordStruct(Cell(start.X + step_x, start.Y));
        }
        if (unknown_func_423b90(start + Cell(0, step_y))) {
            return CoordStruct(Cell(start.X, start.Y + step_y));
        }
    }
    if (unknown_geometry_k.X == 0 && unknown_geometry_k.Y == 0) {
        int dir = unknown_id_source_a3d290->Random()(0, 7);
        for (int i = 0; i < 8; i++) {
            int facing = (dir + i) % 8;
            Cell const &offset = AdjacentCell[facing & 7];
            Cell newcell = start + offset;
            if (unknown_func_423b90(newcell)) {
                return CoordStruct(newcell);
            }
        }
    }
    return unknown_geometry_l;
}


// bool AnimClass::unknown_func_423b90(Cell const &where) (0x00423B90)
// Whether the animation may come down in a cell: over a bridge-like cell
// (flag 0x100) well above its level only when it is free (+0x128 & 0xE0);
// otherwise not on land types 3, 4 and 10, not on an overlay of the
// +0x278 kind, not on an occupied cell (+0x124 & 0xE0) and not more than
// two levels below the ground.

bool AnimClass::unknown_func_423b90(Cell const &where)
{
    UnknownAnimCell *cell = (UnknownAnimCell *)&Map[where];
    CoordStruct coord;
    if ((cell->Flags & 0x100) && (coord = Coord, coord.Z - cell->unknown_11b * unknown_geometry_g > unknown_geometry_g * 2)) {
        if (cell->unknown_128 & 0xE0) {
            return false;
        }
    } else {
        if (cell->unknown_ec == 3 || cell->unknown_ec == 10 || cell->unknown_ec == 4) {
            return false;
        }
        if (cell->unknown_44 != -1 && unknown_overlay_types_a35de8[cell->unknown_44]->unknown_278) {
            return false;
        }
        if (cell->unknown_124 & 0xE0) {
            return false;
        }
        coord = Coord;
        if (cell->unknown_11b * unknown_geometry_g - coord.Z > unknown_geometry_g * 2) {
            return false;
        }
    }
    return true;
}


// void __fastcall Unknown_00423C70() (0x00423C70)
// Gives every IsAnimatedTiberium animation the 0x00AB6EF8 entry value of
// the tiberium in its cell (as unknown_bc).

void __fastcall Unknown_00423C70()
{
    for (int index = 0; index < Anims.Count(); index++) {
        AnimClass *anim = Anims[index];
        if (anim->Class->IsAnimatedTiberium) {
            CoordStruct coord = anim->Coord;
            int tiberium = ((UnknownAnimCell *)&Map[coord])->Unknown_0047A220();
            if (tiberium != -1) {
                UnknownAnimTiberium *type = unknown_tiberiums_ac0b30[tiberium];
                if (type != 0) {
                    anim->unknown_bc = ColorSchemes[type->unknown_c0]->Converter;
                }
            }
        }
    }
}


class Leaf_423d10 { public: bool get(); };
bool Leaf_423d10::get() { return false; }


class Leaf_423d20 { public: unsigned int get(unsigned int unused_argument_1, unsigned int unused_argument_2); };
unsigned int Leaf_423d20::get(unsigned int unused_argument_1, unsigned int unused_argument_2) { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; return zero_value; }


class Leaf_423d30 { public: bool get(); };
bool Leaf_423d30::get() { return false; }


class Leaf_423d40 { public: unsigned int get(); };
unsigned int Leaf_423d40::get() { return 0x7cecb0; }


// bool ObjectClass::unknown_slot47() (0x00423D90): slot 47 (+0xBC); the base answer is true
// (CellClass::Occupy_Down 0x00473B30 asks it before moving a placed object to the cell's coordinate).
// Its body sits in this unit, after AnimClass's members (the original's copy may be an inline of
// the class emitted with the AnimClass vtable; as an inline in shared/object.hpp it moves
// Print_CRCs 0x0062A360 through the declaration counter).

bool ObjectClass::unknown_slot47()
{
    return true;
}


class Leaf_423da0 { public: bool get(); };
bool Leaf_423da0::get() { return false; }


// 0x00423db0: function not yet identified
//
// Owner: ObjectClass, virtual slot 49 (vtable +0xc4) (from RTTI/vtables).
// Body: 5 bytes (frozen inventory bound).
// Referenced from 15 sites (data).
// Ghidra prototype: undefined1 FUN_00423db0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00423db0 {

unsigned char __stdcall FUN_00423db0(int unused_0)
{
    return 0;
}

}  // namespace seed_00423db0


class Leaf_423dc0 { public: void get(unsigned int unused_argument_1, unsigned int unused_argument_2); };
void Leaf_423dc0::get(unsigned int unused_argument_1, unsigned int unused_argument_2) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_423dd0 { public: void get(unsigned int unused_argument_1, unsigned int unused_argument_2); };
void Leaf_423dd0::get(unsigned int unused_argument_1, unsigned int unused_argument_2) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_423de0 { public: void get(unsigned int unused_argument_1); };
void Leaf_423de0::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_423df0 { public: void get(unsigned int unused_argument_1); };
void Leaf_423df0::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_423e00 { public: unsigned int get(unsigned int unused_argument_1, unsigned int unused_argument_2, unsigned int unused_argument_3, unsigned int unused_argument_4, unsigned int unused_argument_5); };
unsigned int Leaf_423e00::get(unsigned int unused_argument_1, unsigned int unused_argument_2, unsigned int unused_argument_3, unsigned int unused_argument_4, unsigned int unused_argument_5) { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; return zero_value; }


class Leaf_423e10 { public: unsigned int get(unsigned int unused_argument_1, unsigned int unused_argument_2, unsigned int unused_argument_3, unsigned int unused_argument_4, unsigned int unused_argument_5); };
unsigned int Leaf_423e10::get(unsigned int unused_argument_1, unsigned int unused_argument_2, unsigned int unused_argument_3, unsigned int unused_argument_4, unsigned int unused_argument_5) { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; return zero_value; }


class Leaf_423e20 { public: bool get(); };
bool Leaf_423e20::get() { return false; }


class Leaf_423e30 { public: bool get(); };
bool Leaf_423e30::get() { return false; }


class Leaf_423e40 { public: bool get(); };
bool Leaf_423e40::get() { return false; }


class Leaf_423e50 { public: unsigned char get(); };
unsigned char Leaf_423e50::get() { return 1; }


class Leaf_423e60 { public: int get(); };
int Leaf_423e60::get() { return 0; }


// int AnimClass::Size_Of() const (0x00423E70): inline in shared/anim.hpp, emitted with the unit's vtable; no source of its own.


// HRESULT __stdcall AnimClass::GetClassID(GUID *clsid) (0x00423E80): inline in shared/anim.hpp, emitted with the unit's vtable; no source of its own.


// int AnimClass::Kind_Of() const (0x00423EC0): inline in shared/anim.hpp, emitted with the unit's vtable; no source of its own.


// AnimClass::`scalar deleting destructor' (0x00423ED0): compiler-generated with the unit's vtable; no source of its own.


// UnknownFacingDir coordinate constructor00423EF0: independent99code+13padding.
// Equal XY first writes Raw0 but still computes the direction afterward.
// Preserve the original four-byte value copy; the second word is not initialized.
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
UnknownFacingDir::UnknownFacingDir(CoordStruct const &from,CoordStruct const &to)
{
    if(from.X==to.X && from.Y==to.Y) Raw=0;
    *this=UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y)-to.Y,
        double(to.X)-from.X)-unknown_facing_origin_79b630)*unknown_facing_scale_79b628));
}


class Leaf_423f60 { public: unsigned int get(unsigned int unused_argument_1); };
unsigned int Leaf_423f60::get(unsigned int unused_argument_1) { unsigned int object_address = (unsigned int)this; unsigned int result_address; unsigned int unused_value; result_address = object_address; return result_address; }
