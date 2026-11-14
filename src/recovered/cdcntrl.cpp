// cdcntrl.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cdcntrl.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cd_control.hpp"
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | ((WORD)((BYTE)(high))) << 8))


// CDControlClass::CDControlClass() (0x0046EA70)
// Clears the whole object.

CDControlClass::CDControlClass()
{
    memset(this, 0, sizeof(*this));
}


// CDControlClass::~CDControlClass() (0x0046EA90)
// Nothing to release.

CDControlClass::~CDControlClass()
{
}


// void CDControlClass::Force_CD_Eject(char drive) (0x0046EAA0)
// Ejects the CD in drive (0 = A:): through VWIN32 when the Windows 95 flag is
// set, else through the volume handle (Eject_CD, expanded inline).
// Assumes: the flag's meaning (hypothesis).

void CDControlClass::Force_CD_Eject(char drive)
{
    if (unknown_flag_839c4c) {
        Eject_CD_Win95(drive);
    } else {
        Eject_CD(drive);
    }
}


// HANDLE CDControlClass::Open_Removable_Volume(char drive) (0x0046EC60)
// Opens "\\.\X:" for a removable or CD-ROM drive (0 = A:), read/write for
// removable media and read-only for CD-ROMs; INVALID_HANDLE_VALUE otherwise.

HANDLE CDControlClass::Open_Removable_Volume(char drive)
{
    char root[5];
    char volume_name[8];
    DWORD access;

    sprintf(root, "%c:\\", drive + 'A');
    switch (GetDriveTypeA(root)) {
    case DRIVE_REMOVABLE:
        access = GENERIC_READ | GENERIC_WRITE;
        break;
    case DRIVE_CDROM:
        access = GENERIC_READ;
        break;
    default:
        Debug_Printf("Attempt to open non-removable volume for locking or ejection\n");
        return INVALID_HANDLE_VALUE;
    }

    sprintf(volume_name, "\\\\.\\%c:", drive + 'A');
    HANDLE volume = CreateFileA(volume_name, access, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);
    if (volume == INVALID_HANDLE_VALUE) {
        Debug_Printf("Unable to open drive %c: for ejection\n", drive + 'A');
    }
    return volume;
}


// bool CDControlClass::Close_Removable_Volume(HANDLE volume) (0x0046ED10)

bool CDControlClass::Close_Removable_Volume(HANDLE volume)
{
    return CloseHandle(volume) ? true : false;
}


// bool CDControlClass::Lock_Volume(HANDLE volume) (0x0046ED30)
// Locks the volume (FSCTL), up to 20 attempts 500 ms apart.

bool CDControlClass::Lock_Volume(HANDLE volume)
{
    DWORD returned;
    for (int attempt = 0; attempt < 20; attempt++) {
        if (DeviceIoControl(volume, FSCTL_LOCK_VOLUME, 0, 0, 0, 0, &returned, 0)) {
            return true;
        }
        Debug_Printf("DeviceIoControl failed to lock volume. Error %d - %s\n", GetLastError(), 0);
        Sleep(500);
    }
    return false;
}


// bool CDControlClass::Unlock_Volume(HANDLE volume) (0x0046EDA0)
// Unlocks the volume (FSCTL), up to 20 attempts 500 ms apart.

bool CDControlClass::Unlock_Volume(HANDLE volume)
{
    DWORD returned;
    for (int attempt = 0; attempt < 20; attempt++) {
        if (DeviceIoControl(volume, FSCTL_UNLOCK_VOLUME, 0, 0, 0, 0, &returned, 0)) {
            return true;
        }
        Debug_Printf("DeviceIoControl failed to unlock volume. Error %d - %s\n", GetLastError(), 0);
        Sleep(500);
    }
    return false;
}


// bool CDControlClass::Dismount_Volume(HANDLE volume) (0x0046EE10)

bool CDControlClass::Dismount_Volume(HANDLE volume)
{
    DWORD returned;
    bool result = DeviceIoControl(volume, FSCTL_DISMOUNT_VOLUME, 0, 0, 0, 0, &returned, 0);
    if (!result) {
        Debug_Printf("DeviceIoControl failed to dismount volume. Error %d - %s\n", GetLastError(), 0);
    }
    return result;
}


