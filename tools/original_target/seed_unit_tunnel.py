"""Typed scratch rendering of the frozen Unit Tunnel_AI export."""
import hashlib
from seed_guarded_cfg import guarded_graph, require

ENTRY=0x6faa30
RAW_SHA256='97352825157ef88d9d623e97fe3a1d25601a6a3676e26490e8b99b42a5a5b277'
BODY_SHA256='b861bc01ffc5bebcde71ef7c8e7433767e53757a4b3c50927762edda6dc9885c'
PROVIDERS={4306480: ('??0UnknownVector3@@QAE@HHH@Z', 12), 4306816: ('?Length@UnknownVector3@@QBEHXZ', 0), 4341488: ('??0UnknownFacingDir@@QAE@ABUCoordStruct@@0@Z', 8), 4409584: ('??GCoordStruct@@QBE?AU0@ABU0@@Z', 8), 4676960: ('?unknown_475d60@CellClass@@QBE?AUCoordStruct@@XZ', 4), 4694320: ('?Cell_Tube_0047a130@@YIPAVTubeClass@@PBVCellClass@@@Z', 0), 4950800: ('?Set_Current@FacingClass@@QAE_NABUUnknownFacingDir@@@Z', 4), 4957312: ('?Fast_Sin@@YANN@Z', 0), 4957392: ('?Fast_Cos@@YANN@Z', 0), 5537792: ('??AMapClass@@QAEAAVCellClass@@ABUCoordStruct@@@Z', 4), 5612160: ('?Get_Height_GL@MapClass@@QAEHABUCoordStruct@@@Z', 4)}

def verify_graph(graph, receivers):
    require(graph['body_sha256']==BODY_SHA256, 'Frozen Unit tunnel body differs')
    for target,(symbol,cleanup) in PROVIDERS.items():
        require(receivers.accepted(target)['symbol']==symbol and receivers.cleanup(target)==cleanup,
                'Unit tunnel provider ABI differs')

def verify_native(inventory, repository):
    import catalog_io
    from seed_capture_receivers import Receivers
    from verify_manifest import Audit
    audit=Audit(repository, 'private/original-target-20261020/inputs/game.exe')
    graph=guarded_graph(audit, inventory, ENTRY)
    catalog=catalog_io.load(catalog_io.locate(repository), references=False)
    receivers=Receivers(repository,catalog,audit,inventory,graph,None)
    verify_graph(graph,receivers)

def optional_tunnel_seed(raw,inventory,repository):
    if hashlib.sha256(raw.encode()).hexdigest()!=RAW_SHA256:
        return None
    try:
        verify_native(inventory,repository)
    except (ValueError,KeyError,TypeError,OSError):
        return None
    return CANDIDATE

