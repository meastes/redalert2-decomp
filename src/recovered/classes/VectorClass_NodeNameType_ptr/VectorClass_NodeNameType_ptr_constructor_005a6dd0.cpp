// VectorClass<NodeNameType *>::VectorClass(unsigned size, NodeNameType * const *array) (0x005A6DD0; stores the
// VectorClass<NodeNameType *> vtable 0x007a6884, whose slot 0 is the deleting destructor 0x005A6C40). Layout and
// behaviour: shared/vector_class.hpp.
// @ref ??_7?$VectorClass@PAUNodeNameType@@@@6B@=007a6884
// @ref ??2@YAPAXI@Z=00782017
// @ref ??_E?$VectorClass@PAUNodeNameType@@@@UAEPAXI@Z=005a6c40
// @ref ??8?$VectorClass@PAUNodeNameType@@@@UBE_NABV0@@Z=005a6730
// @ref ?Resize@?$VectorClass@PAUNodeNameType@@@@UAE_NHPBQAUNodeNameType@@@Z=005a6770
// @ref ?Clear@?$VectorClass@PAUNodeNameType@@@@UAEXXZ=005a6820
// @ref ?ID@?$VectorClass@PAUNodeNameType@@@@UAEHABQAUNodeNameType@@@Z=005a6850
// @ref ?ID@?$VectorClass@PAUNodeNameType@@@@UAEHPBQAUNodeNameType@@@Z=005a6890
// @ref ?Fetch@?$VectorClass@PAUNodeNameType@@@@UBEPAUNodeNameType@@H@Z=005a6460
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#include "shared/vector_class.hpp"

struct NodeNameType;

template class VectorClass<NodeNameType *>;
