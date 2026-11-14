// revent.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/revent.json carries the addresses and the rows that
// stay in their own files.


#define RA2_RADAR_ADD_THREADED
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define RA2_RECT_POINT_INLINE
#include "shared/fast_math.hpp"
#define RA2_POINT_LENGTH
#include "shared/radar_event_class.hpp"
#include "shared/radar.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#define VECTOR_IMPLEMENT_ALL
#include "shared/crt_math.hpp"
#include "shared/dsurface.hpp"
#include "shared/rect_intersect.hpp"
#include "shared/matrix3d.hpp"
#include "shared/com_persist.hpp"


class Leaf_63b5a0 { public: void get(); };
void Leaf_63b5a0::get() {  }


// bool __fastcall Submit_Radar_Event(RadarEventType event, Cell cell) -- 0x0063B5B0 (TS revent.cpp)
// Starts a radar event at a cell unless the type is suppressible and an event of the same type is
// already closer than its suppression distance.

bool __fastcall Submit_Radar_Event(RadarEventType event, Cell cell)
{
    if (Can_Suppress_Radar_Event(event)) {
        for (int index = 0; index < unknown_radar_events_ab67e8.Count(); index++) {
            if (unknown_radar_events_ab67e8[index]->Type == event) {
                if (cell.Distance_To(unknown_radar_events_ab67e8[index]->Location) < unknown_radar_events_ab67e8[index]->Unknown_0063C630()) {
                    return false;
                }
            }
        }
    }
    new RadarEventClass(event, cell);
    return true;
}


// void __fastcall Plot_Radar_Event_Point(Point2D const &point) -- 0x0063B6A0 (TS revent.cpp)
// The plotter RadarEventClass::Plot passes to Plot_Line: marks one radar pixel (RadarClass 0x00632620,
// TS Radar_Pixel).

void __fastcall Plot_Radar_Event_Point(Point2D const &point)
{
    Map.Radar_Pixel(point);
}


// bool __fastcall Can_Suppress_Radar_Event(RadarEventType event) -- 0x0063B6B0 (TS revent.cpp)
// Whether events of this type are dropped near an event of the same type (table 0x007A8D58 +0x0C).

bool __fastcall Can_Suppress_Radar_Event(RadarEventType event)
{
    return unknown_radar_event_types_7a8d58[event].CanSuppress;
}


// Initializes a radar ping, its cell history and its active-list entry.
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

RadarEventClass::RadarEventClass(RadarEventType type, Cell cell)
    : Type(type), RotationAngle(0.7853981633974483f),
      RotationSpeed(Rules->RadarEventRotationSpeed), ColorFactor(0),
      ColorSpeed(Rules->RadarEventColorSpeed), Location(cell),
      DurationTimer(0), VisibilityTimer(0), IsRotating(true), IsVisible(true)
{
    Offset = Map.Cell_To_Radar_Pixel(cell).Top_Left() - Map.unknown_1498.Top_Left();
    Radius = MAX(Offset.X, MAX(Offset.Y, MAX(Map.unknown_1498.Width - Offset.X, Map.unknown_1498.Height - Offset.Y)));
    RadarEventClass_unknown_remember_cell(cell);
    unknown_radar_events_ab67e8.Add(this);
}


// RadarEventClass::~RadarEventClass() -- 0x0063B840 (TS revent.cpp)
// Takes the event out of the radar event list.

RadarEventClass::~RadarEventClass()
{
    RadarEventClass *self = this;
    unknown_radar_events_ab67e8.Delete(self);
}


// void RadarEventClass_unknown_all_63b910() -- 0x0063B910
// Purpose: runs 0x0063B940 on every radar event of the list at 0x00AB67E8
// (TS Process_Radar_Events). Names are hypotheses.

void RadarEventClass_unknown_all_63b910()
{
    for (int index = 0; index < unknown_radar_events_ab67e8.Count(); index++) {
        unknown_radar_events_ab67e8[index]->Unknown_0063B940();
    }
}


// Per-frame update: shrinking radar ping, rotation and pulsing color.
#ifndef MAX
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

