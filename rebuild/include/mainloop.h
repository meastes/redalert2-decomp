/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/***********************************************************************************************
 ***                            Confidential - Westwood Studios                              ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Commando                                                     *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/Commando/mainloop.h                          $*
 *                                                                                             *
 *                      $Author:: Denzil_l                                                    $*
 *                                                                                             *
 *                     $Modtime:: 10/18/01 6:21p                                              $*
 *                                                                                             *
 *                    $Revision:: 4                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "keyboard.h"

bool Main_Loop(void);
void Keyboard_Process(KeyNumType & input);
void Draw_Multiplayer_Debug(bool noframecheck); // RA2: 0x005419E0; was Multiplayer_Debug_Print
bool Is_In_Main_Loop(void); // RA2: 0x0053F460
void Enter_Main_Loop(void); // RA2: 0x0053F470
void Leave_Main_Loop(void); // RA2: 0x0053F480
void Disconnect_Gracefully(void); // RA2: 0x0053F780; its own debug text names it
bool Process_Game_End(void); // RA2: 0x0053F840; acts on the win, lose, restart and abort flags

// Declarations moved from the units that used them (one declaration per name).
extern int TeamEvent;
extern int TeamNumber;
