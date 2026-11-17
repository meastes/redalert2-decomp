// VectorClass<rc_ptr<WDTTerritory>>::operator=(VectorClass<rc_ptr<WDTTerritory>> const &) (0x0072CAC0).
// Replaces the contents with an owned copy (shared/vector_class.hpp); the copy kept out of line
// (0x0072BEF0 calls it).
#define VECTOR_IMPLEMENT_VECTOR_ASSIGN
#include "shared/vector_class.hpp"
#include "shared/reference_counted.hpp"

class WDTTerritory;

template class VectorClass<rc_ptr<WDTTerritory> >;
