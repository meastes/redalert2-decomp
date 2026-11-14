// mission.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mission.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/mission.hpp"
#include "shared/ccini_class.hpp"


// Header-static dynamic initializers of the unit at 0x00594310:  the
// shared/unknown_geometry.hpp constants (0x00594310-0x005944D1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00594340, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x00594360, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x00594380, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x005943A0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x005943C0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x005943E0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x00594420, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x00594450, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x00594480, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x005944B0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer (0x005944C0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00594310.cpp; no source of its own.


// Dynamic initializer of unknown_static_rect (0x005944E0, _$E) from
// shared/unknown_static_rect.hpp (0x00A70C50 in this unit); emitted by the
// headers; no source of its own.


// MissionClass::MissionClass() (0x00594500)
// No mission, nothing queued or suspended; the timer starts now.

MissionClass::MissionClass() :
    Mission(-1),
    SuspendedMission(-1),
    MissionQueue(-1),
    unknown_a4(false),
    unknown_a8(0),
    unknown_ac(0)
{
}


// int MissionClass::Do_MISSION_SLEEP() (0x00594570, slot 123): a mission handler; 450.

int MissionClass::Do_MISSION_SLEEP()
{
    return 450;
}


// int MissionClass::Do_MISSION_HARMLESS() (0x00594580, slot 124): a mission handler; 450.

int MissionClass::Do_MISSION_HARMLESS()
{
    return 450;
}


// int MissionClass::Do_MISSION_AMBUSH() (0x00594590, slot 125): a mission handler; 450.

int MissionClass::Do_MISSION_AMBUSH()
{
    return 450;
}


// int MissionClass::Do_MISSION_ATTACK() (0x005945A0, slot 126): a mission handler; 450.

int MissionClass::Do_MISSION_ATTACK()
{
    return 450;
}


// int MissionClass::Do_MISSION_CAPTURE() (0x005945B0, slot 127): a mission handler; 450.

int MissionClass::Do_MISSION_CAPTURE()
{
    return 450;
}


// int MissionClass::Do_MISSION_EATEN() (0x005945C0, slot 128): a mission handler; 450.

int MissionClass::Do_MISSION_EATEN()
{
    return 450;
}


// int MissionClass::Do_MISSION_GUARD() (0x005945D0, slot 129): a mission handler; 450.

int MissionClass::Do_MISSION_GUARD()
{
    return 450;
}


// int MissionClass::Do_MISSION_GUARD_AREA() (0x005945E0, slot 130): a mission handler; 450.

int MissionClass::Do_MISSION_GUARD_AREA()
{
    return 450;
}


// int MissionClass::Do_MISSION_HARVEST() (0x005945F0, slot 131): a mission handler; 450.

int MissionClass::Do_MISSION_HARVEST()
{
    return 450;
}


// int MissionClass::Do_MISSION_HUNT() (0x00594600, slot 132): a mission handler; 450.

int MissionClass::Do_MISSION_HUNT()
{
    return 450;
}


// int MissionClass::Do_MISSION_MOVE() (0x00594610, slot 133): a mission handler; 450.

int MissionClass::Do_MISSION_MOVE()
{
    return 450;
}


// int MissionClass::Do_MISSION_RETREAT() (0x00594620, slot 134): a mission handler; 450.

int MissionClass::Do_MISSION_RETREAT()
{
    return 450;
}


// int MissionClass::Do_MISSION_RETURN() (0x00594630, slot 135): a mission handler; 450.

int MissionClass::Do_MISSION_RETURN()
{
    return 450;
}


// int MissionClass::Do_MISSION_STOP() (0x00594640, slot 136): a mission handler; 450.

int MissionClass::Do_MISSION_STOP()
{
    return 450;
}


// int MissionClass::Do_MISSION_UNLOAD() (0x00594650, slot 137): a mission handler; 450.

int MissionClass::Do_MISSION_UNLOAD()
{
    return 450;
}


// int MissionClass::Do_MISSION_ENTER() (0x00594660, slot 138): a mission handler; 450.

int MissionClass::Do_MISSION_ENTER()
{
    return 450;
}


// int MissionClass::Do_MISSION_CONSTRUCTION() (0x00594670, slot 139): a mission handler; 450.

int MissionClass::Do_MISSION_CONSTRUCTION()
{
    return 450;
}


// int MissionClass::Do_MISSION_DECONSTRUCTION() (0x00594680, slot 140): a mission handler; 450.

int MissionClass::Do_MISSION_DECONSTRUCTION()
{
    return 450;
}


// int MissionClass::Do_MISSION_REPAIR() (0x00594690, slot 141): a mission handler; 450.

int MissionClass::Do_MISSION_REPAIR()
{
    return 450;
}


// int MissionClass::Do_MISSION_MISSILE() (0x005946A0, slot 142): a mission handler; 450.

