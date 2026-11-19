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

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/wwlib/Except.cpp                             $*
 *                                                                                             *
 *                      $Author:: Steve_t                                                     $*
 *                                                                                             *
 *                     $Modtime:: 2/07/02 12:28p                                              $*
 *                                                                                             *
 *                    $Revision:: 14                                                          $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *                                                                                             *
 * Exception_Proc -- Windows dialog callback for the exception dialog                          *
 * Exception_Dialog -- Brings up the exception options dialog.                                 *
 * Add_Txt -- Add the given text to the machine state dump buffer.                             *
 * Dump_Exception_Info -- Dump machine state information into a buffer                         *
 * Exception_Handler -- Exception handler filter function                                      *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h" // RA2: as above
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "_surface.h" // RA2: as above
#include "data.h" // RA2: as above
#include "dbgprint.h" // RA2: as above
#include "getcpu.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "language\language.h" // RA2: as above
#include "ownrdraw.h" // RA2: as above
#include "surface.h" // RA2: as above
#include "version.h" // RA2: as above
#include "win.h" // RA2: as above
#include "windlg.h" // RA2: as above
#include "winfix.h" // RA2: as above
#include "winstub.h" // RA2: as above

#include <imagehlp.h> // RA2: as above

#include <conio.h> // RA2: _cprintf and _getch, the statically linked CRT console routines the recursion notice uses

void __fastcall Audio_File_Flush(void);						// RA2: 0x00522E10; under the audio file critical section, flushes the file the audio streamer is reading (meastes: Audio_File_Flush)
void __fastcall Add_Audio_File_Window(HWND window);		// RA2: 0x00522B20; files a dialog window in the audio file window list (as ownrdraw.cpp declares it)
void __fastcall Remove_Audio_File_Window(HWND window);	// RA2: 0x00522B80; takes it out of the list again
char const * Get_CPU_Type_Name(void);						// RA2: 0x004D2B80, the name of the CPU type ("Pentium" ...)

#define EH_BUFFER_SIZE 65536 // RA2: as above
#define NUM_CODE_BYTES 32 // RA2: as above
#define MAX_STACK_DUMP 1024 // RA2: as above

/*
**	Buffer to dump machine state information to. We don't want to allocate this at run-time
**	in case the exception was caused by a malfunction in the memory system.
*/
static char ExceptionText [EH_BUFFER_SIZE];	// RA2: 0x00855BA8

/*
**	Register dump variables. These are used to allow the game to restart from an arbitrary
**	position after an exception occurs.
*/
unsigned int ExceptionReturnStack = 0;		// RA2: 0x00865C14
unsigned int ExceptionReturnAddress = 0;	// RA2: 0x00865C18
unsigned int ExceptionReturnFrame = 0;		// RA2: 0x00865C1C

/*
**	Number of times the exception handler has recursed. Recursions are bad.
*/
int ExceptionRecursions = -1;				// RA2: 0x007D6AF8

bool UnusedEHBool = true;					// RA2: 0x007D6AFC

/*
**	Definitions to allow run-time linking to the Imagehlp.dll functions.
*/
struct tImageHelpStruct { // RA2: as above
	BOOL  (WINAPI *SymCleanup)(HANDLE hProcess); // RA2: as above
	BOOL  (WINAPI *SymGetSymFromAddr)(HANDLE hProcess, DWORD dwAddr, PDWORD pdwdisplacementacement, PIMAGEHLP_SYMBOL Symbol); // RA2: as above
	BOOL  (WINAPI *SymInitialize)(HANDLE hProcess, PCSTR UserSearchPath, BOOL fInvadeProcess); // RA2: as above
	DWORD (WINAPI *SymLoadModule)(HANDLE hProcess, HANDLE hFile, PCSTR ImageName, PCSTR ModuleName, DWORD BaseOfDll, DWORD SizeOfDll); // RA2: as above
	DWORD (WINAPI *SymSetOptions)(DWORD SymOptions); // RA2: as above
	BOOL  (WINAPI *SymUnloadModule)(HANDLE hProcess, DWORD BaseOfDll); // RA2: as above
	void * StackWalk;						// RA2: the walk also fills the three entry points the exception code never calls: the table has nine names
	void * SymFunctionTableAccess;			// RA2: as above
	void * SymGetModuleBase;				// RA2: as above
} ImageHelp = {								// RA2: 0x00865C20
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL, // RA2: as above
	NULL // RA2: as above
};

