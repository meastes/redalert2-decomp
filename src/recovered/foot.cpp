// foot.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/foot.json carries the addresses and the rows that
// stay in their own files.


#define RGB_DECLARE_ACCESSORS
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_COORD_AS_CELL
// Header-static dynamic initializers of the unit at 0x004C26C0 and the
// unit's FootClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
// The list is declared (extern) before it is defined: VC6 emits the
// initializer pair a second time for an extern redeclaration that follows
// the definition.
#include "shared/foot.hpp"

// Member-wise sum of two cells as this unit's code compiles it (Foot.CPP; a
// free sum built through TPoint2D<short>, hypothesis): Search_For_Tiberium_Weighted
// 0x004CBB40, 0x004C45B0 and slot 93 0x004C7220 order the component loads as
// this form does, where TPoint2D<short>::operator+ orders them otherwise. The
// guard is shared with BuildingClass slot 52's unit-local copy.
#ifndef RA2_CELLSTRUCT_PLUS
#define RA2_CELLSTRUCT_PLUS
inline Cell operator+(Cell const &left, Cell const &right)
{
    return TPoint2D<short>(short(left.X + right.X), short(left.Y + right.Y));
}
#endif

// The unit's list of FootClass objects (0x00865F38; name is a hypothesis).
DynamicVectorClass<FootClass *> unknown_FootClass_list_00865f38;

#include "shared/cell.hpp"


// Dynamic initializer (0x004C26F0, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2710, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2730, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2750, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2770, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2790, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C27D0, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2800, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2830, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2860, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2870, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C2890, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C28B0, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// Dynamic initializer (0x004C28F0, _$E) emitted by HeaderStatics_diagonal_init_004c26c0.cpp; no source of its own.


// FootClass::FootClass(HouseClass *house) (0x004C2930)
// A foot unit of a house: no path, no team, no destination; listed in the FootClass list
// (0x00865F38) and in the list at 0x00AC0C18.
#include "shared/object_lists.hpp"

FootClass::FootClass(HouseClass *house) :
    TechnoClass(house),
    unknown_418(-1),
    unknown_41c(0, 0),
    unknown_420(0, 0),
    unknown_428(0.0),
    unknown_430(0),
    unknown_434(false),
    unknown_438(0),
    unknown_450(0, 0),
    unknown_454(0, 0),
    unknown_458(0, 0),
    unknown_45c(0, 0, 0),
    unknown_468(0.0),
    unknown_470(1.0),
    unknown_478(0, 0),
    unknown_490(0),
    unknown_494(0),
    unknown_498(0),
    unknown_49c(0, 0),
    unknown_4b4(-1),
    unknown_4b8(0),
    unknown_4bc(0),
    unknown_4c1(false),
    Team(0),
    unknown_4c8(0),
    unknown_4cc(0),
    unknown_53c(10),
    unknown_568(unknown_geometry_l),
    unknown_574(-1),
    unknown_575(false),
    unknown_576(0),
    unknown_577(false),
    unknown_578(false),
    unknown_579(false),
    unknown_57a(false),
    unknown_57b(false),
    unknown_57c(false),
    unknown_57d(false),
    unknown_57e(false),
    unknown_580(0),
    unknown_584(0),
    unknown_588(0),
    unknown_598(false),
    unknown_599(false),
    unknown_59a(false),
    unknown_59b(false),
    unknown_59c(false),
    unknown_59d(false),
    unknown_59e(false),
    unknown_59f(false),
    unknown_5a0(true),
    unknown_5a1(false),
    unknown_5a2(false)
{
    unknown_4d0 = -1;
    unknown_43c.Init();
    unknown_FootClass_list_00865f38.Add(this);
    AbstractFlags.IsFoot = 1;
    unknown_list_ac0c18.Add(this);
}


// FootClass::FootClass(NoInitClass const &noinit) (0x004C2C60)
// Load path: the vectors' vtables and a cleared locomotor pointer.

FootClass::FootClass(NoInitClass const &noinit) :
    TechnoClass(noinit),
    unknown_478(noinit),
    unknown_49c(noinit),
    unknown_530(noinit),
    unknown_540(noinit),
    unknown_54c(noinit),
    unknown_558(noinit),
    unknown_58c(noinit)
{
}


// FootClass::~FootClass() (0x004C2CB0)
// Leaves its team (only while unknown_flag_a40958 is set) and both lists,
// deletes the owned object at +0x588 and stops the sound controller; the
// member destructors release the locomotor and the two vectors.
#include "shared/parasite.hpp"

FootClass::~FootClass()
{
    if (Team != 0 && unknown_flag_a40958) {
        Team->Unknown_006B8FB0(this, -1, 0);
    }
    unknown_FootClass_list_00865f38.Delete(this);
    unknown_list_ac0c18.Delete(this);
    if (unknown_588 != 0) {
        delete unknown_588;
        unknown_588 = 0;
    }
    unknown_43c.Unknown_00406070();
    unknown_43c.Unknown_00405C10();
}


// void FootClass::unknown_slot306(double value) (0x004C2DF0)
// Stores a factor clamped to 0..1 (hypothesis: the unit's speed fraction).

template <class T>
inline T Unknown_Clamp(T const &value, T const &low, T const &high)
{
    if (value >= high) {
        return high;
    }
    if (value <= low) {
        return low;
    }
    return value;
}

void FootClass::unknown_slot306(double value)
{
    unknown_468 = Unknown_Clamp(value, 0.0, 1.0);
}


// bool FootClass::Mark(int arg1) (0x004C2E60)
// Mode 2 always succeeds; otherwise TechnoClass's slot 71, and in layer 2
// the map is told about the object at its cell (0x0054AF70 for mode 0,
// 0x0054AB70 for modes 1 and 3).
#include "shared/map_view.hpp"

bool FootClass::Mark(int arg1)
{
    if (arg1 == 2) {
        return true;
    }
    if (TechnoClass::Mark(arg1)) {
        if (In_Which_Layer() == 2) {
            Cell cell = unknown_slot104();
            switch (arg1) {
            case 0:
                Map.Pick_Up(cell, this);
                break;
            case 1:
            case 3:
                Map.Place_Down(cell, this);
                return true;
            }
        }
        return true;
    }
    return false;
}


// bool FootClass::unknown_slot171(CoordStruct const &coord) (0x004C2EF0)
// Whether the unit's movement zone can reach the cell of a coordinate from
// its own cell (the map's path test 0x0054F7C0, with the target cell's
// bridge level); always for zone -1, never for the zero cell.
#include "shared/house.hpp"
#include "shared/techno_type.hpp"

bool FootClass::unknown_slot171(CoordStruct const &coord)
{
    int zone = Techno_Type_Class()->MovementZone;
    if (zone == -1) {
        return true;
    }
    Cell cell = Coord_Cell(coord);
    if (cell == unknown_geometry_k) {
        return false;
    }
    Cell here = Coord_Cell(unknown_slot19(0));
    return Map.Unknown_0054F7C0(here, cell, zone, Is_Moving_Onto_Bridge(0),
        (unsigned char)((((UnknownCellClass *)&Map[cell])->Flags >> 8) & 1), unknown_slot189());
}


// Basic_Path004C3000: search for a legal destination and record a path.
#include "shared/team.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/crt_memory.hpp"
static inline int Path_Length3(UnknownVector3 vector)
{
    double x=vector.X;
    double y=vector.Y;
    double z=vector.Z;
    return (int)unknown_sqrt_4ba410(x*x+y*y+z*z);
}
static inline int Path_Distance3(CoordStruct const &a,CoordStruct const &b)
{
    return Path_Length3(UnknownVector3(a.X-b.X,a.Y-b.Y,a.Z-b.Z));
}
static inline int Path_Length2(CoordStruct const &vector)
{
    return (int)unknown_sqrt_4ba410((double)vector.X*vector.X+(double)vector.Y*vector.Y);
}
template<class T> static inline T Path_Max(T a,T b) { return a>b?a:b; }
template<class T> static inline T Path_Min(T a,T b) { return a<b?a:b; }
bool FootClass::unknown_4c3000(Cell cell,int path_offset,int avoidance)
{
    if(path_offset==0) Path[0]=-1;
    if(!unknown_slot171(CoordStruct(cell.X*256+128,cell.Y*256+128,0))) {
        Path[0]=-1;
        return false;
    }
    int distance=Path_Distance3(Center_Coord(),Cell_Center(cell));
    int close_enough=Team?Team->Unknown_006BE6A0():Rules->unknown_1380;
    int max_distance=0;
    bool train=Techno_Type_Class()->IsTrain;
    int move=Can_Enter_Cell(&Map[cell],-1,-1,0,true);
    if(move==6 && distance>close_enough && !train) {
        int zone=unknown_zone_table_7a1200[Techno_Type_Class()->MovementZone];
        bool check=Techno_Type_Class()->unknown_af5 && Map[cell].Can_Burrow_Here();
        Cell nearby=Map.Nearby_Location(cell,Techno_Type_Class()->SpeedType,
            Map.Zone(unknown_slot104(),zone,unknown_79),zone,unknown_79,Point2D(1,1),false,true,check,true,unknown_slot104(),0);
        int dest_y=cell.Y;
        int nearby_y=nearby.Y;
        int dest_x=cell.X;
        max_distance=Path_Max(abs(nearby.X-dest_x),abs(nearby_y-dest_y));
        if(nearby!=unknown_geometry_k && Path_Length2(CoordStruct(dest_x*256+128,dest_y*256+128,0)-CoordStruct(nearby.X*256+128,nearby_y*256+128,0))<distance) {
            int walk=Search.Test_Cell_Walk(nearby,cell,this,
                (Map[nearby].unknown_140&0x100)!=0,(Map[cell].unknown_140&0x100)!=0,check?0:-1);
            if(walk<=max_distance+6) {
                Assign_Destination(&Map[nearby],true);
                cell=nearby;
            }
        }
    }
    if(move==7 && !train) {
        if(Map[cell].Cell_Building()!=0) {
            int zone=unknown_zone_table_7a1200[Techno_Type_Class()->MovementZone];
            bool check=Techno_Type_Class()->unknown_af5 && Map[cell].Can_Burrow_Here();
            Cell nearby=Map.Nearby_Location(cell,Techno_Type_Class()->SpeedType,
                Map.Zone(unknown_slot104(),zone,unknown_79),zone,unknown_79,Point2D(1,1),false,true,check,true,unknown_slot104(),0);
            Assign_Destination(&Map[nearby],true);
            cell=nearby;
        }
    }
    Mark(0);
    if(path_offset==0) Path[0]=-1;
    bool found=false;
    PathStruct saved_path;
    int work_path[2000];
    PathStruct *path=Find_Path_004bb350(&cell,work_path,2000,0,path_offset,avoidance);
    if(path && path->Cost) {
        memcpy(&saved_path,path,sizeof(saved_path));
        found=true;
    }
    if(found) {
        unknown_slot305((int)&saved_path);
        memcpy(&Path[path_offset],work_path,Path_Min(path->Length,24-path_offset)*sizeof(Path[0]));
    }
    Mark(1);
    unknown_530=0;
    if(Kind_Of()==1 && Path[0]!=-1) {
        UnitClass *unit=static_cast<UnitClass *>(this);
        FootClass *follower=unit->unknown_5b0;
        int length=0;
        if(path) length=path->Length-2;
        Cell next_cell=cell;
        if(follower && !unit->unknown_5b8) {
            while(follower) {
                CellClass *current=&Map[next_cell];
                next_cell=current->Adjacent_Cell((work_path[length]+4)%8).MapCoords;
                AbstractClass *destination=follower->unknown_494;
                if(!destination || Coord_Cell(destination->unknown_slot19(0))!=next_cell) {
                    follower->QueueMission(2,false);
                    follower->Assign_Destination(&Map[next_cell],true);
                    follower->unknown_4c3000(next_cell,0,0);
                }
                follower=static_cast<UnitClass *>(follower)->unknown_5b0;
            }
        }
    }
    if(path) {
        unknown_450=unknown_slot104();
        return true;
    }
    unknown_530=(int)(Rules->unknown_13c8*900.0);
    unknown_slot291();
    CoordStruct coord=Coord;
    Cell current=Coord_Cell(coord);
    max_distance=Path_Max(abs(current.X-cell.X),abs(current.Y-cell.Y));
    if(max_distance>1 || (!unknown_79 && (Map[cell].unknown_140&0x100))) {
        if(Team) {
            Locomotor.Checked()->Lock();
            Team->Unknown_006B8FB0(this,-1,false);
            Locomotor.Checked()->Unlock();
        }
        Assign_Destination(0,true);
        unknown_slot220(0);
        if(!House->Unknown_004F52F0()) {
            QueueMission(11,false);
            if(unknown_game_mode_a3d298!=0) {
                Cell guard=House->Unknown_004EB270(this);
                if(guard!=unknown_geometry_k) Assign_Destination(&Map[guard],true);
            }
        } else QueueMission(5,false);
    }
    return false;
}


// int FootClass::Do_MISSION_MOVE() (0x004C38D0)
// A stopped unit without a pending mission and nothing at +0x494 gives up
// (slot 261) and is called again next frame; otherwise the mission's rate
// in frames (minutes times 900) plus a random 0..2.