int MissionClass::Do_MISSION_MISSILE()
{
    return 450;
}


// int MissionClass::Do_MISSION_OPEN() (0x005946B0, slot 143): a mission handler; 450.

int MissionClass::Do_MISSION_OPEN()
{
    return 450;
}


// int MissionClass::Do_MISSION_RESCUE() (0x005946C0, slot 144): a mission handler; 450.

int MissionClass::Do_MISSION_RESCUE()
{
    return 450;
}


// int MissionClass::Do_MISSION_PATROL() (0x005946D0, slot 145): a mission handler; 450.

int MissionClass::Do_MISSION_PATROL()
{
    return 450;
}


// int MissionClass::Do_MISSION_PARADROP_APPROACH() (0x005946E0, slot 146): a mission handler; 450.

int MissionClass::Do_MISSION_PARADROP_APPROACH()
{
    return 450;
}


// int MissionClass::Do_MISSION_PARADROP_OVERFLY() (0x005946F0, slot 147): a mission handler; 450.

int MissionClass::Do_MISSION_PARADROP_OVERFLY()
{
    return 450;
}


// int MissionClass::Do_MISSION_WAIT() (0x00594700, slot 148): a mission handler; 450.

int MissionClass::Do_MISSION_WAIT()
{
    return 450;
}


// void MissionClass::Set_Mission(int mission) (0x00594710, slot 118)
// Sets the mission (not over mission 28 to 5), clears the queue and
// restarts the mission timer.

void MissionClass::Set_Mission(int mission)
{
    if (Mission == 28 && mission == 5) {
        return;
    }
    Mission = mission;
    MissionQueue = -1;
    unknown_a4 = false;
    unknown_a8 = 0;
    unknown_ac = Frame;
    Timer = CDTimerClass<FrameTimerClass>(0);
}


// int MissionClass::Get_Mission() (0x00594770, slot 91): the mission, or the queued one.

int MissionClass::Get_Mission()
{
    if (Mission == -1) {
        return MissionQueue;
    }
    return Mission;
}


// void MissionClass::AI() (0x00594790, slot 23)
// ObjectClass's part; then, for a live object (+0x7D) whose mission timer
// has run out and that has strength left, the slot of the current mission
// (123-148, each returning the frames until the next step) restarts the
// timer. Unknown missions (and 3) use slot 123.

void MissionClass::AI()
{
    ObjectClass::AI();
    if (unknown_7d && Timer.Value() == 0 && Strength > 0) {
        switch (Mission) {
        case 0:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_SLEEP());
            break;
        case 23:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_HARMLESS());
            break;
        case 7:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_ENTER());
            break;
        case 18:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_CONSTRUCTION());
            break;
        case 19:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_DECONSTRUCTION());
            break;
        case 1:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_ATTACK());
            break;
        case 4:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_RETREAT());
            break;
        case 10:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_HARVEST());
            break;
        case 11:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_GUARD_AREA());
            break;
        case 12:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_RETURN());
            break;
        case 13:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_STOP());
            break;
        case 14:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_AMBUSH());
            break;
        case 16:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_UNLOAD());
            break;
        case 20:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_REPAIR());
            break;
        case 24:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_OPEN());
            break;
        case 22:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_MISSILE());
            break;
        case 5:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_GUARD());
            break;
        case 8:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_CAPTURE());
            break;
        case 9:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_EATEN());
            break;
        case 2:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_MOVE());
            break;
        case 15:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_HUNT());
            break;
        case 21:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_RESCUE());
            break;
        case 25:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_PATROL());
            break;
        case 26:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_PARADROP_APPROACH());
            break;
        case 27:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_PARADROP_OVERFLY());
            break;
        case 28:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_WAIT());
            break;
        case 6:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_GUARD());
            break;
        case 17:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_CAPTURE());
            break;
        default:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_SLEEP());
            break;
        }
    }
}


// bool MissionClass::Commence() (0x00594C50, slot 117)
// Starts the queued mission; false when nothing is queued.

bool MissionClass::Commence()
{
    if (MissionQueue != -1) {
        Mission = MissionQueue;
        MissionQueue = -1;
        Timer = CDTimerClass<FrameTimerClass>(0);
        unknown_a8 = 0;
        unknown_ac = Frame;
        unknown_a4 = false;
        return true;
    }
    return false;
}


// void MissionClass::QueueMission(int mission, bool commence) (0x00594CB0, slot 116)
// Queues a mission (not over missions 28/5 and 19) and optionally starts it.

void MissionClass::QueueMission(int mission, bool commence)
{
    if (Mission == 28 && mission == 5) {
        return;
    }
    if (Mission == 19) {
        return;
    }
    if (mission != -1 && (Mission != mission || (MissionQueue != mission && MissionQueue != -1))) {
        MissionQueue = mission;
        unknown_a4 = false;
    }
    if (commence && unknown_slot122()) {
        Commence();
    }
}


