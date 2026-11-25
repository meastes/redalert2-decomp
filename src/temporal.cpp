/*
**	Command & Conquer Red Alert 2
**	TEMPORAL.CPP: the chrono legionnaire's temporal beam.
**
**	A temporal object belongs to the firing unit. It holds one target and erases it from time
**	step by step; several beams on one target form a chain (the first one does the work, the
**	others add their warp per step). When the target is gone it is removed from the game and the
**	firing unit is credited with the kill.
*/

#include "always.h"
#include "geometry.h"
#include "rect.h"
#include "temporal.h"
#include "anim.h"
#include "building.h"
#include "capmgr.h"
#include "globals.h"
#include "house.h"
#include "revent.h"
#include "rules.h"
#include "_rules.h"
#include "spawnmanager.h"
#include "super.h"
#include "swizzle.h"
#include "techno.h"
#include "techtype.h"
#include "unit.h"
#include "unittype.h"
#include "vox.h"
#include "_weapon.h"
#include "weapon.h"


DynamicVectorClass<TemporalClass *> Temporals;


TemporalClass::TemporalClass(void)
{
	Temporals.Add(this);
}


TemporalClass::TemporalClass(TechnoClass * owner)
{
	Owner = owner;
	Target = NULL;
	LifeTimer = 0;
	NextTemporal = NULL;
	PrevTemporal = NULL;
	Temporals.Add(this);
	WarpRemaining = 0;
	WarpPerStep = 0;
	Unused_38 = NULL;
	SourceSW = NULL;
}


TemporalClass::TemporalClass(NoInitClass const & x) :
	AbstractClass(x)
{
}


TemporalClass::~TemporalClass(void)
{
	Temporals.Delete(this);
}


void TemporalClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
}


HRESULT STDMETHODCALLTYPE TemporalClass::Load(IStream * stream)
{
	HRESULT hr = BASECLASS::Load(stream);
	if (SUCCEEDED(hr)) {
		new (this) TemporalClass(NoInitClass());
		Swizzle_Pointer(&Owner);
		Swizzle_Pointer(&Target);
		Swizzle_Pointer(&NextTemporal);
		Swizzle_Pointer(&PrevTemporal);
		Swizzle_Pointer(&Unused_38);
		Swizzle_Pointer(&SourceSW);
	}
	return(hr);
}


HRESULT STDMETHODCALLTYPE TemporalClass::Save(IStream * stream, BOOL cleardirty)
{
	return(BASECLASS::Save(stream, cleardirty));
}


HRESULT STDMETHODCALLTYPE TemporalClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) {
		return(E_POINTER);
	}
	*retval = CLSID_TemporalClass;
	return(S_OK);
}


/*
**	The target loses the warp of this beam and of every helper; at none left it is gone.
*/
void TemporalClass::AI(void)
{
	if (Target != NULL && Target->TemporalTargetingMe == this && NextTemporal != NULL) {
		Target->TemporalTargetingMe = NULL;
		Target->BeingWarpedOut = false;
		Detach();
		return;
	}

	WarpRemaining -= GetWarpPerStep(0);
	if (WarpRemaining <= 0) {
		if (Target == NULL) {
			Target = NULL;
			PrevTemporal = NULL;
			NextTemporal = NULL;
			SourceSW = NULL;
			Unused_38 = NULL;
		} else {
			new AnimClass(Rule->WarpAway, Target->PositionCoord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL), 0);
		}

		if (Owner->Techno_Type_Class()->IsTrainable) {
			TechnoTypeClass const * victim = Target->Techno_Type_Class();
			TechnoTypeClass const * killer = Owner->Techno_Type_Class();
			Owner->Veterancy.Made_A_Kill(killer->Cost_Of(Owner->House), victim->Cost_Of(Target->House));
		}

		if (Target != NULL) {
			if (Target->Fetch_RTTI() == RTTI_BUILDING) {
				BuildingClass * building = (BuildingClass *)Target;
				if (building != NULL) {
					if (building->GetOccupantCount() > 0) {
						building->KillOccupants(NULL);
					}
					if (!ScenarioInit) {
						while (building->Cargo.Is_Something_Attached()) {
							FootClass * passenger = building->Cargo.Detach_Object();
							delete passenger;
						}
					}
				}
				if (SourceSW != NULL) {
					SourceSW->Suspend(false);
					SourceSW = NULL;
				}
				Target->Death_Announcement(Owner);
				Target->Record_The_Kill(Owner);
				Target->Delete_Me();
				building->House->IsRecalcNeeded = true;
			} else {
				Target->Death_Announcement(Owner);
				Target->Record_The_Kill(Owner);
				Target->Delete_Me();
			}
		}

		Target = NULL;
		PrevTemporal = NULL;
		NextTemporal = NULL;
		SourceSW = NULL;
		Unused_38 = NULL;
	}
}


