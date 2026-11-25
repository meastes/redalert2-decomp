/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

class Point2D;
class EventClass;
class TechnoClass;

/*
**	RA2: planning mode takes the mouse over the tactical view while it is active. Each
**	routine gets the mouse position relative to the tactical view; those that return a
**	value return true when planning mode handled the input.
*/
bool __fastcall Planning_Left_Press(Point2D xy);
bool __stdcall Planning_Left_Release(Point2D point);
void __stdcall Planning_Right_Press(int x, int y);
void __stdcall Planning_Right_Release(int x, int y);
bool __stdcall Planning_Mouse_AI(int x, int y);

/*
**	RA2: whether planning mode is on, and the scold (message and sound) for an event planning mode
**	cannot queue (deploy, stop, scatter).
*/
bool Planning_Is_Active(void); // RA2: 0x614840
void __fastcall Planning_Cancel_Token(TechnoClass * techno);
void Planning_Refuse_Event(EventClass * event); // RA2: 0x616D70

// Declarations moved from the units that used them (one declaration per name).
extern void __fastcall Planning_Clear_Techno(TechnoClass * techno, EventClass * event); // RA2: 0x00615480 (planmgr.cpp), lets go of the planning token of a techno that was given an order
