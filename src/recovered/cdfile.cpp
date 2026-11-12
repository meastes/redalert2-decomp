/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// From mischa85/RedAlert2 dbca015 src/cdfile.cpp (matching RA2 1.006 decomp; TibSun cdfile.cpp); modified:
// the function bodies are this tree's accepted ones, Is_Disk_Inserted is a static member, the find
// handle is the static member FindHandle, and the template's disabled Set_Search_Drives (#if 0) is
// left out (modules.json cdfile).

/* $Header: /CounterStrike/CDFILE.CPP 1     3/03/97 10:24a Joe_bostic $ */
/***********************************************************************************************
 ***             C O N F I D E N T I A L  ---  W E S T W O O D   S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Westwood Library                                             *
 *                                                                                             *
 *                    File Name : CDFILE.CPP                                                   *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 18, 1994                                             *
 *                                                                                             *
 *                  Last Update : September 22, 1995 [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CDFileClass::Clear_Search_Drives -- Removes all record of a search path.                  *
 *   CDFileClass::Open -- Opens the file object -- with path search.                           *
 *   CDFileClass::Open -- Opens the file wherever it can be found.                             *
 *   CDFileClass::Set_Name -- Performs a multiple directory scan to set the filename.          *
 *   CDFileClass::Set_Search_Drives -- Sets a list of search paths for file access.            *
 *   Is_Disk_Inserted -- Checks to see if a disk is inserted in specified drive.               *
 *   harderr_handler -- Handles hard DOS errors.                                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#define CD_FILE_DECLARE_WSTRING_CTOR
#include "shared/cd_file_class.hpp"
#include "shared/wstring.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/win32_find.hpp"

/*
**	Pointer to the first search path record.
*/
CDSearchDrive *CDFileClass::First = 0;

int  CDFileClass::CurrentCDDrive = 0;
int  CDFileClass::LastCDDrive = 0;
char CDFileClass::RawPath[512] = {0};


/// <summary>
/// Constructs a CD file object for the file specified.
/// The name is run through the multiple drive scan, so the object ends up referring to
/// wherever the file actually lives -- the hard drive, a search path, or the CD.
/// </summary>
/// <param name="filename">The name of the file this object should refer to.</param>
CDFileClass::CDFileClass(char const *filename) :
    IsDisabled(false)
{
    CDFileClass::Set_Name(filename);
}


/**
 *  RA2: Constructs a file object for the file named by the string object.
 */
CDFileClass::CDFileClass(Wstring const &filename) : IsDisabled(false)
{
    CDFileClass::Set_Name(filename.Peek_Buffer());
}


/// <summary>
/// Constructs a CD file object with no file attached to it yet.
/// Use Set_Name to give the object a file to work with before trying to open it.
/// </summary>
CDFileClass::CDFileClass() :
    IsDisabled(false)
{
}


/***********************************************************************************************
 * Is_Disk_Inserted -- Checks to see if a disk is inserted in specified drive.                 *
 *                                                                                             *
 *    This routine will examine the drive specified to see if there is a disk inserted. It     *
 *    can be used for floppy drives as well as for the CD-ROM.                                 *
 *                                                                                             *
 * INPUT:   disk  -- The drive number to examine. 0=A, 1=B, etc.                               *
 *                                                                                             *
 * OUTPUT:  bool; Is a disk inserted into the specified drive?                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/20/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
int __fastcall CDFileClass::Is_Disk_Inserted(int drive)
{
    char scan[] = "?:\\*.*";
    WIN32_FIND_DATA finddata;
    scan[0] = (char)(drive + 'A');
    HANDLE handle = FindFirstFileA(scan, &finddata);
    if (handle == INVALID_HANDLE_VALUE) {
        return 0;
    }
    FindClose(handle);
    return 1;
}


/***********************************************************************************************
 * CDFileClass::Open -- Opens the file object -- with path search.                             *
 *                                                                                             *
 *    This will open the file object, but since the file object could have been constructed    *
 *    with a pathname, this routine will try to find the file first. For files opened for      *
 *    writing, then use the existing filename without performing a path search.                *
 *                                                                                             *
 * INPUT:   rights   -- The access rights to use when opening the file                         *
 *                                                                                             *
 * OUTPUT:  bool; Was the open successful?                                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int CDFileClass::Open(int rights)
{
    return BufferIOFileClass::Open(rights);
}


/***********************************************************************************************
 * CDFC::Refresh_Search_Drives -- Updates the search path when a CD changes or is added        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/22/96 9:01AM ST : Created                                                              *
 *=============================================================================================*/
void __fastcall CDFileClass::Refresh_Search_Drives()
{
    Clear_Search_Drives();
    Set_Search_Drives(RawPath);
}



