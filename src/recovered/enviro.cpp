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
// Function comment blocks from mischa85/RedAlert2 392e904 src/enviro.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// enviro.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/enviro.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/scenario_checkpoint.hpp"
#include "shared/abstract.hpp"
#include "shared/house.hpp"
#include "shared/crt_memory.hpp"
#include "shared/id_source.hpp"
#include "shared/com_persist.hpp"


// Header-static dynamic initializers of the unit at 0x004B5830:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x004B5830-0x004B5A16); the other
// drafts of this plan are emitted by these includes.

extern int Frame;
EnvironmentClass Environment;


// Dynamic initializer (0x004B5860, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B5880, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B58A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B58C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B58E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B5900, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B5940, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B5970, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B59A0, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B59D0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B59E0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Dynamic initializer (0x004B5A00, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_004b5830.cpp; no source of its own.


// Emitted by the checkpoint file-global object in the first static draft.


/// <summary>
/// Creates the carry over environment in its empty state.
/// This routine ensures that a scenario played without a predecessor inherits nothing
/// from a previous mission.
/// </summary>
// Emitted by the checkpoint file-global object in the first static draft.


EnvironmentClass::EnvironmentClass() : Credits(0),TimerRemaining(0),Difficulty(1),Stage(0)
{
 memset(Variables,0,sizeof(Variables));
}


/// <summary>
/// Destroys the carry over environment object.
/// </summary>
EnvironmentClass::~EnvironmentClass() {}


void EnvironmentClass::Capture_004B5AB0()
{
 for(int i=0;i<50;++i) Variables[i]=Scen->Variables[i].Value;
 Credits=PlayerPtr->Available_Money();
 TimerRemaining=Scen->MissionTimer.Value();
 Difficulty=PlayerPtr->Difficulty;
 Stage=Scen->Stage;
}


// void EnvironmentClass::Restore_004B5B40() (0x004B5B40)
// Puts a captured checkpoint back: the 50 local variables, the credits scaled by the
// scenario's carry-over factor (capped by its carry-over cap unless -1) as a refund that
// is also added to +0x124, the difficulty, and with TimerInherit a positive remaining
// time restarts the mission timer; then the +0x2304 word.

void EnvironmentClass::Restore_004B5B40()
{
    for (int i = 0; i < 50; ++i) {
        Scen->Set_Global_To(i, Variables[i]);
    }
    int cap = Scen->CarryOverCap;
    double money = Credits * Scen->CarryOverMoney;
    if (cap != -1) {
        money = money < (double)cap ? money : (double)cap;
    }
    int credits = (int)money;
    PlayerPtr->Refund_Money(credits);
    PlayerPtr->unknown_124 += credits;
    PlayerPtr->Assign_Handicap(Difficulty);
    if (Scen->IsInheritTimer) {
        if (TimerRemaining > 0) {
            Scen->MissionTimer = CDTimerClass<FrameTimerClass>(TimerRemaining);
            Scen->MissionTimer.Start();
        }
    }
    Scen->Stage = Stage;
}


void EnvironmentClass::Read_004B5C50(IStream *stream)
{
    stream->Read(this,sizeof(*this),0);
}


void EnvironmentClass::Write_004B5C70(IStream *stream)
{
    stream->Write(this,sizeof(*this),0);
}