/*
**	The warp per step of this beam and of the helpers behind it (fifty at most).
*/
int TemporalClass::GetWarpPerStep(int helpercount)
{
	int helpers = 0;
	if (PrevTemporal != NULL && helpercount <= 50) {
		helpers = PrevTemporal->GetWarpPerStep(helpercount + 1);
	}
	WarpPerStep = Owner->Get_Class_Weapon_Data(Owner->What_Weapon_Should_I_Use(NULL))->Weapon->Attack;
	return(WarpPerStep + helpers);
}


/*
**	An object is going away: the owner lets go; the target is simply forgotten.
*/
void TemporalClass::Detach_Techno(AbstractClass const * target)
{
	if (target == Owner) {
		if (Target != NULL) {
			LetGo();
			Target = NULL;
		}
	} else if (target == Target) {
		Target = NULL;
		PrevTemporal = NULL;
		NextTemporal = NULL;
		SourceSW = NULL;
		Unused_38 = NULL;
	}
}


/*
**	Lets go of the target: the chain closes up behind this beam, or (the last beam) the target
**	is free again and a building is powered.
*/
void TemporalClass::LetGo(void)
{
	if (NextTemporal == NULL) {
		if (PrevTemporal != NULL) {
			if (Target != NULL) {
				Target->TemporalTargetingMe = PrevTemporal;
			}
			PrevTemporal->NextTemporal = NULL;
			PrevTemporal->WarpRemaining = WarpRemaining;
		} else if (Target != NULL) {
			Target->TemporalTargetingMe = NULL;
			Target->BeingWarpedOut = false;
			if (Target->Fetch_RTTI() == RTTI_BUILDING) {
				BuildingClass * building = (BuildingClass *)Target;
				building->House->RecalcPower = true;
				building->EnableTemporal();
				building->House->IsRecalcNeeded = true;
			}
			Target->Mark(MARK_CHANGE);
		}
	} else {
		if (PrevTemporal != NULL) {
			PrevTemporal->NextTemporal = NextTemporal;
			if (NextTemporal != NULL) {
				NextTemporal->PrevTemporal = PrevTemporal;
			}
		} else {
			NextTemporal->PrevTemporal = NULL;
		}
	}
	Target = NULL;
	PrevTemporal = NULL;
	NextTemporal = NULL;
	SourceSW = NULL;
	Unused_38 = NULL;
}


void TemporalClass::Release_Target(void)
{
	if (Target != NULL) {
		Target->TemporalTargetingMe = NULL;
		Target->BeingWarpedOut = false;
		Target = NULL;
	}
}


void TemporalClass::Restore_Target(void)
{
	if (Target != NULL) {
		Target->TemporalTargetingMe = NULL;
		Target->BeingWarpedOut = false;
		if (Target->Fetch_RTTI() == RTTI_BUILDING) {
			BuildingClass * building = (BuildingClass *)Target;
			building->House->RecalcPower = true;
			building->EnableTemporal();
			building->House->IsRecalcNeeded = true;
		}
		Target->Mark(MARK_CHANGE);
	}
}


void TemporalClass::Empty(void)
{
}


/*
**	Lets go of the target at once and detaches both neighbours of the chain.
*/
void TemporalClass::JustLetGo(void)
{
	if (Target != NULL) {
		Target->BeingWarpedOut = false;
		Target->TemporalTargetingMe = NULL;
		Target = NULL;
	}
	if (PrevTemporal != NULL) {
		if (PrevTemporal->NextTemporal == this) {
			PrevTemporal->NextTemporal = NULL;
		}
		PrevTemporal->Detach();
		PrevTemporal = NULL;
	}
	if (NextTemporal != NULL) {
		if (NextTemporal->PrevTemporal == this) {
			NextTemporal->PrevTemporal = NULL;
		}
		NextTemporal->Detach();
		NextTemporal = NULL;
	}
	PrevTemporal = NULL;
	NextTemporal = NULL;
	SourceSW = NULL;
	Unused_38 = NULL;
}


