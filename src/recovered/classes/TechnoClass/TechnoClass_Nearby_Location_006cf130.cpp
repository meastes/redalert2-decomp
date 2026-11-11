// Find a nearby reachable cell using this object's speed and movement zone.
#include "shared/techno.hpp"
#include "shared/map_view.hpp"
Cell TechnoClass::unknown_6cf130(AbstractClass *target)
{
    int speed=Techno_Type_Class()->SpeedType;
    if(speed==4) speed=1;
    Cell cell;
    if(target) cell=Cell(target->Center_Coord());
    else cell=Cell(Center_Coord());
    int movement_zone=Techno_Type_Class()->MovementZone;
    int zone=movement_zone!=-1 ? Map.Zone(cell,movement_zone,unknown_79) : -1;
    return Map.Nearby_Location(cell,speed,zone,movement_zone,unknown_79,Point2D(1,1),0,0,0,1,Cell(0,0),0);
}
