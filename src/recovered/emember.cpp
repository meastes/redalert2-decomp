// emember.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/emember.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/task_force.hpp"
#include "shared/infantry_type.hpp"
#include "shared/unit_type.hpp"
#include "shared/aircraft_type.hpp"


// Header-static dynamic initializers of the unit at 0x004B4690:  the
// shared/unknown_geometry.hpp constants (0x004B4690-0x004B4851); the other
// drafts of this plan are emitted by these includes.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


// Emitted by the per-unit shared headers.


extern "C" int __cdecl sscanf(char const *,char const *,...);
extern "C" char TaskForceFormat_007d6510[];
TaskForceEntryStruct::TaskForceEntryStruct(char const *text):Amount(0),Type(0) {
 char name[64];
 int count;
 sscanf(text,TaskForceFormat_007d6510,&count,name);
 Amount=count;
 int index=InfantryTypeClass::From_Name(name);
 if(index!=-1) { Type=InfantryTypes[index];return; }
 index=UnitTypeClass::From_Name(name);
 if(index!=-1) { Type=UnitTypes[index];return; }
 index=AircraftTypeClass::From_Name(name);
 if(index!=-1) Type=AircraftTypes[index];
}
extern "C" int __cdecl sprintf(char *,char const *,...);
extern "C" char TaskForceText_00855960[64];


char const *TaskForceEntryStruct::Text() const {
 sprintf(TaskForceText_00855960,TaskForceFormat_007d6510,Amount,(char const *)Type->ID);
 return TaskForceText_00855960;
}


void TaskForceEntryStruct::Compute_CRC_004B4930(WWCRCEngine &crc) const
{
    crc((long)Amount);
    if(Type) {
        crc((long)Type->Kind_Of());
        crc(Type->Fetch_ID());
    }
}
