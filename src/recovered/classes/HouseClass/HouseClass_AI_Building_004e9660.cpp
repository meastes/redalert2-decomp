// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int HouseClass::AI_Building() (0x004E9660)
// Picks the next base node to build. Block structure from ra2-decomp (YR 0x004FE3E0): everything
// after the naval-node retry sits inside `if (node != NULL) { ... }` (an early return there
// leaves the node copy's stores in source order: 99.42).
int HouseClass::AI_Building()
{
    if (unknown_553c != -1) return 15;
    if (unknown_50.Count() == 0) return 15;

    BaseNodeClass *node = unknown_55f0.Next_Buildable();
    if (node == 0) return 15;

    if (node->Type >= 0 && BuildingTypes[node->Type]->Naval && !unknown_138) {
        unknown_55f0.Nodes.Delete(unknown_55f0.Nodes.ID(node));
        node = unknown_55f0.Next_Buildable();
    }
    if (node != 0) {
        if (node->Type == -3) {
            unknown_55f0.Nodes.Delete(unknown_55f0.Nodes.ID(node));
            AI_Build_Wall();
            return 1;
        }

        BaseNodeClass copy = *node;
        if (copy.Type == -1 || (BuildingTypes[copy.Type] == Rules->unknown_78c && node->Cell == Cell(0, 0))) {
            int index = unknown_55f0.Nodes.ID(node);
            if (Random_Pick(0, 99) < Rules->AIPickWallDefensePercent[unknown_c8] && unknown_func_4f5d30(index)) {
                unknown_55f0.Nodes.Delete(unknown_55f0.Nodes.ID(copy));
                return 1;
            }
            DynamicVectorClass<Cell> *cells = 0;
            if (unknown_55f0.unknown_38.Count() > 0) cells = &unknown_55f0.unknown_38;
            if (!AI_Build_Defense(index, cells)) {
                if (node->Type == Rules->unknown_78c->ArrayIndex) {
                    unknown_55f0.Nodes.Delete(index);
                }
                unknown_55f0.Nodes.Delete(index);
                return 1;
            }
        }

        if (node->Type == -2) return 15;

        BuildingTypeClass *type = BuildingTypes[node->Type];
        if (Drain + type->PowerDrain_c6c > Power - unknown_15f88) {
            bool found = false;
            for (int index = 0; index < Rules->unknown_7c8.Count(); index++) {
                if (Rules->unknown_7c8[index] == type) {
                    found = true;
                    break;
                }
            }
            if (!found && type->PowerDrain_c6c > 0) {
                BuildingTypeClass *power;
                if (unknown_130 == 0) {
                    power = Rules->GDIPowerPlant;
                } else {
                    DynamicVectorClass<BuildingTypeClass const *> owned;
                    for (int bindex = Buildings.Count() - 1; bindex >= 0; bindex--) {
                        owned.Add((BuildingTypeClass const *)Buildings[bindex]);
                    }
                    if (unknown_func_4ef740(Rules->NodAdvancedPower, owned, owned.Count())) {
                        power = Rules->NodAdvancedPower;
                    } else {
                        power = Rules->NodRegularPower;
                    }
                }
                int at = unknown_55f0.Nodes.ID(node);
                unknown_55f0.Nodes.Insert_After(BaseNodeClass(power->ArrayIndex, Cell(0, 0), false, 0), at - 1);
                return 1;
            }
        }

        if (type->PowersUpToLevel <= 0 && type->PowersUpToLevel == -1 && node->Cell != unknown_cell_a40f10 && type->PowersUpBuilding[0] != 0) {
            BuildingClass *building = (BuildingClass *)Map[node->Cell].unknown_4717b0();
            BuildingTypeClass *upgrade = BuildingTypes[BuildingTypeClass::From_Name(type->PowersUpBuilding)];
            if (building == 0) {
                node->Cell = Cell(0, 0);
            } else if (building->Type != upgrade) {
                node->Cell = Cell(0, 0);
            } else if ((building->Type->PowersUpToLevel == -1 && building->unknown_5c6 >= building->Type->Upgrades)
                    || (building->Type->PowersUpToLevel > 0 && building->unknown_5c6 > 0)) {
                node->Cell = Cell(0, 0);
            }
        }
        unknown_553c = node->Type;
    }
    return 15;
}
