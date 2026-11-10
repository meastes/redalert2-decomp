#include "shared/scroll.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/cell_class.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/house.hpp"
#include "shared/id_source.hpp"
#include "shared/special.hpp"
#include "shared/tactical.hpp"
// @ref ?Get_Selectable_Object_006aa990@Tactical@@QAEPAVObjectClass@@ABUPoint2D@@@Z=006aa990
// @ref _unknown_geometry_k=00ab7068
// @ref _unknown_geometry_l=00ab7088
bool ScrollClass::Unknown_00669650(Point2D const &xy,Cell &cell,CoordStruct &coord,ObjectClass *&object,bool &fog,bool &shadow)
{
    object=0;
    cell=unknown_geometry_k;
    coord=unknown_geometry_l;
    fog=false;
    shadow=false;
    if (xy.X<0 || xy.Y<0) return false;
    cell = unknown_tactical_83996c ? unknown_tactical_83996c->Unknown_006A6BD0(xy+Point2D(unknown_rect_839630.X,unknown_rect_839630.Y)) : unknown_geometry_k;
    coord = unknown_tactical_83996c ? unknown_tactical_83996c->Unknown_006A2B10(xy+Point2D(unknown_rect_839630.X,unknown_rect_839630.Y)) : unknown_geometry_l;
    if (unknown_tactical_83996c && Map.Unknown_0054AB00(coord)) {
        UnknownCoord434820 adjusted = UnknownCoord434820(cell,Map.Get_Height_GL(UnknownCoord434820(cell)));
        shadow=Map.Is_Shrouded(adjusted) && unknown_b263d8!=0;
        fog=false;
        if (unknown_id_source_a3d290->Special().FogOfWar) {
            BuildingClass *building=static_cast<BuildingClass *>(Map[adjusted].Cell_Building());
            if (building) fog=building->unknown_5b7;
            else fog=Map.Is_Fogged(adjusted) && unknown_b263d8!=0;
        }
        if (!shadow && !fog) {
            object=unknown_tactical_83996c->Get_Selectable_Object_006aa990(xy);
            TechnoClass *techno=object->As_Techno();
            if (techno && !techno->unknown_372) {
                if ((techno->unknown_1b8==2 && !Map[techno->Center_Coord()].unknown_47c2e0(unknown_player_a35db4->ArrayIndex)) || techno->unknown_slot32()->Invisible) object=0;
            }
            if (object && object->Kind_Of()==6) {
                BuildingClass *building=static_cast<BuildingClass *>(object);
                if (!building->unknown_372 && ((building->unknown_5bd==15 && !Map[techno->Center_Coord()].unknown_47c2e0(unknown_player_a35db4->ArrayIndex)) || building->Type->InvisibleInGame)) object=0;
            }
        }
        return true;
    }
    return false;
}
