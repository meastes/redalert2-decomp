// object.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/object.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/unknown_geometry.hpp"
#include "shared/object_type.hpp"
#include "shared/object.hpp"
#include "shared/facing.hpp"
#include "shared/fast_math.hpp"
#include "shared/anim.hpp"
#include "shared/anim_type.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/rules.hpp"
#include "shared/foot.hpp"
#include "shared/infantry.hpp"
#include "shared/techno.hpp"
#include "shared/house.hpp"
#include "shared/techno_type.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/tactical.hpp"
#include "shared/unknown_static_rect.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#define RA2_DRAW_SHAPE_OPERATIONS
#include "shared/shape_file.hpp"
#include "shared/building_type.hpp"
#include "shared/rect_intersect.hpp"
#include "shared/unknown_list_832470.hpp"
#include "shared/terrain_type.hpp"
#include "shared/alpha_shape.hpp"
#include "shared/infantry_type.hpp"
#include "shared/tag.hpp"
#include "shared/techno_map.hpp"
#include "shared/rect.hpp"
#include "shared/coord_distance.hpp"


// Expose the real canonical vector accessors for this complete Object unit.
// Header-static dynamic initializers of the unit at 0x005D20F0:  the
// shared/unknown_geometry.hpp constants (0x005D20F0-0x005D22B1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005D2120, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2140, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2160, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2180, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D21A0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D21C0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2200, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2230, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2260, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D2290, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Dynamic initializer (0x005D22A0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005d20f0.cpp; no source of its own.


// Object.cpp header initializer005D22C0: existing canonical23+9, zero new credit.
// Emitted by shared/unknown_static_rect.hpp when the genuine Distance source includes BuildingType.
// Independent full32 and ordered13-entry original CRT proof: R563 review.json.
// Preserve canonical path and semantic_name absence; replace prior hardcoded-address Leaf spelling.


// ObjectClass::ObjectClass() (0x005D22E0)

ObjectClass::ObjectClass() :
    unknown_30(0),
    unknown_34(0),
    unknown_38(0),
    CustomSound(-1),
    BombVisible(false),
    Strength(0xFF),
    unknown_70(0xFF),
    IsDown(false),
    IsToDisplay(false),
    IsInLimbo(true),
    IsSelected(false),
    IsAnimAttached(false),
    IsOnBridge(false),
    IsFalling(false),
    IsABomb(false),
    unknown_7c(false),
    IsActive(true),
    Layer(-1),
    IsSubmittedToLayer(false),
    IsVisible(true),
    Coord(unknown_geometry_l),
    LineTrailer(0)
{
    unknown_3c.Init();
    unknown_50.Init();
    unknown_objects_a40368.Add(this);
    ObjectPtrTracker.Add(this);
    AbstractTypePtrTracker.Add(this);
    TagPtrTracker.Add(this);
    AbstractFlags.IsObject = 1;
}


// ObjectClass::ObjectClass(NoInitClass const &noinit) (0x005D2500)

ObjectClass::ObjectClass(NoInitClass const &noinit) :
    AbstractClass(noinit),
    unknown_24(noinit)
{
}


// ObjectClass::~ObjectClass() (0x005D2530)

ObjectClass::~ObjectClass()
{
    ObjectsToDelete.Delete(this);
    unknown_objects_a40368.Delete(this);
    ObjectPtrTracker.Delete(this);
    AbstractTypePtrTracker.Delete(this);
    TagPtrTracker.Delete(this);
    unknown_30 = 0;
    IsActive = false;
    unknown_3c.Unknown_00405D50();
    unknown_50.Unknown_00405D50();
    unknown_3c.Unknown_00405C10();
    unknown_50.Unknown_00405C10();
    unknown_func_5d2710();
}


// void ObjectClass::unknown_func_5d2710() (0x005D2710)

void ObjectClass::unknown_func_5d2710()
{
    if (LineTrailer != 0) {
        LineTrailer->Clear_Owner();
        LineTrailer = 0;
    }
}


// Existing ObjectClass direct Aircraft dependency; source spelling is a hypothesis.
static inline UnknownFacingDir Object_Geometry_Direction_005D2730(CoordStruct const &origin, CoordStruct const &destination)
{
    int origin_x = origin.X;
    int destination_x = destination.X;
    return UnknownFacingDir((unsigned short)((Fast_Atan2((double)origin.Y - destination.Y,
        (double)destination_x - origin_x) - 1.5707963267948966) * -10430.060040584269));
}
UnknownFacingDir ObjectClass::Direction_To(AbstractClass const *target) const
{
    return Object_Geometry_Direction_005D2730(Center_Coord(), target->Center_Coord());
}


// void *ObjectClass::Get_Image_Data() (0x005D27B0)

void *ObjectClass::Get_Image_Data()
{
    return Class_Of()->Get_Image_Data();
}


// bool ObjectClass::unknown_func_5d27d0(char const *name) const (0x005D27D0)

bool ObjectClass::unknown_func_5d27d0(char const *name) const
{
    return *Class_Of() == name;
}


// ObjectClass::AI005D27F0; original709+11.
// Private strict candidate; field/source type hypotheses remain explicit.
// Source types/field meanings remain explicit recovery hypotheses.
extern double const unknown_falling_decrement_007a7668;

static inline CoordStruct Object_AI_Splash_Coord(ObjectClass const *object)
{
    CoordStruct result = object->Coord;
    result.Z = (int)((unsigned int)result.Z + 3u);
    return result;
}

static inline int Object_AI_Clamped_Speed(int floor, int speed)
{
    return speed > floor ? speed : floor;
}

