// DynamicVectorClass<DiskID>::ID (T const &) (0x00673C70): the member the reference tables bind at this address.
// Layout and behaviour: shared/vector_class.hpp; enum DiskID: shared/diskid.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT
#include "shared/vector_class.hpp"
#include "shared/diskid.hpp"

template class DynamicVectorClass<DiskID>;
