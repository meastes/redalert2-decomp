// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void BuildingClass::Cloaking_AI(int fast) 0x0044FB50 (slot 234, Cloaking_AI), from ra2-decomp
// BuildingClass::Cloaking_AI (YR 0x00454DB0, src/building.cpp): steps the translucency of a cloaking or
// uncloaking building, restarts its natural particle system when it is fully visible, and grows or
// shrinks a cloak generator's covered circle one cell of radius per call.
// Name map: Cloak = unknown_1b8, TranslucencyLevel = unknown_5bd, IsToDisplay = unknown_75,
// Set_Anim_Translucency = unknown_44d2d0, ParticleSystems = unknown_25c, CloakGeneratorState = unknown_5bb,
// CurrentCloakRadius = unknown_5bc, CloakingSurface = unknown_cloak_surface_8502a0,
// Is_Cloaked/Cloaked_By/Uncloaked_By = Is/Set/Clear_House_Bit, Cell_Building = unknown_4717b0,
// IsPowerOnline = unknown_slot195, Should_Uncloak/Is_Ready_To_Cloak = unknown_slot161/160,
// Do_Uncloak/Do_Cloak = unknown_slot252/253, Enable_Cloak_Generator written out inline.
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/gscreen.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
#include "shared/particle_system.hpp"
#include "shared/surface.hpp"
// @ref _unknown_geometry_l=0084ed30
void BuildingClass::Cloaking_AI(int fast)
{
    if (!Type) return;
    Cell center=Cell(Center_Coord());
    if (unknown_1b8==1) {
        if (unknown_5bd<15) {
            ++unknown_5bd;
            if (unknown_5bd==1 || unknown_5bd==6 || unknown_5bd==11) unknown_75=true;
            unknown_44d2d0(unknown_5bd);
            if (unknown_5bd==15) {
                unknown_1b8=2;
                if (unknown_25c[2]) {delete unknown_25c[2];unknown_25c[2]=0;}
            }
        }
    } else if (unknown_1b8==3) {
        if (unknown_5bd>=0) {
            if (unknown_5bd>0) --unknown_5bd;
            if (unknown_5bd==0 || unknown_5bd==5 || unknown_5bd==10) unknown_75=true;
            unknown_44d2d0(unknown_5bd);
            if (unknown_5bd==0) {
                unknown_1b8=0;
                if (!unknown_25c[2] && (Type->NaturalParticleLocation!=unknown_geometry_l)) {
                    CoordStruct offset=Type->NaturalParticleLocation;
                    unknown_25c[2]=new ParticleSystemClass(Type->NaturalParticleSystem,
                        Position_Coord_00450950(*this)+offset,&Map[Position_Coord_00450950(*this)],0,unknown_geometry_l);
                }
            }
        }
    }
    if (unknown_1b8==2 && unknown_slot161()) unknown_slot252(false);
    if (unknown_1b8==0 && unknown_slot160()) unknown_slot253(false);
    if (unknown_5bb!=0 && Type->CloakGenerator) {
        int house=House->ArrayIndex;
        BSurface *surface=unknown_cloak_surface_8502a0;
        int width=surface->Get_Width();
        int half=width/2;
        if (unknown_5bb>0) {
            if (unknown_5bc==Type->CloakRadiusInCells) unknown_5bb=0;
            else {
                ++unknown_5bc;
                surface->Fill(0);
                surface->Fill_Circle(Point2D(half,half),unknown_5bc,surface->Get_Rect(),1);
                char *data=(char *)surface->Lock();
                Cell origin=center-Cell(half,half);
                for (int y=0;y<width;++y) {
                    for (int x=0;x<width;++x) {
                        if (*data++!=0) {
                            Cell current=origin+Cell(x,y);
                            CellClass *cell=&Map[current];
                            if (!cell->Is_House_Bit(house)) {
                                cell->Set_House_Bit(house);
                                Cell_Occupants_Unknown_Slot238_0044fb10(cell);
                                BuildingClass *building=static_cast<BuildingClass *>(cell->unknown_4717b0());
                                if (building && building->House->ArrayIndex==house && Cell(building->Center_Coord())==current) {
                                    --unknown_5bc;
                                    return;
                                }
                            }
                        }
                    }
                }
            }
        } else if (unknown_5bc==0) unknown_5bb=0;
        else {
            --unknown_5bc;
            surface->Fill(2);
            surface->Fill_Circle(Point2D(half,half),Type->CloakRadiusInCells,surface->Get_Rect(),0);
            if (unknown_5bc!=0) surface->Fill_Circle(Point2D(half,half),unknown_5bc,surface->Get_Rect(),1);
            char *data=(char *)surface->Lock();
            Cell origin=center-Cell(half,half);
            for (int y=0;y<width;++y) {
                for (int x=0;x<width;++x) {
                    if (*data++==0) {
                        Cell current=origin+Cell(x,y);
                        CellClass *cell=&Map[current];
                        if (cell->Is_House_Bit(house)) {
                            cell->Clear_House_Bit(house);
                            Cell_Occupants_Unknown_Slot238_0044fb10(cell);
                            BuildingClass *building=static_cast<BuildingClass *>(cell->unknown_4717b0());
                            if (building && building->House->ArrayIndex==house && Cell(building->Center_Coord())==current && (char)fast==0) {
                                ++unknown_5bc;
                                return;
                            }
                        }
                    }
                }
            }
            if (unknown_5bc==0) {
                for (int i=0;i<unknown_BuildingClass_list_00a40af8.Count();++i) {
                    BuildingClass *building=unknown_BuildingClass_list_00a40af8[i];
                    if (building->unknown_7d && building!=this && building->Type->CloakGenerator &&
                        building->unknown_slot195() && building->unknown_5bb==0) {
                        Cell diff=center-Cell(building->Center_Coord());
                        int dy=diff.Y;
                        int dx=diff.X;
                        if (dy*dy+dx*dx<4*((Type->CloakRadiusInCells+2)*(Type->CloakRadiusInCells+2))) {
                            if (building->Type->CloakGenerator) {
                                building->unknown_5bb=1;
                                if (building->unknown_5bc==building->Type->CloakRadiusInCells) building->unknown_5bc=0;
                                building->unknown_75=true;
                            }
                            if (building->unknown_5bc!=0) --building->unknown_5bc;
                        }
                    }
                }
            }
        }
    }
}
