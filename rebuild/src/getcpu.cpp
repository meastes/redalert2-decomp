/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Contains material derived from the ra2-decomp project, copyright 2026 its contributors.
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/GETCPU.CPP 1     3/03/97 10:24a Joe_bostic $*/
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : GETCPU                                                       *
 *                                                                                             *
 *                    File Name : GETCPU.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Steve Tall                                                   *
 *                                                                                             *
 *                   Start Date : 6/26/96                                                      *
 *                                                                                             *
 *                  Last Update : June 26th 1996 [ST]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Overview:                                                                                   *
 *   Example of interface to assembly language code to find CPU type                           *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 * Functions:                                                                                  *
 *   Get_CPU_Type -- interface to ASM detection code                                           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"

#include "getcpu.h"

#include <cstdio>
#include <cstring>

/***********************************************************************************************
 * Get_CPU_Type -- Find out what kind of CPU we are running on                                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    int   - reference to cpu type                                                     *
 *           bool  - reference to mmx availability flag                                        *
 *           char* - ptr to buffer to receive chip vendor info                                 *
 *           int   - length of above buffer                                                    *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    6/26/96 10:15AM ST : Created                                                             *
 *=============================================================================================*/
void Get_CPU_Type(int & cpu_type, bool & mmx, char * vendor_id, int vendor_id_length)
{
	/*
	**	Call the asm CPU detection code
	*/
	mmx = Detect_MMX_Availability();
	Detect_CMOV_Availability();

	/*
	**	Return the promised results
	*/
	cpu_type = (int)CPUType;

	if (vendor_id != NULL) {
		char * vendor_ptr = &VendorID;
		strncpy(vendor_id, vendor_ptr, vendor_id_length);
	}
}


/***********************************************************************************************
 * Get_CPU_Type -- Classify the processor for the display name                                 *
 *                                                                                             *
 * Text from ra2-decomp src/getcpu.cpp (Yuri's Revenge 0x004E6190, matched there), with this   *
 * tree's names.                                                                               *
 *                                                                                             *
 * OUTPUT:   CPUType enumeration value, CPU_UNKNOWN if the processor was not identified        *
 *=============================================================================================*/
int Get_CPU_Type(void)
{
	extern char CPUModel;	// RA2: 0x008015A4, the CPUID model number detproc.asm stores
	char vendor[64];

	Detect_MMX_Availability();
	Detect_CMOV_Availability();
	int cpu = (int)CPUType;
	if (vendor != NULL) {
		strncpy(vendor, &VendorID, 64);
	}
	if (cpu == 0 || CPUModel == 0) {
		return(CPU_UNKNOWN);
	}
	if (strcmpi(vendor, "AuthenticAMD") == 0) {
		return(CPU_AMD);
	}
	if (strcmpi(vendor, "CyrixInstead") == 0) {
		return(CPU_CYRIX);
	}
	switch (cpu) {
		case 4:
			return(CPU_486);
		case 5:
			return(CPU_PENTIUM);
		case 6:
			switch (CPUModel) {
				case 3:
				case 5:
					return(CPU_PII);
				case 4:
					return(CPU_PPRO);
				case 6:
					return(CPU_CELERON);
				case 7:
				case 8:
				case 10:
					return(CPU_PIII);
				default:
					return(CPU_PPRO);
			}
	}
	return(CPU_PIV);
}


char const * Get_CPU_Type_Name(void)
{
	static char const * const names[CPU_COUNT] = {
		"486", "Pentium", "Pentium Pro", "Pentium II", "Pentium III",
		"Celeron", "Pentium 4", "AMD", "Cyrix"
	};
	int type = Get_CPU_Type();
	if (type > CPU_UNKNOWN && type < CPU_COUNT) {
		return(names[type]);
	}
	return("Unknown");
}
