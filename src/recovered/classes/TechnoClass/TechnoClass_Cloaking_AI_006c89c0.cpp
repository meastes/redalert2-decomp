// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void TechnoClass::unknown_slot234(int arg1) 0x006C89C0 (slot 234, Cloaking_AI), from ra2-decomp
// TechnoClass::Cloaking_AI (YR 0x006FB740, src/techno.cpp), cases in the twin's order (uncloaking,
// cloaking, cloaked): uncloaked, an object that may cloak (slot 154 and none of slots 201/202/111/112,
// or the rank's ability 6) and is not docked at a weapons factory steps the cloak stage and starts
// cloaking (slot 253) once slot 160 allows, at once above ConditionRed health, else with a 4% chance.
// Uncloaking (3) ends visible: the stage resets and the cloak delay timer (+0x1D4) restarts. Cloaking (1)
// ends hidden: state 2, every techno targeting it that can no longer see it drops the target (slot 220),
// and a computer unit scatters; while still dark a hurt object may give up (slot 252, 10%). Cloaked (2)
// uncloaks when slot 161 says so. arg1 is not read.
// Name map: Cloak = unknown_1b8, CloakingDevice = unknown_1bc, CloakDelay = unknown_1d4,
// Visual_Character = unknown_slot25, Mark = unknown_slot71, Has_Ability = unknown_6d7910,
// Is_Ready_To_Cloak/Should_Uncloak = unknown_slot160/161, Do_Uncloak/Do_Cloak = unknown_slot252/253,
// Detach_All = unknown_slot54, Assign_Target = unknown_slot220, Do_Shimmer = unknown_slot61,
// Scatter = unknown_slot87, ConditionRed = unknown_1370, Array = Technos.
// The stage's timer test calls TibSun's operator long (RA2_CDTIMER_CONVERSION_VALUE_BODY, 0x00453EB0).
#include "shared/techno.hpp"
#include "shared/foot.hpp"
#include "shared/aircraft.hpp"
#include "shared/rules.hpp"
#include "shared/gscreen.hpp"
#include "shared/techno_map.hpp"
#include "shared/unknown_geometry.hpp"
// @ref ?unknown_7133a0@UnknownTechnoPair@@QBE_NXZ=007133a0
// @ref ?unknown_713420@UnknownTechnoPair@@QBE_NXZ=00713420
// @ref ?Contact@RadioClass@@QBEPAVTechnoClass@@H@Z=00636e10
// @ref ??B?$CDTimerClass@VFrameTimerClass@@@@QBEJXZ=00453eb0
// @ref ?Health_Ratio@ObjectClass@@QBENXZ=005d4540
// @ref ?Unknown_004F52F0@HouseClass@@QAE_NXZ=004f52f0
// @ref ?unknown_47c2e0@UnknownCellClass@@QAE_NH@Z=0047c2e0
// @ref __real@8@4008e100000000000000=0079b608
// @ref _unknown_geometry_l=00ac0188
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ??_7?$DynamicVectorClass@PAVTechnoClass@@@@6B@=0079a79c
// @ref ??_G?$DynamicVectorClass@PAVTechnoClass@@@@UAEPAXI@Z=0040bbc0
// @ref ?Clear@?$DynamicVectorClass@PAVTechnoClass@@@@UAEXXZ=0040b5b0
// @ref ?ID@?$DynamicVectorClass@PAVTechnoClass@@@@UAEHABQAVTechnoClass@@@Z=0040baa0
// @ref ?ID@?$DynamicVectorClass@PAVTechnoClass@@@@UAEHPBQAVTechnoClass@@@Z=0040b5e0
// @ref ?Resize@?$DynamicVectorClass@PAVTechnoClass@@@@UAE_NHPBQAVTechnoClass@@@Z=0040b9f0
// @ref ??0?$VectorClass@PAVTechnoClass@@@@QAE@IPBQAVTechnoClass@@@Z=0040bc10

void TechnoClass::unknown_slot234(int arg1)
{
    if (unknown_1b8 == 0) {
        if ((unknown_slot154() && !unknown_slot201() && !unknown_slot202() && !unknown_slot111() && !unknown_slot112())
            || unknown_6d7910(6)) {
            TechnoClass *contact = Contact(0);
            if (contact == 0 || !(contact->Kind_Of() == 6 && ((BuildingClass *)contact)->Type->WeaponsFactory)) {
                unknown_1bc.Graphic_Logic();
                if (unknown_slot160()) {
                    if (Health_Ratio() > Rules->unknown_1370) {
                        unknown_slot253(false);
                    } else {
                        if (unknown_id_source_a3d290->Random()(0, 99) < 4) {
                            unknown_slot253(false);
                        }
                    }
                }
            }
        }
    } else {
        unknown_1bc.Graphic_Logic();
        if (unknown_1bc.Fetch_Stage() < 0) {
            unknown_1bc.Set_Stage(0);
        }
        switch (unknown_1b8) {
        case 3:
            unknown_slot71(2);
            switch (unknown_slot25(true, 0)) {
            case 0:
                unknown_1bc.Set_Rate(0);
                unknown_1bc.Set_Stage(0);
                unknown_1b8 = 0;
                unknown_1d4 = CDTimerClass<FrameTimerClass>((int)(Rules->CloakDelay * 900.0));
                unknown_slot71(2);
                break;
            case 1:
                if (unknown_slot160()) {
                    unknown_slot253(true);
                }
                break;
            }
            break;
        case 1:
            unknown_slot71(2);
            if (!unknown_1bc.Fetch_Rate()) {
                unknown_1bc.Set_Rate(1);
            }
            switch (unknown_slot25(true, 0)) {
            case 2:
                if (Health_Ratio() <= Rules->unknown_1370 && unknown_id_source_a3d290->Random()(0, 99) < 10) {
                    unknown_slot252(true);
                }
                break;
            case 3:
            case 5:
                unknown_1b8 = 2;
                unknown_1bc.Set_Rate(0);
                unknown_1bc.Set_Stage(0);
                unknown_slot71(2);
                if (Kind_Of() == 1 && ((UnitClass *)this)->unknown_5b4 != -1) {
                    unknown_slot61();
                } else {
                    DynamicVectorClass<TechnoClass *> attackers;
                    for (int index = Technos.Count() - 1; index >= 0; index--) {
                        TechnoClass *techno = Technos[index];
                        if (techno->unknown_228 == this) {
                            if (((UnknownCellClass *)&Map[Center_Coord()])->unknown_47c2e0(techno->House->ArrayIndex)
                                || techno->House == House) {
                                attackers.Add(techno);
                            }
                        }
                    }
                    unknown_slot54(false);
                    for (int i = attackers.Count() - 1; i >= 0; i--) {
                        attackers[i]->unknown_slot220(this);
                    }
                }
                if (Kind_Of() == 1 && !House->Unknown_004F52F0()) {
                    unknown_slot87(unknown_geometry_l, true, false);
                }
                break;
            }
            break;
        case 2:
            if (unknown_slot161()) {
                unknown_slot252(false);
            }
            break;
        }
    }
}
