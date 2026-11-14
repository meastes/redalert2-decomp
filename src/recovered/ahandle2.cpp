// ahandle2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ahandle2.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_AS_CELL
#define VECTOR_IMPLEMENT_ALL
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/vector_class.hpp"
#include "shared/foot.hpp"
#include "shared/rect.hpp"
#define Tracker_Min(a,b) (((a)<(b)) ? (a) : (b))
#define Tracker_Max(a,b) (((a)>(b)) ? (a) : (b))
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/cell.hpp"


// TibSun Coord::As_Cell for the tracker unit: Unknown_004128A0 calls the out-of-line 0x00413770 copy.
// Header-static dynamic initializers of the unit at 0x004123E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004123E0-0x004125C6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00412410, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412430, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412450, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412470, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412490, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x004124B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x004124F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412520, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412550, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412580, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x00412590, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// Dynamic initializer (0x004125B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004123e0.cpp; no source of its own.


// The tracker global and its genuine vector lifetime.
// Full extent is independently adjacent to unknown_geometry_angle45 at83C438.
// Clear each airborne bin and its working list, preserving capacities.
void AircraftTrackerClass::Clear_00413540()
{
    for (int i=0;i<400;++i) {
        int capacity=Bins[i].Length();
        Bins[i].Clear();
        Bins[i].Resize(capacity,0);
    }
    int capacity=Working.Length();
    Working.Clear();
    Working.Resize(capacity,0);
}

inline AircraftTrackerClass::~AircraftTrackerClass()
{
    Clear_00413540();
}
AircraftTrackerClass unknown_tracker_839ea0;

// Original-evidenced vector vtables, their slots, constructor and global lifetime.


// Compiler-generated global destructor of unknown_tracker_839ea0.
// Its implementation and object definition are in UnknownTracker839EA0_Global_Initialize_004125d0.cpp.


// Shift a bin index by an offset, clamping each axis to the 20 by 20 grid
// (the column and row locals are clamped in place).
int AircraftTrackerClass::Shift_Bin_004127a0(int index,Point2D offset)
{
    int x=index%20;
    int y=index/20;
    x=Tracker_Min(19,Tracker_Max(0,x+offset.X));
    y=Tracker_Min(19,Tracker_Max(0,y+offset.Y));
    return x+y*20;
}


// Map a cell to one of the tracker's twenty by twenty spatial bins.
int AircraftTrackerClass::Region_Index_00412820(Cell cell)
{
    int x=cell.X<0 ? 0 : cell.X;
    int y=cell.Y<0 ? 0 : cell.Y;
    int column=19<x/(Map.unknown_128/20) ? 19 : x/(Map.unknown_128/20);
    y/=Map.unknown_12c/20;
    y=y>19 ? 19 : y;
    return column+y*20;
}


// Gathers the objects of the bins within radius cells of the cell's bin into Working: the
// cell's own bin, the bins walked back to it from the four points radius cells away, the
// diagonal bins up to the shorter of each pair of arm lengths and, when both arms are longer
// than one bin, the two bins beside the first diagonal.
// The out-of-line Append_Bin (0x00412720) and As_Cell (0x00413770) copies are accepted in the
// neighbouring populations; this unit's Append_Bin definition is the one Unknown_004128A0 inlines.
void AircraftTrackerClass::Append_Bin_00412720(int index)
{
    int count=Bins[index].Count();
    for (int i=0;i<count;++i) Working.Add(Bins[index].Fetch(i));
}

inline Cell Object_Cell(CellClass *cell) { return cell->Center_Coord().As_Cell(); }