void ObjectClass::AI()
{
    if (!IsInLimbo) {
        if (Class_Of() && Class_Of()->AmbientSound != -1) {
            CoordStruct current = Coord;
            Voc_Play_At(Class_Of()->AmbientSound, current, &unknown_3c);
        }
        int sound = CustomSound;
        if (sound != -1) {
            CoordStruct current = Coord;
            Voc_Play_At(sound, current, &unknown_50);
        }
    }
    if (!IsFalling) return;

    int previous_state = In_Which_Layer();
    int new_height = (int)((unsigned int)Get_Height() +
                          (unsigned int)unknown_24.unknown_08);
    if (IsDown) {
        Mark(0);
        Coord.Z = new_height;
        Mark(1);
    } else {
        Coord.Z = new_height;
    }
    if (Get_Height_AGL() <= 0) {
        Set_Height_AGL(0);
        IsFalling = false;
        unknown_slot93(2);
        Shorten_Attached_Anims(this);
    }
    if (!IsInLimbo) {

        int velocity;
        if (IsAnimAttached) velocity = Object_AI_Clamped_Speed(Rules->ParachuteMaxFallRate, (unknown_24.unknown_08 = (int)((unsigned int)unknown_24.unknown_08 - 1u)));
        else velocity = Object_AI_Clamped_Speed(Rules->NoParachuteMaxFallRate, (unknown_24.unknown_08 = (int)((double)unknown_24.unknown_08 - unknown_falling_decrement_007a7668)));
        unknown_24.unknown_08 = velocity;
        if (previous_state != In_Which_Layer()) Map.Submit(this);
        if (!IsFalling) {
        if (unknown_7c && Strength > 0) {
            int damage = Strength;
            Take_Damage(&damage, 0, Rules->C4Warhead, 0, true, 1);
        }
        if (Kind_Of() == 4) {
            AnimClass *animation = static_cast<AnimClass *>(this);
            if (animation->Class->IsFlamingGuy) {
                animation->IsFlamingGuyEnd = true;
                animation->Set_Rate(1);
                animation->Stage = (int)((unsigned int)animation->Class->RunningFrames * 8u + 1u);
                if (Map[CoordStruct(Coord)].Land == 2) {
                    // New allocates before evaluating constructor arguments on this target.
                    // The coordinate helper copies after allocation, as the original does.
                    new AnimClass(Rules->SplashList[0], Object_AI_Splash_Coord(this), 0, 1, 0x600, 0);
                }
            }
        }
        }
    } else {
        Map.Remove(this);
    }
}


// void ObjectClass::Fall_From_Height() (0x005D2AC0, slot 57)
// Sets +0x7A and +0x7C, takes the object off the map's lists (slot 71 (0),
// 0x00499A90), clears +0x79 and puts it back (0x00499A40, slot 71 (1)).
// A foot then re-marks its cell: through slot 59 at its coordinate when its
// locomotor is not moving, else the locomotor clears its occupation bits.

void ObjectClass::Fall_From_Height()
{
    IsFalling = true;
    unknown_7c = true;
    Mark(0);
    Map.Remove(this);
    IsOnBridge = false;
    Map.Submit(this);
    Mark(1);
    bool foot;
    if (this != 0) {
        foot = AbstractFlags.IsFoot;
    }
    if (this != 0 && foot) {
        if (!((FootClass *)this)->Locomotor.Checked()->Is_Moving()) {
            CoordStruct coord = Coord;
            Clear_Occupy_Bit(coord);
            return;
        }
        ((FootClass *)this)->Locomotor.Checked()->Mark_All_Occupation_Bits(0);
    }
}


// int ObjectClass::unknown_slot28(ObjectClass *object, bool arg2) (0x005D2BA0)

int ObjectClass::unknown_slot28(ObjectClass *object, bool arg2)
{
    return 0;
}


// int ObjectClass::What_Action(Cell const *cell, bool arg2, bool arg3) (0x005D2BB0)

int ObjectClass::What_Action(Cell const *cell, bool arg2, bool arg3)
{
    return 0;
}


// int ObjectClass::In_Which_Layer() const (0x005D2BC0)

int ObjectClass::In_Which_Layer() const
{
    if (!In_Air()) {
        return 2;
    }
    return (Get_Height_AGL() >= Rules->JumpjetCruiseHeight) + 3;
}


// int ObjectClass::Get_Ownable() (0x005D2C00)

int ObjectClass::Get_Ownable()
{
    return 0x7FFFFFFF;
}


// bool ObjectClass::Can_Repair() (0x005D2C10)

bool ObjectClass::Can_Repair()
{
    return false;
}


// bool ObjectClass::Can_Demolish() (0x005D2C20)

bool ObjectClass::Can_Demolish()
{
    return false;
}


// bool ObjectClass::Can_Player_Fire() (0x005D2C30)

bool ObjectClass::Can_Player_Fire()
{
    return false;
}


// bool ObjectClass::Can_Player_Move() (0x005D2C40)

bool ObjectClass::Can_Player_Move()
{
    return false;
}


// void ObjectClass::Record_The_Kill(TechnoClass *killer) (0x005D2C50)

void ObjectClass::Record_The_Kill(TechnoClass *killer)
{
}


// void ObjectClass::Do_Shimmer() (0x005D2C60)

void ObjectClass::Do_Shimmer()
{
}


// int ObjectClass::Exit_Object(TechnoClass *techno) (0x005D2C70)

int ObjectClass::Exit_Object(TechnoClass *techno)
{
    return 0;
}


// void ObjectClass::Hidden() (0x005D2C80)

void ObjectClass::Hidden()
{
}


// void ObjectClass::Look(bool arg1, bool arg2) (0x005D2C90)

void ObjectClass::Look(bool arg1, bool arg2)
{
}


// bool ObjectClass::unknown_slot78(int arg1, ObjectClass *arg2, bool arg3) (0x005D2CA0)

bool ObjectClass::unknown_slot78(int arg1, ObjectClass *arg2, bool arg3)
{
    return false;
}


// bool ObjectClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4) (0x005D2CB0)

bool ObjectClass::unknown_slot77(int arg1, Cell const *arg2, Cell const *arg3, bool arg4)
{
    return false;
}


// void ObjectClass::Clicked_As_Target(int arg1) (0x005D2CC0)

void ObjectClass::Clicked_As_Target(int arg1)
{
}


// bool ObjectClass::In_Range(CoordStruct const &coord, int weapon) (0x005D2CD0)

bool ObjectClass::In_Range(CoordStruct const &coord, int weapon)
{
    return false;
}


// int ObjectClass::Weapon_Range(int arg1) (0x005D2CE0)

int ObjectClass::Weapon_Range(int arg1)
{
    return 0;
}


// void ObjectClass::Scatter(CoordStruct const &source, bool forced, bool nokidding) (0x005D2CF0)

void ObjectClass::Scatter(CoordStruct const &source, bool forced, bool nokidding)
{
}


// bool ObjectClass::Catch_Fire() (0x005D2D00)

bool ObjectClass::Catch_Fire()
{
    return false;
}


// void ObjectClass::Fire_Out() (0x005D2D10)

void ObjectClass::Fire_Out()
{
}


// int ObjectClass::Value() (0x005D2D20)

int ObjectClass::Value()
{
    return 0;
}


// int ObjectClass::Get_Mission() (0x005D2D30)

int ObjectClass::Get_Mission()
{
    return -1;
}


// void ObjectClass::Repair(int arg1) (0x005D2D40)

void ObjectClass::Repair(int arg1)
{
}


// void ObjectClass::Sell_Back(int arg1) (0x005D2D50)

void ObjectClass::Sell_Back(int arg1)
{
}


