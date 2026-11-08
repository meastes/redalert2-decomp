// Initialised globals WinMain (0x0068E280, native/drafts.json) reads before
// Main_Game, from adopted text: the TibSun archive (GPL-3.0-or-later with EA
// section 7 terms, THIRD_PARTY.md), whose startup/_surface/sidebar/_pk files are
// the same globals one year earlier. Each was compared privately with the pinned
// target at the address named (all equal); no bytes from the executable.

// 0x0079AA70: the key text Init_Keys loads (TibSun code/_pk.cpp Keys[], release
// build: the public key only).
char startup_keys_79aa70[] =
    "[PublicKey]\n"
    "1=AihRvNoIbTn85FZRYNZRcT+i6KpU+maCsEqr3Q5q+LDB5tH7Tz2qQ38V\n"
    "\n";

// 0x007F504C: Startup.CPP's __FILE__ for StringTable::Load_String's reports
// (the module path form of Main_Game's "D:\\Ra2\\Conquer.CPP").
char startup_source_file_7f504c[] = "D:\\Ra2\\Startup.CPP";

// 0x007AB988: the COM servers WinMain makes sure are registered (TibSun
// startup.cpp RegisterTheseDLLs: CLSID_BlowfishObject, "blowfish.dll").
struct ra2_guid;
extern "C" const ra2_guid ra2_clsid_blowfish __asm__("_CLSID_Blowfish");
struct RegStruct { ra2_guid const *clsid; char const *name; };
extern "C" const RegStruct RegisterTheseDLLs[1] = {{&ra2_clsid_blowfish, "blowfish.dll"}};

// 0x007CE008: HicolorFlag (TibSun code/_surface.cpp: true). The tree knows the
// address by two names; one storage.
bool unknown_hicolor_7ce008 = true;
__asm__(".globl _unknown_flag_7ce008\n.set _unknown_flag_7ce008, _unknown_hicolor_7ce008\n");

// 0x007F4CC4: VideoBackBufferAllowed (TibSun startup.cpp: true; WinMain then
// reads [Video] VideoBackBuffer with that default).
bool unknown_tile_sysmem_7f4cc4 = true;

// 0x007ADD80: the sidebar width (TibSun sidebar.h SIDE_WIDTH = 168).
int unknown_int_7add80 = 168;

// 0x00A4168C (.bss): set by the command line to call Main_Game without the
// exception handler (zero, as the loader gives .bss).
bool startup_no_exception_handler_a4168c;

// 0x007D515C..0x007D530C: the launcher handshake (CopyProtection) strings, from
// ra2-decomp src/copyprotection.cpp (GPL-3.0-or-later, the Yuri's Revenge twin
// of RA2's 0x004918F0/0x00491930/0x00491B00; THIRD_PARTY.md). The native host
// plays the launcher's part with the same two names (platform/sdl/main.cpp).
extern "C" char const launch_mutex_name_007d515c[] = "48BC11BD-C4D7-466b-8A31-C6ABBAD47B3E";
extern "C" char const launch_check_log_007d5184[] = "COPYPROTECTION - Checking if launcher is running\n";
extern char const launcher_notify_failed_log_007d51b8[] = "***** Failed to notify launcher!\n";
extern char const launcher_notified_log_007d51dc[] = "Launcher notified.\n";
extern char const launcher_event_007d51f0[] = "D6E7FC97-64F9-4d28-B52C-754EDF721C6F";
extern char const launcher_notify_log_007d5218[] = "COPYPROTECTION - Notify launcher\n";
extern char const launcher_message_log_007d523c[] = "The message says: %s\n";
extern char const launcher_map_failed_log_007d5254[] = "***** MapViewOfFileEx() Failed!";
extern char const launcher_received_log_007d5274[] = "COPYPROTECTION - Received message from launcher.\n";
extern char const launcher_registry_value_007d52a8[] = "ID";
extern char const launcher_registry_key_007d52ac[] = "WChat\\SysID";
extern char const launcher_shutdown_log_007d530c[] = "COPYPROTECTION - Shutdown\n";

// 0x007F3214: the call-waiting labels SessionClass (0x0066EAA0) loads during
// String_Table_Init. The recovered source names the first (TXT_CWAIT_STAR70,
// session unit comment); the rest follow Red Alert's SESSION.CPP call-wait list
// ("*70,", "70#,", "1170,", then the custom entry) spelled as labels of the
// user's ra2.csf. ORDER AND NAMES 2-4 ARE A HYPOTHESIS from that text (compared
// privately with the target: equal); check when Session.CPP's data is recovered.
char const *unknown_call_wait_labels_7f3214[4] = {
    "TXT_CWAIT_STAR70", "TXT_CWAIT_70NUM", "TXT_CWAIT_1170", "TXT_CUSTOM",
};

// 0x007D5E9C: the entry Query_DLL_Version (0x0049EE00) asks a system DLL for:
// the documented shell DLL version export (shlwapi.h DLLGETVERSIONPROC).
extern char const dll_version_entry_name_007d5e9c[] = "DllGetVersion";
