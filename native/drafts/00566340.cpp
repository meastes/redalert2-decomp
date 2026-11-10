// NON-EXACT DRAFT 0x566340 MapClass::Unknown_00566340 (rebuild the subzone graph around a changed cell)
// entry 0x566340; exact-match score 64.37 (misses_index best; shape 98.45, calls equal); plan "draft, verify"
// origin lane: Scout664 (build/class-work/Scout664/w/keep/c_6437.cpp, overlay w/h2; Scout662 c_5141 before it)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1111 (native/drafts.json). Not original bytes. Native adaptation (Scout1111): compiled
// against HEAD's headers (one population macro added, marked NATIVE); reached when the computer player's
// building or wall marks an overlay (OverlayClass::Mark -> CellClass 0x00475960).
// Rebuild the subzone graph around one changed map cell, original00566340.
#define RA2_MAP_COLLAPSE_CLIFF   // NATIVE: MapClass::Fine_Subzone (shared/map.hpp; Scout664's overlay had it unconditionally)
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/cell_subzone.hpp"
#include "shared/path_estimator.hpp"
// @ref ?Unknown_00564290@MapClass@@QAEHPAUCellSubzoneStruct@@HIABVRect@@ABVCell@@@Z=00564290
// @ref ?Reset@AStarClass@@QAEXXZ=00429a90
// @ref ?Search@@3UUnknownPathEstimator@@A=008315c8
// @ref ??_7?$VectorClass@G@@6B@=0079c534
// @ref ??_7?$DynamicVectorClass@G@@6B@=0079c554

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

inline int Cell_Passability(MapClass *map, Cell const &cell)
{
    return map->CellZones[map->Get_Cell_Zone_Index(cell)].unknown_00;
}