void RadarEventClass::Unknown_0063B940()
{
    if (IsVisible) {
        if (!IsRotating && VisibilityTimer == 0) IsVisible = false;
        Plot();
        Radius = MAX(Radius - Rules->RadarEventSpeed, (float)Rules->RadarEventMinRadius);
        float normalized = RotationAngle + (3.14159265358979323846 / 4) - (int)((RotationAngle + (3.14159265358979323846 / 4)) * (2 / 3.14159265358979323846)) * 3.14159265358979323846 / 2;
        if (IsRotating) {
            if (fabs(Radius - Rules->RadarEventMinRadius) < 0.01) {
                if (normalized < RotationSpeed) {
                    RotationAngle = normalized + RotationAngle;
                    IsRotating = false;
                    VisibilityTimer = Unknown_0063C610();
                    DurationTimer = Unknown_0063C620();
                } else {
                    RotationAngle += RotationSpeed;
                    RotationSpeed = MAX(Rules->RadarEventRotationSpeed * (1.0f / 3.0f), RotationSpeed - Rules->RadarEventRotationSpeed * 0.02f);
                }
            } else {
                RotationAngle = RotationSpeed + RotationAngle;
            }
        }
        if (RotationAngle > 6.28318530717958647692) RotationAngle -= 6.28318530717958647692;
        float next_color = ColorSpeed;
        next_color += ColorFactor;
        ColorFactor = next_color;
        if (next_color < 0.0f && ColorSpeed < 0.0f) {
            ColorSpeed = -ColorSpeed;
            ColorFactor = 0.0f;
        } else if (next_color > 1.0f && ColorSpeed > 0.0f) {
            ColorSpeed = -ColorSpeed;
            ColorFactor = 1.0f;
        }
    }
}


// void RadarEventClass::Draw_Events() -- 0x0063BB40 (TS revent.cpp name)
// Draws every radar event whose visibility timer is still running or that
// is rotating.

void RadarEventClass::Draw_Events()
{
    for (int index = 0; index < unknown_radar_events_ab67e8.Count(); index++) {
        RadarEventClass *event = unknown_radar_events_ab67e8[index];
        if (event->VisibilityTimer > 0 || event->IsRotating) {
            event->Draw();
        }
    }
}


// void RadarEventClass::Draw() -- 0x0063BB90 (TS revent.cpp)
// Draws the event's square on the radar surface as gradient lines between its type's two colours
// (nothing for a type whose bright colour is black) and, with the radar showing the tactical map and
// active, adds the square to the radar area to redraw.
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

void RadarEventClass::Draw()
{
    RGBClass color1 = Get_Max_Color();
    RGBClass color2 = Get_Min_Color();
    if (color1 == RGBClass(0, 0, 0)) {
        return;
    }

    Point2D event_rect[4];
    int i;

    Get_Event_Rect(event_rect);
    for (i = 0; i < 4; i++) {
        event_rect[i] += Offset;
    }
    float color_factor = ColorFactor;
    int dy = abs(event_rect[0].Y - event_rect[1].Y);
    int dx = abs(event_rect[0].X - event_rect[1].X);
    int maxdist = MAX(dx, dy);
    float rate = (Radius * 2) * 0.7071067811865475 / (float)maxdist * ColorSpeed;
    for (i = 0; i < 4; i++) {
        ((DSurface *)Map.unknown_1218)->Draw_Gradient_Line(Map.unknown_1218->Get_Rect(), event_rect[i], event_rect[(i + 1) % 4], color1, color2, rate, color_factor);
    }
    for (i = 0; i < 4; i++) {
        Clip_Line(event_rect[i], event_rect[(i + 1) % 4], Map.unknown_1218->Get_Rect());
    }
    if (Map.unknown_14ac == 1 && Map.unknown_14a8 == 1) {
        for (i = 0; i < 4; i++) {
            Map.unknown_1208 = Union(Map.unknown_1208, Rect(event_rect[i].X, event_rect[i].Y, 1, 1) + Map.unknown_1498.Top_Left()).Intersect(Map.unknown_1498);
        }
    }
}


// void RadarEventClass::Remove_Finished() -- 0x0063BEF0 (TS revent.cpp name)
// Deletes every radar event whose duration timer has run out and that is not
// rotating, last first; the inlined destructor takes each out of the list.

void RadarEventClass::Remove_Finished()
{
    for (int index = unknown_radar_events_ab67e8.Count() - 1; index >= 0; index--) {
        if (unknown_radar_events_ab67e8[index]->DurationTimer == 0 && !unknown_radar_events_ab67e8[index]->IsRotating) {
            delete unknown_radar_events_ab67e8[index];
        }
    }
}


// bool __fastcall Try_Suppress_Radar_Event(RadarEventType event, Cell cell) -- 0x0063BFA0 (TS revent.cpp)
// Whether an event of this type at the cell would be suppressed: the type is suppressible and an event
// of the same type is closer than its suppression distance. (TS passes an unused int between the two.)

