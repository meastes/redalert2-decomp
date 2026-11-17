// An interface pointer, the millisecond timer and the mixfile lists defined in the unit at 0x0040CE70 (.CRT$XCU): each definition emits its
// dynamic initializer (the inline DynamicVectorClass constructor) and the atexit destructor
// (the inline VectorClass destructor: Clear of owned storage).
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/cd.hpp"
#include "shared/milsectmr.hpp"
#include "shared/com_api.hpp"

// An interface pointer nothing else in the game reads; released at exit (0x008324B0; the
// interface type is unknown).
CComPtr<IUnknown> unknown_com_pointer_8324b0;

// The game's millisecond timer (0x00837448).
MillisecondTimerClass MillisecondTimer;

DynamicVectorClass<MixFileClass *> unknown_expansion_mixes_837450;
DynamicVectorClass<MixFileClass *> unknown_mixes_837480;
DynamicVectorClass<MixFileClass *> unknown_maps_mixes_837468;
DynamicVectorClass<MixFileClass *> unknown_movies_mixes_8374a0;
