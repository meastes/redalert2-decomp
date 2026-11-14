# Route A container member residuals by cause

Listing only (lane C1, after the container member joins; docs/DECISIONS.md "Container member joins on their own
frozen bound"). Rows whose file is a `shared/vector_class.hpp` member and that stay in their own files. No
calibration is to be used to move them. The class-factory and TypeList members are other headers and are not
listed. Regenerate from manifests/original_target/modules.json and the join reports.

| Cause | Rows | Bytes | Units |
| --- | ---: | ---: | --- |
| The unit's own text does not use the list (not emitted by the unit's own use) | 195 | 13,431 | mapgen 33, house 20, wdtmap 20, dropship 14, building 10, coopcamp 10, isotype 10, netshare 10, revent 10, rocket2 10, spawnmanager 10, map 9, ddraw 7, ccini 5, globals 5, theme 4, rules 2, wdtnet 2, factory 1, wdtconflict 1, wdtvoice 1, wonline 1 |
| The unit's own rows differ without its member switches | 57 | 4,614 | session 37, multiplayer 18, uicmnds 2 |
| The unit without member switches emits a vtable that is not an original copy (strict audit) | 20 | 1,747 | alphashp 10, wdtnet 9, revent 1 |
| Its donor population changes when it leaves | 4 | 690 | foot 1, map 1, planmgr 1, teamtype 1 |
| The row needs header state the unit lacks | 4 | 381 | wdtmap 2, empulse 1, house 1 |
| **Total** | **280** | **20,863** | |

## The unit's own text does not use the list (not emitted by the unit's own use)

