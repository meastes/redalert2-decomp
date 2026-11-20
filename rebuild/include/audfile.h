// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// RA2: the session audio file that -record writes and -play reads back (audfile.cpp).
#pragma once

int __fastcall Audio_File_Init(int count, int size); // RA2: 0x00522560
void __fastcall Audio_File_Shutdown(void); // RA2: 0x00522930
int __fastcall Audio_File_Create_Stream(char const * name); // RA2: 0x00522C50
int __fastcall Audio_File_Open_Stream(char const * name); // RA2: 0x00522CB0
