// Native M25 (Scout1202): explicit instantiations template_instances.py refuses (element
// types that need a definition), for the VectorClass destructors the native link misses:
// the colour scheme table's hash vector (behaviour_scheme.cpp, unit p3294), the
// passability vector (DynamicVectorClass<PassabilityType>::Resize's unit) and the
// world domination tour's palettes. Weak definitions: an owner unit's definition wins.
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/scheme_table.hpp"
#include "shared/wdt_map.hpp"

enum PassabilityType : int;
template class VectorClass<PassabilityType>;
template class VectorClass<HashObject<HashString, SchemeNode *>>;
template class VectorClass<WorldDominationTour::Map::AnimationPalette *>;
template class DynamicVectorClass<HashObject<HashString, SchemeNode *>>;

// Scout1248 (the -O2 iOS build): List<INIClass::INISection *>'s constructor, which units
// built with LIST_DECLARE_CONSTRUCTOR call out of line; at -O0 another unit's inline copy
// defined it, at -O2 every copy is inlined away.
#include "shared/ini_class.hpp"
template class List<INIClass::INISection *>;