| Unit | Entry | Bytes | Kind | Member |
| --- | --- | ---: | --- | --- |
| building | 00453cc0 | 42 | split | DynamicVectorClass<ParticleSystemTypeClass *>::Clear |
| building | 00453cf0 | 27 | split | DynamicVectorClass<ParticleSystemTypeClass *>::ID_pointer |
| building | 00453d10 | 13 | split | VectorClass<ParticleSystemTypeClass *>::Fetch |
| building | 00453ed0 | 62 | split | VectorClass<ParticleSystemTypeClass *>::operator equal |
| building | 00453f10 | 174 | split | VectorClass<ParticleSystemTypeClass *>::Resize |
| building | 00453fc0 | 39 | split | VectorClass<ParticleSystemTypeClass *>::Clear |
| building | 00453ff0 | 51 | split | VectorClass<ParticleSystemTypeClass *>::ID_object |
| building | 00454030 | 27 | split | VectorClass<ParticleSystemTypeClass *>::ID_pointer |
| building | 00454050 | 176 | split | DynamicVectorClass<ParticleSystemTypeClass *>::Resize |
| building | 00454100 | 38 | split | DynamicVectorClass<ParticleSystemTypeClass *>::ID_object |
| ccini | 0046cd40 | 42 | split | DynamicVectorClass<RGBClass>::Clear |
| ccini | 0046cd70 | 35 | split | DynamicVectorClass<RGBClass>::ID_pointer |
| ccini | 0046cda0 | 37 | split | VectorClass<RGBClass>::Fetch |
| ccini | 0046d880 | 39 | split | VectorClass<RGBClass>::Clear |
| ccini | 0046d900 | 35 | split | VectorClass<RGBClass>::ID_pointer |
| coopcamp | 00491060 | 42 | split | DynamicVectorClass<char *>::Clear |
| coopcamp | 00491090 | 27 | split | DynamicVectorClass<char *>::ID_pointer |
| coopcamp | 004910b0 | 13 | split | VectorClass<char *>::Fetch |
| coopcamp | 004910e0 | 62 | split | VectorClass<char *>::operator equal |
| coopcamp | 00491120 | 174 | split | VectorClass<char *>::Resize |
| coopcamp | 004911d0 | 39 | split | VectorClass<char *>::Clear |
| coopcamp | 00491200 | 51 | split | VectorClass<char *>::ID_object |
| coopcamp | 00491240 | 27 | split | VectorClass<char *>::ID_pointer |
| coopcamp | 00491260 | 176 | split | DynamicVectorClass<char *>::Resize |
| coopcamp | 00491310 | 38 | split | DynamicVectorClass<char *>::ID_object |
| ddraw | 004964c0 | 13 | split | VectorClass<_DDSURFACEDESC *>::Fetch |
| ddraw | 004964d0 | 62 | split | VectorClass<_DDSURFACEDESC *>::operator equal |
| ddraw | 00496510 | 174 | split | VectorClass<_DDSURFACEDESC *>::Resize |
| ddraw | 004965c0 | 39 | split | VectorClass<_DDSURFACEDESC *>::Clear |
| ddraw | 004965f0 | 51 | split | VectorClass<_DDSURFACEDESC *>::ID_object |
| ddraw | 00496630 | 27 | split | VectorClass<_DDSURFACEDESC *>::ID_pointer |
| ddraw | 00496650 | 176 | split | DynamicVectorClass<_DDSURFACEDESC *>::Resize, 0x00496650. |
| dropship | 004a9b50 | 13 | split | VectorClass<ButtonFadeEffect *>::Fetch |
| dropship | 004a9bb0 | 13 | split | VectorClass<CrossDissolveEffect *>::Fetch |
| dropship | 004a9bc0 | 62 | split | VectorClass<ButtonFadeEffect *>::operator equal |
| dropship | 004a9c00 | 174 | split | VectorClass<ButtonFadeEffect *>::Resize |
| dropship | 004a9cb0 | 39 | split | VectorClass<ButtonFadeEffect *>::Clear |
| dropship | 004a9ce0 | 51 | split | VectorClass<ButtonFadeEffect *>::ID_object |
| dropship | 004a9d20 | 27 | split | VectorClass<ButtonFadeEffect *>::ID_pointer |
| dropship | 004a9d40 | 176 | split | Members: shared/vector_class.hpp (this file selects Resize only). |
| dropship | 004a9e20 | 62 | split | VectorClass<CrossDissolveEffect *>::operator equal |
| dropship | 004a9e60 | 174 | split | VectorClass<CrossDissolveEffect *>::Resize |
| dropship | 004a9f10 | 39 | split | VectorClass<CrossDissolveEffect *>::Clear |
| dropship | 004a9f40 | 51 | split | VectorClass<CrossDissolveEffect *>::ID_object |
| dropship | 004a9f80 | 27 | split | VectorClass<CrossDissolveEffect *>::ID_pointer |
| dropship | 004a9fa0 | 176 | split | Members: shared/vector_class.hpp (this file selects Resize only). |
| factory | 004ba090 | 27 | split | Original RTTI at vtable007A0F44 encodes pointers to const TechnoTypeClass. |
| globals | 004d4c40 | 13 | split | VectorClass<ThemeControl *>::Fetch |
| globals | 004d69b0 | 62 | flipped | VectorClass<ThemeControl *>::operator equal |
| globals | 004d69f0 | 174 | flipped | VectorClass<ThemeControl *>::Resize |
| globals | 004d6aa0 | 39 | flipped | VectorClass<ThemeControl *>::Clear |
| globals | 004d6ad0 | 51 | flipped | VectorClass<ThemeControl *>::ID_object |
| house | 004f6d40 | 42 | split | DynamicVectorClass<HouseClass::BuildChoiceClass *>::Clear |
| house | 004f6d70 | 27 | split | DynamicVectorClass<HouseClass::BuildChoiceClass *>::ID_pointer |
| house | 004f6d90 | 13 | split | VectorClass<HouseClass::BuildChoiceClass *>::Fetch |
| house | 004f6da0 | 42 | split | DynamicVectorClass<TeamTypeClass *>::Clear |
| house | 004f6dd0 | 27 | split | DynamicVectorClass<TeamTypeClass *>::ID_pointer |
| house | 004f6df0 | 13 | split | VectorClass<TeamTypeClass *>::Fetch |
| house | 004f7980 | 62 | split | VectorClass<HouseClass::BuildChoiceClass *>::operator equal |
| house | 004f79c0 | 174 | split | VectorClass<HouseClass::BuildChoiceClass *>::Resize |
| house | 004f7a70 | 39 | split | VectorClass<HouseClass::BuildChoiceClass *>::Clear |
| house | 004f7aa0 | 51 | split | VectorClass<HouseClass::BuildChoiceClass *>::ID_object |
| house | 004f7ae0 | 27 | split | VectorClass<HouseClass::BuildChoiceClass *>::ID_pointer |
| house | 004f7b00 | 176 | split | DynamicVectorClass<HouseClass::BuildChoiceClass *>::Resize |
| house | 004f7bb0 | 38 | split | DynamicVectorClass<HouseClass::BuildChoiceClass *>::ID_object |
| house | 004f7be0 | 62 | split | VectorClass<TeamTypeClass *>::operator equal |
| house | 004f7c20 | 174 | split | VectorClass<TeamTypeClass *>::Resize |
| house | 004f7cd0 | 39 | split | VectorClass<TeamTypeClass *>::Clear |
| house | 004f7d00 | 51 | split | VectorClass<TeamTypeClass *>::ID_object |
| house | 004f7d40 | 27 | split | VectorClass<TeamTypeClass *>::ID_pointer |
| house | 004f7d60 | 176 | split | DynamicVectorClass<TeamTypeClass *>::Resize |
| house | 004f7e10 | 38 | split | DynamicVectorClass<TeamTypeClass *>::ID_object |
| isotype | 0052e730 | 42 | split | DynamicVectorClass<IsometricTileTypeClass::TileInsertType *>::Clear |
| isotype | 0052e760 | 27 | split | DynamicVectorClass<IsometricTileTypeClass::TileInsertType *>::ID_pointer |
| isotype | 0052e780 | 13 | split | VectorClass<IsometricTileTypeClass::TileInsertType *>::Fetch |
| isotype | 0052e9f0 | 62 | split | VectorClass<IsometricTileTypeClass::TileInsertType *>::operator equal |
| isotype | 0052ea30 | 174 | split | VectorClass<IsometricTileTypeClass::TileInsertType *>::Resize |
| isotype | 0052eae0 | 39 | split | VectorClass<IsometricTileTypeClass::TileInsertType *>::Clear |
| isotype | 0052eb10 | 51 | split | VectorClass<IsometricTileTypeClass::TileInsertType *>::ID_object |
| isotype | 0052eb50 | 27 | split | VectorClass<IsometricTileTypeClass::TileInsertType *>::ID_pointer |
| isotype | 0052eb70 | 176 | split | DynamicVectorClass<IsometricTileTypeClass::TileInsertType *>::Resize |
| isotype | 0052ec20 | 38 | split | DynamicVectorClass<IsometricTileTypeClass::TileInsertType *>::ID_object |
| map | 0056adb0 | 42 | split | DynamicVectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::Clear |
| map | 0056ade0 | 36 | split | DynamicVectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::ID_pointer |
| map | 0056aee0 | 144 | split | DynamicVectorClass<SubzoneConnectionStruct>::DynamicVectorClass(unsigned, SubzoneConnectio |
| map | 0056bcd0 | 174 | split | VectorClass<PassabilityType>::Resize |
| map | 0056bd80 | 39 | split | VectorClass<PassabilityType>::Clear |
| map | 0056bdf0 | 27 | split | VectorClass<PassabilityType>::ID_pointer |
| map | 0056be10 | 176 | split | DynamicVectorClass<PassabilityType>::Resize |
| map | 0056c7d0 | 39 | split | VectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::Clear |
| map | 0056c840 | 36 | split | VectorClass<HashObject<unsigned int, SubzoneConnectionStruct>>::ID_pointer |
| mapgen | 0058db80 | 132 | split | Original callers6456D1/645BC1 take the TypeList<int> returned by Get_Voc_List. |
| mapgen | 0058dc10 | 42 | split | DynamicVectorClass<MapRegionClass *>::Clear |
| mapgen | 0058dc40 | 27 | split | DynamicVectorClass<MapRegionClass *>::ID_pointer |
| mapgen | 0058dc60 | 13 | split | VectorClass<MapRegionClass *>::Fetch |
| mapgen | 0058dca0 | 132 | split | Original caller64C68E assigns RulesClass.BuildBarracks at+81C;13 call sites |
| mapgen | 0058dd80 | 13 | split | VectorClass<_WIN32_FIND_DATAA *>::Fetch |
| mapgen | 0058dde0 | 47 | split | VectorClass<TRect<int>>::Fetch |
| mapgen | 0058de10 | 42 | split | DynamicVectorClass<CellClass *>::Clear |
| mapgen | 0058deb0 | 13 | split | VectorClass<double>::Fetch |
| mapgen | 0058df10 | 62 | split | VectorClass<MapRegionClass *>::operator equal |
| mapgen | 0058df50 | 174 | split | VectorClass<MapRegionClass *>::Resize |
| mapgen | 0058e030 | 51 | split | VectorClass<MapRegionClass *>::ID_object |
| mapgen | 0058e070 | 27 | split | VectorClass<MapRegionClass *>::ID_pointer |
| mapgen | 0058e090 | 176 | split | DynamicVectorClass<MapRegionClass *>::Resize |
| mapgen | 0058e140 | 38 | split | DynamicVectorClass<MapRegionClass *>::ID_object |
| mapgen | 0058e2d0 | 62 | split | VectorClass<_WIN32_FIND_DATAA *>::operator equal |
| mapgen | 0058e310 | 174 | split | VectorClass<_WIN32_FIND_DATAA *>::Resize |
| mapgen | 0058e3c0 | 39 | split | VectorClass<_WIN32_FIND_DATAA *>::Clear |
| mapgen | 0058e3f0 | 51 | split | VectorClass<_WIN32_FIND_DATAA *>::ID_object |
| mapgen | 0058e430 | 27 | split | VectorClass<_WIN32_FIND_DATAA *>::ID_pointer |
| mapgen | 0058e450 | 176 | split | Actual original RTTI at7A5E8C names DynamicVector<_WIN32_FIND_DATAA*>. |
| mapgen | 0058e530 | 102 | split | VectorClass<Rect>::operator== |
| mapgen | 0058e5a0 | 226 | split | VectorClass<Rect>::Resize |
| mapgen | 0058e690 | 39 | split | VectorClass<TRect<int>>::Clear |
| mapgen | 0058e6c0 | 90 | split | VectorClass<TRect<int>>::ID_object |
| mapgen | 0058e720 | 27 | split | VectorClass<TRect<int>>::ID_pointer |
| mapgen | 0058e880 | 176 | split | DynamicVectorClass<CellClass *>::Resize |
| mapgen | 0058e930 | 38 | split | DynamicVectorClass<CellClass *>::ID_object |
| mapgen | 0058e960 | 62 | split | VectorClass<double>::operator equal |
| mapgen | 0058e9a0 | 182 | split | VectorClass<double>::Resize |
| mapgen | 0058ea90 | 66 | split | VectorClass<double>::ID_object |
| mapgen | 0058eae0 | 27 | split | VectorClass<double>::ID_pointer |
| mapgen | 0058eb00 | 189 | split | Actual original RTTI at7A5FAC is .?AV?$DynamicVectorClass@N@@. |
| netshare | 005cd590 | 42 | split | DynamicVectorClass<MultiMission *>::Clear |
| netshare | 005cd5c0 | 27 | split | DynamicVectorClass<MultiMission *>::ID_pointer |
| netshare | 005cd5e0 | 13 | split | VectorClass<MultiMission *>::Fetch |
| netshare | 005cd5f0 | 62 | split | VectorClass<MultiMission *>::operator equal |
| netshare | 005cd630 | 174 | split | VectorClass<MultiMission *>::Resize |
| netshare | 005cd6e0 | 39 | split | VectorClass<MultiMission *>::Clear |
| netshare | 005cd710 | 51 | split | VectorClass<MultiMission *>::ID_object |
| netshare | 005cd750 | 27 | split | VectorClass<MultiMission *>::ID_pointer |
| netshare | 005cd7d0 | 176 | split | DynamicVectorClass<MultiMission *>::Resize |
| netshare | 005cd8d0 | 38 | split | DynamicVectorClass<MultiMission *>::ID_object |
| revent | 0063c7c0 | 42 | split | DynamicVectorClass<RadarEventClass *>::Clear |
| revent | 0063c7f0 | 27 | split | DynamicVectorClass<RadarEventClass *>::ID_pointer |
| revent | 0063c810 | 13 | split | VectorClass<RadarEventClass *>::Fetch |
| revent | 0063c820 | 62 | split | VectorClass<RadarEventClass *>::operator equal |
| revent | 0063c860 | 174 | split | VectorClass<RadarEventClass *>::Resize |
| revent | 0063c910 | 39 | split | VectorClass<RadarEventClass *>::Clear |
| revent | 0063c940 | 51 | split | VectorClass<RadarEventClass *>::ID_object |
| revent | 0063c980 | 27 | split | VectorClass<RadarEventClass *>::ID_pointer |
| revent | 0063c9a0 | 176 | split | DynamicVectorClass<RadarEventClass *>::Resize |
| revent | 0063ca90 | 38 | split | DynamicVectorClass<RadarEventClass *>::ID_object |
| rocket2 | 0063ff10 | 42 | split | DynamicVectorClass<OwnerTalkClass::ConnectionListStruct *>::Clear |
| rocket2 | 0063ff40 | 27 | split | DynamicVectorClass<OwnerTalkClass::ConnectionListStruct *>::ID_pointer |
| rocket2 | 0063ff60 | 13 | split | VectorClass<OwnerTalkClass::ConnectionListStruct *>::Fetch |
| rocket2 | 0063ff70 | 62 | split | VectorClass<OwnerTalkClass::ConnectionListStruct *>::operator equal |
| rocket2 | 0063ffb0 | 174 | split | VectorClass<OwnerTalkClass::ConnectionListStruct *>::Resize |
| rocket2 | 00640060 | 39 | split | VectorClass<OwnerTalkClass::ConnectionListStruct *>::Clear |
| rocket2 | 00640090 | 51 | split | VectorClass<OwnerTalkClass::ConnectionListStruct *>::ID_object |
| rocket2 | 006400d0 | 27 | split | VectorClass<OwnerTalkClass::ConnectionListStruct *>::ID_pointer |
| rocket2 | 006400f0 | 176 | split | DynamicVectorClass<OwnerTalkClass::ConnectionListStruct *>::Resize |
| rocket2 | 006401a0 | 38 | split | DynamicVectorClass<OwnerTalkClass::ConnectionListStruct *>::ID_object |
| rules | 00652840 | 45 | split | VectorClass<InfantryTypeClass *>::~VectorClass |
| rules | 00652870 | 45 | split | VectorClass<TerrainTypeClass *>::~VectorClass |
| spawnmanager | 0068aa20 | 42 | split | DynamicVectorClass<SpawnControl *>::Clear |
| spawnmanager | 0068aa50 | 27 | split | DynamicVectorClass<SpawnControl *>::ID_pointer |
| spawnmanager | 0068aa70 | 13 | split | VectorClass<SpawnControl *>::Fetch |
| spawnmanager | 0068aae0 | 62 | split | VectorClass<SpawnControl *>::operator equal |
| spawnmanager | 0068ab20 | 174 | split | VectorClass<SpawnControl *>::Resize |
| spawnmanager | 0068abd0 | 39 | split | VectorClass<SpawnControl *>::Clear |
| spawnmanager | 0068ac00 | 51 | split | VectorClass<SpawnControl *>::ID_object |
| spawnmanager | 0068ac40 | 27 | split | VectorClass<SpawnControl *>::ID_pointer |
| spawnmanager | 0068ac60 | 176 | split | DynamicVectorClass<SpawnControl *>::Resize |
| spawnmanager | 0068ad10 | 38 | split | DynamicVectorClass<SpawnControl *>::ID_object |
| theme | 006e85b0 | 42 | split | DynamicVectorClass<ThemeControl *>::Clear |
| theme | 006e85e0 | 27 | split | DynamicVectorClass<ThemeControl *>::ID_pointer |
| theme | 006e8600 | 176 | split | DynamicVectorClass<ThemeControl *>::Resize |
| theme | 006e86b0 | 38 | split | DynamicVectorClass<ThemeControl *>::ID_object |
| wdtconflict | 00727bc0 | 27 | split | DynamicVectorClass<rc_ptr<WorldDominationTour::GameOption>>::ID_pointer |
| wdtmap | 0072af80 | 43 | split | DynamicVectorClass<rc_ptr<WorldDominationTour::Territory>>::Clear |
| wdtmap | 0072afb0 | 27 | split | DynamicVectorClass<rc_ptr<WorldDominationTour::Territory>>::ID_pointer |
| wdtmap | 0072afd0 | 47 | split | VectorClass<rc_ptr<WorldDominationTour::Territory>>::Fetch |
| wdtmap | 0072b000 | 42 | split | DynamicVectorClass<WorldDominationTour::Map::AnimationPalette *>::Clear |
| wdtmap | 0072b030 | 27 | split | DynamicVectorClass<WorldDominationTour::Map::AnimationPalette *>::ID_pointer |
| wdtmap | 0072b050 | 13 | split | VectorClass<WorldDominationTour::Map::AnimationPalette *>::Fetch |
| wdtmap | 0072b060 | 40 | split | VectorClass<WorldDominationTour::Centroid>::Fetch |
| wdtmap | 0072b110 | 70 | split | VectorClass<rc_ptr<WorldDominationTour::Territory>>::operator== |
| wdtmap | 0072b2b0 | 55 | split | VectorClass<rc_ptr<WorldDominationTour::Territory>>::ID_object |
| wdtmap | 0072b2f0 | 27 | split | VectorClass<rc_ptr<WorldDominationTour::Territory>>::ID_pointer |
| wdtmap | 0072b310 | 285 | split | DynamicVectorClass<rc_ptr<WorldDominationTour::Territory>>::Resize |
| wdtmap | 0072b430 | 42 | split | DynamicVectorClass<rc_ptr<WorldDominationTour::Territory>>::ID_object |
| wdtmap | 0072b460 | 62 | split | VectorClass<WorldDominationTour::Map::AnimationPalette *>::operator equal |
| wdtmap | 0072b580 | 51 | split | VectorClass<WorldDominationTour::Map::AnimationPalette *>::ID_object |
| wdtmap | 0072b5c0 | 27 | split | VectorClass<WorldDominationTour::Map::AnimationPalette *>::ID_pointer |
| wdtmap | 0072b5e0 | 176 | split | DynamicVectorClass<WorldDominationTour::Map::AnimationPalette *>::Resize |
| wdtmap | 0072b690 | 38 | split | DynamicVectorClass<WorldDominationTour::Map::AnimationPalette *>::ID_object |
| wdtmap | 0072b6c0 | 92 | split | VectorClass<WorldDominationTour::Centroid>::operator== |
| wdtmap | 0072b850 | 80 | split | VectorClass<WorldDominationTour::Centroid>::ID_object |
| wdtmap | 0072b8a0 | 36 | split | VectorClass<WorldDominationTour::Centroid>::ID_pointer |
| wdtnet | 0072ca50 | 27 | split | DynamicVectorClass<rc_ptr<WDTTerritory>>::ID_pointer |
| wdtnet | 0072cd60 | 27 | split | VectorClass<rc_ptr<WDTTerritory>>::ID_pointer |
| wdtvoice | 00732c50 | 45 | split | VectorClass<unsigned int>::~VectorClass |
| wonline | 007666a0 | 176 | split | DynamicVectorClass<tConnInfoStruct *>::Resize |

## The unit's own rows differ without its member switches

| Unit | Entry | Bytes | Kind | Member |
| --- | --- | ---: | --- | --- |
| multiplayer | 005b8fa0 | 42 | split | DynamicVectorClass<MultiplayerTeam *>::Clear |
| multiplayer | 005b8fd0 | 27 | split | DynamicVectorClass<MultiplayerTeam *>::ID_pointer |
| multiplayer | 005b8ff0 | 13 | split | VectorClass<MultiplayerTeam *>::Fetch |
| multiplayer | 005b9050 | 13 | split | VectorClass<MultiplayerGameMode *>::Fetch |
| multiplayer | 005b91e0 | 62 | split | VectorClass<MultiplayerTeam *>::operator equal |
| multiplayer | 005b9220 | 174 | split | VectorClass<MultiplayerTeam *>::Resize |
| multiplayer | 005b92d0 | 39 | split | VectorClass<MultiplayerTeam *>::Clear |
| multiplayer | 005b9300 | 51 | split | VectorClass<MultiplayerTeam *>::ID_object |
| multiplayer | 005b9340 | 27 | split | VectorClass<MultiplayerTeam *>::ID_pointer |
| multiplayer | 005b9360 | 176 | split | DynamicVectorClass<MultiplayerTeam *>::Resize |
| multiplayer | 005b9410 | 38 | split | DynamicVectorClass<MultiplayerTeam *>::ID_object |
| multiplayer | 005b9440 | 62 | split | VectorClass<MultiplayerGameMode *>::operator equal |
| multiplayer | 005b9480 | 174 | split | VectorClass<MultiplayerGameMode *>::Resize |
| multiplayer | 005b9530 | 39 | split | VectorClass<MultiplayerGameMode *>::Clear |
| multiplayer | 005b9560 | 51 | split | VectorClass<MultiplayerGameMode *>::ID_object |
| multiplayer | 005b95a0 | 27 | split | VectorClass<MultiplayerGameMode *>::ID_pointer |
| multiplayer | 005b9620 | 176 | split | DynamicVectorClass<MultiplayerGameMode *>::Resize |
| multiplayer | 005b96d0 | 38 | split | DynamicVectorClass<MultiplayerGameMode *>::ID_object |
| session | 006737d0 | 42 | split | DynamicVectorClass<DiskID>::Clear |
| session | 00673800 | 27 | split | DynamicVectorClass<DiskID>::ID_pointer |
| session | 00673830 | 83 | split | DynamicVectorClass<Wstring>::Clear |
| session | 00673890 | 27 | split | DynamicVectorClass<Wstring>::ID_pointer |
| session | 006738b0 | 29 | split | VectorClass<Wstring>::Fetch |
| session | 00673920 | 13 | split | VectorClass<MultiMission *>::Fetch |
| session | 00673980 | 13 | split | VectorClass<GlobalPacketType *>::Fetch |
| session | 006739e0 | 13 | split | VectorClass<PhoneEntryClass *>::Fetch |
| session | 00673a80 | 174 | split | VectorClass<DiskID>::Resize |
| session | 00673b30 | 39 | split | VectorClass<DiskID>::Clear |
| session | 00673ba0 | 27 | split | VectorClass<DiskID>::ID_pointer |
| session | 00673bc0 | 176 | split | DynamicVectorClass<DiskID>::Resize |
| session | 00673ca0 | 76 | split | VectorClass<Wstring>::operator== |
| session | 00673cf0 | 320 | split | VectorClass<Wstring>::Resize |
| session | 00673e30 | 80 | split | VectorClass<Wstring>::Clear |
| session | 00673e80 | 74 | split | VectorClass<Wstring>::ID_object |
| session | 00673ed0 | 27 | split | VectorClass<Wstring>::ID_pointer |
| session | 00673ef0 | 313 | split | DynamicVectorClass<Wstring>::Resize |
| session | 00674030 | 59 | split | DynamicVectorClass<Wstring>::ID_object |
| session | 00674070 | 62 | split | VectorClass<MultiMission *>::operator equal |
| session | 006740b0 | 174 | split | VectorClass<MultiMission *>::Resize |
| session | 00674160 | 39 | split | VectorClass<MultiMission *>::Clear |
| session | 00674190 | 51 | split | VectorClass<MultiMission *>::ID_object |
| session | 006741d0 | 27 | split | VectorClass<MultiMission *>::ID_pointer |
| session | 006741f0 | 176 | split | DynamicVectorClass<MultiMission *>::Resize |
| session | 006742d0 | 62 | split | VectorClass<GlobalPacketType *>::operator equal |
| session | 00674310 | 174 | split | VectorClass<GlobalPacketType *>::Resize |
| session | 006743c0 | 39 | split | VectorClass<GlobalPacketType *>::Clear |
| session | 006743f0 | 51 | split | VectorClass<GlobalPacketType *>::ID_object |
| session | 00674430 | 27 | split | VectorClass<GlobalPacketType *>::ID_pointer |
| session | 00674450 | 176 | split | DynamicVectorClass<GlobalPacketType *>::Resize |
| session | 00674530 | 62 | split | VectorClass<PhoneEntryClass *>::operator equal |
| session | 00674570 | 174 | split | VectorClass<PhoneEntryClass *>::Resize |
| session | 00674620 | 39 | split | VectorClass<PhoneEntryClass *>::Clear |
| session | 00674650 | 51 | split | VectorClass<PhoneEntryClass *>::ID_object |
| session | 00674690 | 27 | split | VectorClass<PhoneEntryClass *>::ID_pointer |
| session | 006746b0 | 176 | split | DynamicVectorClass<PhoneEntryClass *>::Resize |
| uicmnds | 006f8a80 | 93 | split | DynamicVectorClass<TechnoClass *>::DynamicVectorClass(unsigned size, TechnoClass *const *a |
| uicmnds | 006f8ae0 | 93 | split | DynamicVectorClass<char const *>::DynamicVectorClass(unsigned size, char const *const *arr |

## The unit without member switches emits a vtable that is not an original copy (strict audit)

| Unit | Entry | Bytes | Kind | Member |
| --- | --- | ---: | --- | --- |
| alphashp | 0041fa90 | 42 | split | DynamicVectorClass<AlphaShapeClass *>::Clear |
| alphashp | 0041fac0 | 27 | split | DynamicVectorClass<AlphaShapeClass *>::ID_pointer |
| alphashp | 0041fae0 | 13 | split | VectorClass<AlphaShapeClass *>::Fetch |
| alphashp | 0041faf0 | 62 | split | VectorClass<AlphaShapeClass *>::operator equal |
| alphashp | 0041fb30 | 174 | split | VectorClass<AlphaShapeClass *>::Resize |
| alphashp | 0041fbe0 | 39 | split | VectorClass<AlphaShapeClass *>::Clear |
| alphashp | 0041fc10 | 51 | split | VectorClass<AlphaShapeClass *>::ID_object |
| alphashp | 0041fc50 | 27 | split | VectorClass<AlphaShapeClass *>::ID_pointer |
| alphashp | 0041fc70 | 176 | split | DynamicVectorClass<AlphaShapeClass *>::Resize |
| alphashp | 0041fd20 | 38 | split | DynamicVectorClass<AlphaShapeClass *>::ID_object |
| revent | 0063ca50 | 49 | split | bool DynamicVectorClass<RadarEventClass *>::Delete(int index) |
| wdtnet | 0072ca20 | 43 | split | DynamicVectorClass<rc_ptr<WDTTerritory>>::Clear |
| wdtnet | 0072ca70 | 47 | split | VectorClass<rc_ptr<WDTTerritory>>::Fetch |
| wdtnet | 0072cac0 | 185 | split | VectorClass<rc_ptr<WDTTerritory>>::operator=(VectorClass<rc_ptr<WDTTerritory>> const &) |
| wdtnet | 0072cb80 | 70 | split | VectorClass<rc_ptr<WDTTerritory>>::operator== |
| wdtnet | 0072cbd0 | 282 | split | VectorClass<rc_ptr<WDTTerritory>>::Resize |
| wdtnet | 0072ccf0 | 40 | split | VectorClass<rc_ptr<WDTTerritory>>::Clear |
| wdtnet | 0072cd20 | 55 | split | VectorClass<rc_ptr<WDTTerritory>>::ID_object |
| wdtnet | 0072cd80 | 285 | split | DynamicVectorClass<rc_ptr<WDTTerritory>>::Resize |
| wdtnet | 0072cea0 | 42 | split | DynamicVectorClass<rc_ptr<WDTTerritory>>::ID_object |

## Its donor population changes when it leaves

| Unit | Entry | Bytes | Kind | Member |
| --- | --- | ---: | --- | --- |
| foot | 004ce710 | 165 | split | DynamicVectorClass<DiscreteDistributionClass<CellClass>::DistributionObject<CellClass *>>: |
| map | 0056b330 | 298 | split | bool DynamicVectorClass<ZoneConnectionClass>::Resize(int newsize, ZoneConnectionClass cons |
| planmgr | 0061ad90 | 65 | split | DynamicVectorClass<PlanningMemberClass *>::Delete(PlanningMemberClass * const &) |
| teamtype | 006c09a0 | 162 | split | VectorClass<DiscreteDistributionClass<AITriggerTypeClass>::DistributionObject<AITriggerTyp |

## The row needs header state the unit lacks

| Unit | Entry | Bytes | Kind | Member |
| --- | --- | ---: | --- | --- |
| empulse | 004b5550 | 174 | split | Container members stay undefined from the header here: this file keeps its own Resize |
| house | 004f7270 | 161 | split | bool DynamicVectorClass<BaseNodeClass>::Insert_After(BaseNodeClass const &object, int inde |
| wdtmap | 0072af50 | 21 | split |  |
| wdtmap | 0072b090 | 25 | split |  |
