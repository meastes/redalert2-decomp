// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (THIRD_PARTY.md).
// From TibSun version.cpp: the version object and its text, clipping and protocol lookups.

#include "shared/version.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"

extern "C" unsigned long __stdcall GetFileVersionInfoSizeA(char const *filename, unsigned long *handle);
extern "C" int __stdcall GetFileVersionInfoA(char const *filename, unsigned long handle, unsigned long size,
    void *data);
extern "C" int __stdcall VerQueryValueA(void const *block, char const *sub_block, void **buffer,
    unsigned int *length);


enum CommProtocolType {
    COMM_PROTOCOL_SINGLE_NO_COMP = 0,   // single frame with no compression
    COMM_PROTOCOL_SINGLE_E_COMP,        // single frame with event compression
    COMM_PROTOCOL_MULTI_E_COMP,         // multiple frame with event compression
    COMM_PROTOCOL_COUNT,
    DEFAULT_COMM_PROTOCOL = COMM_PROTOCOL_MULTI_E_COMP
};

struct VersionProtocolType {
    unsigned int Version;
    CommProtocolType Protocol;
};


// The communications protocol introduced at each version number. A protocol is used from
// its version up to, but not including, the next entry's; the last entry is this version's.
static VersionProtocolType VersionProtocol[] = {
    {0x00001000, COMM_PROTOCOL_SINGLE_NO_COMP},  // (obsolete)
    {0x00002000, COMM_PROTOCOL_SINGLE_E_COMP},   // (obsolete)
    {0x00010000, COMM_PROTOCOL_MULTI_E_COMP},
};


VersionClass::VersionClass(void) :
    Version(0),
    MajorVer(0),
    MinorVer(0),
    MinClipVer(0),
    MaxClipVer(0),
    VersionInit(false),
    MajorInit(false),
    MinorInit(false),
    TextInit(false)
{
    VersionText[0] = '\0';
    VersionName[0] = '\0';
}


unsigned int VersionClass::Version_Number(void)
{
    if (!TextInit) {
        Read_Text_String();
        TextInit = 1;
    }

    if (VersionInit) {
        return Version;
    }

    Version = (Major_Version() << 16) | Minor_Version();
    VersionInit = 1;

    return Version;
}


unsigned short VersionClass::Major_Version(void)
{
    if (!TextInit) {
        Read_Text_String();
        TextInit = 1;
    }

    if (MajorInit) {
        return MajorVer;
    }

    MajorVer = MAJOR_VERSION;
    MajorInit = 1;

    return MajorVer;
}


unsigned short VersionClass::Minor_Version(void)
{
    if (!TextInit) {
        Read_Text_String();
        TextInit = 1;
    }

    if (MinorInit) {
        return MinorVer;
    }

    MinorVer = MINOR_VERSION;
    MinorInit = 1;

    return MinorVer;
}


char *VersionClass::Version_Name(void)
{
    unsigned short adjusted_minor;
    int i;

    adjusted_minor = Minor_Version();
    for (i = 0; i < 4; i++) {
        if ((adjusted_minor & 0x000f) != 0) {
            break;
        }
        adjusted_minor >>= 4;
    }

    sprintf(VersionName, "%d.%3.3d", VerNum.Major_Version(), adjusted_minor);

    return VersionName;
}


// Reads the version label from VERSION.TXT, dropping trailing carriage returns.
void VersionClass::Read_Text_String(void)
{
    RawFileClass file("VERSION.TXT");

    if (file.Is_Available()) {
        file.Read(VersionText, sizeof(VersionText));
        VersionText[sizeof(VersionText) - 1] = '\0';
        while (VersionText[strlen(VersionText) - 1] == '\r') {
            VersionText[strlen(VersionText) - 1] = '\0';
        }
    } else {
        VersionText[0] = '\0';
    }
}


// The protocol of the last table entry not newer than the version, else the newest protocol.
int VersionClass::Version_Protocol(int version)
{
    int i, j;

    j = sizeof(VersionProtocol) / sizeof(VersionProtocolType);

    for (i = j - 1; i >= 0; i--) {
        if ((unsigned int)version >= VersionProtocol[i].Version) {
            return VersionProtocol[i].Protocol;
        }
    }

    return VersionProtocol[j - 1].Protocol;
}


void VersionClass::Init_Clipping(void)
{
    MinClipVer = Min_Version();
    MaxClipVer = Max_Version();
}


// Narrows the clipping range to its overlap with another program's range and returns the
// highest shared version: 0xFFFFFFFF when that range lies above this one, 0 when below.
unsigned int VersionClass::Clip_Version(unsigned int minver, unsigned int maxver)
{
    if (minver > MaxClipVer) {
        return 0xffffffff;
    }

    if (maxver < MinClipVer) {
        return 0;
    }

    if (minver > MinClipVer) {
        MinClipVer = minver;
    }

    if (maxver < MaxClipVer) {
        MaxClipVer = maxver;
    }

    return MaxClipVer;
}


unsigned int VersionClass::Min_Version(void)
{
    return MIN_VERSION;
}


unsigned int VersionClass::Max_Version(void)
{
    return MAX_VERSION;
}


// The ProductVersion string of the program's own version resource, looked up once.
char const *__fastcall Version_Name(void)
{
    static char buffer[512] = {"zzz"};
    static bool empty = true;

    int size;
    void *block;
    unsigned int translate_len;
    unsigned long handle;
    struct LANGANDCODEPAGE {
        unsigned short wLanguage;
        unsigned short wCodePage;
    } *translate;
    char query[128];
    char filename[260];

    if (empty) {
        empty = false;
        if (GetModuleFileNameA(ProgramInstance, filename, sizeof(filename)) > 0) {
            handle = 1;
            size = GetFileVersionInfoSizeA(filename, &handle);
            if (size > 0) {
                block = new unsigned char[size];
                if (GetFileVersionInfoA(filename, handle, size, block)) {
                    VerQueryValueA(block, "\\VarFileInfo\\Translation", (void **)&translate, &translate_len);
                    if (translate_len > 0) {
                        sprintf(query, "\\StringFileInfo\\%04d%04d\\ProductVersion", translate->wLanguage,
                            translate->wCodePage);
                        VerQueryValueA(block, query, (void **)&translate, &translate_len);
                        if (translate_len > 0) {
                            strcpy(buffer, (char const *)translate);
                        }
                    }
                }
                delete [] block;
            }
        }
    }

    return buffer;
}
