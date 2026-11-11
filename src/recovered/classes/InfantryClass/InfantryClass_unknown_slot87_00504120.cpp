// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void InfantryClass::unknown_slot87(CoordStruct const &threat, bool forced, bool nokidding) -- 0x00504120
// TibSun/YR InfantryClass::Scatter: scatters the infantry away from a threat, or out of its cell when there is none.
// Head from ra2-decomp (YR 0x0051D0D0): the threat test through operator!=, the direction through the file-local
// Coord_Direction on a copy of Coord, and the cell fraction through Cell_Fraction. Direct position.X/.Y reads there
// let VC6 fold the height product into its operand register (imul edi,[g]) and flip the Adjacent_Cell sum.
#include "shared/infantry.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/tactical.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/random2.hpp"
#include "shared/unknown_geometry.hpp"
// @ref _unknown_geometry_k=00a410d8
// @ref _unknown_geometry_g=00a41130

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

void InfantryClass::unknown_slot87(CoordStruct const &threat, bool forced, bool nokidding)
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
        int height=origin_cell->unknown_11b+unknown_slot46(0) ? 4 : 0;
        CoordStruct destination=unknown_slot19(0);
        destination.Z=height*unknown_geometry_g;
        for (int index=0;index<8;++index) {
            int direction=(facing+index)&7;
            Cell check=Adjacent_Cell(origin,direction);
            CellClass *candidate=&Map[check];
            if (Map.Unknown_0055A660(check,true) && unknown_slot101(candidate,direction,Unknown_005D47D0(),0,1)==0) {
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
            unknown_slot116(2,false);
            Assign_Destination(&Map[chosen],true);
        }
    }
}
