// DynamicVectorClass<rc_ptr<WDTTerritory>>::Clear (0x0072CA20).
// Layout and behaviour: shared/vector_class.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_CLEAR
#include "shared/vector_class.hpp"
#include "shared/reference_counted.hpp"
namespace WorldDominationTour {
class Conflict;
class GameOption;
class Territory;
}
class WDTTerritory;


template class DynamicVectorClass<rc_ptr<WDTTerritory> >;