// Existing Object placement interface; semantic names are hypotheses.
static inline CoordStruct Object_Translation_005D2D60(TPoint2D<int> const &offset, CoordStruct origin)
{
    return CoordStruct((int)((unsigned int)origin.X + (unsigned int)offset.X),
        (int)((unsigned int)origin.Y + (unsigned int)offset.Y), origin.Z);
}
void ObjectClass::Move(int facing)
{
    Mark(0);
    CoordStruct destination = Object_Translation_005D2D60(unknown_facing_offsets_851ac0[facing & 7], Coord);
    if (Can_Enter_Cell(&Map[destination], facing, -1, 0, 1) == 0) Set_Coord(destination);
    Mark(1);
}


// void ObjectClass::Unselect() (0x005D2DF0)

void ObjectClass::Unselect()
{
    if (IsSelected) {
        CurrentObject.Delete(this);
        IsSelected = false;
        if (Map.Unknown_0049EC40() == this) {
            Map.Unknown_0049EC60(0);
        }
    }
}


// bool ObjectClass::unknown_slot80() (0x005D2E70, slot 80)
// Selects the object (hypothesis): refused while the object is in limbo or
// already selected (+0x77) or slot 76 says no, and for a techno with +0x32C
// set while slot 39 holds; a selection of another house's objects (by
// HouseClass 0x004F52B0) is cleared first. Technos whose type has +0xAA8 set
// go to the head of the selection list 0x00A40C60, others to its end.
extern int unknown_833684;    // 0x00833684
void __cdecl Clear_Selected_Objects_00482bc0();
void __fastcall Set_Type_Select_Mode(unsigned int value);

bool ObjectClass::unknown_slot80()
{
    if (!Debug_Map && (IsInLimbo || IsSelected || !CanBeSelected())) {
        return false;
    }
    TechnoClass *techno = As_Techno();
    if (!Debug_Map && Can_Player_Move() && techno && techno->unknown_32c) {
        return false;
    }
    if (unknown_833684) {
        return false;
    }
    if (CurrentObject.Count() > 0) {
        HouseClass *house = Owning_House();
        HouseClass *other = CurrentObject[0]->Owning_House();
        if (other->Is_Player_Control() != house->Is_Player_Control() || !other->Is_Player_Control()) {
            Clear_Selected_Objects_00482bc0();
        }
    }
    bool techno_flag;
    if (this) {
        techno_flag = AbstractFlags.IsTechno;
    }
    if (this && techno_flag && static_cast<TechnoTypeClass *>(Class_Of())->unknown_aa8) {
        CurrentObject.Add_Head(this);
    } else {
        CurrentObject.Add(this);
    }
    Set_Type_Select_Mode(0);
    IsSelected = true;
    return true;
}


// The current artwork rectangle in view-relative pixels, or the unit's empty rectangle.
// The unit's shape frames go through the range-checked Is_Shape_Index_Valid form (Get_Render_Rect 0x005D31C0
// keeps its first Get_Frame_Rect's Rect constructor out of line only with it); this is the unit's first include.
extern Rect TacticalRect;
Rect ObjectClass::Get_Visual_Rect()
{
    Point2D pixel;
    TacticalMap->Coord_To_Pixel(unknown_slot42(),pixel);
    ShapeFileStruct const *shape=static_cast<ShapeFileStruct const *>(Get_Image_Data());
    if (!shape) return unknown_static_rect;
    Rect frame=shape->Get_Frame_Rect(0);
    Rect visual(pixel.X+frame.X-shape->Get_Width()/2,
                pixel.Y+frame.Y-shape->Get_Height()/2,
                frame.Width,frame.Height);
    if (TacticalRect.Is_Overlapping(visual+TacticalRect.Top_Left())) return visual;
    return unknown_static_rect;
}


// ObjectClass::Get_Render_Rect (slot 73, 0x005D31C0; TibSun object.cpp): the screen rectangle the object's
// artwork covers, the union of the first frame and its shadow frame (frame + count / 2), plus a building's bib,
// centred on the object's render point.

// Whether the object is a building (Kind_Of 6). The inline test at the bib, and the Point2D the result is
// built from, are the two /Ob2 sites after the first frame rectangle that keep its Rect constructor out of line.
static inline bool Object_Is_Building(ObjectClass const *object)
{
    return object->Kind_Of() == 6;
}

Rect ObjectClass::Get_Render_Rect()
{
    Point2D drawpoint;
    TacticalMap->Coord_To_Pixel(unknown_slot42(), drawpoint);

    ShapeFileStruct const *sdata = (ShapeFileStruct const *)Class_Of()->Get_Image_Data();
    if (sdata == 0) {
        return SHAPE_FILE_EMPTY_RECT;
    }
    int frame = 0;
    BuildingClass *bptr = (BuildingClass *)this;
    if (Kind_Of() == 6 && bptr->Type->FirestormWall) {
        frame = 15;
    }

    Rect rect1 = sdata->Get_Frame_Rect(frame);
    Rect rect2 = sdata->Get_Frame_Rect(frame + (sdata->Get_Count() / 2));

    int width = sdata->Get_Width();
    int height = sdata->Get_Height();

    Rect a = Union(rect1, rect2);

    if (Object_Is_Building(this) && bptr->Type->BibShape) {
        Rect rect2 = ((ShapeFileStruct const *)bptr->Type->BibShape)->Get_Frame_Rect(0);
        a = Union(a, rect2);
    }

    int x = drawpoint.X + a.X - width / 2;
    int y = drawpoint.Y + a.Y - height / 2;
    Point2D position(x, y);
    return Rect(position.X, position.Y, a.Width, a.Height);
}


// ObjectClass clipping/draw gate existing ABI/types; diagnostic only.
extern bool Debug_Map;
extern Rect TacticalRect;

bool ObjectClass::Render(Rect *bounds, bool redraw, bool arg3)
{
    Point2D pixel;
    if (!Debug_Map && unknown_b263d8 && ((!redraw && !IsToDisplay) || IsInLimbo)) goto not_drawable;
    IsToDisplay = false;
    if (TacticalMap->Coord_To_Pixel(unknown_slot42(), pixel) || Kind_Of() == 24)
        goto draw_clipped;
not_drawable:
    return false;
draw_clipped:
    *bounds = bounds->Intersect(TacticalRect);
    if (bounds->X > TacticalRect.X)
        pixel.X = (int)((unsigned)pixel.X + (unsigned)TacticalRect.X - (unsigned)bounds->X);
    if (bounds->Y > TacticalRect.Y)
        pixel.Y = (int)((unsigned)pixel.Y + (unsigned)TacticalRect.Y - (unsigned)bounds->Y);
    Draw_It(pixel, *bounds);
    return true;
}


// void ObjectClass::Mark_For_Redraw() (0x005D36E0)

void ObjectClass::Mark_For_Redraw()
{
    if (!IsToDisplay) {
        IsToDisplay = true;
        Map.Flag_To_Redraw(0);
    }
}