// bool CDControlClass::Prevent_Removal_Of_Volume(HANDLE volume, bool prevent) (0x0046EE60)
// Allows or prevents ejecting the media (IOCTL_STORAGE_MEDIA_REMOVAL).

bool CDControlClass::Prevent_Removal_Of_Volume(HANDLE volume, bool prevent)
{
    DWORD returned;
    PREVENT_MEDIA_REMOVAL removal;
    removal.PreventMediaRemoval = prevent;
    bool result = DeviceIoControl(volume, IOCTL_STORAGE_MEDIA_REMOVAL, &removal, sizeof(removal), 0, 0,
                                  &returned, 0);
    if (!result) {
        Debug_Printf("DeviceIoControl failed to prevent media removal. Error %d - %s\n", GetLastError(), 0);
    }
    return result;
}


// bool CDControlClass::Auto_Eject_Volume(HANDLE volume) (0x0046EEB0)
// Ejects the media (IOCTL_STORAGE_EJECT_MEDIA).

bool CDControlClass::Auto_Eject_Volume(HANDLE volume)
{
    DWORD returned;
    bool result = DeviceIoControl(volume, IOCTL_STORAGE_EJECT_MEDIA, 0, 0, 0, 0, &returned, 0);
    if (!result) {
        Debug_Printf("DeviceIoControl failed to eject media. Error %d - %s\n", GetLastError(), 0);
    }
    return result;
}


// bool CDControlClass::Eject_CD(char drive) (0x0046EF00)
// Windows NT: opens the volume, locks and dismounts it, allows removal and
// ejects the media; true when the media was ejected.

bool CDControlClass::Eject_CD(char drive)
{
    bool ejected = false;
    HANDLE volume = Open_Removable_Volume(drive);
    if (volume == INVALID_HANDLE_VALUE) {
        return false;
    }
    if (Lock_Volume(volume) && Dismount_Volume(volume) && Prevent_Removal_Of_Volume(volume, false)
        && Auto_Eject_Volume(volume)) {
        ejected = true;
    }
    Close_Removable_Volume(volume);
    return ejected;
}


// bool CDControlClass::Lock_CD_Tray_NT(char drive) (0x0046F0C0)
// Windows NT: with the volume locked, prevents ejection of the media.
// False only when the volume cannot be opened.

bool CDControlClass::Lock_CD_Tray_NT(char drive)
{
    HANDLE volume = Open_Removable_Volume(drive);
    if (volume == INVALID_HANDLE_VALUE) {
        return false;
    }
    bool changed = false;   // computed but unused: the result is true either way
    if (Lock_Volume(volume) && Prevent_Removal_Of_Volume(volume, true)) {
        changed = true;
    }
    Unlock_Volume(volume);
    Close_Removable_Volume(volume);
    return true;
}


// bool CDControlClass::Unlock_CD_Tray_NT(char drive) (0x0046F250)
// Windows NT: with the volume locked, allows ejection of the media.
// False only when the volume cannot be opened.

bool CDControlClass::Unlock_CD_Tray_NT(char drive)
{
    HANDLE volume = Open_Removable_Volume(drive);
    if (volume == INVALID_HANDLE_VALUE) {
        return false;
    }
    bool changed = false;   // computed but unused: the result is true either way
    if (Lock_Volume(volume) && Prevent_Removal_Of_Volume(volume, false)) {
        changed = true;
    }
    Unlock_Volume(volume);
    Close_Removable_Volume(volume);
    return true;
}


// void CDControlClass::Eject_CD_Win95(char drive) (0x0046F3E0)
// Windows 95: takes the logical volume lock, removes the media locks and
// ejects the media of drive (0 = A:) through VWIN32.
// The early failure paths leave the VWIN32 handle open, as in the original.