void AircraftTrackerClass::Unknown_004128A0(CellClass *cell, int radius)
{
    Cell center = Object_Cell(cell);
    int index = Region_Index_00412820(center);
    Append_Bin_00412720(index);

    int up = 0;
    int down = 0;
    int right = 0;
    int left = 0;

    Cell edge = center;
    edge.X += radius;
    int bin = Region_Index_00412820(edge);
    while (bin != index) {
        right++;
        Append_Bin_00412720(bin);
        bin = Shift_Bin_004127a0(bin, Point2D(-1, 0));
    }

    edge = center;
    edge.Y -= radius;
    bin = Region_Index_00412820(edge);
    while (bin != index) {
        up++;
        Append_Bin_00412720(bin);
        bin = Shift_Bin_004127a0(bin, Point2D(0, 1));
    }

    edge = center;
    edge.Y += radius;
    bin = Region_Index_00412820(edge);
    while (bin != index) {
        down++;
        Append_Bin_00412720(bin);
        bin = Shift_Bin_004127a0(bin, Point2D(0, -1));
    }

    edge = center;
    edge.X -= radius;
    bin = Region_Index_00412820(edge);
    while (bin != index) {
        left++;
        Append_Bin_00412720(bin);
        bin = Shift_Bin_004127a0(bin, Point2D(1, 0));
    }

    if (up && left) {
        for (int i = Tracker_Min(up, left), bin = index; i > 0; i--) {
            bin = Shift_Bin_004127a0(bin, Point2D(-1, -1));
            Append_Bin_00412720(bin);
        }
        if (up >= 2 && left >= 2) {
            Append_Bin_00412720(Shift_Bin_004127a0(index, Point2D(-2, -1)));
            Append_Bin_00412720(Shift_Bin_004127a0(index, Point2D(-1, -2)));
        }
    }

    if (down && left) {
        for (int i = Tracker_Min(down, left), bin = index; i > 0; i--) {
            bin = Shift_Bin_004127a0(bin, Point2D(-1, 1));
            Append_Bin_00412720(bin);
        }
        if (down >= 2 && left >= 2) {
            int side;
            side = Shift_Bin_004127a0(index, Point2D(-2, 1));
            Append_Bin_00412720(side);
            side = Shift_Bin_004127a0(index, Point2D(-1, 2));
            Append_Bin_00412720(side);
        }
    }

    if (up && right) {
        for (int i = Tracker_Min(up, right), bin = index; i > 0; i--) {
            bin = Shift_Bin_004127a0(bin, Point2D(1, -1));
            Append_Bin_00412720(bin);
        }
        if (up >= 2 && right >= 2) {
            int side;
            side = Shift_Bin_004127a0(index, Point2D(2, -1));
            Append_Bin_00412720(side);
            side = Shift_Bin_004127a0(index, Point2D(1, -2));
            Append_Bin_00412720(side);
        }
    }

    if (down && right) {
        for (int i = Tracker_Min(down, right), bin = index; i > 0; i--) {
            bin = Shift_Bin_004127a0(bin, Point2D(1, 1));
            Append_Bin_00412720(bin);
        }
        if (down >= 2 && right >= 2) {
            int side;
            side = Shift_Bin_004127a0(index, Point2D(2, 1));
            Append_Bin_00412720(side);
            side = Shift_Bin_004127a0(index, Point2D(1, 2));
            Append_Bin_00412720(side);
        }
    }
}


class Leaf_4131d0 { public: void get(unsigned int unused_argument); };
void Leaf_4131d0::get(unsigned int unused_argument) { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; }


// Register an arriving foot in its current spatial bin.
void AircraftTrackerClass::Unknown_004131E0(FootClass *foot)
{
    Cell cell=foot->unknown_slot104();
    foot->unknown_slot179(cell);
    Bins[Region_Index_00412820(cell)].Add(foot);
}


// Whether another object occupies the cell beneath this foot.
bool AircraftTrackerClass::Unknown_004132E0(FootClass *foot)
{
    void *occupant=static_cast<CellClass *>(foot->Get_Cell_Ptr())->unknown_e0;
    if (occupant && occupant!=foot) return true;
    return false;
}


// Remove a departing foot from the spatial bin recorded on it.
void AircraftTrackerClass::Unknown_00413310(TechnoClass *foot)
{
    Cell cell=foot->unknown_slot178();
    foot->unknown_slot179(unknown_geometry_k);
    Bins[Region_Index_00412820(cell)].Delete(static_cast<TechnoClass *>(foot));
}


// Return and remove the first airborne object selected by the tracker.
TechnoClass *AircraftTrackerClass::Fetch_Next_004134e0()
{
    if (Working.Count()==0) return 0;
    TechnoClass *object=Working[0];
    Working.Delete(object);
    return object;
}


// Serialize the airborne spatial bins; the temporary query list is not stored.
long AircraftTrackerClass::Save_00413590(IStream *stream)
{
    for (int i=0;i<400;++i) {
        int count=Bins[i].Count();
        long result=stream->Write(&count,sizeof(count),0);
        if (result<0) return result;
        for (int j=0;j<count;++j) {
            result=stream->Write(&Bins[i][j],sizeof(Bins[i][j]),0);
            if (result<0) return result;
        }
    }
    return 0;
}


// 0x00412710: function not yet identified
//
// Body: 8 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_00412710(undefined4 param_1)
// Callees with accepted source:
//   FUN_004068f0 -> src/recovered/004068f0.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00412710 {

extern void  FUN_004068f0(int);

void __fastcall FUN_00412710(unsigned int argument_value)
{
    FUN_004068f0((int)(argument_value));
    return;
}

}  // namespace seed_00412710


// 0x004132d0: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_004132d0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004132d0 {

unsigned int  FUN_004132d0(void)
{
    return 0x2ee;
}

}  // namespace seed_004132d0


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<TechnoClass *>::Add (0x004137B0): emitted by this unit's use of its list.