// Existing ObjectClass removal interface, diagnostic only.
// Existing shape-file header supports a unit's own shared empty rectangle.
// Keep the actual Rect initializer; do not suppress emitted startup output.
extern bool GameActive;
bool ObjectClass::Limbo()
{
    if (!GameActive || IsInLimbo) return false;
    Unselect();
    unknown_slot54(true);
    Mark(0);
    Map.Remove(this);
    unknown_3c.Unknown_00405D50();
    unknown_50.Unknown_00405FE0();
    if (Class_Of() && Class_Of()->IsSentient) {
        if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5 ||
            Fetch_ID() != -2)
            unknown_list_832470.Remove(this);
    }
    if (Class_Of() && Class_Of()->unknown_ac) {
        Point2D pixel;
        CoordStruct location = Center_Coord();
        TacticalMap->Coord_To_Pixel(location, pixel);
        ShapeFileStruct *shape = reinterpret_cast<ShapeFileStruct *>(Class_Of()->unknown_ac);
        Point2D half;
        half.X = shape->Width / 2;
        half.Y = shape->Height / 2;
        pixel.X = (int)((unsigned int)pixel.X - (unsigned int)half.X);
        pixel.Y = (int)((unsigned int)pixel.Y - (unsigned int)half.Y);
        TacticalMap->Register_Dirty_Area(
            Rect(pixel.X, pixel.Y, shape->Width, shape->Height), 1);
    }
    Hidden();
    IsInLimbo = true;
    IsToDisplay = false;
    return true;
}


// ObjectClass placement005D3890, original824+8; private typed candidate.
// Interface/source types remain explicit compatible hypotheses.
extern bool GameActive;
extern int ScenarioInit;
static inline void Object_Placement_Add_Point(Point2D &point, Point2D const &offset)
{
    Point2D delta = offset;
    point.X = (int)((unsigned int)point.X + (unsigned int)delta.X);
    point.Y = (int)((unsigned int)point.Y + (unsigned int)delta.Y);
}

bool ObjectClass::Unlimbo(CoordStruct const *coord, int direction)
{
    if (!GameActive || !IsInLimbo || IsDown) return false;
    if (!ScenarioInit && Can_Enter_Cell(&Map[*coord], -1, -1, 0, 0))
        return false;
    IsInLimbo = false;
    IsToDisplay = false;
    ObjectTypeClass *type = Class_Of();
    CoordStruct placed = *coord;
    if (type) placed = type->Coord_Fixup(*coord);
    Set_Coord(placed);
    if (Mark(1)) {
        if (IsActive) {
            if (In_Which_Layer() != -1) Map.Submit(this);
            if (type) {
                if (type->IsSentient &&
                    (Kind_Of() != 0x24 ||
                     (Kind_Of() == 0x24 && static_cast<TerrainTypeClass *>(type)->SpawnsTiberium)) &&
                    (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5 || Fetch_ID() != -2))
                    unknown_list_832470.Submit(this, false);
                if (type->unknown_ac) {
                    CoordStruct location = Center_Coord();
                    Point2D pixel;
                    TacticalMap->Coord_To_Pixel(location, pixel);
                    Object_Placement_Add_Point(pixel, Point2D(TacticalMap->TacPixelX, TacticalMap->TacPixelY));
                    ShapeFileStruct *shape = reinterpret_cast<ShapeFileStruct *>(Class_Of()->unknown_ac);
                    Point2D half;
                    half.X = shape->Width / 2;
                    half.Y = shape->Height / 2;
                    pixel.X = (int)((unsigned int)pixel.X - (unsigned int)half.X);
                    pixel.Y = (int)((unsigned int)pixel.Y - (unsigned int)half.Y);
                    new AlphaShapeClass(this, pixel.X, pixel.Y);
                    if (!ScenarioInit) {
                        TacticalMap->Register_Dirty_Area(Rect(
                            (int)((unsigned int)pixel.X - (unsigned int)TacticalMap->TacPixelX),
                            (int)((unsigned int)pixel.Y - (unsigned int)TacticalMap->TacPixelY),
                            shape->Width, shape->Height), 1);
                    }
                }
            }
            if (Class_Of()->UseLineTrail) {
                LineTrailer = new LineTrail();
                if (LineTrailer) {
                    RGBClass override_color = Rules->LineTrailColorOverride;
                    if (override_color == RGBClass(0, 0, 0)) {
                        RGBClass color = Class_Of()->LineTrailColor;
                        LineTrailer->Color = color;
                    } else {
                        RGBClass color = Rules->LineTrailColorOverride;
                        LineTrailer->Color = color;
                    }
                    LineTrailer->Set_Decrement(Class_Of()->LineTrailColorDecrement);
                    LineTrailer->Owner = this;
                }
            }
            return true;
        }
        return true;
    }
    IsInLimbo = true;
    return false;
}


// void ObjectClass::Detach(AbstractClass *target, bool all) (0x005D3BD0)

void ObjectClass::Detach(AbstractClass *target, bool all)
{
    if (unknown_34 == target && unknown_34 != 0) {
        unknown_34->AttachCount--;
        unknown_34 = 0;
    }
    if (all && target == unknown_30 && target != 0) {
        unknown_30 = unknown_30->unknown_30;
    }
}


// void ObjectClass::unknown_slot54(bool arg1) (0x005D3C10, slot 54)
// Releases the +0x94 object; calls slot 81 when arg1 is set, the object has
// no owner, or the owner fails 0x004F52B0 and the object is not a techno
// passing TechnoClass slot 191; clears the map's 0x0049EC40 selection when it
// is this object; then flags the object in its owner's lists (0x006ECB90).

void ObjectClass::unknown_slot54(bool arg1)
{
    if (LineTrailer != 0) {
        LineTrailer->Clear_Owner();
        LineTrailer = 0;
    }
    HouseClass *house = Owning_House();
    bool keep = false;
    if (!arg1) {
        bool techno;
        if (this != 0) {
            techno = AbstractFlags.IsTechno;
        }
        if (this != 0 && techno && ((TechnoClass *)this)->unknown_slot191()) {
            keep = true;
        }
    }
    if (arg1 || house == 0 || (!house->Is_Player_Control() && !keep)) {
        Unselect();
    }
    if (Map.Unknown_0049EC40() == this) {
        Map.Unknown_0049EC60(0);
    }
    Detach_This_From_All(this, arg1);
}


// int ObjectClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param) (0x005D3CB0)

int ObjectClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param)
{
    switch (message) {
    case 0x22:
        if (Health_Ratio() < Rules->unknown_1360) {
            return 1;
        }
        return 10;
    case 0x0D:
        Mark(2);
        return 1;
    }
    return 0;
}


