// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (THIRD_PARTY.md).
// Shape from TibSun version.h; Red Alert 2's version numbers and range.
#ifndef RA2_SHARED_VERSION_HPP
#define RA2_SHARED_VERSION_HPP

class VersionClass {
public:
    VersionClass(void);
    virtual ~VersionClass(void) {}

    // The version number: the major version in the high word, the minor in the low word,
    // both read in hex.
    unsigned int Version_Number(void);
    unsigned short Major_Version(void);
    unsigned short Minor_Version(void);

    // The version as text ("major.minor"), with the minor's trailing zero digits trimmed.
    char *Version_Name(void);

    // The version label read from VERSION.TXT.
    char *Version_Text(void) { return VersionText; }

    // The default communications protocol (CommProtocolType) for a version number.
    int Version_Protocol(int version);

    // Version clipping: Init_Clipping starts a session with this program's own range, each
    // Clip_Version narrows it to the overlap with another program's range.
    void Init_Clipping(void);
    unsigned int Clip_Version(unsigned int minver, unsigned int maxver);
    unsigned int Get_Clipped_Version(void) { return MaxClipVer; }

    // The lowest and highest version numbers this program connects to, before clipping.
    unsigned int Min_Version(void);
    unsigned int Max_Version(void);

private:
    void Read_Text_String(void);

    // The program's major and minor version numbers.
    enum VersionEnum {
        MAJOR_VERSION = 0x0001,
        MINOR_VERSION = 0x0006
    };

    // The program connects only to its own version.
    enum VersionRangeEnum {
        MIN_VERSION = 0x20000,
        MAX_VERSION = 0x20000
    };

    unsigned int Version;
    unsigned short MajorVer;
    unsigned short MinorVer;

    // Version_Name's formatted text.
    char VersionName[30];

    // The label from VERSION.TXT (such as "Beta"); empty without the file.
    char VersionText[16];

    unsigned int MinClipVer;
    unsigned int MaxClipVer;

    unsigned VersionInit : 1;
    unsigned MajorInit : 1;
    unsigned MinorInit : 1;
    unsigned TextInit : 1;
};

// The program's version object (defined in the globals unit). lobby.hpp carries a second
// declaration of this class for the units compiled against it: their instruction scheduling
// depends on the declaration set that header shows them (a VC6 trait), so it cannot include
// this file yet. Keep the two declarations' member signatures equal.
extern VersionClass VerNum;

// The product version text from the program's version resource ("zzz" without one).
char const *__fastcall Version_Name(void);

#endif
