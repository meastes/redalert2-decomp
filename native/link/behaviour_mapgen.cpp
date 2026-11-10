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
MapGeneratorClass::MapGeneratorClass()
{
    MapSeeder = 0;
    MapPreview = 0;
    LocalWidth = 0;
    LocalHeight = 0;
    RMGMinimumTiberium = 2500;
    RMGMaximumTiberium = 5500;
    MaxTrees = 500;
    PreviewStale = true;
    SeededWaterAmount = 0;
    CellHeight = 4;
    PlaceWaterfall = false;
}