// ObjectClass damage handler005D3D20; original1079+9.
// Private strict candidate; source and field types remain explicit hypotheses.
// Existing six-argument ABI and field meanings remain explicit hypotheses.
int __fastcall Modify_Damage(int damage, WarheadTypeClass *warhead, int armour, int distance);
extern double const unknown_cyborg_recovery_007a7670;
int ObjectClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead,
    TechnoClass *source, bool forced, int unknown_argument)
{
    int state = 0;
    {
    int old_strength = Strength;
    if (old_strength <= 0 || *damage == 0 || (!forced && Class_Of()->IsImmune)) goto damage_done;
    int maximum_strength = Class_Of()->MaxStrength;
    if (!forced) *damage = Modify_Damage(*damage, warhead, Class_Of()->Armor, distance);
    if (*damage == 0) return 0;
    if (*damage < 0) {
        int before_heal = Strength;
        Strength = (int)((unsigned int)before_heal - (unsigned int)*damage);
        if (Strength > maximum_strength) Strength = maximum_strength;
        if (before_heal != Strength) Clicked_As_Target(7);
        return 0;
    }
    state = 1;
    if (old_strength > *damage) {
        if (old_strength >= (maximum_strength >> 1) &&
            (int)((unsigned int)old_strength - (unsigned int)*damage) < (maximum_strength >> 1)) state = 2;
    } else *damage = old_strength;
    double threshold = (double)maximum_strength * Rules->unknown_1370;
    if ((double)old_strength > threshold &&
        (double)(int)((unsigned int)old_strength - (unsigned int)*damage) < threshold) state = 3;
    Strength = (int)((unsigned int)old_strength - (unsigned int)*damage);
    if (Strength <= 0 && Kind_Of() == 15 && !forced) {
        InfantryClass *infantry = static_cast<InfantryClass *>(this);
        if (infantry->Type->Cyborg && !infantry->unknown_5c3) {
            new AnimClass(Rules->InfantryExplode, CoordStruct(Coord), 0, 1, 0x600, 0);
            Strength = (int)((double)infantry->Type->MaxStrength * unknown_cyborg_recovery_007a7670);
            int restored_strength = Strength;
            if (restored_strength <= 1) restored_strength = 1;
            Strength = restored_strength;
            infantry->unknown_5c3 = true;
            infantry->unknown_slot311(6, true, false);
            state = 3;
        }
    }
    int strength_after_damage = Strength;
    if (state == 2) {
        if (source && unknown_34) unknown_34->Spring(39, this, unknown_geometry_k, 0, 0);
        if (!IsActive) goto damage_interrupted;
        if (unknown_34) unknown_34->Spring(42, this, unknown_geometry_k, 0, 0);
    }
    if (IsActive) {
        if (state == 3) {
            if (source && unknown_34) unknown_34->Spring(40, this, unknown_geometry_k, 0, 0);
            if (!IsActive) goto damage_interrupted;
            if (unknown_34) unknown_34->Spring(43, this, unknown_geometry_k, 0, 0);
        }
        if (IsActive) {
            if (Strength != old_strength && Class_Of() && old_strength == Class_Of()->MaxStrength) {
                if (source && unknown_34) unknown_34->Spring(38, this, unknown_geometry_k, 0, 0);
                if (unknown_34 && IsActive) unknown_34->Spring(41, this, unknown_geometry_k, 0, 0);
                if (unknown_34) {
                    if (!IsActive) goto damage_interrupted;
                    if (source) unknown_34->Spring(41, this, unknown_geometry_k, 0, reinterpret_cast<int>(source));
                }
            }
            if (IsActive && (strength_after_damage <= 0 || Strength > 0)) {
                if (Strength == 0) {
                    Record_The_Kill(source);
                    state = 4;
                    unknown_slot54(true);
                }
                if (!IsActive) goto damage_done;
                if (source && unknown_34 && state != 4)
                    unknown_34->Spring(6, this, unknown_geometry_k, 0, reinterpret_cast<int>(source));
                if (!IsActive) goto damage_done;
                if (source && unknown_34 && state != 4)
                    unknown_34->Spring(44, this, unknown_geometry_k, 0, reinterpret_cast<int>(source));
                if (!IsActive) goto damage_done;
                if (state == 0) goto damage_done;
                if (!IsSelected) goto damage_done;
                Mark(2);
                goto damage_done;
            }
        }
    }
    goto damage_interrupted;
    }
damage_done:
    return state;
damage_interrupted:
    return 5;
}


// ObjectClass slot71 existing ABI/layout only; diagnostic, not accepted.
// Full BuildingType include emits held existing Rect initializer; no suppression.
bool ObjectClass::Mark(int request)
{
    int action = request;
    if (!IsInLimbo) {
        if (action == 2) {
            if (!IsToDisplay && IsDown) {
                Mark_For_Redraw();
                return true;
            }
        } else {
            if (Kind_Of() != 6 ||
                !static_cast<BuildingClass *>(this)->Type->ToTile || action != 0) {
                TechnoClass *techno = As_Techno();
                if (techno) {
                    techno->unknown_slot168();
                    techno->Owner();
                    unknown_slot104();
                }
            }
            if ((action == 1 || action == 3) && !IsDown) {
                IsDown = true;
                Mark_For_Redraw();
                return true;
            }
            if (action == 0 && IsDown) {
                IsDown = false;
                return true;
            }
        }
    }
    return false;
}


// bool ObjectClass::Revealed(HouseClass *arg1) (0x005D4230)

bool ObjectClass::Revealed(HouseClass *arg1)
{
    return arg1 != 0;
}


// bool ObjectClass::unknown_slot56(CoordStruct const &coord) (0x005D4240, slot 56)
// Places the object at coord with an arrival animation: refused off the map
// (0x0055A7F0), without a cell, on a bridge cell lacking 0x200, or, for a
// techno, when the cell is not enterable for its SpeedType in the zone of its
// MovementZone (0x0054F8F0, 0x00478760); then slot 53 (facing 0x80) and slot
// 103 at coord, and a new animation (rules +0xA78 for kind 8, else +0xA7C
// 0x4B above coord) attached to the object; for other kinds it also takes
// slot 115 (+0xBC) and the cell's +0x10A (+0xE4). +0x7A is set, and +0x79 on
// a bridge cell.

