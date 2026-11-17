// VectorClass<rc_ptr<WorldDominationTour::Territory>>::operator== (0x0072B110).
// Layout and behaviour: shared/vector_class.hpp.
#define VECTOR_IMPLEMENT_VECTOR_EQUAL
#include "shared/vector_class.hpp"
#include "shared/reference_counted.hpp"
namespace WorldDominationTour {
class Conflict;
class GameOption;
class Territory;
}
class WDTTerritory;


template class VectorClass<rc_ptr<WorldDominationTour::Territory> >;
