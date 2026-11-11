// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnitClass destination assignment, original RA2 0x007053F0.
// Assign_Destination(AbstractClass *, bool): vtable slot 260.
// Statement shape follows the TibSun reconstruction unit.cpp 5117-5449 (shape only; see THIRD_PARTY.md).
#include "shared/unit.hpp"
#include "shared/building_type_class.hpp"
#include "shared/aircraft.hpp"
#include "shared/locomotion.hpp"
#include "shared/map_view.hpp"
#include "shared/map_elements.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_geometry.hpp"
extern "C" GUID const CLSID_DriveLocomotionClass, CLSID_TunnelLocomotionClass;
extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;
extern Cell const unknown_unload_offset_851a88;
// The native view is the known MapClass::ZoneConnections member at Map+0x4C.
extern DynamicVectorClass<ZoneConnectionClass> unknown_zone_connections_83252c;
// @ref ?Query@?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QAEJABV?$UnknownComPtr@UILocomotion@@$1?unknown_uuid_ILocomotion_7ce600@@3UGUID@@B@@@Z=00454c70
// @ref ?Release_Held@?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QAEXXZ=00509860
// @ref ?_AddRef@?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QBEXXZ=0069ed20
// @ref ?_AddRef@?$UnknownComPtr@UILocomotion@@$1?unknown_uuid_ILocomotion_7ce600@@3UGUID@@B@@QBEXXZ=00453ea0
// @ref ?Assign_Destination@FootClass@@UAEXPAVAbstractClass@@_N@Z=004c8030
// @ref _CLSID_DriveLocomotionClass=007a1f70
// @ref _CLSID_TunnelLocomotionClass=007a1f90
// @ref ?unknown_unload_offset_851a88=00851a88
// @ref ?unknown_zone_connections_83252c=0083252c
// Westwood runtime-kind cast: null input or a mismatched class yields null.
template <class T,int kind> inline T *Unit_Dynamic_Cast(AbstractClass *target)
{
    if(!target) return 0;
    return target->Kind_Of()==kind ? (T *)target : 0;
}
// comip's _Release: Unit.cpp releases the held locomotor inline.
template <> inline void UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}
// comip's operator=: the new reference through _AddRef, which stays out of line
// here (0x0069ED20 for IPiggyback, 0x00453EA0 for ILocomotion; as SuperClass::Place 0x0069D850).
template <> void UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::_AddRef() const;
template <> inline UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> &
UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::operator=(UnknownComPtr const &other)
{
    if (Pointer != other.Pointer) {
        IPiggyback *old = Pointer;
        Pointer = other.Pointer;
        _AddRef();
        if (old != 0) {
            old->Release();
        }
    }
    return *this;
}
template <> void UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::_AddRef() const;
template <> inline UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> &
UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600>::operator=(UnknownComPtr const &other)
{
    if (Pointer != other.Pointer) {
        ILocomotion *old = Pointer;
        Pointer = other.Pointer;
        _AddRef();
        if (old != 0) {
            old->Release();
        }
    }
    return *this;
}
// TibSun target.h Is_Target_Cell: a cell target (the RTTI read is the virtual Kind_Of in RA2).
inline bool Is_Target_Cell(AbstractClass *target) {return(target != 0 && target->Kind_Of() == 11);}
// True when the unit type lists the building type among its docks.
inline bool Unit_Is_Dock(UnitTypeClass const *type, BuildingTypeClass const *building)
{
    for (int i = 0; i < type->Dock.Count(); i++) {
        if (type->Dock[i] == building) return true;
    }
    return false;
}
void UnitClass::Assign_Destination(AbstractClass *target, bool immediate)
{
	/*
	**	Abort early if there is anything wrong with the parameters
	**	or the unit already is assigned the specified destination.
	*/
	if (Type->BalloonHover && !target && unknown_494 && unknown_228) {
        if (unknown_494==unknown_228) return;
        if (Is_Target_Cell(unknown_494)) {
            CellClass *destination=Unit_Dynamic_Cast<CellClass,11>(unknown_494);
            if(destination->OccupierPtr==unknown_228) return;
        }
    }
    if(target==unknown_494 && !unknown_190) return;
    unknown_190=false;
    if(unknown_5c8) {FootClass::unknown_4cd9b0();return;}

	/*
	**	Transport vehicles must tell all passengers that are about to load, that they
	**	cannot proceed. This is accomplished with a radio message to this effect.
	*/
	if (In_Radio_Contact() && Type->Passengers > 0 && Contact(0)->Kind_Of() == 15) {
		if(target && target->AbstractFlags.IsTechno && !Is_Radio_Contact((TechnoClass *)target)) unknown_slot149(3);
	}

	/*
	 * If tethered to a carryall while sleeping, untether before moving.
	 */
	if (target != 0 && In_Radio_Contact() && Get_Mission() == 0 && unknown_370 && Contact(0)->Kind_Of() == 2) {
		AircraftClass * a = (AircraftClass *)Contact(0);
		if (a->Type->Carryall) {
			a->Assign_Destination(0, true);
			unknown_slot149(25);
			unknown_slot149(3);
		}
	}

	BuildingClass * b=Unit_Dynamic_Cast<BuildingClass,6>(target);
    if(Get_Mission()==16 || MissionQueue==16) {
        Cell unload_cell=unknown_slot104()+unknown_unload_offset_851a88;
        BuildingClass *refinery=(BuildingClass *)Map[unload_cell].Cell_Building();
        if(refinery && Type->Harvester) {
            if(refinery->unknown_25c[5]) refinery->unknown_25c[5]->Delete_Me();
            if(refinery->unknown_25c[6]) refinery->unknown_25c[6]->Delete_Me();
            if(refinery->unknown_454[10]) refinery->unknown_44cfd0(10);
        }
    }

	/*
	**	Handle entry logic here.
	*/
	if (Get_Mission() == 7 || MissionQueue == 7) {

		/*
		**	If not already in radio contact (presumed with the transport), then
		**	either try to establish contact if allowed, or just move close and
		**	wait until radio contact can be established.
		*/
		if (!In_Radio_Contact()) {
			TechnoClass *destination_techno=target->As_Techno();
            if(destination_techno && destination_techno->Kind_Of()==1) {
                unknown_3f8=Unit_Dynamic_Cast<UnitClass,1>(target);
                unknown_6d6e50(0);target=0;
            } else if (b != 0 && b->Kind_Of()==6) {

				/*
				**	Determine if the transport is already in radio contact. If so, then just move
				**	toward the transport and try to establish contact at a later time.
				*/
				if (b->In_Radio_Contact()) {
					unknown_6d6e50(target);

					/*
					**	HACK ALERT: The repair bay is counting on the assignment of the unknown_494 by this routine.
					**	The refinery must NOT have the navcom assigned by this routine.
					*/
					if (b->Type->UnitRepair) {
						if(Health_Ratio()<Rules->unknown_1360) {target=0;unknown_3f8=b;}
                        else {unknown_slot149(3);target=0;}
					}
				} else {
					AbstractClass * oldnav = unknown_494;
					if (unknown_slot150(14, b) != 1) {
						unknown_slot149(3);
						if (b->Type->UnitRepair) {
							unknown_6d6e50(target);
						}
					} else {
						if (b->Type->DockUnload && unknown_494 != oldnav) {
							target = unknown_494;
						}
					}
				}
			} else {
				TechnoClass * techno = target && target->AbstractFlags.IsTechno ? (TechnoClass *)target : 0;
				if (techno != 0) {

					/*
					**	Determine if the transport is already in radio contact. If so, then just move
					**	toward the transport and try to establish contact at a later time.
					*/
					if (techno->In_Radio_Contact()) {
						unknown_6d6e50(target);
					} else {
						if (unknown_slot150(2, techno) == 1) {
							if (unknown_slot149(14) == 1) {
                            // RA2 retains the destination and continues after the handshake.
                        } else
							unknown_slot149(3);
						}
					}
				}
			}
		} else {
			unknown_4d0 = -1;
		}
	} else {
		unknown_4d0 = -1;
	}

	bool useplan = false;

	/*
	 * Subterranean units prepend their navigation target onto the queue and
	 * re-target the nearest reachable cell when driving rather than burrowing.
	 */
	if (target != 0 && Type->unknown_af5 && Locomotor.Checked()->Is_Moving()) {
		UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Locomotor);
		GUID clsid;
		persist.Checked()->GetClassID(&clsid);
		if (memcmp(&clsid,&CLSID_DriveLocomotionClass,sizeof(GUID))==0) {
			unknown_49c.Add_Head(target);
			unknown_478.Clear();
			CellClass * tcell = Get_Target_Cell_Ptr();
			target = tcell;
			immediate = false;
			if (tcell != 0 && ((tcell->unknown_140&0x100)!=0) && !Is_Moving_Onto_Bridge(0)) {
				Cell nearby = Map.Nearby_Location(tcell->MapCoords, 1, Map.Zone(tcell->MapCoords, Type->MovementZone, false), Type->MovementZone, false, Point2D(1, 1), false, true, false, false, Cell(0,0), 0);
				if (nearby != unknown_geometry_k) {
					target = &Map[nearby];
				}
			}
		}
	}

	bool needsswap = false;

	if (target != 0) {
		bool surface = false;

		/*
		 * A subterranean unit that is told to move immediately must decide whether
		 * it can burrow straight to the destination or has to plan a surface route.
		 */
		if (Type->unknown_af5 && immediate) {
			bool isbridge = Is_Moving_Onto_Bridge(0);

			Cell from = Cell(unknown_slot19(0));
			bool canburrowfrom = Map[from].Can_Burrow_Here();

			Cell to = Cell(target->Center_Coord());
			CellClass * tocell = &Map[to];
			bool isunderbridgeto = ((tocell->unknown_140&0x100)!=0);
			bool canburrowto = tocell->Can_Burrow_Here();
			int height = Get_Height_AGL();

			if (isbridge && isunderbridgeto) {
				int z1 = Map.Zone_Connection_Index(from, 3, 0);
				if (z1 == Map.Zone_Connection_Index(to, 3, 0) && z1 != -1) {
					if (!unknown_zone_connections_83252c[z1].unknown_08) {
						Cell d1 = Map.Unknown_005651E0(from, from);
						Cell d2 = Map.Unknown_005651E0(to, to);
						if (Cell_Length(d1-d2) <= 2) {
							needsswap = true;
						}
					} else {
						needsswap = true;
					}
				}
			}
			if ((isbridge || isunderbridgeto) && !needsswap) {
				useplan = true;
			}

			if (((!canburrowfrom && height >= 0) || !canburrowto) && !needsswap) {
				useplan = true;
			}
		}

		if (useplan) {
			target = Plan_Route_00707f60(target);
			surface = true;
		} else if (immediate) {
			unknown_478.Clear();
			surface = true;
		}

		/*
		 * Surfacing tunneler keeps its current cell at the route front.
		 */
		if (surface) {
			if (Locomotor.Checked()->Is_Surfacing()) {
				unknown_478.Add_Head(target);
				target = &Map[Position_Coord_004f3f40()];
			}
		}

	} else { unknown_478.Clear(); }

    if(Type->Teleporter && !unknown_210) {
        TechnoClass *contact=Contact(0);
        UnknownComPtr<IPersist,&unknown_uuid_IPersist_7cf208> persist(Locomotor);
        GUID tclsid;
        persist.Checked()->GetClassID(&tclsid);
        bool drive=true;
        if(contact) {
            if(contact->Kind_Of()==6) {
                BuildingClass *building=Unit_Dynamic_Cast<BuildingClass,6>(contact);
                CellClass *cell=Unit_Dynamic_Cast<CellClass,11>(target);
                if(building->Type->DockUnload && cell && !cell->Cell_Unit(false)) {
                    drive=false;
                    if(memcmp(&tclsid,&unknown_clsid_7a1fd0,sizeof(GUID))!=0) {
                        UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
                        if(piggy && piggy.Checked()->Is_Piggybacking() && piggy.Checked()->Is_Ok_To_End()) {
                            piggy.Checked()->End_Piggyback(&Locomotor);
                        } else {
                            Locomotor.Checked()->Stop_Moving();
                            Set_Mission(-1);
                            unknown_slot116(7,false);
                            unknown_598=true;
                            unknown_190=true;
                        }
                    }
                }
            }
        }
        if((contact == 0 || drive == true) && memcmp(&tclsid,&CLSID_DriveLocomotionClass,sizeof(GUID))!=0) {
            UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
            if(piggy && piggy.Checked()->Is_Piggybacking() && piggy.Checked()->Is_Ok_To_End()) piggy.Checked()->End_Piggyback(&Locomotor);
            UnknownComPtr<ILocomotion,&unknown_uuid_ILocomotion_7ce600> walk(CLSID_DriveLocomotionClass);
            walk.Checked()->Link_To_Object(this);
            piggy=UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30>(walk);
            if(piggy) {piggy.Checked()->Begin_Piggyback(Locomotor);Locomotor=walk;}
        }
    }
	/*
	 * If the locomotor is a tunnel locomotor and the unit is on the ground,
	 * surface the tunneler by swapping the tunnel locomotor for a drive one.
	 * (Mirrors BuildingClass weapons-factory exit, building.cpp:6236-6251.)
	 */
	if (target != 0 && !Locomotor.Checked()->Is_Moving()) {
		UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist;
        if(Locomotor) {
            HRESULT result=persist.Query_Held(Unit_Checked_Locomotor_004cde90(Locomotor));
            if(FAILED(result)&&result!=E_NOINTERFACE) _com_issue_error(result);
        }
		GUID clsid;
		persist.Checked()->GetClassID(&clsid);
		if (memcmp(&clsid,&CLSID_TunnelLocomotionClass,sizeof(GUID))==0 && Get_Height_AGL() == 0) {
            CoordStruct tc = target->Center_Coord();
			int gl = Map.Get_Height_GL(tc);
			if (tc.Z < gl) tc.Z = gl;

			bool doswap = needsswap;
			if (!doswap) {
				bool canburrow = !Is_Moving_Onto_Bridge(0)
					&& !(Map[target->Center_Coord()].unknown_140&0x100)
					&& Map[unknown_slot19(0)].Can_Burrow_Here()
					&& Map[target->Center_Coord()].Can_Burrow_Here();
				if (canburrow) {
					Cell to = Cell(target->Center_Coord());
					if (!unknown_709790(&Get_Target_Cell(), &to)) {
						doswap = true;
					}
				} else {
					doswap = true;
				}
			}

			if (doswap) {
				UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggy;
                if(Locomotor) {
                    HRESULT result=piggy.Query_Held(Unit_Checked_Locomotor_004cde90(Locomotor));
                    if(FAILED(result)&&result!=E_NOINTERFACE) _com_issue_error(result);
                }
				if (piggy != 0 && piggy.Checked()->Is_Piggybacking()) {
					piggy.Checked()->End_Piggyback(&Locomotor);
				}
				UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> walk(CLSID_DriveLocomotionClass);
				walk.Checked()->Link_To_Object(this);
				{
                UnknownComPtr<IPiggyback,&unknown_uuid_IPiggyback_7cfc30> walk_piggy;
                if(walk) {
                    HRESULT result=walk_piggy.Query_Held(Unit_Checked_Locomotor_004cde90(walk));
                    if(FAILED(result)&&result!=E_NOINTERFACE) _com_issue_error(result);
                }
                piggy=(IPiggyback *)walk_piggy;
                }
				if (piggy != 0) {
					piggy.Checked()->Begin_Piggyback(Locomotor);
					Locomotor = (ILocomotion *)walk;
					Locomotor.Checked()->Force_New_Slope(Map[Position_Coord_004f3f40()].unknown_11c);
				}
			}
		}
	}

	/*
	**	If the player clicked on a friendly repair facility and the repair
	**	facility is currently not involved with some other unit (radio or unloading).
	*/
	if (b != 0 && b->Type->UnitRepair) {
		if (b->In_Radio_Contact() && b->Contact(0) != this) {
			unknown_6d6e50(target);
		} else {

			/*
			**	Establish radio contact protocol. If the facility responds correctly,
			**	then remain in radio contact and proceed toward the desired destination.
			*/
			if (unknown_slot150(2, b) == 1) {
				if (unknown_slot149(14) == 1) {
					FootClass::Assign_Destination(target, immediate);
					unknown_4d0 = -1;
					return;
				}

				/*
				**	Failure to establish a docking relationship with the refinery.
				**	Bail & await further instructions.
				*/
				unknown_slot149(3);
				target = 0;
			}
		}
	}

	/*
	 * A weapons factory that this unit is exiting wants the unit driven to its
	 * exit cell before anything else; queue the real destination behind it.
	 */
	BuildingClass * wf = (BuildingClass *)Contact(0);
	if (wf != 0 && wf->Kind_Of() == 6 && wf->Type->WeaponsFactory && Mission != 7) {
		Cell exitcell = Cell(wf->Position_Coord_004f3f40()) + Cell(5, 1);
		if (target != &Map[exitcell]) {
			unknown_49c.Clear();
			unknown_478.Clear();
			if (target != 0) {
				unknown_4c8bb0(target);
			}
			return;
		}
	}

	/*
	 * Harvesters and weeders automatically enter their dock building when the
	 * clicked target is that building (and the houses are allied).
	 */
	if (b != 0 && Type->Dock.Count() > 0 && Unit_Is_Dock(Type, b->Type) &&
		Kind_Of() == 1 && (Type->Harvester || Type->Weeder) &&
		Get_Mission() != 16) {

		if (Contact(0) != b && !b->In_Radio_Contact() && unknown_slot150(2, b) == 1 &&
			Get_Mission() != 7 && Get_Mission() != 10) {
			unknown_slot116(7, false);
			target = 0;
		} else {
			if (target != 0 && target->Kind_Of() == 6 && Contact(0) != target) {
				Cell nearby = Map.Nearby_Location(Cell(b->Position_Coord_004f3f40()), 2, Map.Zone(Cell(b->Position_Coord_004f3f40()), Type->MovementZone, false), Type->MovementZone, false, Point2D(1, 1), false, true, false, false, Cell(0,0), 0);
				if (nearby != unknown_geometry_k) {
					target = &Map[nearby];
				} else {
					target = 0;
				}
			}
		}
	}

	/*
	 * If the locomotor is unpowered and we are not in an ion storm, power it
	 * back on when an adjacent friendly repair facility occupies our cell.
	 */
	if (!Locomotor.Checked()->Is_Powered() && !House->Unknown_0051FC80()) {
		CellClass * cellptr = (CellClass *)Get_Cell_Ptr();
		if (!(cellptr->unknown_140&0x100)) {
			ObjectClass * occupier = cellptr->OccupierPtr;
			while (occupier != 0) {
				if (occupier != this && occupier->Kind_Of() == 6 && ((BuildingClass *)occupier)->Type->UnitRepair) {
					Locomotor.Checked()->Power_On();
					break;
				}
				occupier = occupier->unknown_30;
			}
		}
	}

	FootClass::Assign_Destination(target, immediate);
}
