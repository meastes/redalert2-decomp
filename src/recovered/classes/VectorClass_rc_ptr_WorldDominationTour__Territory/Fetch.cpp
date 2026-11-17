// VectorClass<rc_ptr<WorldDominationTour::Territory>>::Fetch (0x0072AFD0).
// Layout and behaviour: shared/vector_class.hpp.
#define VECTOR_IMPLEMENT_VECTOR_FETCH
#include "shared/vector_class.hpp"
#include "shared/reference_counted.hpp"
namespace WorldDominationTour {
class Conflict;
class GameOption;
class Territory;
}
class WDTTerritory;


template class VectorClass<rc_ptr<WorldDominationTour::Territory> >;
