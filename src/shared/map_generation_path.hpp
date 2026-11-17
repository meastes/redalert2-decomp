// Unknown original nonpolymorphic map-generation path/point records; names are hypotheses.
// Original overloads0058B200/250/2C0/340 allocate1/2/3/4 eight-byte records.
// Count+0, Points+4, unknown_08+8; no RTTI identity or accepted ABI change.
#ifndef RA2_SHARED_MAP_GENERATION_PATH_HPP
#define RA2_SHARED_MAP_GENERATION_PATH_HPP
#include "shared/coord.hpp"
struct DirtRoadLink {
    DirtRoadLink(); //0058B120
    DirtRoadLink(Cell const &position,int value); //0058B100
    DirtRoadLink(DirtRoadLink const &other); //0058B140, copyctor-versus-assignment intent qualified
    Cell Position;
    int unknown_04;
};
class DirtRoadTile {
public:
    DirtRoadTile(); //0058B3D0, deliberately leaves unknown_08 uninitialized
    DirtRoadTile(DirtRoadLink const &a); //0058B200
    DirtRoadTile(DirtRoadLink const &a,DirtRoadLink const &b); //0058B250
    DirtRoadTile(DirtRoadLink const &a,DirtRoadLink const &b,DirtRoadLink const &c); //0058B2C0
    DirtRoadTile(DirtRoadLink const &a,DirtRoadLink const &b,DirtRoadLink const &c,DirtRoadLink const &d); //0058B340
    ~DirtRoadTile(); //0058B3E0
    int Count;
    DirtRoadLink *Points;
    int unknown_08;
};
typedef char UnknownMapGenerationPoint_size[(sizeof(DirtRoadLink)==8)?1:-1];
typedef char UnknownMapGenerationPath_size[(sizeof(DirtRoadTile)==12)?1:-1];
#endif
