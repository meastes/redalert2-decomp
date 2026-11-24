// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors and OpenTS contributors
#pragma once

#include "wdtnet.h"

WideWstring __fastcall operator+(WideWstring const & left, WideWstring const & right); // RA2: 0x00771460, the text so far and the separator joined (wdtgameoptions.cpp and the option template below)

template<class T>
WideWstring WorldDominationTour::ValueGameOption<T>::Get_String(rc_ptr<WDTTerritory> const & territory, WideWstring const & prefix) const
{
	wchar_t buffer[64];
	Wstring number = Wstring_FromInt(Getter(territory.Get()));
	WideWstring format(StringTable::LoadString(Name.text(), NULL, "D:\\Ra2\\wdtgameoptions.h", 161));
	swprintf(buffer, format.get(), number.c_str());
	if (prefix.length() != 0) {
		WideWstring separator(L", ");
		operator+(prefix, separator) + buffer;
	}
	return(WideWstring(buffer));
}