static char const * ImagehelpFunctionNames[] =	// RA2: 0x007D6B00
{
	"SymCleanup", // RA2: as above
	"SymGetSymFromAddr", // RA2: as above
	"SymInitialize", // RA2: as above
	"SymLoadModule", // RA2: as above
	"SymSetOptions", // RA2: as above
	"SymUnloadModule", // RA2: as above
	"StackWalk",							// RA2: the three names after the first six are in the executable's table
	"SymFunctionTableAccess",				// RA2: as above
	"SymGetModuleBaseType",					// RA2: the executable spells it with the Type suffix, so this lookup finds nothing
	NULL, // RA2: as above
};


/// <summary>
/// Handles the messages for the exception report dialog.
/// This routine is the dialog procedure used by Exception_Dialog. It fills the detail
/// field with the machine state text the exception handler gathered, and centers the
/// window over the main game window so the player cannot miss it.
/// </summary>
int CALLBACK Exception_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)	// RA2: 0x004B7BF0
{
	switch (message) { // RA2: as above
		case WM_INITDIALOG: // RA2: as above
			DebugString("WM_INITDIALOG\n"); // RA2: as above

			SetDlgItemText(window, IDC_EXCEPT_DETAILS, ExceptionText); // RA2: as above
			SetFocus(window); // RA2: as above
			if (MainWindow) { // RA2: as above
				Center_Window_Within_Window(window, MainWindow); // RA2: as above
			}
			ShowWindow(window, SW_NORMAL); // RA2: as above
			Add_Audio_File_Window(window);	// RA2: the dialog is listed with the audio file windows
			return(0); // RA2: as above

		case WM_CLOSE: // RA2: as above
			EndDialog(window, IDC_EXCEPT_QUIT); // RA2: as above
			Remove_Audio_File_Window(window);	// RA2: and removed again when it closes
			return(0); // RA2: as above

		case WM_COMMAND: // RA2: as above
			switch (LOWORD(wparam)) { // RA2: as above
				case IDC_EXCEPT_SAVE: // RA2: as above
					MessageBox(window, "Sorry, you can only attempt an emergency save if you were in the map editor when the exception occurred.\t", "Bummer", MB_ICONWARNING); // RA2: as above
					return(0); // RA2: as above

				case IDC_EXCEPT_QUIT: // RA2: as above
					EndDialog(window, IDC_EXCEPT_QUIT); // RA2: as above
					return(0); // RA2: as above
			};
			break; // RA2: as above

		case WM_MOVING: // RA2: as above
			return(On_WM_MOVING(window, wparam, lparam)); // RA2: as above

		default: // RA2: as above
			break; // RA2: as above
	}

	return(0); // RA2: as above
}


