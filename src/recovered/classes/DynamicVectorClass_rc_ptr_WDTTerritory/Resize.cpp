// DynamicVectorClass<rc_ptr<WDTTerritory>>::Resize (0x0072CD80).
// Layout and behaviour: shared/vector_class.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_RESIZE
#include "shared/vector_class.hpp"
#include "shared/reference_counted.hpp"
namespace WorldDominationTour {
class Conflict;
class GameOption;
class Territory;
}
class WDTTerritory;


template class DynamicVectorClass<rc_ptr<WDTTerritory> >;
