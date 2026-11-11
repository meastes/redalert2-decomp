// Cell FootClass::unknown_4cca40(Cell const &cell, bool arg2) (0x004CCA40)
// The cell to head for when ordered to cell (slot 77; arg2 is passed on to
// slot 27, which gives the action): no cell (unknown_geometry_k) when the
// cell's ground (bridge height included) is shrouded and the type may not
// move to shroud; otherwise the cell itself, or a nearby cell (0x005502E0)
// when it is off the visible map, shrouded or in another movement zone; a
// type that moves anywhere looks for a nearby cell only on action 2 (in the
// same zone as the slot 19 destination's cell otherwise). Teleporter types
// skip the zone check unless the action is 2. The movement zone maps 6 to
// 0 and 9 to 7 while the house's 0x0051FC80 holds; jumpjet infantry move
// anywhere only while it does not.
#include "shared/foot.hpp"
#include "shared/map_view.hpp"
#include "shared/techno_type.hpp"
#include "shared/infantry.hpp"
#include "shared/infantry_type.hpp"
#include "shared/house.hpp"
#include "shared/unknown_geometry.hpp"
// @ref _unknown_geometry_k=00865f00
// @ref _unknown_geometry_j=00865f60

Cell FootClass::unknown_4cca40(Cell const &cell, bool arg2)
{
    int action = unknown_slot27(&cell, arg2, false);
    bool in_radar = Map.Unknown_0055A660(cell, true);

    CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    if (((UnknownCellClass *)&Map[coord])->Flags & 0x100) {
        coord.Z += unknown_geometry_j;
    }
    if (Techno_Type_Class()->MoveToShroud || !Map.Is_Shrouded(coord)) {

        Cell nearby = cell;
        UnknownCellClass *cptr = (UnknownCellClass *)&Map[unknown_slot19(0)];
        int mzone = Techno_Type_Class()->MovementZone;
        if (mzone == 6) mzone = 0;
        else if (mzone == 9 && House->Unknown_0051FC80()) mzone = 7;

        bool moveanywhere = In_Air() || Techno_Type_Class()->unknown_af5 || (Kind_Of() == 15 && ((InfantryClass *)this)->Type->JumpJet && !House->Unknown_0051FC80()) || Kind_Of() == 2;

        bool ontobridge = Is_Moving_Onto_Bridge(0);
        CoordStruct center = Cell_Coord(cell);
        center.Z = Map.Get_Height_GL(center);
        if (((UnknownCellClass *)&Map[center])->Flags & 0x100) {
            center.Z += unknown_geometry_j;
        }
        bool shrouded = Map.Is_Shrouded(center);

        if (moveanywhere && action == 2) {
            nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, -1, mzone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, Point2D(1, 1), 0, 1, Techno_Type_Class()->unknown_af5 && Get_Height_AGL() < 0, 1, Cell(0, 0), 0);
        } else {
            if (!in_radar || shrouded || !moveanywhere && (!Techno_Type_Class()->Teleporter || action == 2) && !Map.Unknown_0054F7C0(cptr->unknown_24, nearby, mzone, ontobridge, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, false)) {
                nearby = Map.Nearby_Location(cell, Techno_Type_Class()->SpeedType, Map.Zone(cptr->unknown_24, mzone, ontobridge), mzone, ((UnknownCellClass *)&Map[cell])->Flags >> 8 & 1, Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
            }
        }

        if (nearby != unknown_geometry_k) {
            return nearby;
        }
    }
    return unknown_geometry_k;
}
