// DynamicVectorClass<WinsockInterfaceClass::WinsockBufferType *>::ID (T const *) (0x0076BB30): the member the reference tables bind at this address.
// Layout and behaviour: shared/vector_class.hpp; the nested buffer type: shared/winsock_interface.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_ID_POINTER
#include "shared/winsock_interface.hpp"

template class DynamicVectorClass<WinsockInterfaceClass::WinsockBufferType *>;