void MapClass::Unknown_00566340(Cell const &cell)
{
    if (Unknown_0055A660(cell, true)) {
        int y;
        int x;
        Cell trycell;
        for (int level = 2; level >= 0; --level) {
            Rect bounds;
            bounds.Width = 1 << (level + 1);
            bounds.Height = bounds.Width;
            int cx = cell.X;
            int cy = cell.Y;
            bounds.X = cx - cx % bounds.Width;
            bounds.Y = cy - cy % bounds.Height;
            DynamicVectorClass<unsigned short> collected;
            HashTableClass<unsigned int, SubzoneConnectionStruct> *set = unknown_7c[level];
            for (int bucket = 0; bucket < set->BucketCount; ++bucket) set->Buckets[bucket].Clear();
            DynamicVectorClass<SubzoneTrackingStruct> *track = &SubzoneTracking[level];
            for (y = bounds.Y; y < bounds.Y + bounds.Height; ++y) {
                for (x = bounds.X; x < bounds.Width + bounds.X; ++x) {
                    CellSubzoneStruct *subptr = (CellSubzoneStruct *)&Fine_Subzone(Cell(x, y));
                    unsigned short subid = (unsigned short)subptr->SubzoneID[level];
                    if (subid != 0) {
                        int found;
                        for (found = collected.Count() - 1; found >= 0; --found) {
                            if (collected[found] == subid) break;
                        }
                        if (found == -1) collected.Add(subid);
                    }
                    subptr->SubzoneID[level] = 0;
                    subptr->ZoneID = Unknown_0054FA60(Cell(x, y));
                }
            }
            for (int index = collected.Count() - 1; index >= 0; --index) {
                unsigned short id = collected[index];
                SubzoneTrackingStruct *entry = &SubzoneTracking[level][id];
                for (int connection = entry->Connections.Count() - 1; connection >= 0; --connection) {
                    SubzoneTrackingStruct *neighbor = &SubzoneTracking[level][entry->Connections[connection].unknown_00];
                    for (int reverse = neighbor->Connections.Count() - 1; reverse >= 0; --reverse) {
                        if (neighbor->Connections[reverse].unknown_00 == id) {
                            neighbor->Connections.Delete(reverse);
                            break;
                        }
                    }
                }
                entry->Connections.Clear();
            }
            int entry_count = unknown_70[level];
            for (y = bounds.Y; y < bounds.Y + bounds.Height; ++y) {
                for (x = bounds.X; x < bounds.Width + bounds.X; ++x) {
                    if (Unknown_0055A660(Cell(x, y), true)) {
                        CellSubzoneStruct *subptr = (CellSubzoneStruct *)&Fine_Subzone(Cell(x, y));
                        int passability = Cell_Passability(this, Cell(x, y));
                        if (passability != 7 && subptr->SubzoneID[level] == 0) {
                            trycell.Y = (short)y;
                            trycell.X = (short)x;
                            Unknown_00564290(subptr, level, entry_count, bounds, trycell);
                            track->Add(SubzoneTrackingStruct());
                            SubzoneTrackingStruct *added = &(*track)[entry_count];
                            int parent = 0;
                            if (level != 2) parent = subptr->SubzoneID[level + 1];
                            added->unknown_18 = (short)parent;
                            added->unknown_1c = passability;
                            added->Connections.Set_Growth_Step(16);
                            added->unknown_20 = Threat_Region(x, y);
                            ++entry_count;
                            if (entry_count == 0) {
                                Reset_All_Subzones();
                                return;
                            }
                        }
                    }
                }
            }
            unknown_70[level] = entry_count;
            for (int connection_index = ZoneConnections.Count() - 1; connection_index >= 0; --connection_index) {
                ZoneConnectionClass &connection = ZoneConnections[connection_index];
                if ((connection.From.X >= bounds.X && connection.From.X < bounds.Width + bounds.X &&
                     connection.From.Y >= bounds.Y && connection.From.Y < bounds.Y + bounds.Height) ||
                    (connection.To.X >= bounds.X && connection.To.X < bounds.Width + bounds.X &&
                     connection.To.Y >= bounds.Y && connection.To.Y < bounds.Y + bounds.Height)) {
                    if (connection.unknown_08) Unknown_00564B60(&connection, level);
                }
            }
            for (int bucket_index = 0; bucket_index < 256; ++bucket_index) {
                if (set->Buckets[bucket_index].Count() > 0) {
                    HashObject<unsigned int, SubzoneConnectionStruct> *object = &set->Buckets[bucket_index][0];
                    for (int remaining = set->Buckets[bucket_index].Count(); remaining > 0; --remaining) {
                        unsigned int packed = (unsigned int)object->Value.unknown_00;
                        unsigned short first = Packed_Low(packed);
                        unsigned short second = Packed_High(packed);
                        bool cross_block = object->Value.unknown_04;
                        SubzoneConnectionStruct forward;
                        forward.unknown_00 = second;
                        forward.unknown_04 = cross_block;
                        (*track)[first].Connections.Add(forward);
                        SubzoneConnectionStruct reverse;
                        reverse.unknown_00 = first;
                        reverse.unknown_04 = cross_block;
                        (*track)[second].Connections.Add(reverse);
                        ++object;
                    }
                }
            }
        }
        int base_x = cell.X - cell.X % 8;
        int base_y = cell.Y - cell.Y % 8;
        for (y = base_y; y < base_y + 8; ++y) {
            for (x = base_x; x < base_x + 8; ++x) {
                trycell.X = (short)x;
                trycell.Y = (short)y;
                if (Unknown_0055A660(trycell, true)) {
                    CellSubzoneStruct *subptr = (CellSubzoneStruct *)&Fine_Subzone(Cell(x, y));
                    for (int parent_level = 0; parent_level < 2; ++parent_level) {
                        SubzoneTracking[parent_level][subptr->SubzoneID[parent_level]].unknown_18 = subptr->SubzoneID[parent_level + 1];
                    }
                }
            }
        }
        Search.Reset();
    }
}