bool ObjectClass::unknown_slot56(CoordStruct const &coord)
{
    if (Map.Unknown_0055A7F0(coord)) {
        IsFalling = true;
        UnknownCellClass *cell = Map_Cell(coord);
        if (cell != 0 && (!(cell->Flags & 0x100) || (IsOnBridge = true, (cell->Flags & 0x200) != 0))) {
            bool techno;
            if (this != 0) {
                techno = AbstractFlags.IsTechno;
            }
            bool clear = true;
            if (this != 0 && techno) {
                int zone = Techno_Type_Class()->MovementZone;
                Cell position = Cell(coord.X / 256, coord.Y / 256);
                clear = cell->Unknown_00478760(Techno_Type_Class()->SpeedType, 0, 0,
                    Map.Zone(position, zone, IsOnBridge), zone, -1, 1);
            }
            if (clear) {
                if (Unlimbo(&coord, 0x80)) {
                    Set_Coord(coord);
                    AnimClass *anim;
                    if (Kind_Of() == 8) {
                        anim = new AnimClass(Rules->unknown_a78, coord, 0, 1, 0x600, 0);
                    } else {
                        CoordStruct above = coord;
                        above.Z += 0x4B;
                        anim = new AnimClass(Rules->unknown_a7c, above, 0, 1, 0x600, 0);
                    }
                    if (anim != 0) {
                        anim->Attach_To(this);
                        if (Kind_Of() != 8) {
                            anim->unknown_bc = unknown_slot115();
                            anim->AlternativeBrightness = ((UnknownCellClass *)Get_Cell_Ptr())->unknown_10a;
                        }
                    }
                    return true;
                }
            }
        }
    }
    return false;
}


// bool ObjectClass::Attach_Tag(TagClass *tag) (0x005D4430)

bool ObjectClass::Attach_Tag(TagClass *tag)
{
    if (unknown_34 != 0) {
        unknown_34->AttachCount--;
        unknown_34 = 0;
    }
    if (tag != 0) {
        unknown_34 = tag;
        tag->AttachCount++;
        return true;
    }
    return false;
}


// Cell const *ObjectClass::Occupy_List(int arg1) (0x005D4470)
// (the guard's number follows the unit's static-initializer count: 37 in the
// full Object.cpp unit, 1 when compiled alone)

Cell const *ObjectClass::Occupy_List(int arg1)
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    if (Class_Of() == 0) {
        return &unknown_none.Cell;
    }
    return Class_Of()->Occupy_List(arg1);
}


// Exit thunk of the function-local static in ObjectClass::Occupy_List
// (0x005D44F0)
// Compiler generated ($E) right after unknown_slot64: UnknownStaticCell's empty
// destructor, registered with atexit on first use.


// BuildingClass *ObjectClass::Who_Can_Build_Me(bool in_theory, bool legal) (0x005D4500, slot 94)
// The owner's building that can produce this object's type.

BuildingClass *ObjectClass::Who_Can_Build_Me(bool in_theory, bool legal)
{
    return Class_Of()->Who_Can_Build_Me(in_theory, legal, false, Owning_House());
}


// double ObjectClass::Health_Ratio() const (0x005D4540)

double ObjectClass::Health_Ratio() const
{
    return (double)Strength / Class_Of()->MaxStrength;
}


// void ObjectClass::Set_Health_Ratio(double ratio) (0x005D4560)

void ObjectClass::Set_Health_Ratio(double ratio)
{
    if (ratio > 0.0) {
        Strength = (int)(Class_Of()->MaxStrength * ratio);
        if (Strength == 0) {
            Strength = 1;
        }
    } else {
        Strength = 0;
    }
}


// int ObjectClass::Is_Red_HP() const (0x005D45B0)

int ObjectClass::Is_Red_HP() const
{
    if (Health_Ratio() <= Rules->unknown_1370 && Strength > 0) {
        return 1;
    }
    return 0;
}


// int ObjectClass::Is_Yellow_HP() const (0x005D4600)

int ObjectClass::Is_Yellow_HP() const
{
    if (Health_Ratio() > Rules->unknown_1370 && Health_Ratio() <= Rules->unknown_1368) {
        return 1;
    }
    return 0;
}


// int ObjectClass::Is_Green_HP() const (0x005D4670)

int ObjectClass::Is_Green_HP() const
{
    if (Health_Ratio() > Rules->unknown_1368) {
        return 1;
    }
    return 0;
}


// int ObjectClass::GetHealthStatus() const (0x005D46B0)

int ObjectClass::GetHealthStatus() const
{
    if (Health_Ratio() <= Rules->unknown_1370 && Strength > 0) {
        return 0;
    }
    if (Health_Ratio() > Rules->unknown_1370 && Health_Ratio() <= Rules->unknown_1368) {
        return 1;
    }
    return 2;
}


// HRESULT __stdcall ObjectClass::Load(IStream *stream) (0x005D4760)

HRESULT __stdcall ObjectClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    Swizzler.Swizzle((void **)&unknown_30);
    Swizzler.Swizzle((void **)&unknown_34);
    Swizzler.Swizzle((void **)&unknown_38);
    Swizzler.Swizzle((void **)&unknown_18);
    unknown_3c.Init();
    unknown_50.Init();
    LineTrailer = 0;
    return S_OK;
}


// Object cell level plus bridge offset; existing const interface is a hypothesis.
int ObjectClass::Unknown_005D47D0() const
{
    CellClass *cell = static_cast<CellClass *>(const_cast<ObjectClass *>(this)->Get_Cell_Ptr());
    return cell->Height + (IsOnBridge ? 4 : 0);
}


// int ObjectClass::Get_Height() const (0x005D4800)

int ObjectClass::Get_Height() const
{
    return Coord.Z;
}


// int ObjectClass::Get_Height_AGL() const (0x005D4810)

int ObjectClass::Get_Height_AGL() const
{
    CoordStruct coord = Coord;
    int height = Coord.Z - Map.Get_Height_GL(coord);
    if (IsOnBridge) {
        height -= unknown_geometry_j;
    }
    return height;
}


// void ObjectClass::Set_Height_AGL(int height) (0x005D4860)

void ObjectClass::Set_Height_AGL(int height)
{
    if (IsOnBridge) {
        height += unknown_geometry_j;
    }
    if (IsDown) {
        Mark(0);
        CoordStruct coord = Coord;
        Coord.Z = Map.Get_Height_GL(coord) + height;
        Mark(1);
    } else {
        CoordStruct coord = Coord;
        Coord.Z = Map.Get_Height_GL(coord) + height;
    }
}


// void ObjectClass::Set_Height(int z) (0x005D4920)

void ObjectClass::Set_Height(int z)
{
    if (IsDown) {
        Mark(0);
        Coord.Z = z;
        Mark(1);
    } else {
        Coord.Z = z;
    }
}


// void ObjectClass::Set_Occupy_Bit(CoordStruct const &coord) (0x005D4960, slot 58)
// Sets the Monolith occupation bit (0x40) of the cell at coord: the bridge level's (+0x128)
// when coord is at or above the bridge height over a bridge cell, else the
// ground's (+0x124).

void ObjectClass::Set_Occupy_Bit(CoordStruct const &coord)
{
    if (Map.Get_Height_GL(coord) + unknown_geometry_j <= coord.Z && (Map_Cell(coord)->Flags & 0x100)) {
        Map_Cell(coord)->unknown_128.Occupy.Monolith = true;
    } else {
        Map_Cell(coord)->unknown_124.Occupy.Monolith = true;
    }
}


