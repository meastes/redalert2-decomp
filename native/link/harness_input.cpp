// Skirmish harness: scripted input support (README "Playing"; --input-script).
// Test scaffolding, never game behaviour (Scout1115, native M13). The host
// pushes scripted mouse and key events through its SDL event path; a point
// "@ID[:n][+dx,dy]" (or "-dx,dy") names the human player's n-th object of type ID (0 the
// first), moved by dx, dy cells, and this file answers its screen pixel
// (Tactical::Coord_To_Pixel 0x006A29D0 of its centre). At the run's last frame
// "@=X,Y" names the centre of cell X,Y (any object there). At the run's last frame
// it logs harness-input: the selected objects and each of the player's
// objects (type ID, mission, cell; '*' when selected).
#define RA2_COORD_AS_CELL   // CoordStruct::As_Cell (shared/coord.hpp)
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/building_class.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/tactical.hpp"
#include <stdlib.h>
#include <stdio.h>

class TechnoClass;
extern DynamicVectorClass<TechnoClass *> Technos;   // 0x00A40C20 (shared/logic.hpp)

extern "C" {
int ra2port_skirmish_frames(void);
void ra2port_progress(const char *milestone, const char *detail);
void ra2port_set_point_resolver(int (*resolver)(const char *spec, int *x, int *y));
}

namespace {
void Append(char *to, char const *text, int size)
{
    int n = 0;
    while (to[n]) ++n;
    while (*text && n + 1 < size) to[n++] = *text++;
    to[n] = 0;
}
void Append_Int(char *to, int value, int size)
{
    char digits[16];
    int n = 0;
    unsigned v = value < 0 ? -value : value;
    do { digits[n++] = '0' + v % 10; v /= 10; } while (v);
    char text[20];
    int k = 0;
    if (value < 0) text[k++] = '-';
    while (n) text[k++] = digits[--n];
    text[k] = 0;
    Append(to, text, size);
}
bool Same_Name(char const *a, char const *b, int length)
{
    for (int i = 0; i < length; ++i, ++a, ++b) {
        char x = *a >= 'a' && *a <= 'z' ? *a - 32 : *a;
        char y = *b >= 'a' && *b <= 'z' ? *b - 32 : *b;
        if (x != y) return false;
        if (x == 0) return true;
    }
    return *a == 0;
}
int Read_Int(char const *&p)
{
    int sign = 1, value = 0;
    if (*p == '-') { sign = -1; ++p; } else if (*p == '+') ++p;
    while (*p >= '0' && *p <= '9') value = value * 10 + (*p++ - '0');
    return sign * value;
}
char const *Type_ID(TechnoClass *techno)
{
    ObjectTypeClass *type = techno->Class_Of();
    return type ? type->ID.Buffer : "?";
}
bool Is_Selected(ObjectClass *object)
{
    for (int i = 0; i < unknown_objects_a40c60.Count(); ++i) {
        if (unknown_objects_a40c60[i] == object) return true;
    }
    return false;
}

int Resolve(char const *spec, int *x, int *y)
{
    HouseClass *player = unknown_player_a35db4;
    if (player == 0 || unknown_tactical_83996c == 0) return 0;
    if (spec[0] == '=') {
        // "@=X,Y": the centre of cell X,Y (Scout1153, M19: a building of another house, a bridge hut).
        char const *p = spec + 1;
        Cell cell;
        cell.X = (short)Read_Int(p);
        if (*p == ',') ++p;
        cell.Y = (short)Read_Int(p);
        CoordStruct coord = Map[cell].Center_Coord();
        Point2D pixel;
        if (!unknown_tactical_83996c->Coord_To_Pixel(coord, pixel)) return 0;
        *x = pixel.X;
        *y = pixel.Y;
        return 1;
    }
    int length = 0;
    while (spec[length] && spec[length] != ':' && spec[length] != '+' && spec[length] != '-') ++length;
    char const *p = spec + length;
    int index = 0, dx = 0, dy = 0;
    if (*p == ':') { ++p; index = Read_Int(p); }
    // "+dx,dy" or "-dx,dy" (the sign is dx's own; Scout1125: "@GACNST-4,0" did not parse).
    if (*p == '+' || *p == '-') { dx = Read_Int(p); if (*p == ',') { ++p; dy = Read_Int(p); } }
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != player || !Same_Name(Type_ID(techno), spec, length)) continue;
        if (index-- > 0) continue;
        CoordStruct coord = techno->Center_Coord();
        coord.X += dx * 256;
        coord.Y += dy * 256;
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        *x = pixel.X;
        *y = pixel.Y;
        return 1;
    }
    return 0;
}

int const registered = (ra2port_set_point_resolver(Resolve), 0);
}  // namespace

int Native_Session_Message_Count();   // harness_event_test.cpp
// Called by the frame seam (harness_skirmish.cpp) after each Main_Loop frame.
void Native_Harness_Input(int frame)
{
    // RA2_NATIVE_INPUT_LOG=<n>: also every n frames (harness-input-<frame>).
    static int every = -1;
    if (every < 0) {
        char const *text = getenv("RA2_NATIVE_INPUT_LOG");
        every = text ? atoi(text) : 0;
    }
    bool last = frame == ra2port_skirmish_frames();
    if ((!last && (every <= 0 || frame % every != 0)) || unknown_player_a35db4 == 0) return;
    {
        // The view position too (Scout1231: scroll-rate measurements; stderr, so RA2.app runs log it).
        Point2D view = unknown_tactical_83996c ? unknown_tactical_83996c->Get_Tactical_Position() : Point2D(0, 0);
        fprintf(stderr, "harness-view %d @%d,%d msgs %d\n", frame, view.X, view.Y, Native_Session_Message_Count());
    }
    static char names[64][24];
    static int named = 0;
    char const *milestone = "harness-input";
    if (!last) {
        if (named >= 64) return;
        char *name = names[named++];
        name[0] = 0;
        Append(name, "harness-input-", 24);
        Append_Int(name, frame, 24);
        milestone = name;
    }
    char detail[600] = "selected ";
    Append_Int(detail, unknown_objects_a40c60.Count(), sizeof detail);
    Append(detail, ";", sizeof detail);
    for (int i = 0; i < Technos.Count(); ++i) {
        TechnoClass *techno = Technos[i];
        if (techno == 0 || techno->House != unknown_player_a35db4) continue;
        Cell cell = techno->Center_Coord().As_Cell();
        Append(detail, " ", sizeof detail);
        Append(detail, Type_ID(techno), sizeof detail);
        if (Is_Selected(techno)) Append(detail, "*", sizeof detail);
        Append(detail, ":m", sizeof detail);
        Append_Int(detail, techno->Mission, sizeof detail);
        Append(detail, "@", sizeof detail);
        Append_Int(detail, cell.X, sizeof detail);
        Append(detail, ",", sizeof detail);
        Append_Int(detail, cell.Y, sizeof detail);
    }
    ra2port_progress(milestone, detail);
}