// void MissionClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination) (0x00594D20, slot 119)
// Overrides the mission, suspending the current (or queued) one.

void MissionClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    if (Mission == 28 && mission == 5) {
        return;
    }
    if (Mission == 19) {
        return;
    }
    if (MissionQueue != -1) {
        SuspendedMission = MissionQueue;
    } else {
        SuspendedMission = Mission;
    }
    Mission = mission;
    unknown_a4 = false;
}


// bool MissionClass::Restore_Mission() (0x00594D80, slot 120): resumes the suspended mission.

bool MissionClass::Restore_Mission()
{
    if (SuspendedMission != -1) {
        Mission = SuspendedMission;
        SuspendedMission = -1;
        unknown_a4 = false;
        return true;
    }
    return false;
}


// bool __fastcall MissionClass::Is_Recruitable_Mission(int mission) (0x00594DB0): the mission table's flag +0x06 (true for -1).

bool __fastcall MissionClass::Is_Recruitable_Mission(int mission)
{
    if (mission == -1) {
        return true;
    }
    return MissionControl[mission].unknown_06;
}


// MissionControlClass::MissionControlClass() (0x00594DD0)

MissionControlClass::MissionControlClass() :
    Mission(-1),
    unknown_04(false),
    unknown_05(false),
    unknown_06(true),
    unknown_07(false),
    unknown_08(true),
    unknown_09(true),
    unknown_10(0.016),
    unknown_18(0.016)
{
}


// char const *MissionControlClass::Name() const (0x00594E10): "<none>" for -1.

char const *MissionControlClass::Name() const
{
    if (Mission == -1) {
        return "<none>";
    }
    return MissionNames[Mission];
}


// bool MissionControlClass::Read_INI(CCINIClass &ini) (0x00594E30)
// Reads the mission's own section (named by Name()); AARate= defaults to Rate=.

bool MissionControlClass::Read_INI(CCINIClass &ini)
{
    if (ini.Is_Present(Name())) {
        unknown_04 = ini.Get_Bool(Name(), "NoThreat", unknown_04);
        unknown_05 = ini.Get_Bool(Name(), "Zombie", unknown_05);
        unknown_06 = ini.Get_Bool(Name(), "Recruitable", unknown_06);
        unknown_07 = ini.Get_Bool(Name(), "Paralyzed", unknown_07);
        unknown_08 = ini.Get_Bool(Name(), "Retaliate", unknown_08);
        unknown_09 = ini.Get_Bool(Name(), "Scatter", unknown_09);
        unknown_10 = ini.Get_Double(Name(), "Rate", unknown_10);
        unknown_18 = ini.Get_Double(Name(), "AARate", 0);
        if (unknown_18 == 0) {
            unknown_18 = unknown_10;
        }
        return true;
    }
    return false;
}


// int __fastcall MissionClass::Mission_From_Name(char const *name) (0x00594FE0): -1 when unknown.

extern "C" int __cdecl _stricmp(char const *left, char const *right);

int __fastcall MissionClass::Mission_From_Name(char const *name)
{
    if (name != 0) {
        for (int mission = 0; mission < 30; mission++) {
            if (_stricmp(MissionNames[mission], name) == 0) {
                return mission;
            }
        }
    }
    return -1;
}


// char const *__fastcall MissionClass::Name_From_Mission(int mission) (0x00595020)

char const *__fastcall MissionClass::Name_From_Mission(int mission)
{
    if (mission != -1) {
        return MissionNames[mission];
    }
    return "<none>";
}


// void MissionClass::Compute_CRC(WWCRCEngine &crc) const (0x00595040, slot 13)

void MissionClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectClass::Compute_CRC(crc);
    crc((long)Mission);
    crc((long)SuspendedMission);
    crc((long)MissionQueue);
    crc((long)unknown_a8);
    crc((long)Timer.Value());
    crc((long)unknown_ac);
}


// MissionControlClass *MissionClass::Control() const (0x005950D0): the current mission's table entry.

MissionControlClass *MissionClass::Control() const
{
    return &MissionControl[Mission];
}


// bool MissionClass::Has_Suspended_Mission() const (0x005950E0, slot 121): a mission is suspended.

bool MissionClass::Has_Suspended_Mission() const
{
    return SuspendedMission != -1;
}


// int MissionClass::Timer_Value() const (0x005950F0): the mission timer's remaining time.

int MissionClass::Timer_Value() const
{
    return Timer.Value();
}


// void ObjectClass::unknown_slot67(Point2D const &point, Rect const &window) (0x00595120): an empty
// inline virtual of shared/object.hpp whose kept copy is Mission.cpp's;
// no source of its own.


// MissionClass scalar deleting destructor (0x00595130): compiler-generated
// from the inline virtual destructor; no source of its own.
