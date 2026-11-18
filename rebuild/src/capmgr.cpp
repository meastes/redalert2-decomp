/*
**	Command & Conquer Red Alert 2
**	CAPMGR.CPP: the capture manager.
**
**	A mind controller holds one unit at a time. The manager takes a unit over, gives it back
**	to its old house when it lets go, and draws the link between the two.
*/

#include "always.h"
#include "geometry.h"
#include "rect.h"
#include "capmgr.h"
#include "crc.h"
#include "foot.h"
#include "house.h"
#include "swizzle.h"
#include "team.h"
#include "techno.h"
#include "techtype.h"
#include "temporal.h"


DynamicVectorClass<CaptureManagerClass *> CaptureManagerArray;


CaptureManagerClass::CaptureManagerClass(TechnoClass * owner)
{
	Unit = NULL;
	OriginalOwner = NULL;
	Owner = owner;
	CaptureManagerArray.Add(this);
}


CaptureManagerClass::CaptureManagerClass(void)
{
	Unit = NULL;
	OriginalOwner = NULL;
	Owner = NULL;
	CaptureManagerArray.Add(this);
}


CaptureManagerClass::CaptureManagerClass(NoInitClass const & x) :
	AbstractClass(x)
{
}


CaptureManagerClass::~CaptureManagerClass(void)
{
	CaptureManagerArray.Delete(this);
}


/*
**	A unit can be taken over unless it already belongs to the controller's house, is immune,
**	is controlled already or is under the iron curtain.
*/
bool CaptureManagerClass::Can_Capture(TechnoClass * target)
{
	if (target == NULL) return(false);
	if (target->Owner_HouseClass() == Owner->Owner_HouseClass()) return(false);
	if (target->Techno_Type_Class()->ImmuneToPsionics) return(false);
	if (target->MindControlledBy != NULL) return(false);
	if (target->Unknown_Virtual_32C()) return(false);
	return(true);
}


/*
**	The unit held so far goes back to its house; then the target is taken over, leaves its
**	team, stops what it was doing and guards.
*/
#ifdef RA2_NON_MATCHING // not exact: 186 of 299 bytes differ (the early false return is merged with the later one; the original keeps it in place)
bool CaptureManagerClass::Capture(TechnoClass * target)
{
	if (Unit != NULL) {
		Unit->MindControlledBy = NULL;
		Unit->Captured(OriginalOwner, true);
		Unit = NULL;
		OriginalOwner = NULL;
	}

	if (target == NULL || !(target->AbstractFlags & ABSTRACT_TECHNO) || !Can_Capture(target)) {
		return(false);
	}

	OriginalOwner = target->Owner_HouseClass();
	if (!target->Captured(Owner->Owner_HouseClass(), true)) {
		OriginalOwner = NULL;
		return(false);
	}

	Unit = target;
	target->MindControlledBy = Owner;
	if (Unit != NULL && (Unit->AbstractFlags & ABSTRACT_FOOT) && ((FootClass *)Unit)->Team != NULL) {
		((FootClass *)Unit)->Team->Remove((FootClass *)Unit, -1, false);
	}
	Unit->Assign_Destination(NULL, true);
	Unit->Assign_Target(NULL);
	Unit->Assign_Archive_Target(NULL);
	Unit->QueueMission(MISSION_GUARD, true);
	if (Unit->TemporalImUsing != NULL && Unit->TemporalImUsing->Target != NULL) {
		Unit->TemporalImUsing->LetGo();
	}
	return(true);
}
#endif


/*
**	An object is going away: the held unit is forgotten when it is the one.
*/
bool CaptureManagerClass::Detach_Target(AbstractClass const * target)
{
	if (Unit == target) {
		Unit = NULL;
		OriginalOwner = NULL;
	}
	return(true);
}


bool CaptureManagerClass::Detach_Unused(AbstractClass * target)
{
	return(true);
}


/*
**	The link from the controller's firing point to a point above the held unit, in the
**	controller's house color.
*/
void CaptureManagerClass::Draw_Link(void) const
{
	if (Owner != NULL && Unit != NULL) {
		Coord to = Unit->Position;
		to.Z += 70;
		Owner->Draw_Link_Line(Owner->Fire_Coord(0), to, Owner->House->RemapColorRGB);
	}
}


/*
**	The held unit goes back to its house.
*/
void CaptureManagerClass::FreeAll(void)
{
	if (Unit != NULL) {
		Unit->MindControlledBy = NULL;
		Unit->Captured(OriginalOwner, true);
		Unit = NULL;
		OriginalOwner = NULL;
	}
}


void CaptureManagerClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	if (Unit != NULL) crc(Unit->Fetch_ID());
	if (OriginalOwner != NULL) crc(OriginalOwner->Fetch_ID());
	if (Owner != NULL) crc(Owner->Fetch_ID());
}


HRESULT STDMETHODCALLTYPE CaptureManagerClass::Load(IStream * stream)
{
	HRESULT hr = BASECLASS::Load(stream);
	if (SUCCEEDED(hr)) {
		new (this) CaptureManagerClass(NoInitClass());
		Swizzle_Pointer(&Unit);
		Swizzle_Pointer(&OriginalOwner);
		Swizzle_Pointer(&Owner);
	}
	return(hr);
}


HRESULT STDMETHODCALLTYPE CaptureManagerClass::Save(IStream * stream, BOOL cleardirty)
{
	return(BASECLASS::Save(stream, cleardirty));
}


HRESULT STDMETHODCALLTYPE CaptureManagerClass::GetClassID(CLSID * retval)
{
	if (retval == NULL) {
		return(E_POINTER);
	}
	*retval = CLSID_CaptureManagerClass;
	return(S_OK);
}


int CaptureManagerClass::Fetch_Object_Size(void) const
{
	return(sizeof(CaptureManagerClass));
}


RTTIType CaptureManagerClass::Fetch_RTTI(void) const
{
	return(RTTI_CAPTUREMANAGER);
}