/// <summary>
/// Presents the exception report to the player.
/// This routine is called by the exception handler once the machine state has been
/// gathered. A recursive exception gets a progressively cruder response -- a console
/// notice instead of a dialog, and finally an outright exit -- so that a fault raised
/// inside the reporting code cannot trap the player.
/// </summary>
/// <returns>Returns with the dialog control that dismissed the report. IDC_EXCEPT_QUIT
/// means the handler should take the exception rather than pass it along.</returns>
int Exception_Dialog(void)	// RA2: 0x004B7CF0
{
	int retval = 0; // RA2: as above

	Audio_File_Flush();	// RA2: the audio file is flushed first

	switch (ExceptionRecursions) { // RA2: as above

		default: { // RA2: as above
			ShowCursor(TRUE); // RA2: as above

			DLGTEMPLATE const * dialogt = (DLGTEMPLATE const *)Fetch_Resource(MAKEINTRESOURCE(IDD_EXCEPTION_SIMPLE), MAKEINTRESOURCE(5)); // RA2: as above
			if (dialogt != NULL) { // RA2: as above
				DebugString("Resource found\n"); // RA2: as above
				retval = DialogBoxIndirectParam(ProgramInstance, dialogt, MainWindow, Exception_Proc, 0); // RA2: as above
			} else {
				DebugString("Resource not found\n"); // RA2: as above
				retval = DialogBoxParam(ProgramInstance, MAKEINTRESOURCE(IDD_EXCEPTION_SIMPLE), MainWindow, Exception_Proc, 0); // RA2: as above
			}
			ShowCursor(FALSE); // RA2: as above
			return(retval); // RA2: as above
		}

		case 3: // RA2: as above
			ExitProcess(EXIT_SUCCESS); // RA2: as above
			break; // RA2: as above

		case 2: // RA2: as above
			break; // RA2: as above

		case 1: // RA2: as above
			if (!AllocConsole()) { // RA2: as above
				FreeConsole(); // RA2: as above
				AllocConsole(); // RA2: as above
			}
			_cprintf("Error - Recursive exception detected. Program halted.\r\n See EXCEPT.TXT file for details\r\r\n");	// RA2: the CRT console printf at 0x00796714, linked statically
			Sleep(4000); // RA2: as above
			_getch();	// RA2: the CRT console getch at 0x007963F7
			FreeConsole(); // RA2: as above
			break; // RA2: as above

	}

	return(IDC_EXCEPT_QUIT);	// RA2: the cases that show no dialog all leave through this return
}


/// <summary>
/// Are any of the drawing surfaces currently locked?
/// This routine is used by the exception handler to decide whether it dares put a window
/// on the screen. A locked surface holds the Win16 mutex under Windows 9x, and the crash
/// report dialog would hang the machine outright rather than report anything.
/// </summary>
/// <returns>bool; Is at least one of the game surfaces locked?</returns>
static bool Any_Locked(void) // RA2: as above
{
	if (VisibleSurface && VisibleSurface->Is_Locked() || // RA2: as above
		HiddenSurface && HiddenSurface->Is_Locked() || // RA2: as above
		CompositeSurface && CompositeSurface->Is_Locked() || // RA2: as above
		TileSurface && TileSurface->Is_Locked() || // RA2: as above
		SidebarSurface && SidebarSurface->Is_Locked() || // RA2: as above
		AlternateSurface && AlternateSurface->Is_Locked()) { // RA2: as above
		return(true); // RA2: as above
	}

	return(false); // RA2: as above
}


/// <summary>
/// Adds the given text to the machine state dump buffer.
/// The text is dropped when it would not fit in the buffer.
/// </summary>
static void Add_Txt (char const *txt) // RA2: as above
{
	if (lstrlen(txt) + lstrlen(ExceptionText) < EH_BUFFER_SIZE - 1) {	// RA2: the executable uses the Windows string functions
		lstrcat(ExceptionText, txt);	// RA2: lstrcatA (imported) instead of strcat
	}
}


