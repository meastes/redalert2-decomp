/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/veteran.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// veteran.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/veteran.json carries the addresses and the rows that
// stay in their own files.


#define MIN(a,b) (((a)<(b))?(a):(b))


/// <summary>
/// Credits the unit with a kill.
/// This routine is called when a trainable unit destroys something. The experience
/// earned is weighed against what the killer itself is worth, so cheap units are
/// promoted by prey that would barely register for an expensive one. Experience is
/// capped at the ceiling the rules specify.
/// </summary>
/// <param name="cost">The cost of the unit that made the kill.</param>
/// <param name="value">The point value of the victim.</param>
// (0x00713360; TS Made_A_Kill)
// The original loads the cap as an FP rvalue (fld, then fld again for the store), which a
// double-returning inline accessor reproduces; reading the member directly copies it as two dwords.
inline double Veteran_Cap() { return Rules->VeteranCap; }
void VeterancyClass::Made_A_Kill(int cost, int value)
{
    Experience += value / (cost * Rules->VeteranRatio);
    Experience = MIN(Experience, Veteran_Cap());
}


/// <summary>
/// Has the unit earned veteran status?
/// This is the middle rank, one promotion above rookie and one below elite.
/// </summary>
/// <returns>bool; Is the unit a veteran?</returns>
/// <remarks>The ranks are exclusive. An elite unit answers false here, so a caller that
/// wants "veteran or better" must ask about elite as well.</remarks>
bool VeterancyClass::Is_Veteran() const
{
    return Experience >= 1 && Experience < 2;
}


/// <summary>
/// Is the unit still a rookie?
/// This is the rank a unit is built at, and it holds until enough kills have been
/// accumulated to earn the first promotion.
/// </summary>
/// <returns>bool; Is the unit a rookie?</returns>
bool VeterancyClass::Is_Rookie() const
{
    return Experience >= 0 && Experience < 1;
}


/// <summary>
/// Is the unit a raw recruit?
/// A dumbass sits below rookie on the promotion ladder, its negative experience making
/// it perform worse than a unit that has never seen action at all.
/// </summary>
/// <returns>bool; Is the unit a dumbass?</returns>
bool VeterancyClass::Is_Dumbass() const
{
    return Experience < 0;
}


/// <summary>
/// Has the unit reached elite status?
/// This is the top of the promotion ladder. Combat code asks this when deciding whether
/// the unit's elite bonuses and abilities apply.
/// </summary>
/// <returns>bool; Is the unit elite?</returns>
bool VeterancyClass::Is_Elite() const
{
    return Experience >= 2;
}


/// <summary>
/// RA2: Fetches the number of promotions the unit has still to earn.
/// </summary>
/// <returns>Returns with 0 when elite, 1 when veteran, otherwise 2.</returns>
// (0x00713440)
int VeterancyClass::Remaining_Levels() const
{
    if (Experience >= 2) {
        return 0;
    }
    return Experience >= 1 ? 1 : 2;
}


/// <summary>
/// Fetches the multiplier this veterancy applies to a unit statistic.
/// Callers hand over the per-rank bonus from the rules and scale whatever statistic is
/// being adjusted -- firepower, armor and the like -- by the result. A unit with
/// negative experience is penalized rather than rewarded.
/// </summary>
/// <param name="value">The bonus granted for each rank the unit has earned.</param>
/// <returns>Returns with the multiplier to scale the statistic by.</returns>
// (0x00713470)
// The multiplier for value at this experience: 1 + value * level above zero, a fraction
// below it (0x00713470).
double VeterancyClass::Modify(double value)
{
    double lvl = Experience;
    lvl = floor(lvl);

    if (lvl < -0.01) {
        return -1.0 / (lvl - 1.0);
    }
    if (lvl > 0.01) {
        return 1.0 + (value * lvl);
    }
    return 1.0;
}


/// <summary>
/// Marks or unmarks the unit as a green recruit.
/// A dumbass has negative experience, which makes it perform worse than a rookie.
/// Clearing the mark restores it to plain rookie status.
/// </summary>
/// <param name="set">Should the unit be made a dumbass?</param>
void VeterancyClass::Set_Dumbass(bool set)
{
    if (set) {
        Experience = -.25;
    } else {
        Experience = 0;
    }
}


/// <summary>
/// Returns the unit to rookie status.
/// This routine wipes out whatever experience the unit had accumulated, putting it back
/// where it started life.
/// </summary>
void VeterancyClass::Set_Rookie(bool set)
{
    Experience = 0;
}


/// <summary>
/// Promotes or demotes the unit to veteran status.
/// Clearing veteran status drops the unit back to rookie rather than to whatever it
/// happened to be before.
/// </summary>
/// <param name="set">Should the unit be made a veteran?</param>
void VeterancyClass::Set_Veteran(bool set)
{
    if (set) {
        Experience = 1;
    } else {
        Experience = 0;
    }
}


/// <summary>
/// Promotes or demotes the unit to elite status.
/// Clearing elite status does not merely undo the promotion -- it drops the unit all the
/// way back to rookie.
/// </summary>
/// <param name="set">Should the unit be made elite?</param>
void VeterancyClass::Set_Elite(bool set)
{
    if (set) {
        Experience = 2;
    } else {
        Experience = 0;
    }
}


/// <summary>
/// Converts the veterancy into a whole number.
/// This routine is used when the veterancy must be stored somewhere that cannot hold a
/// fractional value, such as a save game record or a scenario file entry.
/// </summary>
/// <returns>Returns with the veterancy expressed as a whole number.</returns>
int VeterancyClass::To_Integer() const
{
    return int(Experience * 100);
}


/// <summary>
/// Sets the veterancy from its whole number form.
/// This is the counterpart to To_Integer, used when reading a unit's veterancy back out
/// of a save game or a scenario file.
/// </summary>
/// <param name="value">The stored veterancy value to restore.</param>
void VeterancyClass::From_Integer(int value)
{
    Experience = value / 100.0;
}
