// bool SidebarClass::Unknown_0067C370(FactoryClass *factory, int type, int id) (0x0067C370)
// Finds the strip a buildable type belongs to and updates it there.
#include "shared/sidebar.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Unknown_004EB550@@YIHHH@Z=004eb550

bool SidebarClass::Unknown_0067C370(FactoryClass *factory, int type, int id)
{
    return Column[Which_Column(type, id)].Unknown_006816D0(factory, type, id);
}
