// bool DynamicVectorClass<char const *>::Add(char const *const &object) (0x006F8B40): the UICmnds COMDAT of the
// name lists' Add. The instance is told apart from the byte-identical DynamicVectorClass<TechnoClass *>::Add by
// elimination: that one is bound at 0x00413770, and the accepted callers bind this name at 0x006F8B40.
// Layout and behaviour: shared/vector_class.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/vector_class.hpp"

template class DynamicVectorClass<char const *>;