bool __fastcall Try_Suppress_Radar_Event(RadarEventType event, Cell cell)
{
    if (Can_Suppress_Radar_Event(event)) {
        for (int index = 0; index < unknown_radar_events_ab67e8.Count(); index++) {
            if (unknown_radar_events_ab67e8[index]->Type == event) {
                if (cell.Distance_To(unknown_radar_events_ab67e8[index]->Location) < unknown_radar_events_ab67e8[index]->Unknown_0063C630()) {
                    return true;
                }
            }
        }
    }
    return false;
}


// void RadarEventClass::Plot() -- 0x0063C080 (TS revent.cpp)
// Marks the radar pixels along the event's square (Plot_Radar_Event_Point for each) and, with the
// radar showing the tactical map and active, adds the square to the radar area to redraw.

void RadarEventClass::Plot()
{
    Point2D event_rect[4];
    int i;

    Get_Event_Rect(event_rect);
    for (i = 0; i < 4; i++) {
        event_rect[i] += Offset;
    }
    for (i = 0; i < 4; i++) {
        Map.unknown_1218->Plot_Line(Map.unknown_1218->Get_Rect(), event_rect[i], event_rect[(i + 1) % 4], Plot_Radar_Event_Point);
    }
    for (i = 0; i < 4; i++) {
        Clip_Line(event_rect[i], event_rect[(i + 1) % 4], Map.unknown_1218->Get_Rect());
    }
    if (Map.unknown_14ac == 1 && Map.unknown_14a8 == 1) {
        for (i = 0; i < 4; i++) {
            Map.unknown_1208 = Union(Map.unknown_1208, Rect(event_rect[i].X, event_rect[i].Y, 1, 1) + Map.unknown_1498.Top_Left()).Intersect(Map.unknown_1498);
        }
    }
}


// void RadarEventClass::Get_Event_Rect(Point2D (&event_rect)[4]) const -- 0x0063C270 (TS revent.cpp)
// The four corners of the event's square: the radius rotated by the rotation angle, then turned by
// quarter turns.

void RadarEventClass::Get_Event_Rect(Point2D (&event_rect)[4]) const
{
    Matrix3D matrix;
    matrix.Make_Identity();
    matrix.Rotate_Z(RotationAngle);
    Vector3 vector(Radius, 0, 0);
    vector = matrix * vector;
    int x_radius = vector.X;
    int y_radius = vector.Y;
    event_rect[0] = Point2D(x_radius, y_radius);
    event_rect[1] = Point2D(-y_radius, x_radius);
    event_rect[2] = Point2D(-x_radius, -y_radius);
    event_rect[3] = Point2D(y_radius, -x_radius);
}


// bool __fastcall RadarEventClass_unknown_load_all(IStream *stream) -- 0x0063C380 (TS RadarEventClass::Load)
// Deletes every radar event, then reads the count and each event's 0x40 bytes into new events, then
// the word at 0x00AB67B8 (see RadarEventClass_unknown_save_all 0x0063C310). A failed event read still
// returns true (TS returns S_OK there).

bool __fastcall RadarEventClass_unknown_load_all(IStream *stream)
{
    for (int i = unknown_radar_events_ab67e8.Count() - 1; i >= 0; i--) {
        delete unknown_radar_events_ab67e8[i];
        unknown_radar_events_ab67e8.Delete(i);
    }
    int count;
    if (FAILED(stream->Read(&count, sizeof(count), 0))) {
        return false;
    }
    for (int index = 0; index < count; index++) {
        RadarEventClass *event = new RadarEventClass(RADAREVENT_NONE, Cell(0, 0));
        HRESULT hr = stream->Read(event, sizeof(*event), 0);
        if (FAILED(hr)) {
            return S_OK;
        }
    }
    if (SUCCEEDED(stream->Read(&unknown_radar_event_word_ab67b8, sizeof(unknown_radar_event_word_ab67b8), 0))) {
        return true;
    }
    return false;
}


// RGBClass RadarEventClass::Get_Max_Color() const -- 0x0063C490 (TS revent.cpp)
// The bright end of the event's colour swing, by event type.

RGBClass RadarEventClass::Get_Max_Color() const
{
    switch (Type) {
        case 0:
        case 3:
        case 4:
            return RGBClass(255, 0, 255);
        case 1:
        case 2:
        case 11:
        case 12:
            return RGBClass(255, 255, 0);
        case 5:
            return RGBClass(0, 255, 255);
        default:
            return RGBClass(0, 0, 0);
    }
}