void CDControlClass::Eject_CD_Win95(char drive)
{
    drive++;
    HANDLE vwin32 = Open_VWin32();
    if (!Lock_Logical_Volume(vwin32, drive, 0, 0)) {
        Debug_Printf("Unable to lock volume %c:\n", drive + 'A' - 1);
        return;
    }
    if (!Unlock_Media(vwin32, drive)) {
        Debug_Printf("Could not unlock media from drive %c:\n", drive + 'A' - 1);
        Unlock_Logical_Volume(vwin32, drive);
        return;
    }
    if (!Eject_Media(vwin32, drive)) {
        Debug_Printf("Could not eject media from drive %c:\n", drive + 'A' - 1);
    }
    Unlock_Logical_Volume(vwin32, drive);
    Close_VWin32(vwin32);
}


// bool CDControlClass::Lock_CD_Drive_95(char drive) (0x0046F510)
// Windows 95: under the logical volume lock, locks the media of drive
// (0 = A:) so the tray cannot be opened.
// Name from its own debug strings.

bool CDControlClass::Lock_CD_Drive_95(char drive)
{
    Debug_Printf("CDControlClass::Lock_CD_Drive_95\n");
    bool result = true;
    Debug_Printf("Preventing ejection on CD drive %c\n", drive + 'A');
    drive++;
    HANDLE vwin32 = Open_VWin32();
    Debug_Printf("About to lock logical volume to enable exclusive access\n");
    if (!Lock_Logical_Volume(vwin32, drive, 0, 0)) {
        Debug_Printf("Unable to lock volume %c:\n", drive + 'A' - 1);
        return false;
    }
    Debug_Printf("Volume locked OK\n");
    Debug_Printf("About to prevent CD tray ejection\n");
    if (!Lock_Media(vwin32, drive)) {
        Debug_Printf("Could not lock CD tray in drive %c:\n", drive + 'A' - 1);
        result = false;
    } else {
        Debug_Printf("CD tray ejection disabled OK\n");
    }
    Unlock_Logical_Volume(vwin32, drive);
    Close_VWin32(vwin32);
    Debug_Printf("CDControlClass::Lock_CD_Drive_95 returning %s\n", result ? "true" : "false");
    return result;
}


// bool CDControlClass::Unlock_CD_Drive_95(char drive) (0x0046F630)
// Windows 95: under the logical volume lock, unlocks the media of drive
// (0 = A:) so the tray can be opened again.
// Name from its own debug strings.

bool CDControlClass::Unlock_CD_Drive_95(char drive)
{
    Debug_Printf("CDControlClass::Unlock_CD_Drive_95\n");
    bool result = true;
    Debug_Printf("Allowing ejection on CD drive %c\n", drive + 'A');
    drive++;
    HANDLE vwin32 = Open_VWin32();
    Debug_Printf("About to lock logical volume to enable exclusive access\n");
    if (!Lock_Logical_Volume(vwin32, drive, 0, 0)) {
        Debug_Printf("Unable to lock volume %c:\n", drive + 'A' - 1);
        return false;
    }
    Debug_Printf("Volume locked OK\n");
    Debug_Printf("About to allow CD tray ejection\n");
    if (!Unlock_Media(vwin32, drive)) {
        Debug_Printf("Could not unlock CD tray in drive %c:\n", drive + 'A' - 1);
        result = false;
    } else {
        Debug_Printf("CD tray ejection enabled OK\n");
    }
    Unlock_Logical_Volume(vwin32, drive);
    Close_VWin32(vwin32);
    Debug_Printf("CDControlClass::Unlock_CD_Drive_95 returning %s\n", result ? "true" : "false");
    return result;
}


