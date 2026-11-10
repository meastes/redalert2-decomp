// NON-EXACT DRAFT 0x563D80 MapClass::Reset_Subzone (Reset_Subzone)
// entry 0x563D80; exact-match score 76.89 (misses_index best); plan "draft, verify"
// origin lane: Scout660 (build/class-work/Scout664/w/keep/a6_7689.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): 0x00A6FC34 under its HEAD name; 0x00564290 declared by a native map.hpp fixup. Changed lines marked NATIVE.
// MapClass::Reset_Subzone: TibSun Reset_Subzone; rebuilds one level of the subzone graph.
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/cell_subzone.hpp"
// @ref ?unknown_last_adjacent_zone_a6fc34@@3IA=00a6fc34
// @ref ?Unknown_00564290@MapClass@@QAEHPAUCellSubzoneStruct@@HIABVRect@@ABVCell@@@Z=00564290
extern int unknown_zone_a6fc34;   // NATIVE: the same global as the accepted Zone_Span 0x0054F250 names it
#define unknown_last_adjacent_zone_a6fc34 unknown_zone_a6fc34   // NATIVE

inline unsigned short Packed_Low(unsigned int packed)
{
    return (unsigned short)packed;
}

inline unsigned short Packed_High(unsigned int packed)
{
    return (unsigned short)(packed >> 16);
}

inline int Threat_Region(int x, int y)
{
    return (short)x / 4 + 130 * ((short)y / 4) + 131;
}

inline void Set_Connection(SubzoneConnectionStruct &connection, int id, bool costly)
{
    connection.unknown_00 = id;
    connection.unknown_04 = costly;
}

void MapClass::Reset_Subzone(int level)
{
    CellSubzoneStruct *subend = (CellSubzoneStruct *)CellSubzones + CellZoneCount;
    HashTableClass<unsigned int, SubzoneConnectionStruct> *set = unknown_7c[level];
    DynamicVectorClass<SubzoneTrackingStruct> *track = &SubzoneTracking[level];
    for (int bucket = 0; bucket < set->BucketCount; ++bucket) set->Buckets[bucket].Clear();
    CellSubzoneStruct *subptr = (CellSubzoneStruct *)CellSubzones;
    UnknownMapElem4 *zoneptr = CellZones;
    if (subptr < subend) {
        do {
            subptr->SubzoneID[level] = 0;
            subptr->ZoneID = zoneptr->unknown_02;
            subptr->Height = zoneptr->unknown_01;
            ++subptr;
            ++zoneptr;
        } while (subptr < subend);
    }
    int entry_count = 1;
    track->Add(SubzoneTrackingStruct());
    (*track)[0].unknown_18 = 0;
    (*track)[0].unknown_1c = 7;
    subptr = (CellSubzoneStruct *)CellSubzones;
    zoneptr = CellZones;
    UnknownMapElem4 *zonerow = zoneptr;
    int width = 1 << (level + 1);
    int regionmask = width - 1;
    int rowwidth = Map.MapSize.Width + Map.MapSize.Height + 1;
    int x = 0;
    int rowx = 0;
    int y = 0;
    Rect bounds(0, 0, width, width);
    while (subptr < subend) {
        int passability = zoneptr->unknown_00;
        int parent;
        if (level + 1 < 3) parent = subptr->SubzoneID[level + 1];
        else parent = 0;
        if (passability == 7 || subptr->SubzoneID[level] != 0) {
            ++subptr;
            ++zoneptr;
            ++x;
            rowx = x;
            zonerow = zoneptr;
        } else {
            Cell cell;
            cell.Y = (short)y;
            unknown_last_adjacent_zone_a6fc34 = 0;
            cell.X = (short)x;
            int count = Unknown_00564290(subptr, level, entry_count, bounds, cell);
            track->Add(SubzoneTrackingStruct());
            SubzoneTrackingStruct *added = &(*track)[(unsigned short)entry_count];
            added->unknown_18 = (short)parent;
            added->unknown_1c = passability;
            added->Connections.Set_Growth_Step(16);
            added->unknown_20 = Threat_Region(x, y);
            zonerow += count;
            zoneptr = zonerow;
            ++entry_count;
            subptr += count;
            rowx += count;
            x = rowx;
        }
        if (x == rowwidth) {
            x = 0;
            ++y;
            rowx = 0;
            bounds.X = 0;
            bounds.Y = y - (regionmask & y);
        } else if ((x & regionmask) == 0) bounds.X = x;
    }
    unknown_70[level] = (unsigned short)entry_count;
    Unknown_00564B20(level);
    for (int bucket_index = 0; bucket_index < 256; ++bucket_index) {
        if (set->Buckets[bucket_index].Count() > 0) {
            HashObject<unsigned int, SubzoneConnectionStruct> *object = &set->Buckets[bucket_index][0];
            for (int left = set->Buckets[bucket_index].Count(); left > 0; --left) {
                unsigned packed = (unsigned)object->Value.unknown_00;
                unsigned short low = Packed_Low(packed);
                unsigned short high = Packed_High(packed);
                bool costly = object->Value.unknown_04;
                SubzoneConnectionStruct forward;
                Set_Connection(forward, high, costly);
                (*track)[low].Connections.Add(forward);
                SubzoneConnectionStruct backward;
                Set_Connection(backward, low, costly);
                (*track)[high].Connections.Add(backward);
                ++object;
            }
        }
    }
}
