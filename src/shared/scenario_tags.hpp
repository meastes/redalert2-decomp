// The scenario's tag lists (Westwood's LogicTags and MapTags): the tags of the
// tag types that attach to the world (0x00866160) and to the map (0x00866238).
// Fill_In_Data 0x0065CDD0 fills them, Clear_Scenario 0x0065D370 clears them,
// TeamClass 0x006B8BB0 springs the world tags. Defined by their dynamic
// initializers 0x004D47E0 and 0x004D4760.
//
// A header of its own: an extern added to shared/tag.hpp or shared/id_source.hpp
// shifts a load pair in BaseClass::Read_INI 0x0042C460 (header hygiene).
#ifndef RA2_SHARED_SCENARIO_TAGS_HPP
#define RA2_SHARED_SCENARIO_TAGS_HPP

#include "shared/vector_class_decl.hpp"

class TagClass;

extern DynamicVectorClass<TagClass *> LogicTags;      // 0x00866160 (LogicTags)
extern DynamicVectorClass<TagClass *> MapTags;      // 0x00866238 (MapTags)
extern int MapTriggerID;                         // 0x00A35E24 (MapTriggerID: the map tag being processed; FootClass slot 93)

#endif