// RGBClass RadarEventClass::Get_Min_Color() const -- 0x0063C550 (TS revent.cpp)
// The dark end of the event's colour swing, by event type.

RGBClass RadarEventClass::Get_Min_Color() const
{
    switch (Type) {
        case 0:
        case 3:
        case 4:
            return RGBClass(128, 0, 128);
        case 1:
        case 2:
        case 11:
        case 12:
            return RGBClass(128, 128, 0);
        case 5:
            return RGBClass(0, 128, 128);
        default:
            return RGBClass(0, 0, 0);
    }
}


// int RadarEventClass::Unknown_0063C610() const -- 0x0063C610 (TS Get_Visibility_Duration)
// The visibility duration of the event's type (table 0x007A8D58 +0x04).

int RadarEventClass::Unknown_0063C610() const
{
    return unknown_radar_event_types_7a8d58[Type].VisibilityDuration;
}


// int RadarEventClass::Unknown_0063C620() const -- 0x0063C620 (TS Get_Duration)
// The duration of the event's type (table 0x007A8D58 +0x08).

int RadarEventClass::Unknown_0063C620() const
{
    return unknown_radar_event_types_7a8d58[Type].Duration;
}


// int RadarEventClass::Unknown_0063C630() const -- 0x0063C630 (TS Get_Suppression_Distance)
// The suppression distance of the event's type (table 0x007A8D58 +0x00).

int RadarEventClass::Unknown_0063C630() const
{
    return unknown_radar_event_types_7a8d58[Type].SuppressionDistance;
}


// void __fastcall RadarEventClass_unknown_remember_cell(Cell const &cell) -- 0x0063C6C0
// Appends a cell to the eight-entry ring of radar event cells (0x00AB6788): advances the newest entry
// (0x00AB67C8) modulo 8, stores the cell there and makes it the current entry (0x00AB6818). RA2's
// replacement for TS's LastRadarEventCell; names are hypotheses.

void __fastcall RadarEventClass_unknown_remember_cell(Cell const &cell)
{
    unknown_event_last_ab67c8 = (unknown_event_last_ab67c8 + 1) % 8;
    unknown_event_cells_ab6788[unknown_event_last_ab67c8] = cell;
    unknown_event_current_ab6818 = unknown_event_last_ab67c8;
}


// Cell __fastcall RadarEventClass_unknown_newest_cell() -- 0x0063C6F0
// The newest cell of the radar event ring (0x00AB6788), made the current entry (0x00AB6818); the empty
// cell (0x00AB67A8) when the ring is empty. RA2 addition; names are hypotheses.

Cell __fastcall RadarEventClass_unknown_newest_cell()
{
    if (unknown_event_last_ab67c8 == -1) {
        return unknown_event_empty_cell_ab67a8;
    }
    unknown_event_current_ab6818 = unknown_event_last_ab67c8;
    return unknown_event_cells_ab6788[unknown_event_last_ab67c8];
}


// Cell __fastcall RadarEventClass_unknown_previous_cell() -- 0x0063C720
// Steps the current entry of the radar event ring back by one (wrapping at eight) and returns its cell;
// when that entry is empty it starts again at the newest. The empty cell (0x00AB67A8) when the ring is
// empty. RA2 addition; names are hypotheses.

Cell __fastcall RadarEventClass_unknown_previous_cell()
{
    if (unknown_event_last_ab67c8 == -1) {
        return unknown_event_empty_cell_ab67a8;
    }
    int index = unknown_event_current_ab6818 - 1;
    if (index < 0) {
        index += 8;
    }
    if (unknown_event_cells_ab6788[index] == unknown_event_empty_cell_ab67a8) {
        return RadarEventClass_unknown_newest_cell();
    }
    unknown_event_current_ab6818 = index;
    return unknown_event_cells_ab6788[index];
}


extern "C" unsigned char vtable_63cac0[]; extern "C" void __cdecl release_buffer_63cac0(void*); extern "C" void __cdecl release_object_63cac0(void*);
class Leaf_63cac0 { public: void* get(unsigned int flags); };
void* Leaf_63cac0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_63cac0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_63cac0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_63cac0(this); return this; }


extern "C" unsigned char vtable_63cb10[]; extern "C" void __cdecl release_buffer_63cb10(void*); extern "C" void __cdecl release_object_63cb10(void*);
class Leaf_63cb10 { public: void* get(unsigned int flags); };
void* Leaf_63cb10::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_63cb10; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_63cb10((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_63cb10(this); return this; }
