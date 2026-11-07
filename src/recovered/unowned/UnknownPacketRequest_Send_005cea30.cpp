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

// Init_Null_Modem from TibSun nulldlg.cpp, typed against RA2's exact 005CEA30 body.
// The former UnknownPacketRequest fields are SerialSettingsType, and the sink is NullModemClass.
#include "shared/null_modem.hpp"
#include "shared/serial_settings.hpp"
extern NullModemClass serial_manager_00a40a28;
// @ref ?serial_manager_00a40a28@@3VNullModemClass@@A=00a40a28
// @ref ?Init@NullModemClass@@QAEHHHPADHDHH@Z=005d00e0
bool __fastcall Init_Null_Modem(SerialSettingsType *settings)
{
    return serial_manager_00a40a28.Init(settings->Port, settings->IRQ,
        settings->ModemName, settings->Baud, 0, 8, 1) != 0 ? true : false;
}
