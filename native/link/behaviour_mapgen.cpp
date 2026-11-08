// Behaviour-written constructor of the random map generator (see
// behaviour_startup.cpp). NOT EXACT.
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/map_region.hpp"
#include "shared/map.hpp"
#include "shared/radar.hpp"
#include "shared/unknown_map_generator.hpp"

// 0x00577010: parked unmatched (no compiling draft). From the row's decompile
// (Scout530 sweep seed): the seed options and the type lists are constructed
// by their own constructors, then the settings below are stored.
UnknownMapGenerator::UnknownMapGenerator()
{
    unknown_178 = 0;
    unknown_17c = 0;
    unknown_180 = 0;
    unknown_184 = 0;
    unknown_2bc = 2500;
    unknown_2c0 = 5500;
    unknown_2fc = 500;
    unknown_300 = true;
    unknown_304 = 0;
    unknown_30c = 4;
    unknown_310 = false;
}
