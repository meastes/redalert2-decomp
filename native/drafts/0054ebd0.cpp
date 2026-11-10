// NON-EXACT DRAFT 0x54EBD0 MapClass::Zone_Reset (Zone_Reset)
// entry 0x54EBD0; exact-match score 84.41 (misses_index best); plan "draft, verify"
// origin lane: Scout641 (build/class-work/Scout641/w/keep/zone_reset_8441.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): overlay names replaced by HEAD's; each changed line is marked NATIVE.
// int MapClass::Zone_Reset() (0x0054EBD0; TibSun MapClass::Zone_Reset)
// Rebuilds the zones: clears the zone adjacency table and the twelve movement zone
// tables, numbers every contiguous span of +0x64 elements (Zone_Span 0x0054F250, the
// passability of each new zone collected in a local list), files the zone pairs of the
// passable zone connections, builds each zone's neighbour list from the table, then
// for each of the 12 rows of 0x007E00C4 floods the zones whose passability the row
// treats alike into movement zones (2 up; zone 0 gets -1). Answers the zone of the
// largest span (TibSun answers its row-0 movement zone).
#include "shared/map.hpp"
// NATIVE: the overlay hZR names (PassabilityType, LastAdjacentZone, Zone_Span) spelled with HEAD's names.
enum PassabilityType { PASSABLE_OUTSIDE = 7 };   // NATIVE: hZR map.hpp (RA2 outside value 7)
extern int unknown_zone_a6fc34;                  // NATIVE: hZR LastAdjacentZone (0x00A6FC34)
// @ref ??2@YAPAXI@Z=00782017
// @ref ??3@YAXPAX@Z=00781d3b
// @ref ?LastAdjacentZone@@3HA=00a6fc34
// @ref ?unknown_table_7e00c4@@3PAY07HA=007e00c4
// @ref ?Zone_Span@MapClass@@QAEHPAUUnknownMapElem4@@HAAH@Z=0054f250
// @ref ??_7?$DynamicVectorClass@W4PassabilityType@@@@6B@=007a5ab8
// @ref ??_7?$VectorClass@W4PassabilityType@@@@6B@=007a5a98

int MapClass::Zone_Reset()
{
    int i;
    int j;
    int k;
    int bestzone = -1;
    int bestspan = -1;

    UnknownMapElem4 *end = &CellZones[CellZoneCount];

    DynamicVectorClass<PassabilityType> vec;
    vec.Set_Growth_Step(300);

    unknown_14->Clear();

    for (i = 0; i < 12; i++) {
        if (unknown_18[i] != 0) {
            delete unknown_18[i];
            unknown_18[i] = 0;
        }
    }

    UnknownMapElem4 *czone = CellZones;
    for (; czone < end; czone++) {
        czone->unknown_02 = 0;
    }

    int zone = 1;

    vec.Add(PASSABLE_OUTSIDE);

    int skip = 0;
    czone = CellZones;
    while (czone < end) {
        PassabilityType pass = (PassabilityType)czone->unknown_00;
        if (pass != PASSABLE_OUTSIDE && czone->unknown_02 == 0) {
            unknown_zone_a6fc34 = 0;   // NATIVE
            int span = Unknown_0054F250(czone, zone, skip);   // NATIVE: Zone_Span
            if (span > bestspan) {
                bestzone = (unsigned short)zone;
                bestspan = span;
            }
            vec.Add(pass);
            zone++;
            czone += skip;
        } else {
            czone++;
        }
    }

    unknown_48 = (unsigned short)zone;

    for (i = ZoneConnections.Count() - 1; i >= 0; i--) {
        if (ZoneConnections[i].unknown_08) {
            int from_zone = Unknown_0054FA60(ZoneConnections[i].From);
            int to_zone = Unknown_0054FA60(ZoneConnections[i].To);
            if (to_zone != from_zone) {
                if (to_zone < from_zone) {
                    int temp = to_zone;
                    to_zone = from_zone;
                    from_zone = temp;
                }
                unknown_14->Add_Object(HashObject<unsigned int, unsigned int>(from_zone, to_zone));
            }
        }
    }

    unsigned short *zone_degree = new unsigned short[unknown_48];
    for (i = 0; i < unknown_48; i++) {
        zone_degree[i] = 0;
    }

    for (i = 0; i < 256; i++) {
        DynamicVectorClass<HashObject<unsigned int, unsigned int> > &bucket = unknown_14->Buckets[i];
        if (bucket.Count() > 0) {
            j = bucket.Count();
            HashObject<unsigned int, unsigned int> *obj = &bucket[0];
            do {
                unsigned int value = obj->Value;
                unsigned short zone1 = (unsigned short)value;
                unsigned short zone2 = (unsigned short)(value >> 16);
                zone_degree[zone1]++;
                zone_degree[zone2]++;
                obj++;
            } while (--j);
        }
    }

    unsigned short **zone_neighbors = new unsigned short *[unknown_48];
    for (i = 0; i < unknown_48; i++) {
        zone_neighbors[i] = new unsigned short[zone_degree[i]];
    }

    for (i = 0; i < unknown_48; i++) {
        zone_degree[i] = 0;
    }

    for (i = 0; i < 256; i++) {
        DynamicVectorClass<HashObject<unsigned int, unsigned int> > &bucket = unknown_14->Buckets[i];
        if (bucket.Count() > 0) {
            j = bucket.Count();
            HashObject<unsigned int, unsigned int> *obj = &bucket[0];
            do {
                unsigned int value = obj->Value;
                unsigned short zone1 = (unsigned short)value;
                unsigned short zone2 = (unsigned short)(value >> 16);
                zone_neighbors[zone1][zone_degree[zone1]] = zone2;
                zone_neighbors[zone2][zone_degree[zone2]] = zone1;
                zone_degree[zone1]++;
                zone_degree[zone2]++;
                obj++;
            } while (--j);
        }
    }

    unsigned char *zone_passability = new unsigned char[unknown_48];
    for (i = 0; i < unknown_48; i++) {
        zone_passability[i] = vec[i];
    }

    unsigned short *stack = new unsigned short[unknown_48];

    for (int mzone = 0; mzone < 12; mzone++) {
        int next_movement_zone = 2;
        int *table = unknown_table_7e00c4[mzone];

        unsigned short *nzone = new unsigned short[unknown_48];
        unknown_18[mzone] = nzone;

        for (j = 0; j < unknown_48; j++) {
            nzone[j] = table[zone_passability[j]] != 1;
        }

        for (int zone_index = 0; zone_index < unknown_48; zone_index++) {
            if (nzone[zone_index] == 0) {
                int stackcount = 1;
                stack[0] = zone_index;
                nzone[zone_index] = next_movement_zone;
                int passable = table[zone_passability[zone_index]];
                while (stackcount) {
                    int current = stack[--stackcount];
                    unsigned short *neighbors = zone_neighbors[current];
                    int degree = zone_degree[current];
                    for (k = degree - 1; k >= 0; k--) {
                        int neighbor = neighbors[k];
                        if (table[zone_passability[neighbor]] == passable && nzone[neighbor] == 0) {
                            stack[stackcount++] = neighbor;
                            nzone[neighbor] = next_movement_zone;
                        }
                    }
                }
                next_movement_zone++;
            }
        }

        nzone[0] = -1;
    }

    for (i = 0; i < unknown_48; i++) {
        delete zone_neighbors[i];
    }

    delete [] zone_passability;
    delete [] zone_neighbors;
    delete [] zone_degree;
    delete [] stack;

    return bestzone;
}