// void ObjectClass::Clear_Occupy_Bit(CoordStruct const &coord) (0x005D49E0, slot 59)
// Clears the Monolith occupation bit (0x40) of the cell at coord: the bridge level's (+0x128)
// when coord is at or above the bridge height over a bridge cell, else the
// ground's (+0x124).

void ObjectClass::Clear_Occupy_Bit(CoordStruct const &coord)
{
    if (Map.Get_Height_GL(coord) + unknown_geometry_j <= coord.Z && (Map_Cell(coord)->Flags & 0x100)) {
        Map_Cell(coord)->unknown_128.Occupy.Monolith = false;
    } else {
        Map_Cell(coord)->unknown_124.Occupy.Monolith = false;
    }
}


// UnknownObjectPair::~UnknownObjectPair() (0x005D4A60)

UnknownObjectPair::~UnknownObjectPair()
{
    delete unknown_00;
    unknown_00 = 0;
    delete unknown_04;
    unknown_04 = 0;
}


// bool ObjectClass::operator<(ObjectClass const &other) const (0x005D4AB0)

bool ObjectClass::operator<(ObjectClass const &other) const
{
    return Sort_Y() < other.Sort_Y();
}


// bool ObjectClass::operator>(ObjectClass const &other) const (0x005D4AE0)

bool ObjectClass::operator>(ObjectClass const &other) const
{
    return Sort_Y() > other.Sort_Y() ? true : false;
}


// void ObjectClass::Compute_CRC(WWCRCEngine &crc) const (0x005D4B10)

void ObjectClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    if (unknown_30 != 0) {
        crc(unknown_30->Fetch_ID());
    }
    if (unknown_34 != 0) {
        crc(unknown_34->Fetch_ID());
    }
    crc((long)Strength);
    crc(IsDown);
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        crc(IsToDisplay);
        crc(IsSelected);
    }
    crc(IsInLimbo);
    crc(IsAnimAttached);
    crc(IsOnBridge);
    crc(IsFalling);
    crc(unknown_7c);
    crc(IsActive);
    crc((long)Coord.X);
    crc((long)Coord.Y);
    crc((long)Coord.Z);
}


// ObjectClass existing three-dimensional distance interface, diagnostic only.
static inline int Object_Spatial_Distance_005D4C00(CoordStruct const &origin, CoordStruct const &destination)
{
    CoordStruct difference = origin - destination;
    double y = difference.Y;
    double z = difference.Z;
    double x = difference.X;
    return (int)unknown_sqrt_4ba410(x*x + z*z + y*y);
}
int ObjectClass::unknown_5d4c00(AbstractClass *target)
{
    if (!target) return 0;
    int distance = Object_Spatial_Distance_005D4C00(Center_Coord(), target->Center_Coord());
    BuildingClass *building = target->Kind_Of() == 6 ? static_cast<BuildingClass *>(target) : 0;
    if (building) {
        Point2D size(building->Type->Width(), building->Type->Foundation_Height(false));
        distance = (int)((unsigned int)distance - ((unsigned int)size.X + (unsigned int)size.Y) * 64U);
        if (distance < 0) distance = 0;
    }
    return distance;
}


// Existing ObjectClass direct Aircraft dependency; source spelling is a hypothesis.
static inline Point2D Object_Geometry_Planar_Position_005D4CE0(CoordStruct const &position)
{
    return Point2D(position.X, position.Y);
}
static inline int Object_Geometry_Planar_Distance_005D4CE0(Point2D origin, Point2D destination)
{
    int origin_x = origin.X;
    int origin_y = origin.Y;
    Point2D difference((int)((unsigned int)origin_x - (unsigned int)destination.X),
        (int)((unsigned int)origin_y - (unsigned int)destination.Y));
    double y = difference.Y;
    double x = difference.X;
    return (int)unknown_sqrt_4ba410(x * x + y * y);
}
int ObjectClass::Planar_Distance(AbstractClass const *target) const
{
    if (!target) return 0;
    int distance = Object_Geometry_Planar_Distance_005D4CE0(Object_Geometry_Planar_Position_005D4CE0(Center_Coord()),
        Object_Geometry_Planar_Position_005D4CE0(target->Center_Coord()));
    BuildingClass const *building = target->Kind_Of() == 6 ? static_cast<BuildingClass const *>(target) : 0;
    if (building) {
        Point2D size(building->Type->Width(), building->Type->Foundation_Height(false));
        distance = (int)((unsigned int)distance - ((unsigned int)size.X + (unsigned int)size.Y) * 64U);
        if (distance < 0) distance = 0;
    }
    return distance;
}


// int ObjectClass::Distance_Squared(ObjectClass const *other) const (0x005D4DA0)

int ObjectClass::Distance_Squared(ObjectClass const *other) const
{
    if (other == 0) {
        return 0;
    }
    CoordStruct here = Center_Coord();
    CoordStruct there = other->Center_Coord();
    int dx = here.X - there.X;
    int dy = here.Y - there.Y;
    return dy * dy + dx * dx;
}


// int ObjectClass::Distance_Squared(CoordStruct const &coord) const (0x005D4E00)

int ObjectClass::Distance_Squared(CoordStruct const &coord) const
{
    CoordStruct here = Center_Coord();
    int dx = here.X - coord.X;
    int dy = here.Y - coord.Y;
    return dy * dy + dx * dx;
}


// CoordStruct ObjectClass::Center_Coord() const (0x005D4E40)

CoordStruct ObjectClass::Center_Coord() const
{
    return Coord;
}


// void ObjectClass::Editor_Draw_It(Point2D const &point, Rect const &window) (0x005D4E70)

void ObjectClass::Editor_Draw_It(Point2D const &point, Rect const &window)
{
    Draw_It(point, window);
}


// void ObjectClass::Delete_Me() (0x005D4E90)

void ObjectClass::Delete_Me()
{
    if (unknown_38 != 0) {
        unknown_38->Disarm();
    }
    Detach_This_From_All(this, true);
    Limbo();
    IsActive = false;
    ObjectsToDelete.Add(this);
}


// bool ObjectClass::Is_Inactive() const (0x005D4F10)

bool ObjectClass::Is_Inactive() const
{
    return !IsActive;
}


// Rect __fastcall Coord_Bounds_005d4f20(DynamicVectorClass<ObjectClass *> const &objects) (0x005D4F20)
// The X/Y rectangle the objects' coordinates span (left, top, width, height), from the first object's
// coordinate grown over the list from its end. Nothing calls it directly; name and purpose hypotheses.
// The unit's earlier sources define the vector and dynamic vector accessors.

