/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "geoconst1.h" // RA2: this unit holds its own copy of the geometry constants (its first initializers)

#include "globals.h" // RA2: the includes this file needs
#include "wdtnet.h"
#include "geoconst2.h" // RA2: the slope and bridge constants (after globals.h)

using namespace WorldDominationTour;


/// <summary>
/// Creates a counted ownership history for a tour state.
/// </summary>
/// <param name="tour">The tour state holding the ownership record.</param>
/// <returns>Returns with the new history.</returns>
rc_ptr<History> __fastcall History::Create(rc_ptr<WDTState> const & tour) // RA2: a counted history, made by a static Create (0x00728E50)
{
	return(rc_ptr<History>(new History(tour))); // RA2: as above
}


/// <summary>
/// Creates the ownership history for a tour state.
/// This routine is used by the campaign, which keeps one history and consults it for who
/// held which territory at any point of the current cycle.
/// </summary>
/// <param name="tour">The tour state holding the ownership record. It may be empty, in
/// which case no tick is ever valid.</param>
History::History(rc_ptr<WDTState> const & tour) : // RA2: the tour state is a counted pointer (0x00728EC0)
	TourState(tour)
{
	/// nothing
}


/// <summary>
/// Fetches the ownership snapshot for one tick of the cycle.
/// This routine is used by the tour selection screen, which walks the cycle a tick at a
/// time to show how the territories changed hands. A tick outside the recorded history
/// still yields a usable snapshot, but one in which nobody owns anything.
/// </summary>
/// <param name="tick">The point in the cycle to look at.</param>
/// <returns>Returns with the snapshot of territory ownership at that tick.</returns>
rc_ptr<State> History::Get_State(int tick) const // RA2: a counted snapshot, made by State::Create (0x00728F00)
{
	return(State::Create(TourState, tick)); // RA2: as above
}


/// <summary>
/// Is the tick one that the tour has a record for?
/// Use this routine before asking for a state, since the history only reaches as far as
/// the ownership record the server sent down for the current cycle.
/// </summary>
/// <param name="tick">The point in the cycle to check.</param>
/// <returns>bool; Does the history cover this tick?</returns>
int History::Is_Tick_Valid(unsigned int tick) const // RA2: the tour state is a counted pointer (0x00728F20); returns int
{
	if (TourState.Get() != NULL && tick < TourState->NumTicks) { // RA2: as above
		return(true); // RA2: as above
	}
	return(false); // RA2: as above
}


/// <summary>
/// Destroys the tour history.
/// The tour state that it reads from belongs to the campaign and is left untouched.
/// </summary>
History::~History(void) // RA2: counted (0x00728F40)
{
	/// nothing
}