int FootClass::Do_MISSION_MOVE()
{
    if (unknown_494 == 0 && !Locomotor.Checked()->Is_Moving() && MissionQueue == -1) {
        unknown_slot261(0, true);
        return 1;
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// int FootClass::Do_MISSION_PATROL() (0x004C3950, slot 145)
// The patrol mission (TibSun Do_MISSION_PATROL shape; the switch on the
// mission status +0xA8 ends in a four-entry jump table 0x004C41D0). The scan
// distance is the threat range (slot 188, mode 2) in cells plus 6; +0x37F is
// set. 3: waits for a destination (+0x494), then 0. 0: takes the greatest
// threat around the unit (slot 219 mode 2); an object in range of its weapon
// (slots 174, 212) or within the walk distance (0x0042AA30) becomes the
// target: the patrol cell (+0x4CC) is the target cell (slot 107), moved to a
// nearby cell (0x005502E0) when it is a bridge cell and the unit is not
// moving onto a bridge (60 when none); the archive target (+0x1B0, through
// 0x006D6E50) becomes the last queued destination (+0x478), the destination
// or the patrol cell; status 1. Without one, a unit that is not moving goes
// idle (slot 261) with status 3. 1: a renovator (slot 193) drops an allied
// building above ConditionRed; a target in reach is approached (slot 304)
// and, once gone, the unit heads back to the archive target with status 0
// (45). Otherwise a new threat around the patrol cell is taken (1), or the
// unit heads to the closer of the patrol cell and the archive target with
// status 2, or goes idle. 2: a threat around the patrol cell is taken
// (status 1); without a destination the unit heads to the archive target
// (status 0) or goes idle. Returns the mission delay plus 0..2.
#include "shared/path_estimator.hpp"
#include "shared/id_source.hpp"

int FootClass::Do_MISSION_PATROL()
{
    int trange = unknown_slot188(2);
    unknown_37f = true;
    int dist = trange / 256 + 6;

    switch (unknown_a8) {
    case 3:
        if (unknown_494 != 0) {
            unknown_a8 = 0;
            return 1;
        }
        break;

    case 0: {
        CoordStruct coord = Coord;
        AbstractClass *target = unknown_slot219(2, &coord, false);
        ObjectClass *optr = target == 0 ? 0 : (target->AbstractFlags.IsObject ? (ObjectClass *)target : 0);
        int primary = unknown_slot174(optr);

        if (optr != 0 && (unknown_slot212(optr, primary) || Search.Test_Cell_Walk(Coord_Cell(unknown_slot19(0)), Coord_Cell(optr->unknown_slot19(0)), this, Is_Moving_Onto_Bridge(0), optr->Is_Moving_Onto_Bridge(0), -1) < dist)) {
            unknown_4cc = (AbstractClass *)Get_Target_Cell_Ptr();
            if ((((UnknownCellClass *)unknown_4cc)->Flags & 0x100) && !Is_Moving_Onto_Bridge(0)) {
                Cell cell = Map.Nearby_Location(((UnknownCellClass *)unknown_4cc)->unknown_24, Techno_Type_Class()->SpeedType, Map.Zone(((UnknownCellClass *)unknown_4cc)->unknown_24, Techno_Type_Class()->MovementZone, false), Techno_Type_Class()->MovementZone, false, Point2D(1, 1), 0, 1, 0, 0, Cell(0, 0), 0);
                if (cell != unknown_geometry_k) {
                    unknown_4cc = (AbstractClass *)&Map[cell];
                } else {
                    unknown_4cc = 0;
                    return 60;
                }
            }

            if (unknown_1b0 == 0) {
                if (unknown_478.Count() > 0) {
                    unknown_6d6e50(unknown_478[unknown_478.Count() - 1]);
                } else if (unknown_494 != 0) {
                    unknown_6d6e50(unknown_494);
                } else {
                    unknown_6d6e50(unknown_4cc);
                }
            }

            Assign_Destination(0, true);
            unknown_slot220(target);
            unknown_a8 = 1;

        } else if (unknown_494 == 0 || !Locomotor.Checked()->Is_Moving()) {
            unknown_slot261(false, true);
            unknown_a8 = 3;
            return 1;
        }
        break;
    }

    case 1: {
        ObjectClass *optr = unknown_228 == 0 ? 0 : (unknown_228->AbstractFlags.IsObject ? (ObjectClass *)unknown_228 : 0);
        int primary = unknown_slot174(optr);

        if (unknown_slot193() && optr != 0 && optr->Kind_Of() == 6 && House->Is_Ally(((TechnoClass *)optr)->House) && optr->Health_Ratio() > Rules->unknown_1370) {
            unknown_slot220(0);
        } else if (optr != 0 && (unknown_slot212(optr, primary) || Search.Test_Cell_Walk(((UnknownCellClass *)unknown_4cc)->unknown_24, Coord_Cell(optr->unknown_slot19(0)), this, ((UnknownCellClass *)unknown_4cc)->Flags >> 8 & 1, optr->Is_Moving_Onto_Bridge(0), -1) < dist)) {
            unknown_slot304();
            if (unknown_228 != 0) {
                break;
            }
            Assign_Destination(unknown_1b0, true);
            unknown_6d6e50(0);
            unknown_4cc = 0;
            unknown_a8 = 0;
            return 45;
        }

        if (unknown_228 == 0) {
            AbstractClass *target = unknown_slot219(2, &unknown_4cc->Center_Coord(), false);
            ObjectClass *scan = target == 0 ? 0 : (target->AbstractFlags.IsObject ? (ObjectClass *)target : 0);
            int weapon = unknown_slot174(scan);
            if (scan != 0 && (unknown_slot212(scan, weapon) || Search.Test_Cell_Walk(((UnknownCellClass *)unknown_4cc)->unknown_24, Coord_Cell(scan->unknown_slot19(0)), this, ((UnknownCellClass *)unknown_4cc)->Flags >> 8 & 1, scan->Is_Moving_Onto_Bridge(0), -1) < dist)) {
                unknown_slot220(target);
                Assign_Destination(0, true);
                return 1;
            }
        }

        unknown_a8 = 2;
        unknown_slot220(0);
        if (unknown_1b0 != 0) {
            UnknownCellClass *tcell = (UnknownCellClass *)Get_Target_Cell_Ptr();
            bool onbridge = Is_Moving_Onto_Bridge(0);
            UnknownCellClass *acell = (UnknownCellClass *)&Map[unknown_1b0->Center_Coord()];
            int patrolwalk = Search.Test_Cell_Walk(tcell->unknown_24, ((UnknownCellClass *)unknown_4cc)->unknown_24, this, onbridge, ((UnknownCellClass *)unknown_4cc)->Flags >> 8 & 1, -1);
            if (patrolwalk < Search.Test_Cell_Walk(tcell->unknown_24, acell->unknown_24, this, onbridge, acell->Flags >> 8 & 1, -1)) {
                Assign_Destination(unknown_4cc, true);
            } else {
                Assign_Destination(unknown_1b0, true);
            }
            return 1;
        }
        unknown_a8 = 0;
        unknown_slot261(false, true);
        unknown_a8 = 3;
        return 1;
    }

    case 2: {
        AbstractClass *target = unknown_slot219(2, &unknown_4cc->Center_Coord(), false);
        ObjectClass *optr = target == 0 ? 0 : (target->AbstractFlags.IsObject ? (ObjectClass *)target : 0);
        int primary = unknown_slot174(optr);

        if (optr != 0 && (unknown_slot212(optr, primary) || Search.Test_Cell_Walk(((UnknownCellClass *)unknown_4cc)->unknown_24, Coord_Cell(optr->unknown_slot19(0)), this, ((UnknownCellClass *)unknown_4cc)->Flags >> 8 & 1, optr->Is_Moving_Onto_Bridge(0), -1) < dist)) {
            unknown_slot220(target);
            Assign_Destination(0, true);
            unknown_a8 = 1;
            break;
        }

        if (unknown_494 == 0) {
            unknown_slot220(0);
            if (unknown_1b0 != 0) {
                Assign_Destination(unknown_1b0, true);
                unknown_6d6e50(0);
                unknown_a8 = 0;
                unknown_4cc = 0;
            } else {
                unknown_a8 = 0;
                unknown_slot261(false, true);
                unknown_a8 = 3;
                return 1;
            }
        }
        break;
    }

    default:
        break;
    }

    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// int FootClass::Do_MISSION_CAPTURE() (0x004C41E0)
// As slot 128 for infantry that can take, occupy, assault or infiltrate
// the building it targets; with no target a computer-controlled unit that
// is not an Occupier drops its target and enters mission 15. Returns the
// mission's rate in frames plus 0..2.
#include "shared/infantry.hpp"

int FootClass::Do_MISSION_CAPTURE()
{
    InfantryClass *infantry = As_Infantry(this);
    if (unknown_228 != 0 && unknown_228->Kind_Of() == 6 && unknown_494 == 0 && infantry != 0) {
        if (infantry->Type->C4 || infantry->unknown_6d7910(14) || infantry->Type->Engineer
            || infantry->Type->Occupier || infantry->Type->Assaulter || infantry->Type->Infiltrate) {
            Assign_Destination(unknown_228, true);
        }
    }
    if (unknown_494 == 0) {
        unknown_slot261(0, true);
        if (((UnknownCellClass *)&Map[Center_Coord()])->Unknown_004717B0() != 0) {
            Scatter(unknown_geometry_l, 1 != 0, 0 != 0);
        }
    }
    if (unknown_228 == 0 && !House->Unknown_004F52F0()
        && (Kind_Of() != 15 || !((InfantryClass *)this)->Type->Occupier)) {
        unknown_slot220(0);
        Assign_Destination(0, true);
        QueueMission(15, false);
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// int FootClass::Do_MISSION_EATEN() (0x004C4370)
// A capture step: an infantry unit that can take buildings (C4, ability 14
// or Engineer) heads for the building it targets; without a destination it
// gives up its order and, standing on a building, scatters (slot 87); with
// no target a computer-controlled... (0x004F52F0) unit drops its target and
// enters mission 5. Returns the mission's rate in frames plus 0..2.

int FootClass::Do_MISSION_EATEN()
{
    InfantryClass *infantry = As_Infantry(this);
    if (unknown_228 != 0 && unknown_228->Kind_Of() == 6 && unknown_494 == 0 && infantry != 0) {
        if (infantry->Type->C4 || infantry->unknown_6d7910(14) || infantry->Type->Engineer) {
            Assign_Destination(unknown_228, true);
        }
    }
    if (unknown_494 == 0) {
        unknown_slot261(0, true);
        if (((UnknownCellClass *)&Map[Center_Coord()])->Unknown_004717B0() != 0) {
            Scatter(unknown_geometry_l, 1 != 0, 0 != 0);
        }
    }
    if (unknown_228 == 0 && !House->Unknown_004F52F0()) {
        unknown_slot220(0);
        Assign_Destination(0, true);
        QueueMission(5, false);
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// int FootClass::Do_MISSION_ATTACK() (0x004C44C0)
// A guard step: a HoverAttack type without a value from slot 108 heads for
// a cell near itself (0x006CF130); a pending scan (+0x57E) picks a target
// (slot 219) and assigns it (slot 220); then slot 304 when there is a
// target, else slot 261. Returns the mission's rate in frames plus 0..2.

int FootClass::Do_MISSION_ATTACK()
{
    if (Techno_Type_Class()->HoverAttack && Get_Height_AGL() == 0) {
        Assign_Destination((AbstractClass *)&Map[unknown_6cf130(this)], true);
    }
    if (unknown_57e) {
        CoordStruct coord = Coord;
        AbstractClass *target = unknown_slot219(1, &coord, 0);
        if (target != 0) {
            unknown_slot220(target);
            unknown_57e = false;
        }
    }
    if (unknown_228 != 0) {
        unknown_slot304();
    } else {
        unknown_slot261(0, true);
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// BuildingClass *FootClass::unknown_4c45b0() (0x004C45B0, body 0xA4 bytes:
// the frozen inventory splits it at the loop branch 0x004C463A)
// The first building next to the unit (the eight adjacent cells in facing
// order) whose type is Overpowerable and whose owner is the unit's.
#include "shared/building_type.hpp"
#include "shared/aircraft.hpp"
#include "shared/techno_map.hpp"

BuildingClass *FootClass::unknown_4c45b0()
{
    for (int facing = 0; facing < 8; facing++) {
        Cell cell = AdjacentCell[facing & 7] + unknown_slot104();
        BuildingClass *building = Map_Cell_At(cell)->Unknown_004717B0();
        if (building != 0 && building->Type->Overpowerable && building->Owning_House() == Owning_House()) {
            return building;
        }
    }
    return 0;
}


// int FootClass::Do_MISSION_GUARD() (0x004C4660)
// A mission step for electric-assault units: without a target, a unit whose
// secondary weapon's warhead is ElectricAssault looks for an Overpowerable
// building of its own house in the eight cells around it, targets it and
// enters mission 1 (otherwise slot 258); with a target a HoverAttack type
// that is not in the air (slot 108) moves to the target's cell. A kind-15
// unit of a computer-controlled... (0x004F52F0) house that can capture
// (C4 or ability 14) switches to mission 17 when it targets a building.
// Returns the remaining fire delay (timer +0x244), 0 for a DistributedFire
// type with +0x3B8 positive, else the mission's rate in frames plus 0..2.

int FootClass::Do_MISSION_GUARD()
{
    if (unknown_228 == 0) {
        if (unknown_slot231(1)->WeaponType != 0 && unknown_slot231(1)->WeaponType->Warhead->ElectricAssault) {
            BuildingClass *building = 0;
            for (int facing = 0; facing < 8; facing++) {
                Cell cell = Adjacent_Cell(unknown_slot104(), facing);
                BuildingClass *candidate = ((UnknownCellClass *)&Map[cell])->Unknown_004717B0();
                if (candidate != 0 && candidate->Type->Overpowerable) {
                    HouseClass *owner = Owning_House();
                    if (candidate->Owning_House() == owner) {
                        building = candidate;
                        break;
                    }
                }
            }
            if (building != 0) {
                unknown_slot220(building);
                unknown_57e = true;
                QueueMission(1, false);
            } else {
                unknown_slot258();
            }
        } else {
            unknown_slot258();
        }
    } else if (Techno_Type_Class()->HoverAttack && Get_Height_AGL() == 0) {
        Assign_Destination((AbstractClass *)&Map[unknown_6cf130(this)], true);
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    if (this != 0 && Kind_Of() == 15 && !House->Unknown_004F52F0()
        && (((InfantryClass *)this)->Type->C4 || unknown_6d7910(14))
        && Get_Mission() != 17 && unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
        QueueMission(17, false);
    }
    int result = 0;
    if (unknown_244.Value() != 0) {
        result = unknown_244.Value();
    } else if (!Techno_Type_Class()->DistributedFire || unknown_3a8.Count() <= 0) {
        result = delay + unknown_id_source_a3d290->Random()(0, 2);
    }
    return result;
}


// int FootClass::Do_MISSION_HUNT() (0x004C48E0)
// A capture step: unless the type is StupidHunt, a unit able to act at its
// location (slot 209) that is infantry heads for its target: an Engineer
// without C4 or ability 14 moves to it in mission 8; with C4 or ability 14 a
// building target puts it in mission 17; a VehicleThief moves to its target
// in mission 8, any other infantry drops the move and calls slot 304 (as do
// non-infantry units). Otherwise a human-controlled... (0x004F52F0) unit in
// a single-player game (0x00A3D298 zero) heads for the player's base centre
// in mission 2. Returns the mission's rate in frames plus 0..2.
// TibSun Do_MISSION_HUNT shape (each branch tests the infantry pointer); the
// base-centre cell starts as unknown_geometry_k and is then assigned (the
// original's dead store of k into the cell's slot).

int FootClass::Do_MISSION_HUNT()
{
    CoordStruct coord;
    if (!Techno_Type_Class()->StupidHunt && unknown_slot209(&(coord = Coord), 0)) {
        InfantryClass *infantry = Kind_Of() == 15 ? (InfantryClass *)this : 0;
        if (infantry != 0 && infantry->Type->Engineer && !infantry->Type->C4 && !infantry->unknown_6d7910(14)) {
            Assign_Destination(unknown_228, true);
            QueueMission(8, false);
            if (unknown_slot122()) {
                Commence();
            }
        } else if (infantry != 0 && (infantry->Type->C4 || infantry->unknown_6d7910(14)) && unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
            Assign_Destination(unknown_228, true);
            QueueMission(17, false);
            if (unknown_slot122()) {
                Commence();
            }
        } else if (infantry != 0 && infantry->Type->VehicleThief) {
            Assign_Destination(unknown_228, true);
            QueueMission(8, false);
            if (unknown_slot122()) {
                Commence();
            }
        } else if (infantry != 0) {
            Assign_Destination(0, true);
            unknown_slot304();
        } else {
            unknown_slot304();
        }
    } else if (!House->Unknown_004F52F0() && unknown_game_mode_a3d298 == 0) {
        Cell cell = unknown_geometry_k;
        cell = Coord_Cell(unknown_player_a35db4->BaseCenter);
        if (cell != unknown_geometry_k) {
            Assign_Destination((AbstractClass *)&Map[cell], true);
            QueueMission(2, true);
        }
    } else {
        unknown_slot258();
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// bool FootClass::unknown_slot291() (0x004C4B60)
// Stops the locomotor (ILocomotion::Stop_Moving); always false.

bool FootClass::unknown_slot291()
{
    Locomotor.Checked()->Stop_Moving();
    return false;
}


// bool FootClass::unknown_slot290(CoordStruct &coord) (0x004C4B90)
// Sends the locomotor to a coordinate (ILocomotion::Move_To) and reports
// whether it is moving.

bool FootClass::unknown_slot290(CoordStruct &coord)
{
    CoordStruct to(coord.X, coord.Y, coord.Z);
    Locomotor.Checked()->Move_To(to);
    return Locomotor.Checked()->Is_Moving() != 0;
}


// void FootClass::unknown_slot210() (0x004C4C00)
// Slot 260 (0, 1), clears unknown_4d0, slot 291, then TechnoClass's.

void FootClass::unknown_slot210()
{
    Assign_Destination(0, true);
    unknown_4d0 = -1;
    unknown_slot291();
    TechnoClass::unknown_slot210();
}


// Existing shared Checked() emits the complete original004CDE90 accessor32 bytes.
// This real-body binding reuses that original copy and earns no new byte credit.
// Approach the current combat target from a reachable cell inside weapon range.
// Existing virtual slot304; source and member names remain explicit hypotheses.
#include "shared/dir_type.hpp"
#include "shared/fast_math.hpp"
extern "C" GUID const CLSID_JumpjetLocomotionClass;
extern int const unknown_approach_angles_7d8178[25];

template<class Interface, GUID const *iid>
HRESULT UnknownComPtr<Interface,iid>::Query(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &other)
    {
        if (other) {
            Interface *pointer;
            HRESULT result=other.Checked()->QueryInterface(*iid,(void **)&pointer);
            if (FAILED(result)) {
                Release_Held();
                Pointer=0;
            } else {
                Interface *attached=pointer;
                Release_Held();
                Pointer=attached;
            }
            return result;
        }
        Interface *old=Pointer;
        if (old) {
            Pointer=0;
            old->Release();
        }
        return E_NOINTERFACE;
    }

// Original free coordinate-distance operation; the three-int difference constructor
// stays out of line, while this caller expands the length in Y,Z,X load order.
inline int Approach_Coord_Distance_004c4c30(CoordStruct const &left,CoordStruct const &right)
{
    UnknownVector3 const &difference=reinterpret_cast<UnknownVector3 const &>(left)-reinterpret_cast<UnknownVector3 const &>(right);
    double y=difference.Y;
    double z=difference.Z;
    double x=difference.X;
    return (int)Fast_Sqrt(x*x+z*z+y*y);
}
inline UnknownFacingDir Approach_Direction_004c4c30(CoordStruct const &from,CoordStruct const &to)
{
    int to_x=to.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)from.Y-(double)to.Y,(double)to_x-(double)from.X)-1.5707963267948966)*-10430.060040584269));
}
inline CoordStruct Approach_Move_Coord_004c4c30(CoordStruct const &start,DirType const &dir,double distance)
{
    double angle=(dir.As_Int()-16383)*-9.587672516830327e-05;
    double start_x=start.X;
    double start_y=start.Y;
    return CoordStruct((int)(start_x+Fast_Cos(angle)*distance),(int)(start_y-Fast_Sin(angle)*distance),start.Z);
}
void FootClass::unknown_slot304()
{
    if (unknown_228) {
    int weapon=unknown_slot174(unknown_228);
    int maximum_range=Weapon_Range(weapon);
    if (maximum_range>=512) maximum_range-=128;
    bool in_range=unknown_slot212(unknown_228,weapon);
    if (!in_range) {
        if (Mission==6) {
            unknown_slot220(0);
            Assign_Destination(0,true);
            return;
        }
        if (!Techno_Type_Class()->CanApproachTarget) {
            bool pursue=false;
            if (Mission==1) pursue=true;
            if (Mission==11 && House->Is_Human_Player()) pursue=true;
            if ((Mission!=15 || House->Is_Human_Player()) && !pursue) {
                unknown_slot220(0);
                Assign_Destination(0,true);
                return;
            }
        }
    }
    bool require_deploy_cell=false;
    if (Kind_Of()==1 && ((UnitClass *)this)->Type->DeployToFire) {
        CoordStruct position=Coord;
        if (!Map[position].Can_Build_Here()) {
            require_deploy_cell=true;
            int near_range=unknown_5d4c00(unknown_228)+512;
            if (maximum_range>=near_range) maximum_range=near_range;
        }
    }
    bool flyer=Kind_Of()==2;
    UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist(Locomotor);
    GUID class_id;
    persist.Checked()->GetClassID(&class_id);
    if (class_id==CLSID_JumpjetLocomotionClass) flyer=true;
    if (unknown_494) {
        if (unknown_228 && Techno_Type_Class()->CanRecalcApproachTarget && !in_range) {
            int separation=Approach_Coord_Distance_004c4c30(unknown_494->Center_Coord(),unknown_228->Center_Coord());
            if (separation>Rules->unknown_bec*maximum_range) {
                unknown_490=0;
                unknown_494=0;
            }
        }
    }
    if ((!unknown_494 || In_Air()) && (!in_range || !unknown_32d || require_deploy_cell)) {
    if (unknown_478.Count()>0) {
        Assign_Destination(unknown_478[0],false);
        unknown_478.Delete(0);
        return;
    }
    if (unknown_228->Kind_Of()==11) {
        CellClass *target_cell=(CellClass *)unknown_228;
        if (target_cell->Is_Tile_Destroyable_Cliff()) {
            Cell destination=unknown_slot19(0).As_Cell();
            Cell nearer=Map.Unknown_00568ED0(target_cell->MapCoords,destination);
            unknown_slot220(&Map[nearer]);
        }
    }
    if (unknown_228->Kind_Of()==6) {
        BuildingClass *building=(BuildingClass *)unknown_228;
        if (building) maximum_range+=(building->Type->Width()+building->Type->Foundation_Height(false))*64;
    }
    double adjusted;
    if (Kind_Of()==15) adjusted=(double)maximum_range-281.6;
    else adjusted=(double)maximum_range-179.2;
    maximum_range=(int)adjusted;
    maximum_range=maximum_range<=0?0:maximum_range;
    if (maximum_range>0 && maximum_range<=255) maximum_range=255;
    CoordStruct target_position=unknown_228->Center_Coord();
    CoordStruct trial_position;
    Cell target_cell=target_position.As_Cell();
    Cell trial_cell=target_cell;
    int direction=Dir_To_256(Approach_Direction_004c4c30(target_position,Center_Coord()));
    bool target_bridge=false;
    if (unknown_228->Kind_Of()==11) target_bridge=(((CellClass *)unknown_228)->unknown_140&0x100)!=0;
    else {
        AbstractClass *target=unknown_228;
        if (target && target->AbstractFlags.IsObject) target_bridge=((ObjectClass *)target)->unknown_79;
    }
    int movement_zone=Techno_Type_Class()->MovementZone;
    bool found=false;
    double floating_radius;
    int radius=maximum_range;
    if ((double)radius>204.8) {
    floating_radius=(double)radius;
    do {
        for (int index=0;index<25;++index) {
            DirType angle((Dir256)(direction+unknown_approach_angles_7d8178[index]));
            trial_position=CoordStruct(Approach_Move_Coord_004c4c30(target_position,angle.Snap_To_256(),floating_radius).As_Cell(),0);
            if (require_deploy_cell && !Map[trial_position].Can_Build_Here()) continue;
            trial_position.Z=Map.Get_Height_GL(trial_position);
            if (Map[trial_position].unknown_140&0x100) trial_position.Z+=unknown_geometry_j;
            if (!unknown_6c4bb0(&trial_position,unknown_228,unknown_slot231(weapon)->WeaponType)) continue;
            trial_cell=trial_position.As_Cell();
            if (!Map.Unknown_0055A660(trial_cell,true)) continue;
            bool clear=Map[trial_cell].Is_Clear_To_Move(Techno_Type_Class()->SpeedType,false,false,
                Map.Zone(unknown_slot19(0).As_Cell(),movement_zone,unknown_79),movement_zone,-1,true);
            if (!clear) {
                if (Kind_Of()==15) continue;
                CoordStruct saved_position=trial_position;
                Cell saved_cell=trial_cell;
                for (int adjacent=0;adjacent<8;++adjacent) {
                    trial_cell=AdjacentCell[adjacent&7].TPoint2D<short>::operator+(trial_cell);
                    trial_position=CoordStruct(trial_cell);
                    if (unknown_6c4bb0(&trial_position,unknown_228,unknown_slot231(weapon)->WeaponType)
                        && Map[trial_cell].Is_Clear_To_Move(Techno_Type_Class()->SpeedType,false,false,
                            Map.Zone(unknown_slot19(0).As_Cell(),movement_zone,unknown_79),movement_zone,-1,true)) {
                        clear=true;
                        break;
                    }
                }
                if (!clear) {
                    trial_position=saved_position;
                    trial_cell=saved_cell;
                    continue;
                }
            }
            int dy=abs((int)trial_cell.Y-target_cell.Y);
            int dx=abs((int)trial_cell.X-target_cell.X);
            int maximum_distance=dx>dy?dx:dy;
            CellClass *cell=&Map[trial_cell];
            if (!flyer && Search.Test_Cell_Walk(trial_cell,target_cell,this,
                    (cell->unknown_140&0x100)!=0,target_bridge,-1)>maximum_distance+8) {
                Cell destination=unknown_slot19(0).As_Cell();
                int dest_dy=abs((int)destination.Y-trial_cell.Y);
                int dest_dx=abs((int)destination.X-trial_cell.X);
                int dest_distance=dest_dx>dest_dy?dest_dx:dest_dy;
                if (Search.Test_Cell_Walk(destination,trial_cell,this,
                        Is_Moving_Onto_Bridge(0),(cell->unknown_140&0x100)!=0,-1)>dest_distance+8) continue;
            }
            found=true;
            break;
        }
        if (found) break;
    } while ((floating_radius=(double)(radius-=256))>204.8);
    }
    if (found) {
        Assign_Destination(&Map[trial_cell],true);
    } else if (Techno_Type_Class()->HunterSeeker || (Kind_Of()==15 && ((InfantryClass *)this)->Type->VehicleThief)) {
        Assign_Destination(unknown_228,true);
    } else {
        trial_cell=Map.Nearby_Location(trial_cell,Techno_Type_Class()->SpeedType,
            Map.Zone(unknown_slot19(0).As_Cell(),movement_zone,Is_Moving_Onto_Bridge(0)),movement_zone,
            (Map[trial_cell].unknown_140&0x100)!=0,Point2D(1,1),false,true,false,false,Cell(0,0),0);
        unknown_slot220(0);
        if (trial_cell!=unknown_geometry_k) Assign_Destination(&Map[trial_cell],true);
        else Assign_Destination(0,true);
    }
    }
    }
}


// int FootClass::Do_MISSION_GUARD_AREA() (0x004C5900, slot 130)
// Guard area (TibSun Do_MISSION_GUARD_AREA shape; hypothesis). The navigation
// queue step is the unit's own 0x004C8B00 (TibSun Handle_Navigation_List),
// inlined here through a guard-area condition helper; its Delete 0x00454B20
// and Add 0x004CE210 stay out of line (Add binds the population-19 row as
// this unit's COMDAT copy). The archive-target stray test spills its flag
// byte (shared `bool; if (target) flag = ...; if (target && flag)` rule).

inline Cell Coord_Cell_Value(CoordStruct coord)
{
    return Coord_Cell(coord);
}

inline void Guard_Navigation(FootClass *foot)
{
    if (!foot->House->Unknown_004F52F0() && foot->unknown_49c.Count() > 0 && foot->unknown_494 == 0 && foot->unknown_49c[0] == foot->unknown_1b0) {
        foot->unknown_4c8b00();
    }
}

int FootClass::Do_MISSION_GUARD_AREA()
{
    Guard_Navigation(this);

    if (unknown_1b0 != 0 && unknown_1b0->Kind_Of() == 11) {
        BuildingClass *building = ((UnknownCellClass *)&Map[unknown_1b0->Center_Coord()])->Unknown_004717B0();
        if (building != 0 && House->Is_Ally(building->House) && !House->Unknown_004F52F0()
            && (Kind_Of() != 1 || !((UnitClass *)this)->Type->Harvester)) {
            Cell nearby = Map.Nearby_Location(Coord_Cell(unknown_1b0->Center_Coord()), 1, Map.Zone(Coord_Cell_Value(Coord), 0, false), 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            if (nearby != unknown_geometry_k) {
                unknown_6d6e50((AbstractClass *)&Map[nearby]);
            }
        }
    }

    if (Kind_Of() == 1 && ((UnitClass *)this)->Type->Harvester) {
        QueueMission(10, false);
        Commence();
        return 1 + unknown_id_source_a3d290->Random()(1, 10);
    }

    if (unknown_1b0 == 0 && MissionQueue == -1) {
        unknown_6d6e50((AbstractClass *)&Map[CoordStruct(Coord)]);
    }

    InfantryClass *infantry = As_Infantry(this);
    if (!House->Unknown_004F52F0() && infantry != 0 && (infantry->Type->C4 || infantry->unknown_6d7910(14))
        && Get_Mission() != 17 && unknown_228 != 0 && unknown_228->Kind_Of() == 6) {
        QueueMission(17, false);
        return 1;
    }

    int maxrange = (int)(unknown_slot188(1) * 0.75);

    AbstractClass *archive = unknown_1b0;
    bool foot;
    if (archive != 0) {
        foot = archive->AbstractFlags.IsFoot;
    }
    if (archive != 0 && foot) {
        maxrange = Rules->GuardModeStray;
    }

    if (unknown_1b0 != 0) {
        if (!unknown_57d && unknown_494 == 0 && unknown_5d4c00(unknown_1b0) > maxrange) {
            unknown_slot220(0);
            Assign_Destination(unknown_1b0, true);
        }

        if (unknown_228 == 0) {
            if (unknown_6d40c0() && unknown_6d9710()) {
                unknown_slot209(&unknown_1b0->Center_Coord(), 2);
            }
            if (unknown_228 != 0) {
                return 1;
            }
            unknown_slot258();
        } else {
            unknown_slot304();
        }
    }

    if (unknown_228 == 0 && unknown_slot231(1)->WeaponType != 0 && unknown_slot231(1)->WeaponType->Warhead->ElectricAssault) {
        for (int facing = 0; facing < 8; facing++) {
            Cell cell = Adjacent_Cell(unknown_slot104(), facing);
            BuildingClass *candidate = ((UnknownCellClass *)&Map[cell])->Unknown_004717B0();
            if (candidate != 0 && candidate->Type->Overpowerable && candidate->Owning_House() == Owning_House()) {
                if (candidate != 0) {
                    unknown_slot220(candidate);
                    unknown_57e = true;
                    QueueMission(1, false);
                }
                break;
            }
        }
    }

    int delay = (int)(Control()->unknown_10 * 900.0);
    if (Kind_Of() == 2) {
        delay *= 2;
    }
    return delay + unknown_id_source_a3d290->Random()(1, 5);
}


// bool FootClass::Unlimbo(CoordStruct const *coord, int dir) (0x004C5E40)
// Puts the unit on the map (TechnoClass's slot 53): the locomotor is
// unlimboed and powered (off for an ion-sensitive locomotor while the
// house's test 0x0051FC80 holds), the owner is told (slot 96), the eight
// neighbouring cells gain one at +0x122, the cell is recorded (or the
// tracker at 0x00839EA0 hears of a ConsideredAircraft type), the threat
// coefficient is copied and a sensor type senses around itself.

bool FootClass::Unlimbo(CoordStruct const *coord, int dir)
{
    if (TechnoClass::Unlimbo(coord, dir)) {
        Locomotor.Checked()->Unlimbo();
        if (House->Unknown_0051FC80() && Locomotor.Checked()->Is_Ion_Sensitive()) {
            Locomotor.Checked()->Power_Off();
        } else {
            Locomotor.Checked()->Power_On();
        }
        Revealed(House);
        unknown_4d0 = -1;
        Cell cell = unknown_slot104();
        for (int facing = 0; facing < 8; facing++) {
            UnknownCellClass *next = (UnknownCellClass *)&Map[Adjacent_Cell(cell, facing)];
            next->unknown_122++;
        }
        if (!In_Air()) {
            unknown_454 = cell;
        } else if (Techno_Type_Class()->ConsideredAircraft) {
            unknown_tracker_839ea0.Unknown_004131E0(this);
        }
        double threat = Techno_Type_Class()->ThreatAvoidanceCoefficient;
        unknown_428 = threat;
        if (Techno_Type_Class()->SensorsSight != 0) {
            unknown_slot285(unknown_geometry_k);
        }
        return true;
    }
    return false;
}


// int FootClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int arg6) (0x004C6000)
// Damage: a Sonic warhead shakes off a riding parasite (and its source
// stops attacking); a parasite resets its timer by the damage beyond the
// SuppressionThreshold, or to 50 frames when healed; then TechnoClass's
// damage. The team hears of a hit; a Whiner team of a computer house calls
// for help, and a unit hit in a mission that allows it (but not one that
// forbids) gives up its order (slot 261).
#include "shared/team_type.hpp"
#include "shared/warhead_type.hpp"

int FootClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int arg6)
{
    if (warhead->Sonic && unknown_580 != 0) {
        unknown_580->unknown_588->Unknown_00607440();
        if (source != 0) {
            source->unknown_slot220(0);
        }
    }
    if (unknown_580 != 0 && source != unknown_580 && *damage > unknown_580->Techno_Type_Class()->SuppressionThreshold) {
        unknown_580->unknown_588->unknown_2c = CDTimerClass<FrameTimerClass>(*damage * 2 - unknown_580->Techno_Type_Class()->SuppressionThreshold);
    }
    if (unknown_580 != 0 && *damage < 0) {
        unknown_580->unknown_588->unknown_2c = CDTimerClass<FrameTimerClass>(50);
        unknown_580->unknown_588->Unknown_00607440();
    }
    int result = TechnoClass::Take_Damage(damage, distance, warhead, source, forced, arg6);
    if (result == 5) {
        return result;
    }
    if (result != 0 && Team != 0) {
        Team->Unknown_006B9AA0(this, result, source);
        return result;
    }
    if (result != 4 && result != 0) {
        if (Team != 0 && Team->Class->Whiner && !House->Unknown_004F52F0()) {
            if (source == 0) {
                return result;
            }
            unknown_6d3400(source);
        }
        if (source != 0 && Control()->unknown_04 && !Control()->unknown_05) {
            unknown_slot261(0, true);
        }
    }
    return result;
}


// bool FootClass::unknown_slot78(int action, ObjectClass *object, bool arg3) (0x004C61A0, slot 78)
// Carries out a click on an object (arg2 is the ObjectClass pointer; arg3
// set for waypoint orders, which ignore the 0x00A40BA4/0x00A40BA8 keys):
// queues the order through slot 200 by action. Guard area (26) sabotages an
// enemy building instead when this is C4 infantry or has ability 14; 55
// queues an event (10) for the owner; a move (1, 62, and 2 when the
// object's cell is visible or the type may move to shroud) looks for a
// nearby reachable cell when the object's zone differs, then moves with the
// object's cell as the second target. 59 guards (area when the first weapon
// heals). TibSun Active_Click_With(ObjectClass *) shape: successes return
// true, failures break to the shared return false.
#include "shared/infantry_type.hpp"
#include "shared/event.hpp"
#include "shared/target.hpp"
#include "shared/keyboard.hpp"
#include "shared/weapon_type.hpp"

// A key binding is down (the bindings are ints; WWKeyboardClass takes 16-bit keys).
inline bool Foot_Key_Down(int key) { return WWKeyboard->Down(key); }

bool FootClass::unknown_slot78(int action, ObjectClass *object, bool arg3)
{

    switch (action) {
    case 26:
        if (Can_Player_Fire() && Can_Player_Move()) {
            if (((Kind_Of() == 15 && ((InfantryClass *)this)->Type->C4) || unknown_6d7910(14))
                && object->Kind_Of() == 6 && !House->Is_Ally((TechnoClass const *)object)) {
                unknown_slot200(0x11, 0, object, 0);
            } else {
                unknown_slot200(0xB, object, 0, 0);
            }
            return true;
        }
        break;

    case 4:
    case 52:
    case 56:
        unknown_slot200(0x10, 0, 0, 0);
        return true;

    case 55:
        OutList.Add(EventClass(unknown_38->unknown_28->ArrayIndex, 10, TargetClass(this)));
        return true;

    case 5:
    case 53:
    case 63:
    case 64:
        if (Can_Player_Fire()) {
            unknown_slot200(1, object, 0, 0);
            return true;
        }
        break;

    case 3:
        if (Can_Player_Move() && object && object->As_Techno()) {
            unknown_slot200(7, 0, object, 0);
            return true;
        }
        break;

    case 9:
        if (Can_Player_Move()) {
            unknown_slot200(8, 0, object, 0);
            return true;
        }
        break;

    case 11:
        if (Can_Player_Move()) {
            unknown_slot200(9, 0, object, 0);
            return true;
        }
        break;

    case 16:
        if (Can_Player_Move()) {
            unknown_slot200(0x11, 0, object, 0);
            return true;
        }
        break;

    case 2:
        if (Map.Is_Shrouded(object->Center_Coord()) && !Techno_Type_Class()->MoveToShroud) {
            return false;
        }
        // Fall into next case.

    case 1:
    case 62:
        if (Can_Player_Move()) {
            AbstractClass *targ = object;

            CoordStruct center = object->Center_Coord();
            if (center.Z <= Map.Get_Height_GL(center)) {
                center.Z = Map.Get_Height_GL(center);
            }

            if (((Techno_Type_Class()->unknown_af5 && !Rules->AllowShroudedSubteranneanMoves) || Kind_Of() == 2) && Map.Is_Shrouded(center)) return true;

            int mzone = Techno_Type_Class()->MovementZone;
            Cell cell = Coord_Cell(object->unknown_slot19(0));
            bool in_radar = Map.Unknown_0055A660(cell, true);
            UnknownCellClass *cptr = (UnknownCellClass *)&Map[unknown_slot19(0)];

            if (mzone == 6) mzone = 0;
            else if (mzone == 9) mzone = 7;

            bool moveanywhere = In_Air() || Techno_Type_Class()->unknown_af5 || (Kind_Of() == 15 && ((InfantryClass *)this)->Type->JumpJet) || Kind_Of() == 2;

            bool ontobridge = Is_Moving_Onto_Bridge(0);
            CoordStruct coord = Cell_Coord(cell);
            coord.Z = Map.Get_Height_GL(coord);
            bool shrouded = Map.Is_Shrouded(coord);
            bool forcemove = !arg3 && (Foot_Key_Down(unknown_key_a40ba4) || Foot_Key_Down(unknown_key_a40ba8));

            if (moveanywhere && action == 2) {
                Cell nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, -1, mzone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, Point2D(1, 1), 0, 1, Techno_Type_Class()->unknown_af5 && Get_Height_AGL() < 0, 1, Cell(0, 0), 0);
                if (nearby != unknown_geometry_k) {
                    targ = &Map[nearby];
                }
            } else {
                if (!in_radar || !moveanywhere && (shrouded || action == 2 || forcemove && Kind_Of() == 15 || !Map.Unknown_0054F7C0(cptr->unknown_24, cell, mzone, ontobridge, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, false))) {
                    Cell nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, Map.Zone(cptr->unknown_24, mzone, ontobridge), mzone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
                    if (nearby != unknown_geometry_k) {
                        targ = &Map[nearby];
                    }
                }
            }

            if (targ != 0) {
                CellClass *from = 0;
                if (targ != object && object != 0) {
                    from = &Map[Coord_Cell(object->Center_Coord())];
                }
                unknown_slot200(2, 0, targ, from);
            }
            return true;
        }
        break;

    case 51:
    case 54:
        return false;

    case 59: {
        WeaponStruct *weapon = unknown_slot231(0);
        bool heals = (weapon != 0 && weapon->WeaponType != 0 && weapon->WeaponType->unknown_a4 < 0);
        unknown_slot200(heals ? 0xB : 5, this, 0, 0);
        return true;
    }

    default:
        return true;
    }
    return false;
}


// bool FootClass::unknown_slot77(int action, Cell const *arg2, Cell const *arg3, bool arg4) (0x004C69F0, slot 77)
// Carries out a click on a cell (arg2 and arg3 are Cell pointers; the
// action argument is recomputed through slot 27): refuses when the map
// rejects the cell unless the action is 2 (move), then queues the order
// through slot 200 by action. A move (1) first plays the move-flash
// animation (Rules +0xA74) when sound is on; in a multiplayer mode it is
// created with a temporary ID and moved from Anims to the second list.
// TibSun Active_Click_With(Cell const &) shape: cases break to the shared
// return true, failures return false.
#include "shared/anim.hpp"

bool FootClass::unknown_slot77(int action, Cell const *arg2, Cell const *arg3, bool arg4)
{
    Cell const &cell = *arg2;
    action = unknown_slot27(&cell, false, arg4);

    if (!Map.Unknown_0055A660(cell, true) && action != 2) {
        return false;
    }

    switch (action) {
    case 51:
        if (Can_Player_Move()) {
            Cell moveto = unknown_4cca40(cell, false);
            unknown_slot200(0x19, 0, &Map[moveto], 0);
            break;
        }
        return false;

    case 26:
        if ((Can_Player_Fire() || unknown_slot193()) && Can_Player_Move()) {
            unknown_slot200(0xB, (AbstractClass *)&Map[cell], 0, 0);
            break;
        }
        return false;

    case 6:
        unknown_slot200(0xA, 0, &Map[cell], 0);
        break;

    case 1:
    case 62:
        if (unknown_sound_enabled_7d88d6) {
            CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
            coord.Z = Map.Get_Height_GL(coord);
            if (((UnknownCellClass *)&Map[coord])->Flags & 0x100) {
                coord.Z += unknown_geometry_j;
            }
            long scenid = unknown_id_source_a3d290->UniqueID;
            if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
                unknown_id_source_a3d290->UniqueID = -3;
                Rules->unknown_a74->YSortAdjust = -5000;
            }
            AnimClass *moveflash = new AnimClass(Rules->unknown_a74, coord, 0, 1, 0x600, 0);
            if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
                unknown_id_source_a3d290->UniqueID = scenid;
                Anims.Delete(moveflash);
                unknown_anims_a35e60.Add(moveflash);
            }
        }
        // Fall into next case.

    case 2: {
        Cell moveto = unknown_4cca40(cell, false);
        if (moveto != unknown_geometry_k) {
            unknown_slot200(2, 0, &Map[moveto], &Map[*arg3]);
            break;
        }
    }
        return false;

    case 5:
    case 63: {
        UnknownCellObject710160 *veinhole = Unknown_00710160(cell);
        if (veinhole != 0 && !veinhole->unknown_e8 && veinhole->unknown_6c > 0) {
            unknown_slot200(1, (AbstractClass *)veinhole, 0, 0);
            break;
        } else {
            unknown_slot200(1, (AbstractClass *)&Map[cell], 0, 0);
            break;
        }
    }
        return false;

    case 9:
        if (Can_Player_Move()) {
            unknown_slot200(8, 0, &Map[cell], 0);
            break;
        }
        return false;

    case 3:
        if (Can_Player_Move()) {
            unknown_slot200(7, 0, &Map[cell], 0);
            break;
        }
        return false;

    case 16:
        unknown_slot200(0x11, 0, &Map[cell], 0);
        break;

    case 35: {
        UnknownTube47A130 *tunnel = ((UnknownCellClass *)&Map[cell])->Get_Tunnel();
        UnknownTube47A130 *exittunnel = ((UnknownCellClass *)&Map[Cell(tunnel->unknown_28)])->Get_Tunnel();
        Cell exit = Adjacent_Cell(Cell(tunnel->unknown_28), (exittunnel->unknown_2c - 4) & 7);
        unknown_slot200(2, 0, &Map[exit], 0);
        break;
    }
    }
    return true;
}


// Resume queued orders, finish piggybacking and settle an otherwise idle foot.
#include "shared/locomotion.hpp"
#include "shared/waypoint_path.hpp"
extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
bool FootClass::unknown_slot261(bool initial,bool resume_waypoint)
{
    if (!unknown_59d) {
        unknown_59d=true;
        TechnoClass::unknown_slot261(initial,resume_waypoint);
        if (unknown_577) {
            unknown_577=false;
            Scatter(unknown_geometry_l,true,false);
        }
        bool was_piggybacking=false;
        UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggyback(Locomotor);
        if (piggyback && piggyback.Checked()->Is_Ok_To_End()) {
            Locomotor.Attach(0);
            piggyback.Checked()->End_Piggyback(&Locomotor.Pointer);
            was_piggybacking=true;
        }
        if (unknown_478.Count()>0) {
            Assign_Destination(unknown_478[0],false);
            unknown_478.Delete(0);
            return true;
        }
        if (Mission!=25 || unknown_a8==0) {
            if (unknown_418!=-1 && resume_waypoint) {
                WaypointClass *waypoint=unknown_player_a35db4->WaypointPaths[unknown_418]->Get_Waypoint(unknown_576);
                WaypointClass *next_waypoint=unknown_player_a35db4->WaypointPaths[unknown_418]->Next_Waypoint(waypoint);
                unknown_4cb110(next_waypoint);
            }
        }
        if (was_piggybacking) return true;
        if (Kind_Of()==15 && unknown_1b0) {
            if (Coord_Cell(unknown_1b0->Center_Coord())!=Coord_Cell(Center_Coord())) {
                AbstractClass *target=unknown_1b0;
                if (Mission!=11) {
                    QueueMission(2,false);
                    unknown_6d6e50(0);
                }
                Assign_Destination(target,true);
            }
        }
        unknown_slot306(0.0);
    }
    return false;
}


// CoordStruct FootClass::unknown_slot241() (0x004C71B0)
// Unless the mission is 1, the coordinate (slot 22) of +0x490, else of
// +0x494; otherwise TechnoClass's.

CoordStruct FootClass::unknown_slot241()
{
    if (Get_Mission() != 1) {
        if (unknown_490 != 0) {
            return unknown_490->unknown_slot22();
        }
        if (unknown_494 != 0) {
            return unknown_494->unknown_slot22();
        }
    }
    return TechnoClass::unknown_slot241();
}


// void FootClass::unknown_slot93(int why) (0x004C7220, slot 93)
// Per-cell processing (TibSun Per_Cell_Process shape). When a move ends in a
// cell (why 2): clears +0x59C and +0x59A; a type with SensorsSight moves its
// sensor cells; the adjacency counts (+0x122) of the cells around the last
// cell (+0x454) move to the current cell, with 0x006D95D0/0x006D95A0 when
// the 0x0054E370 block changes; a cloaked object (+0x1B8 2) shimmers next to
// an enemy with Sensors or ability 12; an attack-type mission (21, 11, 1, 15)
// whose foot target is in range stops; while not cloaked the cell's tag and
// the horizontal, vertical and zone-entry map tags spring (events 1, 0x35
// and 0x36 for agents, 0x19, 0x1A, 0x18); a laser fence below state 8 hurts
// with the C4 warhead; a bridge object off a bridge cell falls (slot 57);
// an object off the visible map that should be deleted is (slots 282, 60);
// a selected object of a non-player house in shroud is unselected. Then a
// waypoint path's next waypoint (or the one being dragged on the map) is
// re-headed for when its offset cell changed, or the path is cleared.
#include "shared/building_type_class.hpp"
#include "shared/tag.hpp"
#include "shared/scenario_tags.hpp"
#include "shared/unknown_globals.hpp"

void FootClass::unknown_slot93(int why)
{
    if (why == 2) {
        unknown_59c = false;
        unknown_59a = false;

        if (Techno_Type_Class()->SensorsSight != 0) {
            unknown_slot286(unknown_454);
            unknown_slot285(unknown_geometry_k);
        }

        if (unknown_454 != Cell(0, 0)) {
            if (Map.Unknown_0054E370(unknown_454) != Map.Unknown_0054E370(unknown_slot104())) {
                unknown_6d95d0((UnknownCellClass *)&Map[unknown_454]);
                unknown_6d95a0((UnknownCellClass *)Get_Cell_Ptr());
            }
            int face;
            for (face = 0; face < 8; face++) {
                Cell c = Adjacent_Cell(unknown_454, face);
                UnknownCellClass *cptr = (UnknownCellClass *)&Map[c];
                cptr->unknown_122--;
            }
            unknown_454 = unknown_slot104();
            for (face = 0; face < 8; face++) {
                Cell c = Adjacent_Cell(unknown_454, face);
                UnknownCellClass *cptr = (UnknownCellClass *)&Map[c];
                cptr->unknown_122++;
            }
        }

        if (unknown_1b8 == 2) {
            for (int face = 0; face < 8; face++) {
                Cell cell = AdjacentCell[face & 7] + unknown_slot104();

                if (Map.Unknown_0055A660(cell, true)) {
                    TechnoClass *techno = ((UnknownCellClass *)&Map[cell])->Unknown_00471660(Point2D(0, 0), 0, 0);

                    if (techno && !techno->House->Is_Ally(this) && (techno->Techno_Type_Class()->Sensors || techno->unknown_6d7910(12))) {
                        Do_Shimmer();
                        break;
                    }
                }
            }
        }

        int primary = unknown_slot174(unknown_228);
        bool inrange = false;
        if (unknown_228 != 0 && unknown_228->AbstractFlags.IsFoot) {
            inrange = In_Range(((FootClass *)unknown_228)->unknown_slot287(), primary);
        }

        if ((Get_Mission() == 21 || Get_Mission() == 11 || Get_Mission() == 1 || Get_Mission() == 15) && inrange && unknown_478.Count() == 0) {
            Assign_Destination(0, true);
            Path[0] = -1;
        }

        Cell cell = unknown_slot104();
        UnknownCellClass *cellptr = (UnknownCellClass *)&Map[cell];

        if (unknown_1b8 != 2) {
            TagClass *tag = cellptr->unknown_3c;
            int x = cell.X;
            int y = cell.Y;
            if ((!(cellptr->Flags & 0x500) || unknown_79) && tag != 0) {
                tag->Spring(1, this, unknown_slot104());
                if (Kind_Of() == 15 && ((InfantryClass *)this)->Type->Agent) {
                    tag->Spring(0x35, this, unknown_slot104());
                    tag->Spring(0x36, this, unknown_slot104());
                }
            }

            if (cellptr->Flags & 0x100000) {
                for (int index = 0; index < Map.unknown_128; index++) {
                    cellptr = (UnknownCellClass *)&Map[Cell(index + Map.unknown_120, y)];
                    tag = cellptr->unknown_3c;
                    if (tag != 0) {
                        if (tag->Is_Cross_Horizontal()) {
                            tag->Spring(0x19, this, unknown_slot104());
                        }
                    }
                }
            }

            if (cellptr->Flags & 0x200000) {
                for (int index = 0; index < Map.unknown_12c; index++) {
                    tag = ((UnknownCellClass *)&Map[Cell(x, index + Map.unknown_124)])->unknown_3c;
                    if (tag != 0) {
                        if (tag->Is_Cross_Vertical()) {
                            tag->Spring(0x1A, this, unknown_slot104());
                        }
                    }
                }
            }

            for (unknown_tag_position_a35e24 = 0; unknown_tag_position_a35e24 < unknown_tags_866238.Count(); unknown_tag_position_a35e24++) {
                tag = unknown_tags_866238[unknown_tag_position_a35e24];
                if (tag != 0) {
                    if (tag->Is_Enters_Zone()) {
                        if (Map.Unknown_0054F7C0(tag->Get_Cell(), Coord_Cell(unknown_slot19(0)), Techno_Type_Class()->MovementZone, ((UnknownCellClass *)&Map[tag->Get_Cell()])->Flags >> 8 & 1, Is_Moving_Onto_Bridge(0), false)) {
                            tag->Spring(0x18, this, Coord_Cell(unknown_slot19(0)));
                        }
                    }
                }
            }
        }

        BuildingClass *building = cellptr->Unknown_004717B0();
        if (building != 0) {
            if (building->Type->LaserFence && building->unknown_4ec < 8) {
                int rtti = Kind_Of();
                if (rtti > 0 && (rtti <= 2 || rtti == 15) && Strength > 0) {
                    Take_Damage(&Strength, 0, Rules->unknown_c74, (TechnoClass *)building, true, true);
                }
            }
        }

        if (unknown_79 && !(((UnknownCellClass *)&Map[CoordStruct(Coord)])->Flags & 0x100)) {
            Fall_From_Height();
        }

        if (!unknown_7d) return;

        if (!Map.Unknown_0055A660(unknown_slot104(), true) && unknown_slot282()) {
            Delete_Me();
            return;
        }

        if (unknown_77 && !House->Is_Player_Control() && Map.Is_Shrouded(CoordStruct(Coord))) {
            Unselect();
        }
    }

    if (unknown_418 != -1) {
        WaypointPathClass *path = unknown_player_a35db4->WaypointPaths[unknown_418];
        WaypointClass *next = path->Get_Waypoint(unknown_576);
        if (next != 0) {
            Cell wp_cell = Coord_Cell(next->Coord);
            if (Map.unknown_11b8 == &next->Coord) {
                wp_cell = Coord_Cell(Map.unknown_11bc);
            }
            if (unknown_41c + wp_cell != unknown_420) {
                unknown_4cb110(next);
            }
        } else {
            unknown_418 = -1;
            unknown_576 = 0;
            unknown_41c = Cell(0, 0);
            unknown_420 = Cell(0, 0);
        }
    }

    TechnoClass::unknown_slot93(why);
}


// void FootClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination) (0x004C7AC0, slot 119)
// The navigation target (+0x494) is remembered in +0x498, TechnoClass's override runs, then
// Assign_Destination (slot 260) takes the destination, immediate. TibSun FootClass::Override_Mission
// has the same shape (SuspendedNavCom = NavCom).

void FootClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    unknown_498 = unknown_494;
    TechnoClass::Override_Mission(mission, target, destination);
    Assign_Destination(destination, true);
}


// bool FootClass::Restore_Mission() (0x004C7B00)
// TechnoClass's; on success also slot 260 with unknown_498.

bool FootClass::Restore_Mission()
{
    if (TechnoClass::Restore_Mission()) {
        Assign_Destination(unknown_498, true);
        return true;
    }
    return false;
}


// int FootClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param) (0x004C7B30)
// The radio message handler (Westwood's Receive_Message; param is the
// message's long argument):
//   17: in mission 7 (now or queued) the techno reply, answered 1;
//   18: a contact standing in this unit's cell answers 20; otherwise a
//       guarding (5) unit with nothing queued queues mission 2, a queued
//       mission 7 is commenced, the unit heads for the contact and restarts
//       the mission timer (answer 1);
//   19: stores the destination (+0x494) in *param; a unit still moving
//       toward it answers 10, else 1;
//   23: drops a destination that is the radio contact, leaves missions 0
//       and 7 for 5 and, without +0x599 and destination, scatters (slot 87);
//   28: 10 while it has a destination;
//   35: 1 when the sender is the building in this unit's cell, else 10;
// everything else goes to TechnoClass.

int FootClass::Receive_Message(RadioClass *from, int message, AbstractClass *&param)
{
    switch (message) {
    case 35:
        return Map_Cell(Center_Coord())->Unknown_004717B0() == (BuildingClass *)from ? 1 : 10;
    case 28:
        if (unknown_494 != 0) {
            return 10;
        }
        break;
    case 23:
        if (HasAnyLink() && unknown_494 == Contact(0)) {
            Assign_Destination(0, true);
        }
        if (Get_Mission() == 0) {
            QueueMission(5, false);
            if (unknown_slot122()) {
                Commence();
            }
        }
        if (Get_Mission() == 7) {
            QueueMission(5, false);
        }
        if (!unknown_599 && unknown_494 == 0) {
            Scatter(unknown_geometry_l, 1 != 0, 1 != 0);
        }
        break;
    case 19:
        param = unknown_494;
        if (unknown_494 != 0 && Locomotor.Checked()->Is_Moving()) {
            return 10;
        }
        return 1;
    case 18:
        if (param != 0) {
            Cell cell = Coord_Cell(param->Center_Coord());
            if (unknown_slot104() == cell) {
                return 20;
            }
        }
        if (Get_Mission() == 5 && MissionQueue == -1) {
            QueueMission(2, false);
        }
        if (MissionQueue == 7 && unknown_slot122()) {
            Commence();
        }
        Assign_Destination(param, true);
        Timer = 0;
        return 1;
    case 17:
        if (Get_Mission() == 7 || MissionQueue == 7) {
            TechnoClass::Receive_Message(from, message, param);
            return 1;
        }
        break;
    }
    return TechnoClass::Receive_Message(from, message, param);
}


// int FootClass::Do_MISSION_ENTER() (0x004C7E10)
// A mission handler (hypothesis: a move mission): with a radio contact (or
// the +0x1B0 target as a techno) the unit asks it for message 0xE; refused
// outside +0x370 it gives up (message 3, slot 261); otherwise an idle unit
// with queued destinations ends a finished piggyback, heads for the next
// queued destination and drops it, and a Teleporter type re-aims at its
// target. Without a contact it clears the target unless one is in hand,
// and commences (slot 117). Returns the mission's rate in frames plus 0..2.

// IPiggyback's interface ID (a copy at 0x007CFC30).
extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
// The object as a techno, or null (0x0040DC20; hypothesis).
TechnoClass *__fastcall Unknown_0040DC20(AbstractClass *object);

int FootClass::Do_MISSION_ENTER()
{
    TechnoClass *contact = Contact(0);
    if (contact == 0) {
        contact = Unknown_0040DC20(unknown_1b0);
    }
    if (contact != 0) {
        if (unknown_slot150(0xE, contact) != 1 && !unknown_370) {
            unknown_slot149(3);
            unknown_slot261(0, true);
        } else if (unknown_494 == 0 && unknown_478.Count() > 0) {
            UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggyback(Locomotor);
            if (piggyback != 0 && piggyback->Is_Ok_To_End()) {
                Locomotor.Attach(0);
                piggyback.Checked()->End_Piggyback(&Locomotor.Pointer);
            }
            if (unknown_478.Count() > 0) {
                Assign_Destination(unknown_478[0], false);
                unknown_478.Delete(0);
            }
        } else if (Techno_Type_Class()->Teleporter) {
            AbstractClass *target = unknown_494;
            unknown_490 = 0;
            unknown_494 = 0;
            Assign_Destination(target, true);
        }
    } else {
        if (!unknown_6d8080() && (unknown_494 == 0 || (unknown_494->Kind_Of() != 1 && unknown_494->Kind_Of() != 2))) {
            unknown_slot261(0, true);
        }
        Commence();
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// void FootClass::unknown_slot259(int arg1) (0x004C8020)

void FootClass::unknown_slot259(int arg1)
{
    unknown_490 = (AbstractClass *)arg1;   // the base signature (int) is a placeholder
}


// void FootClass::Assign_Destination(AbstractClass *target, bool immediate) (0x004C8030)
// Sets the navigation target (+0x494; hypothesis: Assign_Destination):
// with a target, the old waypoint marker (+0x25C) goes, a hover locomotor
// restarts its timer +0x530, and unless +0x598 asks to skip once the
// locomotor moves to the target's slot 19 coordinate; without one, the
// locomotor stops (except an aircraft attacking in mission 1). Then the
// movement timers +0x558 (rules +0x13D0) and +0x530 restart.

extern "C" GUID const CLSID_HoverLocomotionClass;   // 0x007A1F80

void FootClass::Assign_Destination(AbstractClass *target, bool immediate)
{
    unknown_490 = 0;
    unknown_494 = target;
    if (target != 0) {
        if (unknown_25c[0] != 0) {
            unknown_25c[0]->Delete_Me();
            unknown_25c[0] = 0;
        }
        UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Locomotor);
        GUID clsid;
        persist.Checked()->GetClassID(&clsid);
        if (memcmp(&clsid, &CLSID_HoverLocomotionClass, sizeof(GUID)) == 0 && unknown_530.Value() == 0) {
            unknown_530 = CDTimerClass<FrameTimerClass>(1);
        }
        if (unknown_598) {
            unknown_598 = false;
        } else {
            CoordStruct coord = unknown_494->unknown_slot19(this);
            Locomotor.Checked()->Move_To(coord);
        }
    } else if (Kind_Of() != 2 || (Mission != 1 && MissionQueue != 1) || unknown_228 == 0) {
        Locomotor.Checked()->Stop_Moving();
        unknown_494 = 0;
    }
    unknown_5a1 = false;
    unknown_558 = CDTimerClass<FrameTimerClass>(Rules->unknown_13d0);
    unknown_530 = CDTimerClass<FrameTimerClass>(0);
}


// void FootClass::unknown_slot54(bool arg1) (0x004C8220, slot 54)
// Detach_All (TibSun shape; hypothesis): a removal from the team, then the
// radio over-and-out (slot 149, message 3) when releasing or when the
// contact is not an ally.

void FootClass::unknown_slot54(bool arg1)
{
    if (arg1) {
        if (Team != 0 && unknown_a40774 == 0) {
            Team->Unknown_006B8FB0(this, -1, 0);
        }
    }
    if (!arg1) {
        TechnoClass *contact = Contact(0);
        if (contact != 0 && !House->Is_Ally(contact)) {
            unknown_slot149(3);
        }
    } else {
        unknown_slot149(3);
    }
    TechnoClass::unknown_slot54(arg1);
}


// int FootClass::unknown_4c82a0(AbstractClass *target) (0x004C82A0)
// How eagerly to attack a target: the negated rate (slot 168) for the
// current target; nothing for a cloaked current techno target, for a team
// that is not a base defense, in mission 10 or without a rate; otherwise
// the rate times 1024 divided by the frames needed to close the distance.

template <class T>
inline T Max_Of(T a, T b)
{
    return (a > b) ? a : b;
}

int FootClass::unknown_4c82a0(AbstractClass *target)
{
    if (target == 0) {
        return 0;
    }
    if (unknown_228 == target) {
        return -unknown_slot168();
    }
    if (unknown_228 != 0 && unknown_228->AbstractFlags.IsTechno && ((TechnoClass *)unknown_228)->unknown_slot163()) {
        return 0;
    }
    bool defense = (Team != 0 && Team->Class->IsBaseDefense) ? true : false;
    if (Team != 0 && !defense) {
        return 0;
    }
    if (Get_Mission() == 10 || unknown_slot168() == 0) {
        return 0;
    }
    int range = unknown_5d4c00(target);
    int distance = range - Weapon_Range(0);
    int rate = unknown_slot168() << 10;
    if (distance > 0) {
        int speed = (unsigned)unknown_slot206() > 1 ? unknown_slot206() : 1;
        int steps = speed > 0 ? Max_Of(distance / speed, 1) : 1;
        rate = Max_Of(rate / steps, 1);
    }
    return rate;
}


// void FootClass::unknown_slot216(int arg1) (0x004C83C0)
// For a player-controlled house and a type that is not Spawned: raises
// event 7 at the cell of slot 104 and, when that succeeds, announces
// EVA_UnitLost (hypothesis: the unit was lost).

void FootClass::unknown_slot216(int arg1)
{
    if (House->Is_Player_Control() && !Techno_Type_Class()->Spawned) {
        if (Unknown_0063B5B0(7, unknown_slot104())) {
            Speech_Play_Name("EVA_UnitLost", -1, -1);
        }
    }
}


// AbstractClass *FootClass::unknown_slot219(int arg1, CoordStruct const *coord, bool arg3) (0x004C8420)
// TechnoClass's, with flag bit 0 forced and bit 1 cleared while
// unknown_578 is set; a zero result clears unknown_578.

AbstractClass *FootClass::unknown_slot219(int arg1, CoordStruct const *coord, bool arg3)
{
    if (unknown_578) {
        arg1 = (arg1 & ~2) | 1;
    }
    AbstractClass *result = TechnoClass::unknown_slot219(arg1, coord, arg3);
    if (unknown_578 && result == 0) {
        unknown_578 = false;
    }
    return result;
}


// void FootClass::Detach(AbstractClass *target, bool all) (0x004C8460)
// Forgets an object that is going away: the team, the parasite riding this
// unit (it is told too), the next team member, the TechnoClass target at
// +0x1B0, the navigation target (kept while an Occupier still holds a live
// building it is occupying, or while the target stays visible to the
// owner), the pending order's object and every queued destination.

void FootClass::Detach(AbstractClass *target, bool all)
{
    TechnoClass::Detach(target, all);
    if (Team == target) {
        Team = 0;
    }
    if (unknown_580 == target && (unknown_580->Strength == 0 || !unknown_flag_a40d04)) {
        unknown_580 = 0;
    }
    if (unknown_580 != 0 && unknown_580->Strength > 0) {
        unknown_580->unknown_588->Detach(target, true);
    }
    if (target == this) {
        unknown_580 = 0;
    }
    if (unknown_4c8 == target && all && target != 0) {
        unknown_4c8 = ((FootClass *)unknown_4c8)->unknown_4c8;
    }
    if (unknown_1b0 == target) {
        unknown_6d6e50(0);
    }
    if (unknown_498 == target) {
        unknown_498 = 0;
    }
    if (unknown_494 == target) {
        bool forget = true;
        bool object;
        if (!all && target != 0) {
            object = target->AbstractFlags.IsTechno;
        }
        if (!all && target != 0 && object) {
            UnknownCellClass *cell = (UnknownCellClass *)&Map[target->Center_Coord()];
            if (cell->Is_Sensed(House->ArrayIndex)) {
                forget = false;
            }
        }
        if (Get_Mission() == 8 && Kind_Of() == 15 && ((InfantryClass *)this)->Type->Occupier && unknown_494 != 0
            && unknown_494->AbstractFlags.IsObject && ((ObjectClass *)unknown_494)->unknown_7d && ((ObjectClass *)unknown_494)->Strength > 0) {
        } else if (forget) {
            unknown_490 = 0;
            unknown_494 = 0;
        }
    }
    if (unknown_4bc == target) {
        unknown_4cdb60();
    }
    for (int index = 0; index < unknown_49c.Count(); index++) {
        if (unknown_49c[index] == target) {
            unknown_49c.Delete(index);
            index--;
        }
    }
    for (index = 0; index < unknown_478.Count(); index++) {
        if (unknown_478[index] == target) {
            unknown_478.Delete(index);
            index--;
        }
    }
}


// int FootClass::unknown_slot298() (0x004C86D0)

int FootClass::unknown_slot298()
{
    return 0;
}


// int FootClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5) (0x004C86E0)
// Whether the unit may enter cell arg1 (hypothesis: Can_Enter_Cell): with a
// locomotor and arg5 set, the locomotor's answer for the cell's position
// (ILocomotion::Can_Enter_Cell); otherwise 0.

int FootClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5)
{
    if (Locomotor != 0 && arg5) {
        Cell cell = ((UnknownCellClass *)arg1)->unknown_24;
        return Locomotor.Checked()->Can_Enter_Cell(cell);
    }
    return 0;
}


// FootClass Can_Reach004C8730: complete519body+9NOP; existing ABI retained.
#include "shared/adjacent_cell.hpp"
#pragma intrinsic(abs)
int FootClass::unknown_slot102(UnknownCellClass *from,int facing,int *level,bool *ramp,UnknownCellClass *to)
{
    if(to==0) {
        to=(UnknownCellClass *)&Map[Cell(AdjacentCell[(facing-4)&7]+from->unknown_24)];
    }
    if(facing!=-1) {
        if(to==0 || from==0) return 0;
        if(*level==-1 && (to->Flags & 0x100)) {
            *level=to->unknown_11b+4;
            if(!(from->Flags & 0x200)) return 7;
        }
        int current_height;
        int height_difference;
        if(to->Flags & 0x100) {
            current_height=from->unknown_11b;
            height_difference=to->unknown_11b-current_height;
        } else {
            current_height=from->unknown_11b;
            height_difference=*level-current_height;
        }
        switch(abs(height_difference)) {
        case 0:
            if((!(from->Flags & 0x100) || !(from->Flags & 0x200) || !(to->Flags & 0x100)) && *level!=-1 && *level!=current_height) return 7;
            break;
        case 1:
            if(height_difference>0) {
                if(!from->unknown_11c) return 7;
            } else if(!to->unknown_11c) return 7;
            break;
        case 4:
            if(to->unknown_11b==current_height-4) {
                if(*level!=current_height) return 7;
                if(!(to->Flags & 0x100)) return 7;
            }
            if(current_height!=to->unknown_11b-4) return 0;
            if(!(from->Flags & 0x100)) return 7;
            if(!(from->Flags & 0x200)) return 7;
            *ramp=true;
            break;
        default: return 7;
        }
    } else {
        if(*level==-1 && (from->Flags & 0x100)) *level=from->unknown_11b+4;
    }
    return 0;
}


// bool FootClass::Can_Demolish() (0x004C8940)
// True for kinds 1 and 2 in radio contact with a building whose type sets
// +0x1349 when within 128 leptons of it; otherwise ObjectClass's slot 37.

bool FootClass::Can_Demolish()
{
    int kind = Kind_Of();
    if (kind > 0 && kind <= 2 && HasAnyLink()) {
        if (Contact_With_Whom(0)->Kind_Of() == 6 && ((BuildingClass *)Contact_With_Whom(0))->Type->UnitRepair) {
            if (Coord_Length(Center_Coord() - Contact_With_Whom(0)->Center_Coord()) < 0x80) {
                return true;
            }
        }
    }
    return ObjectClass::Can_Demolish();
}


// void FootClass::Sell_Back(int arg1) (0x004C8A40)
// When arg1 is set: announces the sale to a player-controlled house
// (EVA_UnitSold and the rules sound at +0x5C4), refunds slot 167's amount
// to the house, then slots 210, 52 and 60 (hypothesis: selling the unit).

void FootClass::Sell_Back(int arg1)
{
    if (arg1) {
        if (House->Is_Player_Control()) {
            Speech_Play_Name("EVA_UnitSold", -1, -1);
            Voc_Play(Rules->unknown_5c4, 1.0f, 0x2000, 0);
        }
        House->Refund_Money(unknown_slot167());
        unknown_slot210();
        Limbo();
        Delete_Me();
    }
}


// CoordStruct FootClass::unknown_slot287() (0x004C8AC0)
// Slot 40's coordinate.

CoordStruct FootClass::unknown_slot287()
{
    return unknown_slot40();
}


// Cell FootClass::unknown_4c8af0(Cell const &cell) (0x004C8AF0)

Cell FootClass::unknown_4c8af0(Cell const &cell)
{
    return cell;
}


// void FootClass::unknown_4c8b00() (0x004C8B00)
// With nothing at +0x494: takes the first entry of the +0x49C queue as the
// new destination (slot 260), removes it, and re-queues it at the end when
// the queue loops (+0x59B).

void FootClass::unknown_4c8b00()
{
    if (unknown_494 == 0 && unknown_49c.Count() != 0) {
        AbstractClass *next = unknown_49c[0];
        if (next != 0) {
            Assign_Destination(next, true);
            unknown_49c.Delete(0);
            if (unknown_59b) {
                unknown_49c.Add(next);
            }
        }
    }
}


// void FootClass::unknown_4c8bb0(AbstractClass *target) (0x004C8BB0)
// Queues a destination at +0x49C; queueing the unit itself while the queue
// is not empty turns on looping (+0x59B), which an empty queue resets. In
// mission 5 with nothing at +0x494, slot 261 follows unless the radio
// contact is a building whose type sets +0x1357.

void FootClass::unknown_4c8bb0(AbstractClass *target)
{
    if (target != 0) {
        if (target == this && unknown_49c.Count() > 0) {
            unknown_59b = true;
        } else {
            if (unknown_49c.Count() == 0) {
                unknown_59b = false;
            }
            unknown_49c.Add(target);
        }
        if (unknown_494 == 0 && Get_Mission() == 5) {
            TechnoClass *contact = Contact(0);
            if (contact == 0 || contact->Kind_Of() != 6 || !((BuildingClass *)contact)->Type->WeaponsFactory) {
                unknown_slot261(0, true);
            }
        }
    }
}


// void FootClass::unknown_4c8c90() (0x004C8C90)

void FootClass::unknown_4c8c90()
{
    unknown_49c.Clear();
}


// bool FootClass::unknown_slot189() (0x004C8CA0)

bool FootClass::unknown_slot189()
{
    if (!unknown_32d) {
        return false;
    }
    if (Techno_Type_Class()->IsTrain || unknown_32c || Get_Mission() == 4 || (Team != 0 && Team->Unknown_006BA990())) {
        return true;
    }
    return false;
}


// bool FootClass::unknown_4c8d00(HouseClass *house) (0x004C8D00)
// Whether the unit is free to be recruited (by house, when given): owned
// by that house, +0x76 clear, in no team, in an idle mission (0x00594DB0)
// and with +0x379 set.

bool FootClass::unknown_4c8d00(HouseClass *house)
{
    if (house != 0 && house != House) {
        return false;
    }
    if (unknown_76) {
        return false;
    }
    if (Team != 0) {
        return false;
    }
    if (!MissionClass::Is_Recruitable_Mission(Get_Mission())) {
        return false;
    }
    return unknown_379 ? true : false;
}


// bool FootClass::unknown_4c8d60() (0x004C8D60)
// Whether the mission (slot 91) is 7.

bool FootClass::unknown_4c8d60()
{
    return Get_Mission() == 7 ? true : false;
}


// int FootClass::Do_MISSION_UNLOAD() (0x004C8D70)
// MissionClass's (compiled as a jump).

int FootClass::Do_MISSION_UNLOAD()
{
    return MissionClass::Do_MISSION_UNLOAD();
}


// int FootClass::Do_MISSION_RETREAT() (0x004C8D80)
// A two-state retreat: with no destination the unit heads for a cell on
// its house's map edge (0x0049A700), near the team's waypoint when it has
// one, and switches to state 1; state 1 falls back to 0 once it arrives.
// Returns the mission's rate in frames plus 0..2.

int FootClass::Do_MISSION_RETREAT()
{
    switch (unknown_a8) {
    case 0:
        if (unknown_494 != 0) {
            unknown_a8 = 1;
        } else {
            Cell cell(0, 0);
            if (Team != 0 && Team->Class->Unknown_006BFA90() != unknown_geometry_k) {
                cell = Map.Calculated_Cell(House->unknown_128, Team->Class->Unknown_006BFA90(),
                    Coord_Cell(Center_Coord()), Techno_Type_Class()->SpeedType, 1, 0);
            }
            if (cell == unknown_geometry_k) {
                cell = Map.Calculated_Cell(House->unknown_128, unknown_geometry_k,
                    Coord_Cell(Center_Coord()), Techno_Type_Class()->SpeedType, 1, 0);
            }
            Assign_Destination((AbstractClass *)&Map[cell], true);
            unknown_a8 = 1;
        }
        break;
    case 1:
        if (unknown_494 == 0) {
            unknown_a8 = 0;
        }
        break;
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// int FootClass::unknown_slot25(bool arg1, int arg2) (0x004C8FA0)
// The locomotor's visual character (ILocomotion::Visual_Character) when it
// has one, else TechnoClass's.

int FootClass::unknown_slot25(bool arg1, int arg2)
{
    if (Locomotor) {
        int result = Locomotor.Checked()->Visual_Character(arg1);
        if (result != 0) {
            return result;
        }
    }
    return TechnoClass::unknown_slot25(arg1, arg2);
}


// void FootClass::AI() (0x004C8FF0, slot 23)
// TechnoClass's AI, then the unit's own: tiberium damage every rules
// +0x144C frames, a balloon unit settles, a moving unit on the map refreshes
// its sight every 15 frames, the locomotor runs (Process) and the idle and
// walk frame counter +0x430 advances, the move sound (+0x434 playing, +0x438
// frames of grace) and the falling, sinking and crashing sounds start and
// stop, the audio controller follows the unit, an idle unit in an
// impassable spot is moved off it, a finished piggyback hands the
// locomotor back, and the parasite riding the unit runs.

// IPiggyback's interface ID (a copy at 0x007CFC30; IID_IPiggyback is 0x007A2050).
extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;

// An IPiggyback pointer queried from the locomotor (the compiler's
// _com_ptr_t<IPiggyback> as comip.h expands it: Attach(SUCCEEDED(hr) ?
// pointer : NULL) on a fresh pointer).
class UnknownPiggybackPtr {
public:
    UnknownPiggybackPtr(ILocomotion *other) : Pointer(0)
    {
        HRESULT result = Query(other);
        if (FAILED(result) && result != E_NOINTERFACE) {
            _com_issue_error(result);
        }
    }
    ~UnknownPiggybackPtr()
    {
        if (Pointer != 0) {
            Pointer->Release();
        }
    }
    HRESULT Query(ILocomotion *other)
    {
        HRESULT result;
        if (other != 0) {
            IPiggyback *pointer;
            result = other->QueryInterface(unknown_uuid_IPiggyback_7cfc30, (void **)&pointer);
            Attach(SUCCEEDED(result) ? pointer : 0);
        } else {
            result = E_NOINTERFACE;
        }
        return result;
    }
    void Attach(IPiggyback *pointer)
    {
        if (Pointer != 0) {
            Pointer->Release();
        }
        Pointer = pointer;
    }
    operator IPiggyback *() const { return Pointer; }
    IPiggyback *operator->() const { return Pointer; }
    IPiggyback *Pointer;
};

// A random entry of a sound list.
inline int Random_Sound(TypeList<int> &list)
{
    unsigned index = unknown_random_839218();
    return list[index % list.Count()];
}

void FootClass::AI()
{
    TechnoClass::AI();
    if (!unknown_7d) {
        return;
    }
    unknown_59d = false;
    if (Frame % Rules->unknown_144c == 0 && !Techno_Type_Class()->ImmuneToRadiation && !In_Air()) {
        Cell cell = Coord_Cell(Center_Coord());
        int amount = Map_Cell_At(cell)->Unknown_0047CE70();
        if (amount > 0) {
            int damage = (int)(amount * Rules->unknown_1460);
            Take_Damage(&damage, 0, Rules->unknown_147c, 0, false, 1);
            if (!unknown_7d) {
                return;
            }
        }
    }
    if (!unknown_32d && Techno_Type_Class()->BalloonHover
        && Map.Unknown_0055A740((CellClass *)Get_Cell_Ptr(), 1)) {
        unknown_32d = true;
    }
    if (Locomotor.Checked()->Is_Moving_Now() && In_Air() && House->Is_Ally(unknown_player_a35db4)
        && unknown_54c.Value() == 0) {
        unknown_slot263(0, 0, 0, 0);
        unknown_slot262(0, 0, 0, 0);
        if (unknown_1f8 != Get_Height_AGL()) {
            Map.Unknown_0054A550(CoordStruct(Coord), 0, unknown_1f4 + 3, 0);
        } else {
            Map.Unknown_0054A550(CoordStruct(Coord), unknown_1f4 - 3, unknown_1f4 + 3, 0);
        }
        unknown_1f8 = Get_Height_AGL();
        unknown_54c = CDTimerClass<FrameTimerClass>(15);
    }

    int frame = unknown_430;
    if (Locomotor != 0 && !unknown_325 && !unknown_7a) {
        Locomotor.Checked()->Process();
        if (!unknown_7d) {
            return;
        }
        if (((Locomotor.Checked()->Is_Moving_Now() && !unknown_slot112())
                || (unknown_228 != 0 && Techno_Type_Class()->HoverAttack))
            && Frame % Techno_Type_Class()->WalkRate == 0 && !unknown_slot111() && !unknown_slot112()
            || Techno_Type_Class()->IdleRate != 0 && !Locomotor.Checked()->Is_Moving_Now()
            && Frame % Techno_Type_Class()->IdleRate == 0 && !unknown_slot111() && !unknown_slot112()) {
            unknown_430++;
        }
    }
    if ((frame != unknown_430 || Locomotor.Checked()->Is_Moving_Now()) && !unknown_7a && !unknown_37d) {
        if (!unknown_434 && Techno_Type_Class()->MoveSound.Count() > 0) {
            unknown_43c.Unknown_00405D50();
            Voc_Play_At(Random_Sound(Techno_Type_Class()->MoveSound), CoordStruct(Coord), &unknown_43c);
            unknown_434 = true;
        }
        unknown_438 = 3;
    } else if (unknown_434) {
        if (unknown_438 != 0 && !unknown_7a && !unknown_37d) {
            unknown_438--;
        } else {
            unknown_43c.Unknown_00406070();
            unknown_434 = false;
            unknown_438 = 0;
        }
    }

    if (unknown_7a != unknown_7b) {
        if (unknown_7a && Techno_Type_Class()->VoiceFalling != -1 && unknown_7c) {
            Voc_Play_At(Techno_Type_Class()->VoiceFalling, CoordStruct(Coord), 0);
        }
        unknown_7b = unknown_7a;
    }
    if (unknown_325 != unknown_326) {
        if (unknown_325) {
            if (Techno_Type_Class()->VoiceSinking != -1) {
                Voc_Play_At(Techno_Type_Class()->VoiceSinking, CoordStruct(Coord), 0);
            }
            if (Techno_Type_Class()->SinkingSound != -1) {
                Voc_Play_At(Techno_Type_Class()->SinkingSound, CoordStruct(Coord), &unknown_43c);
            } else if (Rules->unknown_200 != -1) {
                Voc_Play_At(Rules->unknown_200, CoordStruct(Coord), &unknown_43c);
            }
        } else if (!unknown_434) {
            unknown_43c.Unknown_00406070();
        }
        unknown_326 = unknown_325;
    }
    if (unknown_37d != unknown_37e) {
        if (unknown_37d) {
            unknown_43c.Unknown_00406070();
            if (Techno_Type_Class()->VoiceCrashing != -1 && House->Is_Player_Control()) {
                Voc_Play_At(Techno_Type_Class()->VoiceCrashing, CoordStruct(Coord), 0);
            }
            if (Techno_Type_Class()->CrashingSound != -1) {
                Voc_Play_At(Techno_Type_Class()->CrashingSound, CoordStruct(Coord), &unknown_43c);
            }
        } else if (!unknown_434) {
            unknown_43c.Unknown_00406070();
        }
        unknown_37e = unknown_37d;
    }
    Voc_Update_At(CoordStruct(Coord), &unknown_43c);

    if ((Frame & 0x3F) == 0x3F && unknown_494 == 0 && !unknown_79
        && !((UnknownCellClass *)Get_Cell_Ptr())->unknown_11c && (unsigned char)unknown_slot164() && Get_Height_AGL() == 0) {
        Scatter(CoordStruct(0, 0, 0), 1 != 0, 0 != 0);
    }

    UnknownPiggybackPtr piggyback(Locomotor);
    if (piggyback != 0 && piggyback->Is_Ok_To_End()) {
        Locomotor.Attach(0);
        piggyback->End_Piggyback(&Locomotor.Pointer);
    }
    unknown_59e = false;
    unknown_6d7f70();
    if (unknown_580 != 0) {
        unknown_580->unknown_588->AI();
    }
}


// Typed voxel-draw ABI; original nine stack arguments and RET36. (0x004C9880)
// Draws the unit (hypothesis: the voxel draw, TechnoClass 0x006D1D00): the
// draw point arg6 (a Point2D) is moved by the locomotor's Draw_Point and
// the transform arg7 is passed as a copy.


// The 0x30-byte transform arg7 points at, copied member-wise (12 floats;
// hypothesis: a Matrix3D without the out-of-line copy constructor 0x0058FE10).
struct UnknownDrawMatrix {
    float Values[12];
};

void FootClass::unknown_slot246(UnknownObjectPair const &voxel, int frame, int key, UnknownBuffer *cache, Rect const &cliprect, Point2D const &draw_point, Matrix3D const &transform, int brightness, unsigned flags)
{
    UnknownDrawMatrix matrix = *(UnknownDrawMatrix const *)&transform;
    Point2D point = draw_point;
    if (Locomotor != 0) {
        Point2D offset = Locomotor.Checked()->Draw_Point();
        point = Point2D_Sum(draw_point, offset);
    }
    TechnoClass::unknown_slot246(voxel, frame, key, cache, cliprect, point, *(Matrix3D const *)&matrix, brightness, flags);
}


// int FootClass::unknown_slot176() (0x004C9930)
// TechnoClass's slot 176 plus the largest of the type's Z fudges that
// apply (column, tunnel, cliff, bridge) plus the locomotor's Z adjustment.

int FootClass::unknown_slot176()
{
    int adjust = 0;
    if (Locomotor) {
        adjust = Locomotor.Checked()->Z_Adjust();
    }
    int fudge = Techno_Type_Class()->ZFudgeColumn * unknown_6cf9d0();
    int tunnel = Techno_Type_Class()->ZFudgeTunnel * unknown_6cfb60();
    int cliff = Techno_Type_Class()->ZFudgeCliff * unknown_6cfda0();
    int bridge = 0;
    if (unknown_6cf670()) {
        bridge = Techno_Type_Class()->ZFudgeBridge;
    }
    if (fudge <= tunnel) {
        fudge = tunnel;
    }
    if (fudge <= cliff) {
        fudge = cliff;
    }
    if (fudge <= bridge) {
        fudge = bridge;
    }
    return TechnoClass::unknown_slot176() + fudge + adjust;
}


// int FootClass::unknown_slot177() (0x004C9A10)
// The locomotor's Z gradient (ILocomotion slot 15), 2 without a locomotor.

int FootClass::unknown_slot177()
{
    if (Locomotor) {
        return Locomotor.Checked()->Z_Gradient();
    }
    return 2;
}


// void FootClass::unknown_4c9a40(int arg1 .. int arg11) (0x004C9A40)
// Draws the unit's shadow (hypothesis): when the locomotor casts one
// (ILocomotion::Is_To_Have_Shadow), the draw point arg6 (a Point2D) moves by
// the locomotor's Shadow_Point and TechnoClass 0x006D21E0 draws.


void FootClass::unknown_4c9a40(UnknownObjectPair const &voxel, int layer, int key, UnknownBuffer *cache, Rect const &cliprect, Point2D const &draw_point, Matrix3D const &transform, bool force_cache, Surface *mask, unsigned unknown_2, unsigned unknown_3)
{
    if (Locomotor != 0 && Locomotor.Checked()->Is_To_Have_Shadow() == true) {
        Point2D point = draw_point;
        if (Locomotor != 0) {
            Point2D offset = Locomotor.Checked()->Shadow_Point();
            point = Point2D_Sum(draw_point, offset);
        }
        Unknown_006D21E0(voxel, layer, key, cache, cliprect, point, transform, force_cache, mask, unknown_2, unknown_3);
    }
}


// int FootClass::unknown_slot303() (0x004C9B10)
// The unit's current speed (hypothesis: Westwood's Apparent_Speed): the
// type's speed (slot 206) times the house's factor for the type
// (0x004F5A20) and +0x470, times the rules' +0x598 with ability 0, times
// +0x468; a unit (kind 1) with +0x5B4 set moves at half that.

int FootClass::unknown_slot303()
{
    int speed = (int)(unknown_slot206() * House->Unknown_004F5A20(Techno_Type_Class()) * unknown_470);
    if (unknown_6d7910(0)) {
        speed = (int)(speed * Rules->unknown_598);
    }
    int result = (int)(speed * unknown_468);
    if (Kind_Of() == 1 && ((UnitClass *)this)->unknown_5b4 != -1) {
        result /= 2;
    }
    return result;
}


class Leaf_4c9bc0 { public: void get(unsigned int unused_argument_1, unsigned int unused_argument_2); };
void Leaf_4c9bc0::get(unsigned int unused_argument_1, unsigned int unused_argument_2) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


// bool FootClass::Limbo() (0x004C9BD0)
// Taking the unit off the map: the eight cells around its cell lose one
// from +0x122, the unit stops (slot 291) unless 0x00A40774 is set, the
// locomotor clears its occupation bits, the sound stops, a sensor type
// clears its sensed cell (slot 286), and the tracker at 0x00839EA0 hears of
// it when slot 178 gives a cell; then TechnoClass's slot 52.

bool FootClass::Limbo()
{
    if (!unknown_76 && Techno_Type_Class() != 0) {
        Cell cell = unknown_454;
        for (int facing = 0; facing < 8; facing++) {
            UnknownCellClass *next = (UnknownCellClass *)&Map[Adjacent_Cell(cell, facing)];
            next->unknown_122--;
        }
        if (unknown_a40774 == 0) {
            unknown_slot291();
        }
        if (Locomotor) {
            Locomotor.Checked()->Mark_All_Occupation_Bits(0);
        }
        unknown_43c.Unknown_00405FE0();
        if (Techno_Type_Class()->SensorsSight != 0) {
            unknown_slot286(unknown_geometry_k);
        }
        if (unknown_slot178() != unknown_geometry_k) {
            unknown_tracker_839ea0.Unknown_00413310(this);
        }
    }
    return TechnoClass::Limbo();
}


// HRESULT __stdcall FootClass::Load(IStream *stream) (0x004C9D10)
// Releases the locomotor, loads the techno, rebuilds and reloads the two
// AbstractClass vectors (count, then each pointer), loads the locomotor
// (OleLoadFromStream) and requests the pointer fix-ups.
#include "shared/win32_api.hpp"

WIN32_IMPORT HRESULT __stdcall OleLoadFromStream(IStream *stream, GUID const &iid, void **object);

HRESULT __stdcall FootClass::Load(IStream *stream)
{
    Locomotor.Attach(0);
    HRESULT result = TechnoClass::Load(stream);
    if (SUCCEEDED(result)) {
        int count;
        result = stream->Read(&count, sizeof(count), 0);
        if (FAILED(result)) {
            return result;
        }
        new (&unknown_478) DynamicVectorClass<AbstractClass *>;
        for (int index = 0; index < count; index++) {
            AbstractClass *pointer = 0;
            result = stream->Read(&pointer, sizeof(pointer), 0);
            if (FAILED(result)) {
                return result;
            }
            unknown_478.Add(pointer);
        }

        result = stream->Read(&count, sizeof(count), 0);
        if (FAILED(result)) {
            return result;
        }
        new (&unknown_49c) DynamicVectorClass<AbstractClass *>;
        for (index = 0; index < count; index++) {
            AbstractClass *pointer = 0;
            result = stream->Read(&pointer, sizeof(pointer), 0);
            if (FAILED(result)) {
                return result;
            }
            unknown_49c.Add(pointer);
        }

        Locomotor.Detach();
        OleLoadFromStream(stream, IID_ILocomotion, (void **)&Locomotor);
        unknown_490 = 0;
        Swizzle_Pointer(Team);
        Swizzle_Pointer(unknown_4c8);
        Swizzle_Pointer(unknown_494);
        Swizzle_Pointer(unknown_498);
        Swizzle_Pointer(unknown_4b8);
        Swizzle_Pointer(unknown_4bc);
        Swizzle_Pointer(unknown_4cc);
        Swizzle_Pointer(unknown_580);
        Swizzle_Pointer(unknown_588);
        unknown_43c.Init();
        unknown_434 = false;
        unknown_438 = 0;
        for (index = 0; index < unknown_49c.Count(); index++) {
            Swizzle_Pointer(unknown_49c[index]);
        }
        for (index = 0; index < unknown_478.Count(); index++) {
            Swizzle_Pointer(unknown_478[index]);
        }
        result = S_OK;
    }
    return result;
}


// Foot.CPP: persist the route and navigation pointer queues, then the locomotor.
// Persistence query from the held locomotor: use its raw held interface after
// the null guard, then release and attach the queried stream interface inline.
template<>
inline HRESULT UnknownComPtr<IPersistStream,&unknown_uuid_IPersistStream_7d5f38>::Query(UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> const &other)
{
    if (other.Pointer) {
        IPersistStream *pointer;
        HRESULT result=other.Pointer->QueryInterface(unknown_uuid_IPersistStream_7d5f38,(void **)&pointer);
        if (FAILED(result)) Attach(0);
        else Attach(pointer);
        return result;
    }
    operator=((IPersistStream *)0);
    return E_NOINTERFACE;
}

HRESULT __stdcall FootClass::Save(IStream *stream,BOOL clear_dirty)
{
    HRESULT result=TechnoClass::Save(stream,clear_dirty);
    if (SUCCEEDED(result)) {
        int index;
        int count=unknown_478.Count();
        result=stream->Write(&count,sizeof(count),0);
        if (FAILED(result)) return result;
        for (index=0;index<count;++index) {
            result=stream->Write(&unknown_478[index],sizeof(unknown_478[index]),0);
            if (FAILED(result)) return result;
        }
        count=unknown_49c.Count();
        result=stream->Write(&count,sizeof(count),0);
        if (FAILED(result)) return result;
        for (index=0;index<count;++index) {
            result=stream->Write(&unknown_49c[index],sizeof(unknown_49c[index]),0);
            if (FAILED(result)) return result;
        }
        UnknownComPtr<IPersistStream,&unknown_uuid_IPersistStream_7d5f38> persist(Locomotor);
        OleSaveToStream(persist,stream);
        result=S_OK;
    }
    return result;
}


// int FootClass::In_Which_Layer() const (0x004CA130)
// The layer: the locomotor decides (ILocomotion::Layer).

int FootClass::In_Which_Layer() const
{
    return Locomotor.Checked()->Layer();
}


// void FootClass::Set_Coord(CoordStruct const &coord) (0x004CA160)
// ObjectClass's slot 103, bracketed by slot 71 (false, then true) while
// the object is on the map (+0x74).

void FootClass::Set_Coord(CoordStruct const &coord)
{
    if (unknown_74) {
        Mark(0);
        ObjectClass::Set_Coord(coord);
        Mark(1);
    } else {
        ObjectClass::Set_Coord(coord);
    }
}


// void FootClass::unknown_4ca1a0() (0x004CA1A0; never called)
// Replaces the locomotor with a new drop pod locomotor that carries the old
// one (the new one is linked to this unit and begins piggybacking it).
#include "shared/droppod_locomotion.hpp"

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;

void FootClass::unknown_4ca1a0()
{
    UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> old(Locomotor);
    UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> pod(CLSID_DropPodLocomotionClass);
    pod.Checked()->Link_To_Object(this);
    UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggyback(pod);
    piggyback.Checked()->Begin_Piggyback(old);
    Locomotor = pod;
}


// bool FootClass::unknown_slot292() (0x004CA2B0)
// With no target (+0x494) and no TechnoClass target (+0x228): slot 261 with
// no target, passing whether the unit may leave — unless it is in mission
// 7 next to a building contact whose type sets +0x1349.

bool FootClass::unknown_slot292()
{
    if (unknown_494 == 0 && unknown_228 == 0) {
        TechnoClass *contact = Contact(0);
        BuildingClass *building = 0;
        if (contact != 0 && contact->Kind_Of() == 6) {
            building = (BuildingClass *)contact;
        }
        bool stay = (Mission == 7 && building != 0 && building->Type->UnitRepair) ? true : false;
        return unknown_slot261(0, !stay);
    }
    return false;
}


// void FootClass::unknown_slot307() (0x004CA330)

void FootClass::unknown_slot307()
{
}


// void FootClass::unknown_slot308() (0x004CA340)

void FootClass::unknown_slot308()
{
}


// bool FootClass::unknown_slot169(AbstractClass *target) (0x004CA350)
// Slot 171 for the centre of the cell holding the target's coordinate
// (its slot 19 as seen from this object).

bool FootClass::unknown_slot169(AbstractClass *target)
{
    Cell cell = Coord_Cell(target->unknown_slot19(this));
    CoordStruct center(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
    return unknown_slot171(center);
}


// void FootClass::Compute_CRC(WWCRCEngine &crc) const (0x004CA3D0)
// Hashes the movement state: distances as lengths, timers by their
// remaining time, the referenced objects by ID.

void FootClass::Compute_CRC(WWCRCEngine &crc) const
{
    TechnoClass::Compute_CRC(crc);
    crc(unknown_428);
    crc((short)Short_Length(unknown_450.X, unknown_450.Y));
    crc((long)Coord_Length(unknown_45c));
    crc(unknown_468);
    crc(unknown_470);
    if (unknown_494 != 0) {
        crc(unknown_494->Fetch_ID());
    }
    if (unknown_498 != 0) {
        crc(unknown_498->Fetch_ID());
    }
    if (Team != 0) {
        crc(Team->Fetch_ID());
    }
    if (unknown_4c8 != 0) {
        crc(unknown_4c8->Fetch_ID());
    }
    crc(unknown_530.Value());
    crc((long)unknown_53c);
    crc(unknown_540.Value());
    crc((long)Coord_Length(unknown_568));
    crc((char)unknown_574);
    crc((char)unknown_575);
    crc(unknown_578);
    crc(unknown_579);
    crc(unknown_57a);
    crc(unknown_57b);
    crc(unknown_57c);
    crc(unknown_57d);
    crc(unknown_599);
    crc(unknown_59a);
    crc(unknown_59b);
    crc(unknown_59c);
    crc(unknown_59d);
    crc((char)unknown_59e);
}


// bool FootClass::unknown_slot154() (0x004CA6A0)
// TechnoClass's test, refused for a CloakStop type while the locomotor is
// moving (ILocomotion::Is_Moving_Now).

bool FootClass::unknown_slot154()
{
    if (TechnoClass::unknown_slot154()) {
        if (!Techno_Type_Class()->CloakStop || !Locomotor.Checked()->Is_Moving_Now()) {
            return true;
        }
    }
    return false;
}


// CoordStruct FootClass::unknown_slot19(TechnoClass *techno) const (0x004CA6F0)
// Inside a tube (+0x574 set): the centre of its cell; otherwise where the
// locomotor is heading (ILocomotion::Head_To_Coord), or the object's own
// coordinate when that is the zero coordinate.

CoordStruct FootClass::unknown_slot19(TechnoClass *techno) const
{
    if (unknown_574 >= 0) {
        Cell cell = unknown_TubeClass_list_008661c8[unknown_574]->ExitCell;
        return CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
    }
    CoordStruct coord = Locomotor.Checked()->Head_To_Coord();
    if (coord != unknown_geometry_l) {
        return coord;
    }
    return Center_Coord();
}


// bool FootClass::unknown_slot222(HouseClass *house, int arg2) (0x004CA7D0)
// TechnoClass's slot 222 (always with true); on success, unless slot 21
// holds, records the cell of slot 104 at +0x454.

bool FootClass::unknown_slot222(HouseClass *house, int arg2)
{
    if (TechnoClass::unknown_slot222(house, true)) {
        if (!In_Air()) {
            unknown_454 = unknown_slot104();
        }
        return true;
    }
    return false;
}


// bool FootClass::unknown_slot283() (0x004CA820)
// True without a primary weapon, for tech level -1, for types that deploy
// into a building, or for speeds up to 13.

bool FootClass::unknown_slot283()
{
    if (unknown_slot231(0) == 0) {
        return true;
    }
    if (Techno_Type_Class()->TechLevel == -1) {
        return true;
    }
    if (Techno_Type_Class()->DeploysInto != 0) {
        return true;
    }
    return Techno_Type_Class()->Speed <= 13 ? true : false;
}


// void FootClass::unknown_slot288() (0x004CA880)
// In mission 15 (hypothesis: unloading) clears slot 220 and calls slot 260.

void FootClass::unknown_slot288()
{
    if (Get_Mission() == 15) {
        unknown_slot220(0);
        Assign_Destination(0, true);
    }
}


// void FootClass::unknown_slot243(bool force, int arg2) (0x004CA8B0)
// Draws the line from the unit to its target (or to the last queued
// destination, lifted onto a bridge's height) in colours taken from the
// drawer's table: skipped unless forced while the timer at 0x00AC0178 runs.
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/gadget.hpp"

void FootClass::unknown_slot243(bool force, int arg2)
{
    CoordStruct from;
    CoordStruct to;
    if (unknown_228 != 0 || unknown_494 != 0) {
        if (!force && unknown_timer_ac0178.Value() <= 0) {
            return;
        }
        if (unknown_228 != 0) {
            from = unknown_slot181(0);
            to = unknown_6d65b0();
            unknown_6d0510(from, to, Unpack_Hicolor_Pixel(unknown_drawer_8323c4->Convert(8)), 0, 0);
            return;
        }
        from = Coord;
        if (unknown_478.Count() == 0) {
            to = unknown_494->Center_Coord();
        } else {
            to = unknown_478[unknown_478.Count() - 1]->Center_Coord();
        }
        if (Map.In_Radar(Coord_Cell(to)) && (((UnknownCellClass *)&Map[to])->Flags & 0x100)) {
            to.Z = Map.Get_Height_GL(to) + unknown_geometry_j;
        }
        unknown_6d0510(from, to, Unpack_Hicolor_Pixel(unknown_drawer_8323c4->Convert(3)), arg2, 0);
    }
}


// Draw the current firing/destination action line (004CAB90).
#include "shared/map_cell_view.hpp"
#include "shared/tactical.hpp"
#include "shared/surface.hpp"
extern Rect unknown_rect_839630;
extern bool const unknown_action_line_pattern_7d81dc[];
extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime();

// DSurface's RGB overload (TibSun dsurface.h), expressed beside the existing
// free three-channel overload. It performs the real channel conversion.
inline int Build_Hicolor_Pixel(RGBClass const &rgb)
{
    return ((rgb.Get_Red() >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((rgb.Get_Green() >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((rgb.Get_Blue() >> unknown_shift_852fc4) << unknown_shift_852fc0);
}

// By-value area explains the native rectangle copies around the intersection.
// The inline helper's original declaration and name remain hypotheses.
static inline void Clipped_Fill(Rect area, int pixel)
{
    Rect clipped = Intersect(unknown_rect_839630, area);
    unknown_surface_839964->Fill_Rect(clipped, pixel);
}

void FootClass::unknown_4cab90()
{
    RGBClass white(255, 255, 255);
    RGBClass color;
    if (unknown_228 == 0 && unknown_494 == 0) return;
    CoordStruct start_coord;
    CoordStruct end_coord;
    if (unknown_228 != 0) {
        start_coord = unknown_slot181(0);
        end_coord = unknown_6d65b0();
    } else {
        start_coord = Coord;
        if (unknown_478.Count() == 0) {
            end_coord = unknown_494->Center_Coord();
        } else {
            end_coord = unknown_478[unknown_478.Count() - 1]->Center_Coord();
        }
        if (Map.In_Radar(Coord_Cell(end_coord)) && (Map[end_coord].unknown_140 & 0x100)) {
            end_coord.Z = Map.Get_Height_GL(end_coord) + unknown_geometry_j;
        }
    }
    Point2D start;
    Point2D end;
    unknown_tactical_83996c->Coord_To_Pixel(start_coord, start);
    unknown_tactical_83996c->Coord_To_Pixel(end_coord, end);
    start.Y += unknown_rect_839630.Y;
    end.Y += unknown_rect_839630.Y;
    color = House->unknown_55e9;
    int pixel = Build_Hicolor_Pixel(color);
    Clipped_Fill(Rect(RectPoint2D(start.X - 2, start.Y - 2), 3, 3), pixel);
    Clipped_Fill(Rect(RectPoint2D(end.X - 2, end.Y - 2), 3, 3), pixel);
    Rect clip = unknown_rect_839630;
    if (Clip_Line(start, end, clip)) {
        unsigned long tick = timeGetTime();
        unsigned amount = tick & 0x3ff;
        if (!(amount & 0x200)) {
            if (amount & 0x100) amount ^= 0xff;
            color.Adjust(amount & 0xff, white);
        }
        unknown_surface_839964->Draw_Dashed_Line_Depth_Masked(start, end, Build_Hicolor_Pixel(color), unknown_action_line_pattern_7d81dc, ((-(int)tick) >> 5) & 15, false);
    }
}


// double FootClass::unknown_4cafb0() const (0x004CAFB0)

double FootClass::unknown_4cafb0() const
{
    if (Team != 0 && Team->Class->AvoidThreats) {
        return 1.0;
    }
    return unknown_428;
}


// bool FootClass::unknown_slot282() (0x004CAFE0)

bool FootClass::unknown_slot282()
{
    if (!unknown_32d) {
        return false;
    }
    if (Team != 0 && !Team->Unknown_006BA990()) {
        return false;
    }
    if (Techno_Type_Class()->IsTrain && unknown_494 != 0) {
        if (Map.Unknown_0055A7F0(unknown_494->Center_Coord())) {
            return false;
        }
    }
    return true;
}


// void FootClass::Set_Waypoint_Path(int path, char waypoint) (0x004CB060)
// Follows one of the player's waypoint paths (+0x418) from a waypoint
// (+0x576): the next cell is that waypoint's (+0x420); -1 clears it.

void FootClass::Set_Waypoint_Path(int path, char waypoint)
{
    if (path != -1) {
        unknown_418 = path;
        unknown_576 = waypoint;
        unknown_420 = Coord_Cell(unknown_player_a35db4->WaypointPaths[path]->Get_Waypoint(waypoint)->Coord);
    } else {
        unknown_418 = -1;
        unknown_576 = 0;
        unknown_420 = Cell(0, 0);
    }
    unknown_41c = Cell(0, 0);
}


// bool FootClass::unknown_4cb110(WaypointClass *waypoint) (0x004CB110)
// Heads for a waypoint of the current path (TibSun Execute_Waypoint_Path
// shape; slots 93 and 194). The player's house fetches the waypoint's path
// (+0x418) and index (+0x576); +0x388 is set while +0x37F holds. A techno in
// the waypoint's cell is clicked (slots 28, 78) when it is an allied building
// that can repair (the unit below full strength, not infantry) or reload it
// (ammo short of the type's) and the action is not 7 or 8, or an enemy whose
// action is not 8, 7, 1 or 2; otherwise the offset cell (+0x41C) moves to a
// nearby cell (0x005502E0) when one is found. The cell plus its offset is
// clicked (slot 77) when its action, the entry test (slot 101) and the
// movement zones agree and the walk (0x0042AA30) is within the offset's
// larger axis plus 3 (at most 5); otherwise the waypoint's cell is, and the
// offset is cleared. Sound is muted meanwhile. Failure clears the path.
// Returns whether the click succeeded.

bool FootClass::unknown_4cb110(WaypointClass *waypoint)
{
    bool succeeded = false;

    if (unknown_37f) {
        unknown_388 = true;
    }

    if (waypoint != 0) {
        unknown_player_a35db4->Fetch_Waypoint_Data(waypoint, &unknown_418, &unknown_576);

        CoordStruct coord = waypoint->Coord;
        ObjectClass *occupier = ((UnknownCellClass *)&Map[coord])->unknown_e4;
        TechnoClass *techno = occupier->As_Techno();

        unknown_420 = Coord_Cell(coord);

        bool sound = unknown_sound_enabled_7d88d6;
        unknown_sound_enabled_7d88d6 = false;

        Cell cell = Coord_Cell(coord);
        int action;
        bool direct = true;
        bool click = false;

        if (techno != 0) {
            action = unknown_slot28(occupier, true);

            if (unknown_player_a35db4->Is_Ally(techno->House)) {
                if (techno->Kind_Of() == 6) {
                    BuildingClass *building = (BuildingClass *)techno;
                    bool repair = building->Type->UnitRepair && Strength < Techno_Type_Class()->MaxStrength && Kind_Of() != 15;
                    bool reload = building->Type->UnitReload && unknown_254 < Techno_Type_Class()->Ammo && unknown_254 != -1;
                    bool allied = building->House->Is_Ally(unknown_player_a35db4);
                    if ((repair || reload) && allied && action != 7 && action != 8) {
                        click = true;
                    }
                }
            } else if (action != 8 && action != 7 && action != 1 && action != 2) {
                click = true;
            }

            if (!click) {
                Cell nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, Map.Zone(cell, Techno_Type_Class()->MovementZone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1), Techno_Type_Class()->MovementZone, false, Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
                if (nearby != unknown_geometry_k) {
                    unknown_41c = nearby - cell;
                    direct = false;
                }
            }
        }

        if (click) {
            action = unknown_slot28(occupier, true);
            succeeded = unknown_slot78(action, occupier, true);
            unknown_41c = Cell(0, 0);
        } else {
            action = unknown_slot27(&cell, false, true);
            Cell offset = unknown_41c;
            Cell adjusted = cell + offset;
            UnknownCellClass *adjusted_cell = (UnknownCellClass *)&Map[adjusted];
            UnknownCellClass *cell_ptr = (UnknownCellClass *)&Map[cell];
            int adjusted_action = unknown_slot27(&adjusted, false, true);
            int zone = Map.Zone(cell, Techno_Type_Class()->MovementZone, cell_ptr->Flags >> 8 & 1);
            int adjusted_zone = Map.Zone(adjusted, Techno_Type_Class()->MovementZone, adjusted_cell->Flags >> 8 & 1);
            int dest_zone = Map.Zone(Coord_Cell(unknown_slot19(0)), Techno_Type_Class()->MovementZone, Is_Moving_Onto_Bridge(0));
            int entry = Can_Enter_Cell((CellClass *)adjusted_cell, -1, adjusted_cell->unknown_11b + (adjusted_cell->Flags >> 8 & 1) * 4, 0, 1);

            int limit = abs(offset.X) > abs(offset.Y) ? abs(offset.X) : abs(offset.Y);
            limit = limit + 3 > 5 ? 5 : limit + 3;

            if ((action == adjusted_action || !direct) && entry == 0 && zone == adjusted_zone && dest_zone == zone
                && (!direct || Search.Test_Cell_Walk(adjusted, cell, this, adjusted_cell->Flags >> 8 & 1, cell_ptr->Flags >> 8 & 1, -1) <= limit)) {
                Cell none(-1, -1);
                succeeded = unknown_slot77(action, &adjusted, &none, true);
                unknown_420 = adjusted;
            } else {
                Cell none(-1, -1);
                succeeded = unknown_slot77(action, &cell, &none, true);
                unknown_41c = Cell(0, 0);
            }
        }

        if (sound) {
            unknown_sound_enabled_7d88d6 = true;
        }
    }

    if (!succeeded) {
        unknown_418 = -1;
        unknown_576 = 0;
        unknown_41c = Cell(0, 0);
        unknown_420 = Cell(0, 0);
    }

    unknown_388 = false;
    return succeeded;
}


// bool FootClass::unknown_4cb6d0(Cell const &cell) (0x004CB6D0)
// Whether a cell suits the unit (cf. 0x004CC260): passes the map test
// 0x0055A660, is not a raised spot for a +0x372 unit, is reachable in the
// unit's movement zone, has land type 5 and is unoccupied (slot 101).

bool FootClass::unknown_4cb6d0(Cell const &cell)
{
    if (Map.Unknown_0055A660(cell, true)) {
        if (unknown_game_mode_a3d298 == 0 && unknown_372) {
            int height = Map.Get_Height_GL(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
            CoordStruct spot = CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, height);
            if (Map.Is_Shrouded(spot)) {
                return false;
            }
        }
        Cell here = Coord_Cell(unknown_slot19(0));
        if (Map.Unknown_0054F7C0(here, cell, Techno_Type_Class()->MovementZone, Is_Moving_Onto_Bridge(0), 0, 0)) {
            CellClass *map_cell = &Map[cell];
            UnknownCellClass *object = (UnknownCellClass *)map_cell;
            if (object->unknown_ec == 5 && Can_Enter_Cell(map_cell, -1, -1, 0, 1) == 0) {
                return true;
            }
        }
    }
    return false;
}


// bool FootClass::unknown_4cb830(int arg1, bool arg2) (0x004CB830)
// With nothing at +0x494: the cell chosen by 0x004CB8F0; true when the unit
// already stands there, otherwise slot 260 toward that cell's object
// (false). False when no cell is chosen.

bool FootClass::unknown_4cb830(int arg1, bool arg2)
{
    if (unknown_494 == 0) {
        Cell cell = unknown_4cb8f0(arg1, arg2);
        if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
            Cell here = Coord_Cell(Center_Coord());
            if (cell.X == here.X && cell.Y == here.Y) {
                return true;
            }
            Assign_Destination((AbstractClass *)&Map[cell], true);
        }
    }
    return false;
}


// Cell FootClass::unknown_4cb8f0(int radius, bool arg2) (0x004CB8F0)
// A cell to dock at: the unit's own cell when its land type is 5;
// otherwise, ring by ring out to the radius, the best-scoring cell
// (0x0047A230) that passes 0x004CB6D0 on the first ring with one (the zero
// cell when none does).

Cell FootClass::unknown_4cb8f0(int radius, bool arg2)
{
    Cell found = unknown_geometry_k;
    Cell center = Center_Coord();
    if (((UnknownCellClass *)&Map[center])->unknown_ec == 5) {
        return center;
    }
    int best = -1;
    for (int ring = 1; ring < radius; ring++) {
        for (int index = -ring; index <= ring; index++) {
            Cell cell = center + Cell(index, -ring);
            if (unknown_4cb6d0(cell)) {
                int score = ((UnknownCellClass *)&Map[cell])->Unknown_0047A230();
                if (score > best) {
                    best = score;
                    found = cell;
                }
            }
            cell = center + Cell(index, ring);
            if (unknown_4cb6d0(cell)) {
                int score = ((UnknownCellClass *)&Map[cell])->Unknown_0047A230();
                if (score > best) {
                    best = score;
                    found = cell;
                }
            }
            cell = center + Cell(-ring, index);
            if (unknown_4cb6d0(cell)) {
                int score = ((UnknownCellClass *)&Map[cell])->Unknown_0047A230();
                if (score > best) {
                    best = score;
                    found = cell;
                }
            }
            cell = center + Cell(ring, index);
            if (unknown_4cb6d0(cell)) {
                int score = ((UnknownCellClass *)&Map[cell])->Unknown_0047A230();
                if (score > best) {
                    best = score;
                    found = cell;
                }
            }
        }
        if (best != -1) {
            break;
        }
    }
    return found;
}


// Cell FootClass::unknown_4cbb40(int radius) (0x004CBB40)
// TibSun Search_For_Tiberium_Weighted: the cells of each ring that pass
// 0x004CB6D0, weighted by their tiberium value scaled by the ring's share
// per harvester, then one weighted random pick.
// The local distribution's vector instantiation (COMDAT copies of the
// accepted members and vtables at 0x004CDF10-0x004CEB30, 0x007A17D4, 0x007A17FC):
#include "shared/discrete_distribution.hpp"

// The cell's tiberium value (CellClass::Get_Tiberium_Value 0x0047A230) read
// through the shared/techno.hpp cell view; the distribution holds CellClass
// pointers. One inline site per read, as the original's budget shows (the
// vector constructor 0x004CE710 and the early return's Clear 0x004CE660 stay
// out of line only with them).
inline int Tiberium_Value(CellClass *cell)
{
    return ((UnknownCellClass *)cell)->Unknown_0047A230();
}

Cell FootClass::unknown_4cbb40(int radius)
{
    DiscreteDistributionClass<CellClass> celldist;

    Cell center = Coord_Cell(Center_Coord());
    if (((UnknownCellClass *)&Map[center])->unknown_ec == 5) {
        return center;
    }

    int numharv = House->unknown_5470.unknown_func_491d10(House->Unknown_004EF6F0(Rules->unknown_9ec)->ArrayIndex);
    if (numharv < 1) {
        numharv = 1;
    }

    double weight;
    for (int ring = 1; ring < radius; ring++) {
        int ringspan = ring * 3;
        int share = ringspan / numharv;
        bool northrun = false;
        bool southrun = false;
        bool westrun = false;
        bool eastrun = false;
        int divisor = 1;
        if (share >= 1) {
            divisor = share;
        }
        double scale = 1.0 / (double)divisor;
        for (int x = -ring; x <= ring; x++) {
            Cell cell;
            cell = Cell(x, -ring) + center;
            if (unknown_4cb6d0(cell)) {
                if (!northrun) {
                    CellClass *cptr = &Map[cell];
                    if (Tiberium_Value(cptr) * scale < 1.0) {
                        weight = 1.0;
                    } else {
                        weight = Tiberium_Value(cptr) * scale;
                    }
                    int intweight = (int)weight;
                    northrun = true;
                    celldist.Add(cptr, intweight);
                }
            } else {
                northrun = false;
            }

            cell = Cell(x, ring) + center;
            if (unknown_4cb6d0(cell)) {
                if (!southrun) {
                    CellClass *cptr = &Map[cell];
                    if (Tiberium_Value(cptr) * scale < 1.0) {
                        weight = 1.0;
                    } else {
                        weight = Tiberium_Value(cptr) * scale;
                    }
                    int intweight = (int)weight;
                    southrun = true;
                    celldist.Add(cptr, intweight);
                }
            } else {
                southrun = false;
            }

            cell = Cell(-ring, x) + center;
            if (unknown_4cb6d0(cell)) {
                if (!westrun) {
                    CellClass *cptr = &Map[cell];
                    if (Tiberium_Value(cptr) * scale < 1.0) {
                        weight = 1.0;
                    } else {
                        weight = Tiberium_Value(cptr) * scale;
                    }
                    int intweight = (int)weight;
                    westrun = true;
                    celldist.Add(cptr, intweight);
                }
            } else {
                westrun = false;
            }

            cell = Cell(ring, x) + center;
            if (unknown_4cb6d0(cell)) {
                if (!eastrun) {
                    CellClass *cptr = &Map[cell];
                    if (Tiberium_Value(cptr) * scale < 1.0) {
                        weight = 1.0;
                    } else {
                        weight = Tiberium_Value(cptr) * scale;
                    }
                    int intweight = (int)weight;
                    eastrun = true;
                    celldist.Add(cptr, intweight);
                }
            } else {
                eastrun = false;
            }
        }
    }

    CellClass *cptr = celldist.Sample();
    if (cptr != 0) {
        return ((UnknownCellClass *)cptr)->unknown_24;
    }
    return unknown_geometry_k;
}


// Cell FootClass::unknown_4cc100(int radius) (0x004CC100)
// The unit's own cell when it passes 0x004CC260; otherwise, ring by ring
// out to the radius, the last cell on the first ring that has one that
// passes (the zero cell when none does).

Cell FootClass::unknown_4cc100(int radius)
{
    Cell found = unknown_geometry_k;
    Cell center = Coord_Cell(Center_Coord());
    if (unknown_4cc260(center, 0, 0)) {
        return center;
    }
    for (int ring = 1; ring < radius; ring++) {
        for (int index = -ring; index <= ring; index++) {
            Cell cell = center;
            if (unknown_4cc260(cell, index, -ring)) {
                found = cell;
            }
            cell = center;
            if (unknown_4cc260(cell, index, ring)) {
                found = cell;
            }
            cell = center;
            if (unknown_4cc260(cell, -ring, index)) {
                found = cell;
            }
            cell = center;
            if (unknown_4cc260(cell, ring, index)) {
                found = cell;
            }
        }
        if (found.X != unknown_geometry_k.X || found.Y != unknown_geometry_k.Y) {
            break;
        }
    }
    return found;
}


// bool FootClass::unknown_4cc260(Cell &cell, int dx, int dy) (0x004CC260)
// Whether the cell at an offset suits the unit (stored back into cell when
// the map's test 0x0055A660 passes): not a raised spot for a +0x372 unit,
// reachable in the unit's movement zone, unoccupied (slot 101), with land
// type 11 and +0x11E of at least 0x30.

bool FootClass::unknown_4cc260(Cell &cell, int dx, int dy)
{
    Cell next = cell + Cell(dx, dy);
    if (Map.Unknown_0055A660(next, true)) {
        cell = next;
        if (unknown_game_mode_a3d298 == 0 && unknown_372) {
            int height = Map.Get_Height_GL(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0));
            CoordStruct spot = CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, height);
            if (Map.Is_Shrouded(spot)) {
                return false;
            }
        }
        Cell here = Coord_Cell(unknown_slot19(0));
        if (Map.Unknown_0054F7C0(here, cell, Techno_Type_Class()->MovementZone, Is_Moving_Onto_Bridge(0), 0, 0)) {
            CellClass *map_cell = &Map[cell];
            UnknownCellClass *object = (UnknownCellClass *)map_cell;
            if (Can_Enter_Cell(map_cell, -1, -1, 0, 1) == 0 && object->unknown_ec == 11 && object->unknown_11e >= 0x30) {
                return true;
            }
        }
    }
    return false;
}


// bool FootClass::unknown_4cc400(int arg1) (0x004CC400)
// With nothing at +0x494: the cell chosen by 0x004CC100; true when the unit
// already stands there, otherwise slot 260 toward that cell's object
// (false). False when no cell is chosen.

bool FootClass::unknown_4cc400(int arg1)
{
    if (unknown_494 == 0) {
        Cell cell = unknown_4cc100(arg1);
        if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
            Cell here = Coord_Cell(Center_Coord());
            if (cell.X == here.X && cell.Y == here.Y) {
                return true;
            }
            Assign_Destination((AbstractClass *)&Map[cell], true);
        }
    }
    return false;
}


// bool FootClass::Is_Moving_Onto_Bridge(TechnoClass *techno) (0x004CC4B0)
// ObjectClass's slot 46 only while +0x574 is negative.

bool FootClass::Is_Moving_Onto_Bridge(TechnoClass *techno)
{
    if (unknown_574 >= 0) {
        return false;
    }
    return ObjectClass::Is_Moving_Onto_Bridge(techno);
}


// bool FootClass::unknown_slot309(AbstractClass *target) (0x004CC4D0)
// Whether the cell of a target is free for this unit: the unit itself or
// its radio contact may be there, and for a Spawned type an object that
// carries spawns (+0x238) or is Spawned; an empty cell must pass the cell
// test 0x00478760 and no other landed aircraft may be heading for the same
// target.

bool FootClass::unknown_slot309(AbstractClass *target)
{
    if (target == 0) {
        return false;
    }
    Cell cell = Coord_Cell(target->Center_Coord());
    if (!Map.Unknown_0055A660(cell, true)) {
        return false;
    }
    TechnoClass *techno = ((UnknownCellClass *)&Map[cell])->Unknown_00471660(Point2D(0, 0), 0, 0);
    if (techno != 0) {
        if (this != techno && Contact_With_Whom(0) != techno) {
            if (!Techno_Type_Class()->Spawned || (techno->SpawnManager == 0 && !techno->Techno_Type_Class()->Spawned)) {
                return false;
            }
        }
    } else {
        if (!((UnknownCellClass *)&Map[cell])->Unknown_00478760(1, 0, 0, -1, 0, -1, 1)) {
            return false;
        }
        for (int index = 0; index < Aircraft.Count(); index++) {
            AircraftClass *aircraft = Aircraft[index];
            if (aircraft->unknown_7d && !aircraft->unknown_76 && aircraft->unknown_494 == target && aircraft != this) {
                return false;
            }
        }
    }
    return true;
}


// int FootClass::unknown_slot27(Cell const *cell, bool arg2, bool arg3) (0x004CC650)
// TechnoClass's answer, except that at the centre of the cell (raised to
// the ground and by a bridge's height) passing the map test 0x00568140 a
// nonzero answer becomes 2, or 1 for a MoveToShroud type when the map test
// 0x0055A660 holds and the answer is not 0x33.

int FootClass::unknown_slot27(Cell const *cell, bool arg2, bool arg3)
{
    int result = TechnoClass::unknown_slot27(cell, arg2, arg3);
    CoordStruct coord(cell->X * 256 + 128, cell->Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    if (Loco_Cell(coord)->Flags & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    if (Map.Is_Shrouded(coord) && result != 0) {
        if (Techno_Type_Class()->MoveToShroud && Map.Unknown_0055A660(*cell, true)) {
            if (result != 0x33) {
                return 1;
            }
        } else {
            return 2;
        }
    }
    return result;
}


// int FootClass::unknown_slot28(ObjectClass *object, bool arg2) (0x004CC740)
// TechnoClass's answer, except that at a ground-level spot of the object
// (raised by a bridge's height) that passes the map test 0x00568140, in a
// game mode of 0, a nonzero answer becomes 1 for a MoveToShroud type and 2
// otherwise.

int FootClass::unknown_slot28(ObjectClass *object, bool arg2)
{
    int result = TechnoClass::unknown_slot28(object, arg2);
    CoordStruct coord = object->Center_Coord();
    coord.Z = Map.Get_Height_GL(coord);
    if (Loco_Cell(coord)->Flags & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    if (Map.Is_Shrouded(coord) && result != 0 && unknown_game_mode_a3d298 == 0) {
        return Techno_Type_Class()->MoveToShroud ? 1 : 2;
    }
    return result;
}


// int FootClass::Do_MISSION_RESCUE() (0x004CC800)
// Area guard: state 0 attacks its target (slot 304) or, guarding a spot
// (+0x1B0, the unit's own cell by default), takes on an enemy (slot 219)
// within one and a half times its range (slot 188); with no target it
// heads for its house's guard cell (0x004EB270) and enters state 1, which
// resumes mission 11 on arrival. Returns the mission's rate in frames plus
// 0..2 (1 while attacking).

int FootClass::Do_MISSION_RESCUE()
{
    CoordStruct coord;
    switch (unknown_a8) {
    case 0:
        if (unknown_228 != 0) {
            unknown_slot304();
            break;
        }
        if (unknown_1b0 == 0) {
            coord = Coord;
            unknown_6d6e50((AbstractClass *)&Map[coord]);
        }
        unknown_578 = false;
        {
            AbstractClass *target = unknown_slot219(0, &unknown_1b0->Center_Coord(), 0);
            if (target != 0) {
                double distance = Offset_Length(target->Center_Coord() - unknown_1b0->Center_Coord());
                if (unknown_slot188(1) * 1.5 > distance) {
                    unknown_slot220(target);
                }
            }
        }
        if (unknown_228 != 0) {
            return 1;
        }
        unknown_a8 = 1;
        {
            Cell cell = House->Unknown_004EB270(this);
            if (cell != unknown_geometry_k) {
                Assign_Destination((AbstractClass *)&Map[cell], true);
            } else {
                Assign_Destination(0, true);
            }
        }
        unknown_6d6e50(0);
        break;
    case 1:
        if (unknown_494 == 0) {
            unknown_6d6e50(0);
            QueueMission(11, false);
            Commence();
        }
        break;
    }
    int delay = (int)(Control()->unknown_10 * 900.0);
    return delay + unknown_id_source_a3d290->Random()(0, 2);
}


// Cell FootClass::unknown_4cca40(Cell const &cell, bool arg2) (0x004CCA40)
// The cell to head for when ordered to cell (slot 77; arg2 is passed on to
// slot 27, which gives the action): no cell (unknown_geometry_k) when the
// cell's ground (bridge height included) is shrouded and the type may not
// move to shroud; otherwise the cell itself, or a nearby cell (0x005502E0)
// when it is off the visible map, shrouded or in another movement zone; a
// type that moves anywhere looks for a nearby cell only on action 2 (in the
// same zone as the slot 19 destination's cell otherwise). Teleporter types
// skip the zone check unless the action is 2. The movement zone maps 6 to
// 0 and 9 to 7 while the house's 0x0051FC80 holds; jumpjet infantry move
// anywhere only while it does not.

Cell FootClass::unknown_4cca40(Cell const &cell, bool arg2)
{
    int action = unknown_slot27(&cell, arg2, false);
    bool in_radar = Map.Unknown_0055A660(cell, true);

    CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    if (((UnknownCellClass *)&Map[coord])->Flags & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    if (Techno_Type_Class()->MoveToShroud || !Map.Is_Shrouded(coord)) {

        Cell nearby = cell;
        UnknownCellClass *cptr = (UnknownCellClass *)&Map[unknown_slot19(0)];
        int mzone = Techno_Type_Class()->MovementZone;
        if (mzone == 6) mzone = 0;
        else if (mzone == 9 && House->Unknown_0051FC80()) mzone = 7;

        bool moveanywhere = In_Air() || Techno_Type_Class()->unknown_af5 || (Kind_Of() == 15 && ((InfantryClass *)this)->Type->JumpJet && !House->Unknown_0051FC80()) || Kind_Of() == 2;

        bool ontobridge = Is_Moving_Onto_Bridge(0);
        CoordStruct center = Cell_Coord(cell);
        center.Z = Map.Get_Height_GL(center);
        if (((UnknownCellClass *)&Map[center])->Flags & 0x100) {
            center.Z += unknown_geometry_j;
        }
        bool shrouded = Map.Is_Shrouded(center);

        if (moveanywhere && action == 2) {
            nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, -1, mzone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, Point2D(1, 1), 0, 1, Techno_Type_Class()->unknown_af5 && Get_Height_AGL() < 0, 1, Cell(0, 0), 0);
        } else {
            if (!in_radar || shrouded || !moveanywhere && (!Techno_Type_Class()->Teleporter || action == 2) && !Map.Unknown_0054F7C0(cptr->unknown_24, nearby, mzone, ontobridge, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, false)) {
                nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, Map.Zone(cptr->unknown_24, mzone, ontobridge), mzone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
            }
        }

        if (nearby != unknown_geometry_k) {
            return nearby;
        }
    }
    return unknown_geometry_k;
}


// void FootClass::unknown_slot194() (0x004CCDF0)
// Following a waypoint path (+0x418, the player's path index; +0x576, the
// waypoint index): heads for the waypoint after the current one.

void FootClass::unknown_slot194()
{
    if (unknown_418 != -1) {
        unknown_4cb110(unknown_player_a35db4->WaypointPaths[unknown_418]->Next_Waypoint(
            unknown_player_a35db4->WaypointPaths[unknown_418]->Get_Waypoint(unknown_576)));
    }
}


// void FootClass::Delete_Me() (0x004CCE40)
// The unit is removed: the owned object at +0x230 is told (0x004681E0),
// the unit leaves its team, then ObjectClass's slot 60.
#include "shared/capture_manager.hpp"

void FootClass::Delete_Me()
{
    if (unknown_230 != 0) {
        ((CaptureManagerClass *)unknown_230)->Release_4681e0();
    }
    if (Team != 0) {
        Team->Unknown_006B8FB0(this, -1, 0);
    }
    TechnoClass::Delete_Me();
}


// bool FootClass::In_Air() const (0x004CCE70)
// ObjectClass's (compiled as a jump).

bool FootClass::In_Air() const
{
    return ObjectClass::In_Air();
}


// void FootClass::Unknown_004CCE80(FootClass *passenger) (0x004CCE80)
// A passenger enters this transport: it joins the cargo, the type's
// EnterTransportSound plays at the transport, and a Gunner type with its
// first passenger runs slot 280.

void FootClass::Unknown_004CCE80(FootClass *passenger)
{
    Cargo.Attach(passenger);
    if (passenger != 0 && Techno_Type_Class()->EnterTransportSound != -1) {
        Voc_Static_Add(Techno_Type_Class()->EnterTransportSound, CoordStruct(Coord), 0);
    }
    if (Techno_Type_Class()->Gunner && Cargo.Count == 1) {
        unknown_slot280((int)passenger);
    }
}


// void FootClass::unknown_4ccf20(FootClass *passenger) (0x004CCF20)
// A given passenger leaves this transport (never called; the other units
// expand it): it leaves the cargo, the type's LeaveTransportSound plays at
// the transport, and a Gunner type left empty runs slot 281.

void FootClass::unknown_4ccf20(FootClass *passenger)
{
    Cargo.Detach(passenger);
    if (passenger != 0 && Techno_Type_Class()->LeaveTransportSound != -1) {
        Voc_Static_Add(Techno_Type_Class()->LeaveTransportSound, CoordStruct(Coord), 0);
    }
    if (Techno_Type_Class()->Gunner && Cargo.Count == 0) {
        unknown_slot281((int)passenger);
    }
}


// FootClass *FootClass::unknown_4ccfc0() (0x004CCFC0)
// The first passenger leaves this transport: a Gunner type left empty runs
// slot 281, and the type's LeaveTransportSound plays at the transport.

FootClass *FootClass::unknown_4ccfc0()
{
    FootClass *passenger = (FootClass *)Cargo.Detach_Object();
    if (Techno_Type_Class()->Gunner && Cargo.Count == 0) {
        unknown_slot281((int)passenger);
    }
    if (passenger != 0 && Techno_Type_Class()->LeaveTransportSound != -1) {
        Voc_Static_Add(Techno_Type_Class()->LeaveTransportSound, CoordStruct(Coord), 0);
    }
    return passenger;
}


class Leaf_4cd060 { public: void get(unsigned int unused_argument_1); };
void Leaf_4cd060::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


class Leaf_4cd070 { public: void get(unsigned int unused_argument_1); };
void Leaf_4cd070::get(unsigned int unused_argument_1) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


// bool FootClass::unknown_slot202() (0x004CD080)
// Whether the timer at +0x58C is still running.

bool FootClass::unknown_slot202()
{
    return unknown_58c.Value() != 0;
}


// void FootClass::unknown_slot285(Cell cell) (0x004CD0C0)
// Enters the cells within the type's SensorsSight of a cell (the unit's own
// cell for the zero cell) for the owning house (0x0047C360):
// units there are told (slot 238) and a building of another house than the
// player's that answers slot 25 is flagged (+0x75).

void FootClass::unknown_slot285(Cell cell)
{
    int range = Techno_Type_Class()->SensorsSight;
    int range_squared = range * range;
    int house = House->ArrayIndex;
    if (cell == unknown_geometry_k) {
        cell = Coord_Cell(Center_Coord());
    }
    for (int y = -range; y < range; y++) {
        for (int x = -range; x < range; x++) {
            Cell spot = Cell(cell.X + x, cell.Y + y);
            if (x * x + y * y < range_squared) {
                UnknownCellClass *object = (UnknownCellClass *)&Map[spot];
                object->Unknown_0047C360(house);
                for (ObjectClass *occupier = object->unknown_e4; occupier != 0; occupier = occupier->unknown_30) {
                    int kind = occupier->Kind_Of();
                    if (kind == 1 || kind == 15 || kind == 2) {
                        ((TechnoClass *)occupier)->unknown_slot238();
                    }
                }
                BuildingClass *building = object->Unknown_004717B0();
                if (building != 0 && building->House != unknown_player_a35db4 && building->unknown_slot25(false, 0)) {
                    building->unknown_75 = true;
                }
            }
        }
    }
}


// void FootClass::unknown_slot286(Cell cell) (0x004CD250)
// Leaves the cells within the type's SensorsSight of a cell (the unit's own
// cell for the zero cell) for the owning house (0x0047C2E0/0x0047C370):
// units there are told (slot 238) and a building of another house than the
// player's that answers slot 25 is flagged (+0x75).

void FootClass::unknown_slot286(Cell cell)
{
    int range = Techno_Type_Class()->SensorsSight;
    int range_squared = range * range;
    int house = House->ArrayIndex;
    if (cell == unknown_geometry_k) {
        cell = Coord_Cell(Center_Coord());
    }
    for (int y = -range; y < range; y++) {
        for (int x = -range; x < range; x++) {
            Cell spot = Cell(cell.X + x, cell.Y + y);
            if (x * x + y * y < range_squared) {
                UnknownCellClass *object = (UnknownCellClass *)&Map[spot];
                if (object->Is_Sensed(house)) {
                    object->Unknown_0047C370(house);
                    for (ObjectClass *occupier = object->unknown_e4; occupier != 0; occupier = occupier->unknown_30) {
                        int kind = occupier->Kind_Of();
                        if (kind == 1 || kind == 15 || kind == 2) {
                            ((TechnoClass *)occupier)->unknown_slot238();
                        }
                    }
                    BuildingClass *building = object->Unknown_004717B0();
                    if (building != 0 && building->House != unknown_player_a35db4 && building->unknown_slot25(false, 0)) {
                        building->unknown_75 = true;
                    }
                }
            }
        }
    }
}


// void FootClass::unknown_slot82(int arg1) (0x004CD3F0)
// An Organic type takes damage equal to its strength from the rules
// warhead at +0xC74; otherwise the parasite riding this unit restarts its
// timer (50 frames) and is told so, the +0x58C timer restarts, and
// TechnoClass's slot 82 runs.

void FootClass::unknown_slot82(int arg1)
{
    if (Techno_Type_Class()->Organic) {
        int damage = Techno_Type_Class()->MaxStrength;
        Take_Damage(&damage, 0, Rules->unknown_c74, 0, false, 0);
        return;
    }
    if (unknown_580 != 0) {
        unknown_580->unknown_588->unknown_2c = CDTimerClass<FrameTimerClass>(50);
        unknown_580->unknown_588->Unknown_00607440();
    }
    unknown_58c = CDTimerClass<FrameTimerClass>(0);
    TechnoClass::unknown_slot82(arg1);
}


// bool FootClass::unknown_slot224(int source) (0x004CD4B0)
// The unit is destroyed: with strength left the tag hears events 0x26 and
// 0x29 and slot 55 runs; then mission 3 (slot 149), slot 210, TechnoClass
// 0x006D3040, random spin rates (+0x288, +0x28C) for non-infantry, and the
// object leaves the scenario lists (0x006ECB90). False when slot 108 is 0.

bool FootClass::unknown_slot224(int source)
{
    if (Get_Height_AGL() > 0) {
        if (Strength > 0) {
            if (source != 0 && unknown_34 != 0) {
                unknown_34->Spring(0x26, this, unknown_geometry_k, 0, 0);
            }
            if (unknown_34 != 0 && unknown_7d) {
                unknown_34->Spring(0x29, this, unknown_geometry_k, 0, 0);
            }
            if (unknown_34 != 0 && unknown_7d && source != 0) {
                unknown_34->Spring(0x29, this, unknown_geometry_k, 0, source);
            }
            if (!unknown_7d) {
                return true;
            }
            Record_The_Kill((TechnoClass *)source);
            if (!unknown_7d) {
                return true;
            }
            Strength = 0;
        }
        unknown_37d = true;
        unknown_slot149(3);
        unknown_slot210();
        unknown_6d3040((TechnoClass *)source);
        if (Kind_Of() != 15 && unknown_a40774 == 0) {
            double spin = unknown_id_source_a3d290->Random()(0, 0x7FFFFFFE);
            spin *= 1.0 / 2147483646.0;
            spin *= 0.15;
            unknown_288 = (float)(spin + 0.1);
            if (unknown_id_source_a3d290->Random()(0, 1) == 0) {
                unknown_288 = -unknown_288;
            }
            double tilt = unknown_id_source_a3d290->Random()(0, 0x7FFFFFFE);
            tilt *= 1.0 / 2147483646.0;
            unknown_28c = (float)(tilt * 0.1);
        }
        Unknown_006ECB90(this, false);
        return true;
    }
    return false;
}


// void *FootClass::Get_Image_Data() (0x004CD650)
// The image to draw: the real type's (slot 50 with false) for an enemy unit
// the player sees through a cell not visible to him; the disguise's (slot
// 50 with true) when slot 245 refuses the player; otherwise ObjectClass's.
#include "shared/object_type.hpp"

void *FootClass::Get_Image_Data()
{
    UnknownCellClass *cell = (UnknownCellClass *)&Map[Coord_Cell(Center_Coord())];
    if (!House->Is_Ally(unknown_player_a35db4) && unknown_slot48() && !cell->unknown_47c300(unknown_player_a35db4->ArrayIndex)) {
        return unknown_slot50(false)->unknown_a4;
    }
    if (!unknown_slot245(unknown_player_a35db4)) {
        return unknown_slot50(true)->unknown_a4;
    }
    return ObjectClass::Get_Image_Data();
}


// TechnoClass *FootClass::unknown_slot301(TechnoTypeClass *type, int arg2, int arg3) (0x004CD730)
// Slot 300 with an output initialized to -1 (discarded).

TechnoClass *FootClass::unknown_slot301(TechnoTypeClass *type, int arg2, int arg3)
{
    int unused = -1;
    return unknown_slot300(type, arg2, arg3, &unused);
}


// TechnoClass *FootClass::unknown_slot300(TechnoTypeClass *type, int arg2, int arg3, int *score) (0x004CD760)
// The best building of a type to dock at (hypothesis): among the house's
// active Buildings of that type that are in radio contact (unless arg3 is
// 1) and reachable (aircraft always), those accepting message 0xF; the
// nearest wins (Distance_Squared into *score, -1 for none yet), a building
// with +0x32B set always replaces the choice. Null when the house has none
// of the type (0x00491D10).

TechnoClass *FootClass::unknown_slot300(TechnoTypeClass *type, int arg2, int arg3, int *score)
{
    TechnoClass *best = 0;
    if (House->unknown_5420.unknown_func_491d10(((BuildingTypeClass *)type)->ArrayIndex) != 0) {
        int count = House->Buildings.Count();
        for (int index = 0; index < count; index++) {
            BuildingClass *building = House->Buildings[index];
            if (building != 0 && !building->unknown_76 && building->Type == type
                && (arg3 == 1 || building->Unknown_00636ED0(this))) {
                if (Kind_Of() != 2) {
                    Cell there = Coord_Cell(building->Center_Coord());
                    Cell here = Coord_Cell(unknown_slot19(0));
                    if (!Map.Unknown_0054F7C0(here, there, Techno_Type_Class()->MovementZone, Is_Moving_Onto_Bridge(0), 0, 0)) {
                        continue;
                    }
                }
                if (unknown_slot150(15, building) == 1) {
                    int distance = Distance_Squared(building);
                    if (*score == -1 || distance < *score || building->unknown_32b) {
                        best = building;
                        *score = distance;
                    }
                }
            }
        }
    }
    return best;
}


// TechnoClass *FootClass::unknown_slot299(DynamicVectorClass<TechnoTypeClass *> const *types, int arg2, int arg3) (0x004CD920)
// Slot 300 for each type; keeps the result with the lowest score, or a
// later one whose +0x32B is set, or any when the best score is -1.

TechnoClass *FootClass::unknown_slot299(DynamicVectorClass<TechnoTypeClass *> const *types, int arg2, int arg3)
{
    TechnoClass *best = 0;
    int best_score = -1;
    for (int index = 0; index < types->Count(); index++) {
        int score = -1;
        TechnoClass *found = unknown_slot300((*types)[index], arg2, arg3, &score);
        if (found != 0) {
            if (best == 0 || score < best_score || best_score == -1 || found->unknown_32b) {
                best = found;
                best_score = score;
            }
        }
    }
    return best;
}


class Leaf_4cd9b0 { public: void get(); };
void Leaf_4cd9b0::get() { unsigned int foot_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(foot_address + 0x490) = zero_value; *(unsigned int*)(foot_address + 0x494) = zero_value; }


// int FootClass::unknown_slot268(UnknownSlot268Arg const *arg) (0x004CD9C0)
// For a kind 0x1D record: unless slot 275 holds, 1 or 2 by whether the first
// reference is set; otherwise records it as the pending order (+0x4B4 = 0x1D)
// with the object of the second reference (+0x4B8, 2) or of the first
// (+0x4BC, 1). Anything else: slot 269 and the kind.

int FootClass::unknown_slot268(UnknownSlot268Arg const *arg)
{
    int kind = arg->unknown_0c;
    if (kind == 0x1D) {
        bool second = arg->unknown_13.unknown_4 != 0;
        bool first = arg->unknown_0e.unknown_4 != 0;
        if (!unknown_slot275()) {
            return first ? 1 : 2;
        }
        unknown_4c1 = false;
        unknown_4b4 = 0x1D;
        if (second) {
            unknown_4b8 = arg->unknown_13.Unknown_006B5810();
            return 2;
        }
        if (first) {
            unknown_4bc = arg->unknown_0e.Unknown_006B5810();
            return 1;
        }
        unknown_slot269();
    } else {
        unknown_slot269();
    }
    return kind;
}


// 0x004cda80: function not yet identified
//
// Body: 31 bytes (frozen inventory bound).
// Referenced from 4 sites (data).
// Ghidra prototype: void __fastcall FUN_004cda80(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004cda80 {

void __fastcall FUN_004cda80(int foot_address)
{
    *(unsigned int *)(foot_address + 0x4b4) = 0xffffffff;
    *(unsigned int *)(foot_address + 0x4b8) = 0;
    *(unsigned int *)(foot_address + 0x4bc) = 0;
    *(unsigned char *)(foot_address + 0x4c1) = 0;
    return;
}

}  // namespace seed_004cda80


// 0x004cdaa0: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 4 sites (data).
// Ghidra prototype: bool __fastcall FUN_004cdaa0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004cdaa0 {

bool __fastcall FUN_004cdaa0(int foot_address)
{
    return *(int *)(foot_address + 0x4b4) != -1;
}

}  // namespace seed_004cdaa0


class Leaf_4cdab0 { public: unsigned char get(); };
unsigned char Leaf_4cdab0::get() { return *(unsigned char*)((char*)this + 0x4c1); }


class Leaf_4cdac0 { public: unsigned int get(); };
unsigned int Leaf_4cdac0::get() { return *(unsigned int*)((char*)this + 0x4b4); }


// CoordStruct FootClass::unknown_slot273() (0x004CDAD0)
// The coordinate of the pending order's object (+0x4B8, else +0x4BC), or
// (-1, -1, -1).

CoordStruct FootClass::unknown_slot273()
{
    if (unknown_4b8 != 0) {
        return unknown_4b8->Center_Coord();
    }
    if (unknown_4bc != 0) {
        return unknown_4bc->Center_Coord();
    }
    return CoordStruct(-1, -1, -1);
}


// void FootClass::unknown_4cdb60() (0x004CDB60)
// Replaces +0x4BC by the map's cell object (0x00548070) at the cell of its
// coordinate, stored at +0x4B8.

void FootClass::unknown_4cdb60()
{
    Cell cell = Coord_Cell(unknown_4bc->Center_Coord());
    unknown_4b8 = (AbstractClass *)&Map[cell];
    unknown_4bc = 0;
}


// 0x004cdbd0: function not yet identified
//
// Body: 15 bytes (frozen inventory bound).
// Referenced from 4 sites (data).
// Ghidra prototype: bool __fastcall FUN_004cdbd0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004cdbd0 {

bool __fastcall FUN_004cdbd0(int foot_address)
{
    return *(int *)(foot_address + 0x4b4) == 0x1d;
}

}  // namespace seed_004cdbd0


// bool FootClass::unknown_slot277() (0x004CDBE0)
// Clears +0x4C1, then resumes a pending order: with +0x4B8, mission 2 and
// slot 260 on it; else with +0x4BC, mission 1 and slot 220 on it; else
// slot 269 and false.

bool FootClass::unknown_slot277()
{
    bool result = true;
    unknown_4c1 = false;
    if (unknown_4b8 != 0) {
        QueueMission(2, true);
        Assign_Destination(unknown_4b8, true);
    } else if (unknown_4bc != 0) {
        QueueMission(1, true);
        unknown_slot220(unknown_4bc);
    } else {
        unknown_slot269();
        result = false;
    }
    return result;
}


// void FootClass::unknown_slot278() (0x004CDC60)
// Resumes a pending order: keeps attacking the current target (slot 215)
// while that still works, otherwise heads for the unit's coordinate (slot
// 209) and on success enters mission 1 and marks the order resumed (+0x4C1).

void FootClass::unknown_slot278()
{
    if (unknown_4b8 != 0) {
        if (unknown_4c1) {
            if (unknown_slot215(unknown_228)) {
                return;
            }
            unknown_228 = 0;
        }
        CoordStruct coord = Coord;
        if (unknown_slot209(&coord, 1)) {
            QueueMission(1, true);
            unknown_4c1 = true;
        }
    } else {
        if (unknown_4bc != 0 && unknown_slot215(unknown_4bc)) {
            return;
        }
        if (unknown_4c1 && unknown_slot215(unknown_228)) {
            return;
        }
        unknown_228 = 0;
        CoordStruct coord = Coord;
        if (unknown_slot209(&coord, 1)) {
            QueueMission(1, true);
            unknown_4c1 = true;
        } else {
            unknown_228 = unknown_4bc;
        }
    }
}


// bool FootClass::unknown_slot279() (0x004CDD70)
// In state 0x1D: a pending unknown_4c1 or mission 5 runs slot 277,
// otherwise slot 269.

bool FootClass::unknown_slot279()
{
    bool result = false;
    if (unknown_4b4 == 0x1D) {
        if (unknown_4c1) {
            unknown_4c1 = false;
            result = unknown_slot277();
        } else if (Get_Mission() == 5 || Get_Mission() == 5) {
            result = unknown_slot277();
        } else {
            unknown_slot269();
            result = false;
        }
    }
    return result;
}


class Leaf_4cddd0 { public: int get(); };
int Leaf_4cddd0::get() { return 0; }


class Leaf_4cdde0 { public: unsigned char get(); };
unsigned char Leaf_4cdde0::get() { return 1; }


// UnknownFacingDir TechnoClass::unknown_slot162() (0x004CDDF0, slot 162)

UnknownFacingDir TechnoClass::unknown_slot162()
{
    return unknown_2e0.Current();
}


extern "C" void __cdecl release_4cde10(void*);
class Leaf_4cde10 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_4cde10::get(unsigned int flags) { cleanup(); if (flags & 1) release_4cde10(this); return this; }


// 0x004cde90: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 8 sites (unconditional call).
// Ghidra prototype: int __fastcall FUN_004cde90(int *param_1)
// Callees with accepted source:
//   FUN_00795940 -> src/recovered/00795940.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004cde90 {

extern void __stdcall FUN_00795940(unsigned int error_code);

int __fastcall FUN_004cde90(int *pointer_words)
{
    if (*pointer_words == 0) {
        FUN_00795940((unsigned int)(0x80004003));
    }
    return *pointer_words;
}

}  // namespace seed_004cde90


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AbstractClass *>::Clear (0x004CDE30; DynamicVectorClass<AbstractClass *> vtable 0x007a177c slot 3).
// DynamicVectorClass<AbstractClass *>::ID_pointer (0x004CDE60; DynamicVectorClass<AbstractClass *> vtable 0x007a177c slot 5).
// VectorClass<AbstractClass *>::Fetch (0x004CDE80; VectorClass<AbstractClass *> vtable 0x007a179c slot 6).
// DynamicVectorClass<FootClass *>::Clear (0x004CDEB0; DynamicVectorClass<FootClass *> vtable 0x007a1234 slot 3).
// DynamicVectorClass<FootClass *>::ID_pointer (0x004CDEE0; DynamicVectorClass<FootClass *> vtable 0x007a1234 slot 5).
// VectorClass<FootClass *>::Fetch (0x004CDF00; DynamicVectorClass<FootClass *> vtable 0x007a1234 slot 6).


extern "C" unsigned char vtable_4cdf30[]; extern "C" void __cdecl release_buffer_4cdf30(void*); extern "C" void __cdecl release_object_4cdf30(void*);
class Leaf_4cdf30 { public: void* get(unsigned int flags); };
void* Leaf_4cdf30::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4cdf30; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4cdf30((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4cdf30(this); return this; }


// void UnknownComPtr<ILocomotion>::Release_Held() (0x004CDF80)
// Releases the held locomotor interface (the _com_ptr_t release step the
// units' Load and Save call out of line).

template <>
void UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}


extern "C" unsigned char vtable_4cdf90[]; extern "C" void __cdecl release_buffer_4cdf90(void*); extern "C" void __cdecl release_object_4cdf90(void*);
class Leaf_4cdf90 { public: void* get(unsigned int flags); };
void* Leaf_4cdf90::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4cdf90; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4cdf90((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4cdf90(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<AbstractClass *>::Add (0x004CE210): emitted by this unit's use of its list.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<AbstractClass *>::operator equal (0x004CDFE0; VectorClass<AbstractClass *> vtable 0x007a179c slot 1).
// VectorClass<AbstractClass *>::Resize (0x004CE020; VectorClass<AbstractClass *> vtable 0x007a179c slot 2).
// VectorClass<AbstractClass *>::Clear (0x004CE0D0; VectorClass<AbstractClass *> vtable 0x007a179c slot 3).
// VectorClass<AbstractClass *>::ID_object (0x004CE100; VectorClass<AbstractClass *> vtable 0x007a179c slot 4).
// VectorClass<AbstractClass *>::ID_pointer (0x004CE140; VectorClass<AbstractClass *> vtable 0x007a179c slot 5).
// DynamicVectorClass<AbstractClass *>::Resize (0x004CE160; DynamicVectorClass<AbstractClass *> vtable 0x007a177c slot 2).
// DynamicVectorClass<AbstractClass *>::ID_object (0x004CE260; DynamicVectorClass<AbstractClass *> vtable 0x007a177c slot 4).
// VectorClass<FootClass *>::operator equal (0x004CE290; VectorClass<FootClass *> vtable 0x007a1254 slot 1).
// VectorClass<FootClass *>::Resize (0x004CE2D0; VectorClass<FootClass *> vtable 0x007a1254 slot 2).
// VectorClass<FootClass *>::Clear (0x004CE380; VectorClass<FootClass *> vtable 0x007a1254 slot 3).
// VectorClass<FootClass *>::ID_object (0x004CE3B0; VectorClass<FootClass *> vtable 0x007a1254 slot 4).
// VectorClass<FootClass *>::ID_pointer (0x004CE3F0; VectorClass<FootClass *> vtable 0x007a1254 slot 5).
// DynamicVectorClass<FootClass *>::Resize (0x004CE410; DynamicVectorClass<FootClass *> vtable 0x007a1234 slot 2).
// DynamicVectorClass<FootClass *>::ID_object (0x004CE4C0; DynamicVectorClass<FootClass *> vtable 0x007a1234 slot 4).
// DynamicVectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *>>::Resize (0x004CE820).


// 0x004ce940: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void FUN_004ce940(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004ce940 {

void  FUN_004ce940(void)
{
    return;
}

}  // namespace seed_004ce940


extern "C" unsigned char vtable_4ce9f0[]; extern "C" void __cdecl release_buffer_4ce9f0(void*); extern "C" void __cdecl release_object_4ce9f0(void*);
class Leaf_4ce9f0 { public: void* get(unsigned int flags); };
void* Leaf_4ce9f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4ce9f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4ce9f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4ce9f0(this); return this; }


extern "C" unsigned char vtable_4cea40[]; extern "C" void __cdecl release_buffer_4cea40(void*); extern "C" void __cdecl release_object_4cea40(void*);
class Leaf_4cea40 { public: void* get(unsigned int flags); };
void* Leaf_4cea40::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4cea40; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4cea40((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4cea40(this); return this; }


extern "C" unsigned char table_4ceb90[]; extern "C" void* __cdecl storage_4ceb90(unsigned int);
class Storage_4ceb90 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4ceb90::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4ceb90; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4ceb90(requested << 2); owns=1; } return this; }


// bool DynamicVectorClass<FootClass *>::Delete(int index) (0x004CEBE0)
// Instantiated from shared/vector_class.hpp; called by ~FootClass for the
// FootClass list (0x00865F38).
// @symbol ?Delete@?$DynamicVectorClass@PAVFootClass@@@@QAE_NH@Z