// Unlock all pending removable-media locks; original body 0046F750.
bool CDControlClass::Unlock_Media(HANDLE vwin32, char drive)
{
    DIOC_REGISTERS regs = {0};
    PARAMBLOCK param = {0};
    DWORD returned;
    param.Operation = 2;
    regs.reg_EAX = 0x440D;
    regs.reg_EBX = drive;
    regs.reg_ECX = 0x848;
    regs.reg_EDX = (DWORD)&param;
    bool result = DeviceIoControl(vwin32, 1, &regs, sizeof(regs), &regs, sizeof(regs), &returned, 0) != 0;
    if (result) {
        if (regs.reg_Flags & 1) {
            result = regs.reg_EAX == 0xB0 || regs.reg_EAX == 1;
        }
    } else {
        Debug_Printf("DeviceIoControl failed to perform DOS IO control function. Error %d - %s\n", GetLastError(), 0);
    }
    if (!result) return false;
    Debug_Printf("Apparent number of locks to undo = %d\n", param.NumLocks);
    if (!param.NumLocks) param.NumLocks = 1;
    for (int count = 0; count < param.NumLocks; ++count) {
        param.Operation = 1;
        regs.reg_EAX = 0x440D;
        regs.reg_EBX = drive;
        regs.reg_ECX = 0x848;
        regs.reg_EDX = (DWORD)&param;
        result = DeviceIoControl(vwin32, 1, &regs, sizeof(regs), &regs, sizeof(regs), &returned, 0) != 0;
        if (!result) {
            Debug_Printf("DeviceIoControl failed to perform DOS IO control function. Error %d - %s\n", GetLastError(), 0);
        }
        result = result && !(regs.reg_Flags & 1);
        if (!result) break;
    }
    return result;
}


// bool CDControlClass::Lock_Media(HANDLE vwin32, char drive) (0x0046F8C0)
// Windows 95: clears pending locks, then locks the removable media in drive
// (1 = A:) so the tray cannot be opened.
// Assumes: name from Microsoft's VWIN32 sample vocabulary.

// Not expanded into its callers in the original (they call it); VC6 would
// expand it here without the pragma.
#pragma auto_inline(off)
bool CDControlClass::Lock_Media(HANDLE vwin32, char drive)
{
    DIOC_REGISTERS regs = {0};
    PARAMBLOCK param = {0};
    DWORD returned;

    Unlock_Media(vwin32, drive);
    param.Operation = 0;
    regs.reg_EAX = 0x440D;
    regs.reg_EBX = drive;
    regs.reg_ECX = 0x0848;
    regs.reg_EDX = (DWORD)&param;
    bool result = DeviceIoControl(vwin32, VWIN32_DIOC_DOS_IOCTL, &regs, sizeof(regs), &regs, sizeof(regs),
                                  &returned, 0);
    if (!result) {
        Debug_Printf("DeviceIoControl failed to perform DOS IO control function. Error %d - %s\n",
                     GetLastError(), 0);
    }
    result = result && !(regs.reg_Flags & CARRY_FLAG);
    return result;
}
#pragma auto_inline(on)


// bool CDControlClass::Eject_Media(HANDLE vwin32, char drive) (0x0046F980)
// Windows 95: ejects the removable media in drive (1 = A:), IOCTL 440Dh/0849h.
// Assumes: name from Microsoft's VWIN32 sample vocabulary.

bool CDControlClass::Eject_Media(HANDLE vwin32, char drive)
{
    DIOC_REGISTERS regs = {0};
    DWORD returned;

    regs.reg_EAX = 0x440D;
    regs.reg_EBX = drive;
    regs.reg_ECX = 0x0849;
    bool result = DeviceIoControl(vwin32, VWIN32_DIOC_DOS_IOCTL, &regs, sizeof(regs), &regs, sizeof(regs),
                                  &returned, 0);
    if (!result) {
        Debug_Printf("DeviceIoControl failed to perform DOS IO control function. Error %d - %s\n",
                     GetLastError(), 0);
    }
    result = result && !(regs.reg_Flags & CARRY_FLAG);
    return result;
}


// HANDLE WINAPI CDControlClass::Open_VWin32() (0x0046FA10)
// Opens the VWIN32 device driver that carries DOS IOCTL calls on Windows 95.
// Assumes: WINAPI member (this on the stack, ret 4), as in Microsoft's
// VWIN32 sample OpenVWin32.

HANDLE WINAPI CDControlClass::Open_VWin32()
{
    return CreateFileA("\\\\.\\vwin32", 0, 0, 0, 0, FILE_FLAG_DELETE_ON_CLOSE, 0);
}


