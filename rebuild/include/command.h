/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#pragma once

class CommandClass
{
	public:
		virtual ~CommandClass(void) {}

		virtual char const * Get_Unique_Name(void) const = 0;
		virtual wchar_t const * Get_Display_Name(void) const { return Get_Description(); } // RA2: wide text, and the default is the description (0x0051AF90 jumps to slot 4)
		virtual wchar_t const * Get_Category(void) const = 0; // RA2: wide text (0x0051AF10 and the other overrides)
		virtual wchar_t const * Get_Description(void) const = 0; // RA2: wide text
		virtual bool PreventCombinationOverride(int key) const { return false; } // RA2: slot 5 (0x0051AE70); the base returns false (0x0051AE70), whether the command also answers for the key with its modifiers
		virtual bool ExtraTriggerCondition(int key) const { return !(key & 0x800); } // RA2: slot 6 (0x0051AE80); the base returns whether the key's release bit (0x800) is clear, whether Keyboard_Process runs the command for this key
		virtual bool Unknown1C(int key) const { return false; } // RA2: slot 7 (0x0051AE90); whether Keyboard_Process then drops the key's repeats
		virtual void Execute(int key) const = 0; // RA2: slot 8; takes the key
};
