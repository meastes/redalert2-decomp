// <comdef.h> for the native YR build (lane Y1): the COM interfaces first (VC6's comdef.h reaches
// them through ole2.h), then mingw's comdef.h, then the smart-pointer typedefs VC6's comdef.h
// ends with (IStreamPtr, IPersistPtr, ...; mingw keeps them in comdefsp.h). mingw's own headers
// include <comdef.h> again from inside it: those nested passes only forward.
#ifdef YR_COMDEF_OUTER
#include_next <comdef.h>
#else
#define YR_COMDEF_OUTER
#include <ole2.h>
#include <objidl.h>
#include <propidl.h>
#include_next <comdef.h>
#include <comdefsp.h>
#undef YR_COMDEF_OUTER
#endif