Rect __fastcall Coord_Bounds_005d4f20(DynamicVectorClass<ObjectClass *> const &objects)
{
    Rect bounds(0, 0, 0, 0);
    if (objects.Count() > 0) {
        bounds.X = CoordStruct(objects[0]->Coord).X;
        bounds.Y = CoordStruct(objects[0]->Coord).Y;
        for (int index = objects.Count() - 1; index >= 0; index--) {
            CoordStruct coord = objects[index]->Coord;
            if (coord.X < bounds.X) {
                bounds.Width += bounds.X - coord.X;
                bounds.X = coord.X;
            }
            if (coord.Y < bounds.Y) {
                bounds.Height += bounds.Y - coord.Y;
                bounds.Y = coord.Y;
            }
            if (coord.X > bounds.X + bounds.Width) {
                bounds.Width = coord.X - bounds.X;
            }
            if (coord.Y > bounds.Y + bounds.Height) {
                bounds.Height = coord.Y - bounds.Y;
            }
        }
    }
    return bounds;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// CoordStruct __fastcall Vector_Center_005d5010(DynamicVectorClass<ObjectClass *> const &list) (0x005D5010)
// Adopted from ra2-decomp Vector_Center (YR 0x005F6790).
// The mean coordinate of the objects in the list. Nothing calls it directly.

CoordStruct __fastcall Vector_Center_005d5010(DynamicVectorClass<ObjectClass *> const &list)
{
    CoordStruct center = CoordStruct(0, 0, 0);
    int count = list.Count();
    for (int i = count - 1; i >= 0; i--) {
        center += list[i]->Position_Coord_004f3f40();
    }
    center.X /= count;
    center.Y /= count;
    center.Z /= count;
    return center;
}


// ObjectClass *__fastcall Closest_Object_005d5090(DynamicVectorClass<ObjectClass *> const &objects,
//     CoordStruct const &coord) (0x005D5090)
// The object of the list nearest to coord, scanning from the end (0x0062941B calls it). The first
// distance is written out with double components (the original's Y, Z, X loads); the others go
// through Coord_Distance. Name and purpose hypotheses.
// The unit's earlier sources define the vector and dynamic vector accessors.

ObjectClass *__fastcall Closest_Object_005d5090(DynamicVectorClass<ObjectClass *> const &objects, CoordStruct const &coord)
{
    ObjectClass *best = 0;
    int best_distance = 99999999;
    for (int index = objects.Count() - 1; index >= 0; index--) {
        if (best == 0) {
            best = objects[index];
            CoordStruct d = coord - CoordStruct(best->Coord);
            double y = d.Y;
            double z = d.Z;
            double x = d.X;
            best_distance = (int)unknown_sqrt_4ba410(x * x + (y * y + z * z));
        } else {
            int distance = Coord_Distance(coord, CoordStruct(objects[index]->Coord));
            if (distance < best_distance) {
                best = objects[index];
                best_distance = distance;
            }
        }
    }
    return best;
}


// void ObjectClass::Set_Coord(CoordStruct const &coord) (0x005D51C0)

void ObjectClass::Set_Coord(CoordStruct const &coord)
{
    Coord = coord;
}


// AbstractClass *ObjectClass::Get_Cell_Ptr() (0x005D51E0)

AbstractClass *ObjectClass::Get_Cell_Ptr()
{
    CoordStruct coord = Coord;
    &Map[coord];
    coord = Coord;
    return (AbstractClass *)&Map[coord];   // the cell object (hypothesis)
}


// Cell ObjectClass::Get_Target_Cell() const (0x005D5240)

Cell ObjectClass::Get_Target_Cell() const
{
    return Coord_Cell(unknown_slot19(0));
}


// CellClass *ObjectClass::Get_Target_Cell_Ptr() const (0x005D5290)

CellClass *ObjectClass::Get_Target_Cell_Ptr() const
{
    return &Map[Coord_Cell(unknown_slot19(0))];
}


// bool ObjectClass::Is_Moving_Onto_Bridge(TechnoClass *techno) (0x005D52F0, slot 46)
// Starts from +0x79 (on a bridge). Compares the ground height under slot 19's
// coordinate for techno with the ground height under the object: off a bridge,
// a drop of more than three height levels (unknown_geometry_g) onto a bridge
// cell answers true; on a bridge, a rise of more than three levels answers
// false. techno is passed to slot 19 (TechnoClass argument; the walk
// locomotor 0x0071E220 passes its LinkedTo).

bool ObjectClass::Is_Moving_Onto_Bridge(TechnoClass *techno)
{
    bool result = IsOnBridge;
    CoordStruct coord = unknown_slot19(techno);
    int there = Map.Get_Height_GL(coord);
    int ground = Map.Get_Height_GL(CoordStruct(Coord));
    if (!IsOnBridge && ground - there > unknown_geometry_g * 3 && (Map_Cell(coord)->Flags & 0x100)) {
        return true;
    }
    if (IsOnBridge && there - ground > unknown_geometry_g * 3) {
        return false;
    }
    return result;
}


// void ObjectClass::Set_Waypoint_Path(int path, char waypoint) (0x005D53D0)

void ObjectClass::Set_Waypoint_Path(int path, char waypoint)
{
}


// bool ObjectClass::On_Ground() const (0x005D53E0)

bool ObjectClass::On_Ground() const
{
    if (IsDown && Get_Height_AGL() < unknown_geometry_g * 2) {
        return true;
    }
    return false;
}


// bool ObjectClass::In_Air() const (0x005D5410)

bool ObjectClass::In_Air() const
{
    if (IsDown && Get_Height_AGL() >= unknown_geometry_g * 2) {
        return true;
    }
    return false;
}


// TechnoTypeClass *ObjectClass::Techno_Type_Class() (0x005D5440)
// The object's techno type: none for a plain object (TechnoClass returns its type).

TechnoTypeClass *ObjectClass::Techno_Type_Class()
{
    return 0;
}


// int ObjectClass::Sort_Y() const (0x005D5450)

int ObjectClass::Sort_Y() const
{
    return unknown_slot42().Y + unknown_slot42().X;
}


// bool ObjectClass::Not_Underground() const (0x005D5490)

bool ObjectClass::Not_Underground() const
{
    return Get_Height_AGL() > -20;
}


// bool ObjectClass::CanBeSelected() (0x005D54B0)

bool ObjectClass::CanBeSelected()
{
    if (IsDisguisedAs(PlayerPtr) && GetDisguiseHouse(true) == 0) {
        return false;
    }
    return Class_Of()->IsSelectable;
}


// CoordStruct ObjectClass::unknown_slot41(int arg1) const (0x005D54F0)

CoordStruct ObjectClass::unknown_slot41(int arg1) const
{
    return Center_Coord();
}


// void ObjectClass::Set_Ambient_Sound(int value) (0x005D5520)

void ObjectClass::Set_Ambient_Sound(int value)
{
    CustomSound = value;
    if (value == -1) {
        unknown_50.Unknown_00405FE0();
    }
}