// bool WINAPI CDControlClass::Close_VWin32(HANDLE vwin32) (0x0046FA30)
// Closes the VWIN32 device handle. The inventory splits this function at
// 0x0046FA35 (the CloseHandle call); it is one 19-byte body.
// Assumes: WINAPI member (this on the stack, ret 8).

bool WINAPI CDControlClass::Close_VWin32(HANDLE vwin32)
{
    return CloseHandle(vwin32) ? true : false;
}


// bool WINAPI CDControlClass::Lock_Logical_Volume(HANDLE vwin32, char drive, char lock_level, short permissions) (0x0046FA50)
// Windows 95: takes a logical volume lock (IOCTL 440Dh minor 4Ah) on drive
// (1 = A:), trying the FAT32 device category 48h before 08h; after both fail
// it logs the DOS error, waits a second and retries, five rounds in all.
// Assumes: WINAPI member; names from Microsoft's VWIN32 sample.


bool WINAPI CDControlClass::Lock_Logical_Volume(HANDLE vwin32, char drive, char lock_level, short permissions)
{
    DIOC_REGISTERS regs = {0};
    DWORD returned;
    int retries = 5;
    bool result;

    do {
        BYTE category = 0;
        do {
            category = category ? 0x08 : 0x48;
            regs.reg_EAX = 0x440D;
            regs.reg_EBX = MAKEWORD(drive, lock_level);
            regs.reg_ECX = MAKEWORD(0x4A, category);
            regs.reg_EDX = permissions;
            result = DeviceIoControl(vwin32, VWIN32_DIOC_DOS_IOCTL, &regs, sizeof(regs), &regs,
                                          sizeof(regs), &returned, 0);
            if (!result) {
                Debug_Printf("DeviceIoControl failed to perform DOS IO control function. Error %d - %s\n",
                             GetLastError(), 0);
            }
            result = result && !(regs.reg_Flags & CARRY_FLAG);
            if (result) {
                return true;
            }
        } while (category != 0x08);

        retries--;
        switch (regs.reg_EAX) {
        case 0x01:
            Debug_Printf("LOCK return eax error 'Function not supported'\n");
            break;
        case 0xB0:
            Debug_Printf("LOCK return eax error 'Volume not locked in drive'\n");
            break;
        case 0xB2:
            Debug_Printf("LOCK return eax error 'Volume not removable'\n");
            break;
        case 0xB4:
            Debug_Printf("LOCK return eax error 'Lock count exceeded'\n");
            break;
        default:
            Debug_Printf("LOCK return eax error %d\n", regs.reg_EAX);
            break;
        }
        Sleep(1000);
    } while (retries > 0);
    return false;
}


// bool WINAPI CDControlClass::Unlock_Logical_Volume(HANDLE vwin32, char drive) (0x0046FC80)
// Windows 95: releases a logical volume lock (IOCTL 440Dh minor 6Ah) on
// drive (1 = A:), trying the FAT32 device category 48h before 08h.
// Assumes: WINAPI member; names from Microsoft's VWIN32 sample.


// Not expanded into its callers in the original (they call it); VC6 would
// expand it here without the pragma.
#pragma auto_inline(off)
bool WINAPI CDControlClass::Unlock_Logical_Volume(HANDLE vwin32, char drive)
{
    DIOC_REGISTERS regs = {0};
    DWORD returned;
    BYTE category = 0;
    bool result;

    do {
        category = category ? 0x08 : 0x48;
        regs.reg_EAX = 0x440D;
        regs.reg_EBX = drive;
        regs.reg_ECX = MAKEWORD(0x6A, category);
        result = DeviceIoControl(vwin32, VWIN32_DIOC_DOS_IOCTL, &regs, sizeof(regs), &regs, sizeof(regs),
                                 &returned, 0);
        if (!result) {
            Debug_Printf("DeviceIoControl failed to perform DOS IO control function. Error %d - %s\n",
                         GetLastError(), 0);
        }
        result = result && !(regs.reg_Flags & CARRY_FLAG);
    } while (!result && category != 0x08);
    return result;
}
#pragma auto_inline(on)