CANDIDATE = """#define RA2_COORD_TPOINT3D
#define RA2_POINT3D_TIBSUN
#define RA2_COORD_FROM_CELL_INLINE
// @ref __real@8@4007b504f333f9de6800=007a3a20
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.
// Copyright 2026 OpenTS contributors
// Provisional typed seed: inline helper copies require complete-unit verification before acceptance.
// From TibSun code/unit.cpp Tunnel_AI, modified for the supplied RA2 executable.
// EA's GPL Section 7 terms and warranty disclaimers apply; see LICENSE.md.
#include "shared/unit.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/locomotion.hpp"
#include "shared/fast_math.hpp"
extern Point2D unknown_tunnel_offsets_851ac0[8];
// @ref ?unknown_tunnel_offsets_851ac0@@3PAUPoint2D@@A=00851ac0
extern TubeClass *__fastcall Cell_Tube_0047a130(CellClass const *cell);
// @ref ?Cell_Tube_0047a130@@YIPAVTubeClass@@PBVCellClass@@@Z=0047a130
// The eight facing offsets read by the original; Z is unchanged.
static inline CoordStruct Tunnel_Adjacent_Coord(CoordStruct const &coord,int facing)
{
    Point2D const &step=unknown_tunnel_offsets_851ac0[facing&7];
    return CoordStruct(coord.X+step.X,coord.Y+step.Y,coord.Z);
}
// TibSun inline.h Move_Coord: keep the height while advancing along a heading.
static inline CoordStruct Tunnel_Move_Coord(CoordStruct const &start,UnknownFacingDir const &dir,int distance)
{
    double radians=((short)dir.Raw-0x3fff)*-9.5876725168303271e-05;
    return UnknownCoord434820((int)(start.X+Fast_Cos(radians)*distance),(int)(start.Y-Fast_Sin(radians)*distance),start.Z);
}
void UnitClass::Unknown_006FAA30()
{
    CoordStruct coord;
    bool finished=unknown_TubeClass_list_008661c8[unknown_574]->Directions[unknown_575]==-1;
    if(!finished) {
        int distance=Distance(unknown_45c);
        TubeClass *tube=unknown_TubeClass_list_008661c8[unknown_574];
        UnknownFacingDir direction_temp;
        UnknownFacingDir direction=direction_temp.Direction(Center_Coord(),unknown_45c);
        int speed=(int)(Type->Speed*1.5);
        int height_step;
        {
            Cell exit_cell=tube->ExitCell;
            CoordStruct exit(exit_cell);
            Cell enter_cell=tube->EnterCell;
            CoordStruct enter(enter_cell);
            height_step=tube->unknown_1c0;
            int enter_height=Map.Get_Height_GL(enter);
            height_step=(Map.Get_Height_GL(exit)-enter_height)/height_step;
        }
        if(distance<=speed) {
            unknown_575++;
            int tube_dir=tube->Directions[unknown_575];
            finished=tube_dir==-1;
            unknown_slot103(unknown_45c);
            if(!finished) {
                coord=Tunnel_Adjacent_Coord(unknown_45c,tube_dir);
                CoordStruct delta=Map[coord].unknown_475d60()-Map[unknown_45c].unknown_475d60();
                unknown_45c+=CoordStruct(delta.X,delta.Y,0);
                unknown_45c.Z+=height_step;
                UnknownFacingDir direction_next;
                UnknownFacingDir dir=direction_next.Direction(Center_Coord(),unknown_45c);
                CoordStruct next=Tunnel_Move_Coord(Position_Coord_004f3f40(),dir,speed-distance);
                double divisor;
                if(tube_dir%2) divisor=256.0*1.4142135623730951;
                else divisor=256.0;
                next.Z+=(speed-distance)/divisor*height_step;
                unknown_slot103(next);
                return;
            }
        } else {
            int tube_dir=tube->Directions[unknown_575];
            CoordStruct next=Tunnel_Move_Coord(Position_Coord_004f3f40(),direction,speed);
            double divisor;
            if(tube_dir%2) divisor=256.0*1.4142135623730951;
            else divisor=256.0;
            next.Z+=speed/divisor*height_step;
            unknown_slot103(next);
            return;
        }
    }
    if(finished) {
        CellClass *cellptr=&Map[Position_Coord_004f3f40()];
        ObjectClass *occupier=cellptr->OccupierPtr;
        if(occupier!=0) {
            // RA2 snapshots all occupants before scattering; TS walks the live chain.
            DynamicVectorClass<ObjectClass *> occupants;
            do {
                occupants.Add(occupier);
                occupier=occupier->unknown_30;
            } while(occupier!=0);
            for(int i=0;i<occupants.Count();i++) {
                ObjectClass *object=occupants[i];
                if(object->Kind_Of()==1 || object->Kind_Of()==15) {
                    if(!((FootClass *)object)->Locomotor.Checked()->Is_Moving())
                        object->unknown_slot87(CoordStruct(0,0,0),true,true);
                }
            }
            unknown_slot306(0.0);
        } else {
            TubeClass *tube=unknown_TubeClass_list_008661c8[unknown_574];
            Cell final_exit_cell=tube->ExitCell;
            CoordStruct exit_coord(final_exit_cell,unknown_45c.Z);
            unknown_slot103(exit_coord);
            unknown_574=-1;
            Mark(1);
            if(unknown_494==cellptr) unknown_slot87(CoordStruct(0,0,0),true,true);
            unknown_57b=true;
            unknown_slot93(2);
            unknown_slot306(1.0);
            TubeClass *tube_here=Cell_Tube_0047a130(&Map[Position_Coord_004f3f40()]);
            if(tube_here) {
                UnknownFacingDir reverse((unsigned short)(((tube_here->Facing<<13)-0x6001)&0xE000));
                unknown_2e0.Set_Current(reverse);
            }
        }
    }
}
"""
