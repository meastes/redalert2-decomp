// void HouseClass::Plan_Naval_Yard_Path_004eecb0(int node) (0x004EECB0)
// The naval yard node `node` of the base plan: needs a large water zone, picks the shore cell nearest
// the base by path for a test harvester, then puts BuildDummy nodes every third step of the path
// before the yard node and gives the yard node that cell. Names are hypotheses (no caller, RA2 only).
#include "shared/house.hpp"
// @ref ??_C@_04LHDC@HARV?$AA@=007da8c8
// @ref ?search_compfunc@?$IndexClass@HVCell@@@@CAHPBX0@Z=004f8cf0
// @ref ?Increase_Table_Size@?$IndexClass@HVCell@@@@QAE_NH@Z=004f8c60
#include "shared/house_type.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/path_estimator.hpp"
#include "shared/unit.hpp"
#include "shared/unit_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/rules.hpp"
#include "shared/ini_class.hpp"
#include "shared/house_adjacent.hpp"

void HouseClass::Plan_Naval_Yard_Path_004eecb0(int node)
{
    Cell center(BaseCenter);
    int zone = Map.Zone(center, 0, false);
    Cell start = Map.Nearby_Location(center, 0, zone, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
    int water_zone = -1;
    if (Map.Unknown_005658C0(water_zone) < 3000) {
        unknown_55f0.Nodes.Delete(node);
        unknown_138 = false;
        return;
    }

    int width = 1;
    int height = 1;
    for (int i = 0; i < Rules->BuildNavalYard.Count(); i++) {
        if (Rules->BuildNavalYard[i]->AIBasePlanningSide == Type->Side || Rules->BuildNavalYard[i]->AIBasePlanningSide == -1) {
            width = Rules->BuildNavalYard[i]->Width() + 1;
            height = Rules->BuildNavalYard[i]->Foundation_Height(false) + 1;
            break;
        }
    }

    DynamicVectorClass<Cell> cells;
    Map.Unknown_00565A50(center, 25, 30, 10, water_zone, cells, width, height);
    if (cells.Count() == 0) {
        unknown_55f0.Nodes.Delete(node);
        unknown_138 = false;
        return;
    }

    IndexClass<int, Cell> sorted;
    for (int c = 0; c < cells.Count(); c++) {
        Cell spot = Map.Nearby_Location(cells[c], 0, zone, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
        int distance = unknown_path_estimator_8315c8.Unknown_0042AA30(center, spot, 0, false, false, 0);
        sorted.Add_Index(cells.Count() * distance + c, cells[c]);
    }
    cells.Clear();
    for (int s = 0; s < sorted.Count(); s++) {
        cells.Add(sorted.Fetch_By_Position(s));
    }

    bool found = false;
    UnknownPathType *path = 0;
    Cell target = unknown_cell_a40f10;
    Cell spot = unknown_cell_a40f10;
    int moves[2048];
    UnitClass *unit = new UnitClass(UnitTypes[UnitTypeClass::From_Name("HARV")], this);
    unit->unknown_slot53(&UnknownCoord434820(start.X * 256 + 128, start.Y * 256 + 128, 0), 0);
    while (!found && cells.Count() > 0) {
        target = cells[0];
        spot = Map.Nearby_Location(cells[0], 0, zone, 0, false, Point2D(1, 1), false, false, false, true, Cell(0, 0), 0);
        if (spot != unknown_cell_a40f10) {
            path = unknown_path_estimator_8315c8.Find_Path_0042a1d0(start, spot, unit, moves, -1, 0, 0);
        } else {
            path = 0;
        }
        if (path != 0 && path->Length <= 25) {
            found = true;
        } else {
            cells.Delete(0);
        }
    }
    unit->unknown_slot52();
    delete unit;
    if (!found) {
        unknown_55f0.Nodes.Delete(node);
        unknown_138 = false;
        return;
    }

    int bit = 1 << ArrayIndex;
    Cell cell = start;
    int first = 0;
    Cell last = start;
    for (int m = 0; m < path->Length; m++) {
        cell = AdjacentCell[moves[m] & 7] + cell;
        if (Map[cell].unknown_dc & bit) {
            first = m + 1;
            last = cell;
        }
    }

    cell = last;
    int counter = 2;
    int at = node - 1;
    int type = -1;
    for (int d = 0; d < Rules->BuildDummy.Count(); d++) {
        if (Rules->BuildDummy[d]->AIBasePlanningSide == Type->Side || Rules->BuildDummy[d]->AIBasePlanningSide == -1) {
            type = Rules->BuildDummy[d]->ArrayIndex;
            break;
        }
    }
    for (int step = first; step < path->Length; step++) {
        if (counter == 0) {
            counter = 3;
            unknown_55f0.Nodes.Insert_After(BaseNodeClass(type, cell, false, 0), at);
            at++;
        } else {
            counter--;
        }
        cell = cell + AdjacentCell[moves[step] & 7];
    }
    unknown_55f0.Nodes[at + 1].Cell = target;
}