/// <summary>
/// Dumps machine state information into the exception text buffer.
/// Builds the report the player sees: what kind of exception it was, where it happened (by
/// symbol name when imagehlp.dll can supply them), the version and processor, the registers,
/// the bytes at the faulting address and a thousand words of the stack.
/// </summary>
/// <param name="e_info">The exception pointers handed to the filter.</param>
void Dump_Exception_Info(EXCEPTION_POINTERS *e_info)	// RA2: 0x004B7DF0
{
	/*
	**	List of possible exceptions
	*/
	static const unsigned int _codes[] = { // RA2: as above
		EXCEPTION_ACCESS_VIOLATION, // RA2: as above
		EXCEPTION_ARRAY_BOUNDS_EXCEEDED, // RA2: as above
		EXCEPTION_BREAKPOINT, // RA2: as above
		EXCEPTION_DATATYPE_MISALIGNMENT, // RA2: as above
		EXCEPTION_FLT_DENORMAL_OPERAND, // RA2: as above
		EXCEPTION_FLT_DIVIDE_BY_ZERO, // RA2: as above
		EXCEPTION_FLT_INEXACT_RESULT, // RA2: as above
		EXCEPTION_FLT_INVALID_OPERATION, // RA2: as above
		EXCEPTION_FLT_OVERFLOW, // RA2: as above
		EXCEPTION_FLT_STACK_CHECK, // RA2: as above
		EXCEPTION_FLT_UNDERFLOW, // RA2: as above
		EXCEPTION_ILLEGAL_INSTRUCTION, // RA2: as above
		EXCEPTION_IN_PAGE_ERROR, // RA2: as above
		EXCEPTION_INT_DIVIDE_BY_ZERO, // RA2: as above
		EXCEPTION_INT_OVERFLOW, // RA2: as above
		EXCEPTION_INVALID_DISPOSITION, // RA2: as above
		EXCEPTION_NONCONTINUABLE_EXCEPTION, // RA2: as above
		EXCEPTION_PRIV_INSTRUCTION, // RA2: as above
		EXCEPTION_SINGLE_STEP, // RA2: as above
		EXCEPTION_STACK_OVERFLOW, // RA2: as above
		0xffffffff // RA2: as above
	};
	/*
	**	Information about each exception type.
	*/
	static char const * _code_txt[] = { // RA2: as above
		"Error code: EXCEPTION_ACCESS_VIOLATION\r\r\nDescription: The thread tried to read from or write to a virtual address for which it does not have the appropriate access.", // RA2: as above
		"Error code: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\r\r\nDescription: The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.", // RA2: as above
		"Error code: EXCEPTION_BREAKPOINT\r\r\nDescription: A breakpoint was encountered.", // RA2: as above
		"Error code: EXCEPTION_DATATYPE_MISALIGNMENT\r\r\nDescription: The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.", // RA2: as above
		"Error code: EXCEPTION_FLT_DENORMAL_OPERAND\r\r\nDescription: One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.", // RA2: as above
		"Error code: EXCEPTION_FLT_DIVIDE_BY_ZERO\r\r\nDescription: The thread tried to divide a floating-point value by a floating-point divisor of zero.", // RA2: as above
		"Error code: EXCEPTION_FLT_INEXACT_RESULT\r\r\nDescription: The result of a floating-point operation cannot be represented exactly as a decimal fraction.", // RA2: as above
		"Error code: EXCEPTION_FLT_INVALID_OPERATION\r\r\nDescription: Some strange unknown floating point operation was attempted.", // RA2: as above
		"Error code: EXCEPTION_FLT_OVERFLOW\r\r\nDescription: The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.", // RA2: as above
		"Error code: EXCEPTION_FLT_STACK_CHECK\r\r\nDescription: The stack overflowed or underflowed as the result of a floating-point operation.", // RA2: as above
		"Error code: EXCEPTION_FLT_UNDERFLOW\r\r\nDescription:\tThe exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.", // RA2: as above
		"Error code: EXCEPTION_ILLEGAL_INSTRUCTION\r\r\nDescription:\tThe thread tried to execute an invalid instruction.", // RA2: as above
		"Error code: EXCEPTION_IN_PAGE_ERROR\r\r\nDescription:\tThe thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.", // RA2: as above
		"Error code: EXCEPTION_INT_DIVIDE_BY_ZERO\r\r\nDescription: The thread tried to divide an integer value by an integer divisor of zero.", // RA2: as above
		"Error code: EXCEPTION_INT_OVERFLOW\r\r\nDescription: The result of an integer operation caused a carry out of the most significant bit of the result.", // RA2: as above
		"Error code: EXCEPTION_INVALID_DISPOSITION\r\r\nDescription: An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.", // RA2: as above
		"Error code: EXCEPTION_NONCONTINUABLE_EXCEPTION\r\r\nDescription: The thread tried to continue execution after a noncontinuable exception occurred.", // RA2: as above
		"Error code: EXCEPTION_PRIV_INSTRUCTION\r\r\nDescription: The thread tried to execute an instruction whose operation is not allowed in the current machine mode.", // RA2: as above
		"Error code: EXCEPTION_SINGLE_STEP\r\r\nDescription: A trace trap or other single-instruction mechanism signaled that one instruction has been executed.", // RA2: as above
		"Error code: EXCEPTION_STACK_OVERFLOW\r\r\nDescription: The thread used up its stack.", // RA2: as above
		"Error code: ?????\r\r\nDescription: Unknown exception." // RA2: as above
	};
	DebugString("Dump exception info\n"); // RA2: as above

	/*
	**	Scrap buffer for constructing dump strings
	*/
	char scrap [256]; // RA2: as above

	/*
	**	Clear out the dump buffer
	*/
	memset(ExceptionText, 0, sizeof (ExceptionText)); // RA2: as above

	/*
	**	Fix up the imagehelp function pointers since imagehlp.dll can't be statically linked.
	*/
	HINSTANCE imagehelp = LoadLibrary("IMAGEHLP.DLL"); // RA2: as above

	if (imagehelp != NULL) { // RA2: as above
		DebugString ("Found IMAGEHLP.DLL - linking to required functions\n"); // RA2: as above
		char const *function_name = NULL; // RA2: as above
		unsigned int *fptr = (unsigned int*)&ImageHelp;	// RA2: as above
		int count = 0; // RA2: as above

		do { // RA2: as above
			function_name = ImagehelpFunctionNames[count]; // RA2: as above
			if (function_name) { // RA2: as above
				*fptr = (unsigned int) GetProcAddress(imagehelp, function_name);	// RA2: as above
				fptr++; // RA2: as above
				count++; // RA2: as above
			}
		} while (function_name); // RA2: as above
	} else {
		DebugString("Unable to load IMAGEHLP.DLL\n"); // RA2: as above
	}

	/*
	**	Retrieve the programs symbols if they are available
	*/
	if (ImageHelp.SymSetOptions != NULL) { // RA2: as above
		ImageHelp.SymSetOptions(SYMOPT_DEFERRED_LOADS); // RA2: as above
	}

	int symload = 0; // RA2: as above
	int symbols_available = false; // RA2: as above

	if (ImageHelp.SymInitialize != NULL && ImageHelp.SymInitialize(GetCurrentProcess(), NULL, false)) { // RA2: as above
		DebugString("Symbols are available\r\n\n");	// RA2: the executable logs this
		symbols_available = true; // RA2: as above
	}

	if (!symbols_available)	{ // RA2: as above
		DebugString ("SymInitialize failed with code %d - %s\n", GetLastError(), Last_Error_Text()); // RA2: as above
	} else {
		if (ImageHelp.SymSetOptions != NULL) { // RA2: as above
			ImageHelp.SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME); // RA2: as above
		}

		char module_name[MAX_PATH];	// RA2: the symbols are loaded for the running executable, whatever its name
		GetModuleFileName(NULL, module_name, sizeof(module_name));	// RA2: the symbols are loaded for the running executable

		if (ImageHelp.SymLoadModule != NULL) { // RA2: as above
			symload = ImageHelp.SymLoadModule(GetCurrentProcess(), NULL, module_name, NULL, 0, 0);	// RA2: loaded under the executable's own file name, no module name
		}

		if (!symload) { // RA2: as above
			DebugString ("SymLoad failed for module %s with code %d - %s\n", module_name, GetLastError(), Last_Error_Text());	// RA2: the message names the module
		}
	}

	struct : IMAGEHLP_SYMBOL { // RA2: as above
		BYTE buffer[(256 - sizeof(IMAGEHLP_SYMBOL))]; // RA2: as above
	} symbol; // RA2: as above
	DWORD displacement; // RA2: as above
	IMAGEHLP_SYMBOL *symptr = (IMAGEHLP_SYMBOL*)&symbol; // RA2: as above

	/*
	**	Get the exception address and the machine context at the time of the exception
	*/
	CONTEXT *context = e_info->ContextRecord; // RA2: as above

	/*
	**	The following are set for access violation only
	*/
	int access_read_write=-1; // RA2: as above
	unsigned int access_address = 0; // RA2: as above

	if (e_info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) { // RA2: as above
		DebugString("Exception is access violation\n"); // RA2: as above
		access_read_write = e_info->ExceptionRecord->ExceptionInformation[0];  // 0=read, 1=write // RA2: as above
		access_address = e_info->ExceptionRecord->ExceptionInformation[1]; // RA2: as above
	} else {
		DebugString ("Exception code is %d\n", e_info->ExceptionRecord->ExceptionCode); // RA2: as above
	}

	/*
	**	Match the exception type with the error string and print it out
	*/
	int i; // RA2: as above
	for (i=0 ; _codes[i] != 0xffffffff ; i++) { // RA2: as above
		if (_codes[i] == e_info->ExceptionRecord->ExceptionCode) { // RA2: as above
			DebugString("Found exception description\n"); // RA2: as above
			break; // RA2: as above
		}
	}
	Add_Txt(_code_txt[i]); // RA2: as above
	Add_Txt("\r\n"); // RA2: as above

	/*
	**	For access violations, print out the violation address and if it was read or write.
	*/
	if (e_info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) { // RA2: as above
		wsprintf(scrap, "Access address:%08X ", access_address);	// RA2: wsprintfA (the executable imports it) instead of sprintf
		Add_Txt(scrap); // RA2: as above
		if (access_read_write) { // RA2: as above
			Add_Txt("was written to.\r\n"); // RA2: as above
		} else {
			Add_Txt("was read from.\r\n"); // RA2: as above
		}
	}


	/*
	**	If symbols are available, print out the exception eip address and the name of the
	**	function it represents.
	*/
	memset(symptr, 0, sizeof (IMAGEHLP_SYMBOL)); // RA2: as above
	symptr->SizeOfStruct = sizeof (IMAGEHLP_SYMBOL); // RA2: as above
	symptr->MaxNameLength = sizeof (symbol.buffer); // RA2: as above
	symptr->Size = 0; // RA2: as above
	symptr->Address = context->Eip; // RA2: as above

	if (!IsBadCodePtr((FARPROC)context->Eip)) { // RA2: as above
		if (ImageHelp.SymGetSymFromAddr != NULL && ImageHelp.SymGetSymFromAddr(GetCurrentProcess(), context->Eip, &displacement, &symbol)) { // RA2: as above
			wsprintf(scrap, "Exception occurred at %08X - %s + %08X\r\n", context->Eip, symptr->Name, displacement);	// RA2: wsprintfA (the executable imports it) instead of sprintf
		} else {
			DebugString ("Failed to get symbol for EIP\r\n");	// RA2: the executable logs this
			if (ImageHelp.SymGetSymFromAddr != NULL) { // RA2: as above
				DebugString("SymGetSymFromAddr failed with code %d - %s\n", GetLastError(), Last_Error_Text()); // RA2: as above
			}
			wsprintf (scrap, "Exception occurred at %08X\r\n", context->Eip);	// RA2: wsprintfA (the executable imports it) instead of sprintf
		}
	} else {
		DebugString ("context->Eip is bad code pointer\n"); // RA2: as above
	}

	Add_Txt (scrap); // RA2: as above

	/*
	**	Add in the version info.
	*/
	wsprintf(scrap, "\r\nVersion %s\r\n", Version_Name());	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above

	wsprintf(scrap, "Internal Version %s\r\n", VerNum.Version_Name());	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above

	char buildinfo[128]; // RA2: as above
	buildinfo[0] = 0; // RA2: as above
	Build_Date_String(buildinfo, sizeof(buildinfo)); // RA2: as above

	char build_number[128]; // RA2: as above
	char build_name[128]; // RA2: as above

	wsprintf(scrap, "Release Build: %s by %s - %s\r\n",	// RA2: wsprintfA (the executable imports it) instead of sprintf
		Build_Number_String(build_number, sizeof(build_number)), // RA2: as above
		Build_By_String(build_name, sizeof(build_name)), // RA2: as above
		buildinfo); // RA2: as above

	Add_Txt(scrap); // RA2: as above

	/*
	**	CPU type
	*/
	int cpu_type = 5; // RA2: as above
	bool mmx = false; // RA2: as above
	Get_CPU_Type(cpu_type, mmx, NULL, 0); // RA2: as above
	wsprintf(scrap, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), mmx ? "Yes" : "No", &VendorID);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above


	Add_Txt("\r\nDetails:\r\n"); // RA2: as above

	DebugString("Register dump...\n"); // RA2: as above

	/*
	**	Dump the registers.
	*/
	wsprintf(scrap, "Eip:%08X\tEsp:%08X\tEbp:%08X\r\n", context->Eip, context->Esp, context->Ebp);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above
	wsprintf(scrap, "Eax:%08X\tEbx:%08X\tEcx:%08X\r\n", context->Eax, context->Ebx, context->Ecx);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above
	wsprintf(scrap, "Edx:%08X\tEsi:%08X\tEdi:%08X\r\n", context->Edx, context->Esi, context->Edi);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above
	wsprintf(scrap, "EFlags:%08X \r\n", context->EFlags);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above
	wsprintf(scrap, "CS:%04x  SS:%04x  DS:%04x  ES:%04x  FS:%04x  GS:%04x\r\n", context->SegCs, context->SegSs, context->SegDs, context->SegEs, context->SegFs, context->SegGs);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap); // RA2: as above

	/*
	**	Dump the bytes at EIP. This will make it easier to match the crash address with later versions of the game.
	*/
	DebugString("EIP bytes dump...\n"); // RA2: as above
	wsprintf(scrap, "\r\nBytes at CS:EIP (%08X)  : ", context->Eip);	// RA2: wsprintfA (the executable imports it) instead of sprintf

	unsigned char *eip_ptr = (unsigned char *) (context->Eip); // RA2: as above
	char bytestr[NUM_CODE_BYTES]; // RA2: as above

	for (int c = 0 ; c < NUM_CODE_BYTES; c++) { // RA2: as above
		if (IsBadReadPtr(eip_ptr, sizeof(*eip_ptr))) { // RA2: as above
			lstrcat(scrap, "?? ");	// RA2: lstrcatA (imported) instead of strcat
		} else {
			wsprintf(bytestr, "%02X ", *eip_ptr);	// RA2: wsprintfA (the executable imports it) instead of sprintf
			lstrcat(scrap, bytestr);	// RA2: lstrcatA (imported) instead of strcat
		}
		eip_ptr++; // RA2: as above
	}

	lstrcat(scrap, "\r\n\r\n");	// RA2: lstrcatA (imported) instead of strcat
	Add_Txt(scrap); // RA2: as above

	/*
	**	Dump out the values on the stack.
	*/
	DebugString("Stack dump...\n"); // RA2: as above
	Add_Txt("Stack dump (* indicates possible code address) :\r\n"); // RA2: as above
	unsigned int *stackptr = (unsigned int*) context->Esp; // RA2: as above

	for (int j = 0; j < MAX_STACK_DUMP; j++) { // RA2: as above
		if (IsBadReadPtr(stackptr, sizeof(*stackptr))) { // RA2: as above
			/*
			**	The stack contents cannot be read so just print up question marks.
			*/
			wsprintf(scrap, "%08X: ", stackptr);	// RA2: wsprintfA (the executable imports it) instead of sprintf
			lstrcat(scrap, "????????\r\n");	// RA2: lstrcatA (imported) instead of strcat
		} else if (IsBadCodePtr((FARPROC)*stackptr)) {	// RA2: a stack word that is no code address is marked DATA_PTR and gets no symbol lookup
			wsprintf(scrap, "%08X: %08X ", stackptr, *stackptr);	// RA2: a word that is no code address is marked as data
			lstrcat(scrap, "DATA_PTR\r\n");	// RA2: lstrcatA (imported) instead of strcat
		} else {
			/*
			**	If this stack address is in our memory space then try to match it with a code symbol.
			*/
			wsprintf(scrap, "%08X: %08X", stackptr, *stackptr);	// RA2: wsprintfA (the executable imports it) instead of sprintf
			if (symbols_available) { // RA2: as above
				symptr->SizeOfStruct = sizeof(symbol); // RA2: as above
				symptr->MaxNameLength = 128; // RA2: as above
				symptr->Size = 0; // RA2: as above
				symptr->Address = *stackptr; // RA2: as above

				if (ImageHelp.SymGetSymFromAddr != NULL && ImageHelp.SymGetSymFromAddr(GetCurrentProcess(), *stackptr, &displacement, symptr)) { // RA2: as above
					char symbuf[160]; // RA2: as above
					wsprintf(symbuf, " - %s + %08X", symptr->Name, displacement);	// RA2: wsprintfA (the executable imports it) instead of sprintf
					lstrcat(scrap, symbuf);	// RA2: lstrcatA (imported) instead of strcat
				}
			} else {
				lstrcat (scrap, " *");	// RA2: lstrcatA (imported) instead of strcat
			}
			lstrcat(scrap, "\r\n");	// RA2: lstrcatA (imported) instead of strcat
		}
		Add_Txt(scrap); // RA2: as above
		stackptr++; // RA2: as above
	}

	/*
	**	Unload the symbols.
	*/
	if (symbols_available) { // RA2: as above
		if (ImageHelp.SymCleanup != NULL) { // RA2: as above
			ImageHelp.SymCleanup(GetCurrentProcess()); // RA2: as above
		}

		if (symload) { // RA2: as above
			if (ImageHelp.SymUnloadModule != NULL) { // RA2: as above
				ImageHelp.SymUnloadModule(GetCurrentProcess(), 0); // RA2: as above
			}
		}
	}

	if (imagehelp != NULL) { // RA2: as above
		FreeLibrary(imagehelp); // RA2: as above
	}

	Add_Txt ("\r\n\r\n"); // RA2: as above
}


