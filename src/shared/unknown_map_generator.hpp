// Westwood's MapGeneratorClass (TibSun mapgen.h; members +0x178..+0x310 named as ra2-decomp include/mapgen.h,
// laid out the same here): the random map generator, global at 0x00A6FD68, constructed at 00577010.
// TypeList fields have original RTTI identities; MapSeedClass SeedData occupies the first 0x178 bytes.
#ifndef RA2_UNKNOWN_MAP_GENERATOR_HPP
#define RA2_UNKNOWN_MAP_GENERATOR_HPP
#include "shared/type_list.hpp"
#include "shared/coord.hpp"
#include "shared/map_seed.hpp"
class BuildingTypeClass;
class CellClass;
class MapGeneratorClass {
public:
    bool Grow_Region(int region,int rings,Rect area,char setHeight,int height);
    void Create_Tiberium_Patch(Cell const &position,int amount,int patchId,bool special);
    void Seed_Hill_Heights();
    void Init_Regions();
    void Init_Map(int fullInit);
    void Generate_Neutral_Buildings();
    void Create_Tiberium();
    void Generate_Hills();
    void Generate_Vegetation();
    // Controller0057A0D0 restores this owner in ECX before both calls; neither callee reads it.
    void Scatter_Green_Tiles();
    void Vary_Water_Tiles();
    bool Init_Start_Points();
    void Generate_Lights();
    void Generate_Arctic_Vegetation();
    void Place_Tile_Patch(CellClass *cell,int tile,int count,int origin,bool flag);
    int Place_Forest(CellClass *cell,int count,double density);
    void Generate_Urban_Areas();
    DynamicVectorClass<Cell> *Create_Urban_Area(Cell const &position,int count);
    void Generate_Urban_Roads(DynamicVectorClass<Cell> &cells);
    Cell Plan_Paved_Road_Junction(Cell const &cell,Rect const &rect,int dir);
    Cell Plan_Paved_Road(Cell const &cell,Rect const &rect,int dir);
    void Generate_Urban_Buildings(DynamicVectorClass<Cell> &cells);
    void Generate_Urban_Units(DynamicVectorClass<Cell> &cells);
    void Generate_Rural_Areas();
    void Generate_Rural_Buildings(Cell const &position);
    void Generate_Rural_Units(Cell const &position);
    bool Generate_Rural_Roads(Cell const &position);
    double Get_Angle_Score(Cell const &from,Cell const &to,double heading);
    void Cleanup();
    void Make_Regions();
    void unknown_00582fd0();
    void Read_Settings_00579960();
    int Pick_Alternate_Cliff(int tile); // caller ECXowner/RET4; operation name hypothesis
    bool Clear_Region_Border(int region,int rings,int newRegion); // ECXowner,RET12; ALpredicate
    DynamicVectorClass<Cell> *Build_Region_Border_Cell_List(int region); // ECXowner,RET4; existingprivateevidence
    void Generate_Urban_Pavement(DynamicVectorClass<Cell> &cells); // pendingr198
    void Fixup_Cliff_Corners(); // caller0058360A restores complete-owner ECX
    bool Expand_All_High_Ground(); //ALresult/RET0, receiver notread
    // The region of a cell's generator record, or -1 without generator data (TibSun
    // RandomMapGen.Get_Cell_Data_Region; ECX owner not read, RET4).
    int Get_Cell_Data_Region(Cell const &cell);
    // Sets the region of a cell's generator record (TibSun Set_Cell_Data_Region; RET8).
    void Set_Cell_Data_Region(Cell const &cell, int region_id);
    // Marks a cell's generator record as reached by a spread (TibSun Set_Cell_Data_Spread; RET8).
    void Set_Cell_Data_Spread(Cell const &cell, int patch_id);
    // The spread that last reached a cell's generator record (TibSun Get_Cell_Data_Spread; RET4).
    int Get_Cell_Data_Spread(Cell const &cell);
#ifdef RA2_MAPGEN_CLEAR_SPREADS_MEMBER
    // Clears every map cell's spread mark (TibSun Clear_Cell_Data_Spreads; never called, ECX not read).
    void Clear_Cell_Data_Spreads();
#endif
    ~MapGeneratorClass();
    MapGeneratorClass(); //00577010 remains parked, no body promoted
    // Proposed rename-only owner correction; same int,int/ECX/RET8 physical ABI.
    void Unknown_0057A0D0(int arg1,int arg2); //0057A0D0, operation unchanged; no body promoted
    void Seed_Water();
    // Observed ECX/plain-RET/ignored-result contracts; unobserved stack aliases and source intent remain hypotheses.
    void Seed_Land();
    void unknown_0057c4f0();
    void Partition_Rect(DynamicVectorClass<Rect> *regions,int count,Rect const &area); //0057C5B7 ECX owner, RET12
    int unknown_0057d3a0(int count,Rect const &area,Point2D const &seed,bool flag,Cell const &origin,double weight,bool last); //RET32; source intent hypotheses
#ifdef RA2_MAPGEN_SHAPE_MEMBER
    // A member: both callers (0x57C9E0, 0x57D3A0) pass this in ECX; ECX is not read (ra2-decomp FUN_0059BAB0).
    bool Cell_In_Generation_Shape_0057d290(Cell const &position,Rect const &area,bool ellipse,double x_weight,double y_weight);
#endif
    void Seed_Land_Type1();
    void Seed_Land_Type2();
    bool Seed_River(Cell const &point,double value,bool flag); //actualcallee, sourceintentqualified
    bool Seed_Lake(Cell const &point); //actualcallee, sourceintentqualified
    bool Grow_Water_Region(int region,float spread,Rect const &bounds,Cell const &seed,bool flag);
    bool Place_Waterfall(int region,Cell const &first,Cell const &second,int type,bool &placed,double &x,double &y);
    int Get_Target_Water_Amount() const; //name/constness hypotheses
    void Clear_Region_Cells(int source_index,int target_index,int level,int tile);
    MapSeedClass SeedData;
    int MapSeeder,MapPreview,LocalWidth,LocalHeight;
    TypeList<int> RMGLevelLightSettings,TemperateAmbientLight,SnowAmbientLight,TemperateAmbientRed,TemperateAmbientGreen;
    TypeList<int> TemperateAmbientBlue,SnowAmbientRed,SnowAmbientGreen,SnowAmbientBlue,RMGVegetationMinimums,RMGVegetationMaximums;
    int RMGMinimumTiberium,RMGMaximumTiberium;
    TypeList<BuildingTypeClass const *> TemperateOrePatchLamps,SnowOrePatchLamps;
    int MaxTrees;
    bool PreviewStale;
    int SeededWaterAmount;
    unsigned int WorkingRegionID; //32-bit increment counter; signedness unknown, unsignedpreserveswrap
    int CellHeight;
    bool PlaceWaterfall;
};
typedef char MapGeneratorClass_dim180[(((unsigned int)&(((MapGeneratorClass*)0)->LocalWidth))==0x180)?1:-1];
typedef char MapGeneratorClass_dim184[(((unsigned int)&(((MapGeneratorClass*)0)->LocalHeight))==0x184)?1:-1];
typedef char MapGeneratorClass_counter308[(((unsigned int)&(((MapGeneratorClass*)0)->WorkingRegionID))==0x308)?1:-1];
typedef char MapGeneratorClass_level30c[(((unsigned int)&(((MapGeneratorClass*)0)->CellHeight))==0x30c)?1:-1];
typedef char MapGeneratorClass_size[(sizeof(MapGeneratorClass)==0x314)?1:-1];
typedef char MapGeneratorClass_seed_prefix[(((unsigned int)&(((MapGeneratorClass*)0)->SeedData))==0)?1:-1];
// Proposed complete global view; original constructor/lifetime scaffolding stays in place.
// Declares storage elsewhere; no global definition or new initializer is introduced.
extern MapGeneratorClass RandomMapGen; //00A6FD68 (TibSun mapgen.h RandomMapGen)
#endif