/***********************************************************************************************
 * CDFileClass::Set_Search_Drives -- Sets a list of search paths for file access.              *
 *                                                                                             *
 *    This routine sets up a list of search paths to use when accessing files. The path list   *
 *    is scanned if the file could not be found in the current directory. This is the primary  *
 *    method of supporting CD-ROM drives, but is also useful for scanning network and other    *
 *    directories. The pathlist as passed to this routine is of the same format as the path    *
 *    list used by DOS -- paths are separated by semicolons and need not end in an antivirgule.*
 *                                                                                             *
 *    If a path entry begins with "?:" then the question mark will be replaced with the first  *
 *    CD-ROM drive letter available. If there is no CD-ROM driver detected, then this path     *
 *    entry will be ignored. By using this feature, you can always pass the CD-ROM path        *
 *    specification to this routine and it will not break if the CD-ROM is not loaded (as in   *
 *    the case during development).                                                            *
 *                                                                                             *
 *    Here is an example path specification:                                                   *
 *                                                                                             *
 *       Set_Search_Drives("DATA;?:\DATA;F:\PROJECT\DATA");                                    *
 *                                                                                             *
 *    In this example, the current directory will be searched first, followed by a the         *
 *    subdirectory "DATA" located off of the current directory. If not found, then the CD-ROM  *
 *    will be searched in a directory called "\DATA". If not found or the CD-ROM is not        *
 *    present, then it will look to the hard coded path of "F:\PROJECTS\DATA" (maybe a         *
 *    network?). If all of these searches fail, the file system will default to the current    *
 *    directory and let the normal file error system take over.                                *
 *                                                                                             *
 * INPUT:   pathlist -- Pointer to string of path specifications (separated by semicolons)     *
 *                      that will be used to search for files.                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/18/1994 JLB : Created.                                                                 *
 *   05/21/1996 ST  : Modified to recognise multiple CD drives                                 *
 *=============================================================================================*/
int __fastcall CDFileClass::Set_Search_Drives(char *pathlist)
{
    bool found = false;
    if (pathlist == 0) {
        return 0;
    }

    char *ptr = _strdup(pathlist);
    if (ptr != RawPath) {
        strcat(RawPath, ";");
        strcat(RawPath, ptr);
    }

    char const *path = strtok(ptr, ";");
    while (path != 0) {
        if (strlen(path) != 0) {
            char pathname[_MAX_PATH];
            strcpy(pathname, path);
            char last = pathname[strlen(pathname) - 1];
            if (last != ':' && last != '\\') {
                strcat(pathname, "\\");
            }
            if (_strnicmp(pathname, "?:", 2) == 0) {
                if (CurrentCDDrive != 0) {
                    found = true;
                    if (Get_CD_Index(CurrentCDDrive, 120) > -1) {
                        pathname[0] = (char)(CurrentCDDrive + 'A');
                        Add_Search_Drive(pathname);
                    }
                }
            } else {
                found = true;
                Add_Search_Drive(pathname);
            }
        }
        path = strtok(0, ";");
    }
    free(ptr);
    return !found;
}


/***********************************************************************************************
 * CDFC::Add_Search_Drive -- Add a new path to the search path list                            *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    path                                                                              *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/22/96 10:12AM ST : Created                                                             *
 *=============================================================================================*/
void __fastcall CDFileClass::Add_Search_Drive(char *path)
{
    CDSearchDrive *drive = new CDSearchDrive;
    drive->Path = _strdup(path);
    drive->Next = 0;

    if (First == 0) {
        First = drive;
    } else {
        CDSearchDrive *last = First;
        while (last->Next != 0) {
            last = last->Next;
        }
        last->Next = drive;
    }
}


/***********************************************************************************************
 * CDFC::Set_CD_Drive -- sets the current CD drive letter                                      *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    5/22/96 9:39AM ST : Created                                                              *
 *=============================================================================================*/
void __fastcall CDFileClass::Set_CD_Drive(int drive)
{
    LastCDDrive = CurrentCDDrive;
    CurrentCDDrive = drive;
}


/***********************************************************************************************
 * CDFileClass::Clear_Search_Drives -- Removes all record of a search path.                    *
 *                                                                                             *
 *    Use this routine to clear out any previous path(s) set with Set_Search_Drives()          *
 *    function.                                                                                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
void __fastcall CDFileClass::Clear_Search_Drives()
{
    CDSearchDrive *drive = First;
    while (drive != 0) {
        CDSearchDrive *next = drive->Next;
        if (drive->Path != 0) {
            free((void *)drive->Path);
        }
        delete drive;
        drive = next;
    }
    First = 0;
}


/***********************************************************************************************
 * CDFileClass::Set_Name -- Performs a multiple directory scan to set the filename.            *
 *                                                                                             *
 *    This routine will scan all the directories specified in the path list and if the file    *
 *    was found in one of the directories, it will set the filename to a composite of the      *
 *    correct directory and the filename. It is used to allow path searching when searching    *
 *    for files. Typical use is to support CD-ROM drives. This routine examines the current    *
 *    directory first before scanning through the path list. If after scanning the entire      *
 *    path list, the file still could not be found, then the file object's name is set with    *
 *    just the raw filename as passed to this routine.                                         *
 *                                                                                             *
 * INPUT:   filename -- Pointer to the filename to set as the name of this file object.        *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the final and complete filename of this file object. This     *
 *          may have a path attached to the file.                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
char const *CDFileClass::Set_Name(char const *filename)
{
    BufferIOFileClass::Set_Name(filename);
    if (IsDisabled || First == 0 || BufferIOFileClass::Is_Available()) {
        return File_Name();
    }
    for (CDSearchDrive *drive = First; drive != 0; drive = drive->Next) {
        char path[260];
        strcpy(path, drive->Path);
        strcat(path, filename);
        BufferIOFileClass::Set_Name(path);
        if (BufferIOFileClass::Is_Available()) {
            return File_Name();
        }
    }
    BufferIOFileClass::Set_Name(filename);
    return File_Name();
}


/***********************************************************************************************
 * CDFileClass::Open -- Opens the file wherever it can be found.                               *
 *                                                                                             *
 *    This routine is similar to the RawFileClass open except that if the file is being        *
 *    opened only for READ access, it will search all specified directories looking for the    *
 *    file. If after a complete search the file still couldn't be found, then it is opened     *
 *    using the normal BufferIOFileClass system -- resulting in normal error procedures.       *
 *                                                                                             *
 * INPUT:   filename -- Pointer to the override filename to supply for this file object. It    *
 *                      would be the base filename (sans any directory specification).         *
 *                                                                                             *
 *          rights   -- The access rights to use when opening the file.                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the file opened successfully? If so then the filename may be different   *
 *                than requested. The location of the file can be determined by examining the  *
 *                filename of this file object. The filename will contain the complete         *
 *                pathname used to open the file.                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/18/1994 JLB : Created.                                                                 *
 *=============================================================================================*/