void TemporalClass::Clear_Links(void)
{
	Target = NULL;
	PrevTemporal = NULL;
	NextTemporal = NULL;
	SourceSW = NULL;
	Unused_38 = NULL;
}


/*
**	Lets go of the target and detaches the whole chain.
*/
void TemporalClass::Detach(void)
{
	Target->BeingWarpedOut = false;
	Target->TemporalTargetingMe = NULL;
	Target = NULL;
	if (PrevTemporal != NULL) {
		if (PrevTemporal->NextTemporal == this) {
			PrevTemporal->NextTemporal = NULL;
		}
		PrevTemporal->Detach();
	}
	if (NextTemporal != NULL) {
		if (NextTemporal->PrevTemporal == this) {
			NextTemporal->PrevTemporal = NULL;
		}
		NextTemporal->Detach();
	}
	PrevTemporal = NULL;
	NextTemporal = NULL;
	SourceSW = NULL;
	Unused_38 = NULL;
}


bool TemporalClass::CanWarpTarget(TechnoClass * target)
{
	if (target == NULL) return(false);
	if (!target->Techno_Type_Class()->Warpable) return(false);
	if (target->Unknown_Virtual_32C()) return(false);
	return(true);
}


static inline UnitClass * As_Unit(TechnoClass * object)
{
	return((object == NULL) ? NULL : ((object->Fetch_RTTI() == RTTI_UNIT) ? (UnitClass *)object : NULL));
}


static inline BuildingClass * As_Building(TechnoClass * object)
{
	return((object == NULL) ? NULL : ((object->Fetch_RTTI() == RTTI_BUILDING) ? (BuildingClass *)object : NULL));
}


/*
**	Fires at a target: its spawned aircraft and captured units are let go, this beam joins the
**	chain on the target (or starts it), a harvester's owner is warned and a building's owner is
**	told it is attacked; the target stops being an active object.
*/
void TemporalClass::Fire(TechnoClass * target)
{
	if (target != NULL) {
		if (target->SpawnManager != NULL) {
			target->SpawnManager->KillNodes();
		}
		if (target->CaptureManager != NULL) {
			target->CaptureManager->FreeAll();
		}
	}
	if (Owner->TemporalImUsing != NULL && Owner->TemporalImUsing->Target != NULL) {
		Owner->TemporalImUsing->LetGo();
	}

	if (target == NULL || !target->Techno_Type_Class()->Warpable || target->Unknown_Virtual_32C() ||
		Owner->TemporalTargetingMe != NULL) {
		return;
	}

	Target = target;
	if (target->TemporalTargetingMe == NULL) {
		target->TemporalTargetingMe = this;
		WarpRemaining = Target->Techno_Type_Class()->MaxStrength * 10;
		if (Target != NULL && Target->Fetch_RTTI() == RTTI_UNIT && As_Unit(Target)->Class->IsToHarvest &&
			Target->House == PlayerPtr) {
			if (Submit_Radar_Event(RADAREVENT_HARVESTER_ATTACKED, Target->Destination_Coord(NULL).As_Cell())) {
				Queue_Speech("EVA_OreMinerUnderAttack", -1, -1);
			}
		}
		if (Target->Fetch_RTTI() == RTTI_BUILDING && !Target->Techno_Type_Class()->IsInsignificant &&
			!Target->Considered_Vehicle()) {
			Target->House->Attacked(As_Building(Target));
		}
	} else {
		NextTemporal = target->TemporalTargetingMe;
		PrevTemporal = NextTemporal->PrevTemporal;
		NextTemporal->PrevTemporal = this;
		if (PrevTemporal != NULL) {
			PrevTemporal->NextTemporal = this;
		}
	}

	Target->BeingWarpedOut = true;
	if (Target->Fetch_RTTI() == RTTI_BUILDING) {
		BuildingClass * building = (BuildingClass *)Target;
		building->House->RecalcPower = true;
		building->DisableTemporal();
		building->House->IsRecalcNeeded = true;
	}
	Target->Mark(MARK_CHANGE);
	if (Target->TemporalImUsing != NULL && Target->TemporalImUsing->Target != NULL) {
		Target->TemporalImUsing->LetGo();
	}
}


int TemporalClass::Fetch_Object_Size(void) const
{
	return(sizeof(TemporalClass));
}


RTTIType TemporalClass::Fetch_RTTI(void) const
{
	return(RTTI_TEMPORAL);
}
