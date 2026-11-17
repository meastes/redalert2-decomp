/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// From mischa85/RedAlert2 dbca015 src/category.cpp (matching RA2 1.006 decomp; TibSun category.cpp); modified:
// the table is the global CategoryNames (ini_enums.hpp) rather than a file-static _categories; the loop starts at CATEGORY_SOLDIER and steps the enum by hand (no CATEGORY_FIRST or incdec.h yet).

#include "shared/ini_enums.hpp"
#include "shared/crt_string.hpp"

CategoryNameStruct CategoryNames[CATEGORY_COUNT] = {
	{ "Soldier", "Soldier" },
	{ "Civilian", "Civilian" },
	{ "VIP/Agent", "VIP" },
	{ "Recon Vehicle", "Recon" },
	{ "Armored Fighting Vehicle", "AFV" },
	{ "Infantry Fighting Vehicle", "IFV" },
	{ "Indirect Fire Support", "LRFS" },
	{ "Misc. Support Vehicle", "Support" },
	{ "Transport Vehicle", "Transport" },
	{ "Air Combat Support", "AirPower" },
	{ "Air Transport", "AirLift" }
};


/// <summary>
/// Fetches the descriptive text for a unit category.
/// This routine yields the long form of the category, suitable for showing to the
/// player rather than for storing in the rules.
/// </summary>
/// <returns>Returns with the description text. An out of range category yields a
/// placeholder string.</returns>
char const * __fastcall Description_From_Category(CategoryType category)
{
	if ((unsigned)category < (unsigned)CATEGORY_COUNT) {
		return(CategoryNames[category].Description);
	}
	return("<none>");
}


/// <summary>
/// Fetches the abbreviated name of a unit category.
/// This routine yields the short form of the category, which is the spelling written
/// out to and read back from the rules.
/// </summary>
/// <returns>Returns with the abbreviated name. An out of range category yields a
/// placeholder string.</returns>
char const * __fastcall Name_From_Category(CategoryType category)
{
	if ((unsigned)category < (unsigned)CATEGORY_COUNT) {
		return(CategoryNames[category].Name);
	}
	return("<none>");
}


/// <summary>
/// Converts a category name back into its category identifier.
/// This routine is the counterpart of the two lookups above and is used when reading
/// the category assigned to an object type from the rules. Either the abbreviated name
/// or the full description is accepted, and the comparison ignores case.
/// </summary>
/// <returns>Returns with the category identified, or CATEGORY_NONE if the name is
/// unknown.</returns>
CategoryType __fastcall Category_From_Name(char const * name)
{
	if (name != 0) {
		for (CategoryType index = CATEGORY_SOLDIER; index < CATEGORY_COUNT; index = CategoryType(index + 1)) {
			if (_stricmp(CategoryNames[index].Name, name) == 0 || _stricmp(CategoryNames[index].Description, name) == 0) {
				return(index);
			}
		}
	}
	return(CATEGORY_NONE);
}