/// <summary>
/// Exception handler filter function.
/// Called when an exception is raised: the first time it writes the machine state to
/// except.txt, then (when it is safe to put a window on the screen) shows the report dialog.
/// </summary>
/// <param name="exception_code">The exception code.</param>
/// <param name="e_info">The exception pointers.</param>
/// <returns>EXCEPTION_EXECUTE_HANDLER to run the body of the __except construct, or
/// EXCEPTION_CONTINUE_SEARCH to pass the exception on.</returns>
int Exception_Handler(int exception_code, EXCEPTION_POINTERS *e_info)	// RA2: 0x004B87F0
{
	DebugString("Exception!\n"); // RA2: as above
	Audio_File_Flush();	// RA2: the audio file is flushed before anything else

	/*
	**	Track recursions because we need to know if something here is failing.
	*/
	ExceptionRecursions++; // RA2: as above

	if (ExceptionRecursions == 0) { // RA2: as above

		/*
		**	Create a dump of the exception info.
		*/
		Dump_Exception_Info(e_info); // RA2: as above

		/*
		**	Log the machine state to disk
		*/
		HANDLE debug_file; // RA2: as above
		DWORD	actual; // RA2: as above
		debug_file = CreateFile("except.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); // RA2: as above
		if (debug_file != INVALID_HANDLE_VALUE){ // RA2: as above
			WriteFile(debug_file, ExceptionText, lstrlen(ExceptionText), &actual, NULL);	// RA2: lstrlenA (imported) instead of strlen
			CloseHandle (debug_file); // RA2: as above
		}
	}

	if (Any_Locked() && WinVersion.Is_Win9x() || GetCurrentThreadId() != (DWORD)MainThread) { // RA2: as above
		DebugString("Can't bring up exception dialog due to Win16 mutex or threading issues\n"); // RA2: as above
		return(EXCEPTION_CONTINUE_SEARCH); // RA2: as above
	}

	DebugString("Exception_Dialog()\n"); // RA2: as above
	if (Exception_Dialog() != IDC_EXCEPT_QUIT) { // RA2: as above
		return(EXCEPTION_CONTINUE_SEARCH); // RA2: as above
	}

	UnusedEHBool = false; // RA2: as above

	if (ExceptionRecursions == 0) { // RA2: as above
		ExceptionRecursions = -1; // RA2: as above
	} else if (ExceptionRecursions == 2) { // RA2: as above
		return(EXCEPTION_CONTINUE_SEARCH); // RA2: as above
	}
	if (g_TopWindow) { // RA2: as above
		OwnerDraw::End_Dialog(g_TopWindow); // RA2: as above
	}

	return(EXCEPTION_EXECUTE_HANDLER); // RA2: as above
}
