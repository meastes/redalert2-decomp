// VectorClass<PlanningNodeClass *>::VectorClass(unsigned size, PlanningNodeClass * const *array) (0x0061BD70; stores the
// VectorClass<PlanningNodeClass *> vtable 0x007a823c). Layout and behaviour: shared/vector_class.hpp.
// @ref ??_7?$VectorClass@PAVPlanningNodeClass@@@@6B@=007a823c
// @ref ??2@YAPAXI@Z=00782017
// @ref ??_E?$VectorClass@PAVPlanningNodeClass@@@@UAEPAXI@Z=0061bb50
// @ref ??8?$VectorClass@PAVPlanningNodeClass@@@@UBE_NABV0@@Z=0061b0d0
// @ref ?Resize@?$VectorClass@PAVPlanningNodeClass@@@@UAE_NHPBQAVPlanningNodeClass@@@Z=0061b110
// @ref ?Clear@?$VectorClass@PAVPlanningNodeClass@@@@UAEXXZ=0061b1c0
// @ref ?ID@?$VectorClass@PAVPlanningNodeClass@@@@UAEHABQAVPlanningNodeClass@@@Z=0061b1f0
// @ref ?ID@?$VectorClass@PAVPlanningNodeClass@@@@UAEHPBQAVPlanningNodeClass@@@Z=0061b230
// @ref ?Fetch@?$VectorClass@PAVPlanningNodeClass@@@@UBEPAVPlanningNodeClass@@H@Z=0061aa30
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#include "shared/vector_class.hpp"

class PlanningNodeClass;

template class VectorClass<PlanningNodeClass *>;