int CDFileClass::Open(char const *filename, int rights)
{
    BufferIOFileClass::Close();
    if (filename == 0) {
        Error(FILE_ERROR_NO_FILE_NAME, false, 0);
    }
    if (!IsDisabled && rights != FILE_RIGHTS_WRITE) {
        Set_Name(filename);
        return BufferIOFileClass::Open(rights);
    }
    BufferIOFileClass::Set_Name(filename);
    return BufferIOFileClass::Open(rights);
}


void *CDFileClass::FindHandle = (void *)-1;     // INVALID_HANDLE_VALUE while no search runs

/// <summary>
/// Begins a search for the files matching the wildcard specified.
/// This routine will look in the current directory first and then work along the search
/// drive list, settling on the first drive that has a match. Only ordinary files qualify;
/// directories and system, hidden, or temporary files are passed over. Any search still
/// in progress is closed off first.
/// </summary>
/// <param name="fname">The wildcard to search for; filled in with the file found.</param>
/// <returns>bool; Was a matching file found?</returns>
/// <remarks>Be sure that the buffer is big enough to hold the filename returned.</remarks>
bool __fastcall CDFileClass::Find_First_File(char *filename)
{
    if (filename == 0) {
        return false;
    }
    if (FindHandle != INVALID_HANDLE_VALUE) {
        FindClose(FindHandle);
        FindHandle = INVALID_HANDLE_VALUE;
    }

    char path[_MAX_PATH];
    WIN32_FIND_DATA finddata;
    strcpy(path, filename);
    HANDLE handle = FindFirstFileA(path, &finddata);
    if (handle != INVALID_HANDLE_VALUE && (finddata.dwFileAttributes & 0x116) == 0) {
        strcpy(filename, finddata.cFileName);
        FindHandle = handle;
        return true;
    }

    for (CDSearchDrive *drive = First; drive != 0; drive = drive->Next) {
        strcpy(path, drive->Path);
        strcat(path, filename);
        handle = FindFirstFileA(path, &finddata);
        if (handle != INVALID_HANDLE_VALUE && (finddata.dwFileAttributes & 0x116) == 0) {
            strcpy(filename, finddata.cFileName);
            FindHandle = handle;
            return true;
        }
    }
    return false;
}


/// <summary>
/// Fetches the next file that matches the search in progress.
/// This routine continues the scan begun by Find_First_File, working through the rest of
/// the matches on whichever drive that routine settled upon.
/// </summary>
/// <param name="buffer">Buffer to fill in with the name of the file found.</param>
/// <returns>bool; Was another matching file found?</returns>
/// <remarks>Be sure that the buffer is big enough to hold the filename returned.</remarks>
bool __fastcall CDFileClass::Find_Next_File(char *filename)
{
    if (filename == 0) {
        return false;
    }
    if (FindHandle != INVALID_HANDLE_VALUE) {
        WIN32_FIND_DATA finddata;
        if (FindNextFileA(FindHandle, &finddata) == TRUE) {
            strcpy(filename, finddata.cFileName);
            return true;
        }
    }
    filename[0] = '\0';
    return false;
}


/// <summary>
/// Closes off the file search that is in progress.
/// Call this routine when the results of a Find_First_File scan are no longer wanted, so
/// that the search handle held on the game's behalf is given back to the system.
/// </summary>
void CDFileClass::Find_Close()
{
    if (FindHandle != INVALID_HANDLE_VALUE) {
        FindClose(FindHandle);
        FindHandle = INVALID_HANDLE_VALUE;
    }
}


/*
**	CDFileClass's scalar deleting destructor (0x00401950, kept from another unit's copy) is
**	compiler generated from the inline virtual destructor.
*/
