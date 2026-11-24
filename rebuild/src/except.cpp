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

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"

#include "_surface.h"
#include "data.h"
#include "dbgprint.h"
#include "getcpu.h"
#include "globals.h"
#include "language\language.h"
#include "ownrdraw.h"
#include "surface.h"
#include "version.h"
#include "win.h"
#include "windlg.h"
#include "winfix.h"
#include "winstub.h"

#include <imagehlp.h>

#include <conio.h> // RA2: _cprintf and _getch, the statically linked CRT console routines the recursion notice uses

void __fastcall Audio_File_Flush(void);						// RA2: 0x00522E10; under the audio file critical section, flushes the file the audio streamer is reading (meastes: Audio_File_Flush)
void __fastcall Add_Audio_File_Window(HWND window);		// RA2: 0x00522B20; files a dialog window in the audio file window list (as ownrdraw.cpp declares it)
void __fastcall Remove_Audio_File_Window(HWND window);	// RA2: 0x00522B80; takes it out of the list again
char const * Get_CPU_Type_Name(void);						// RA2: 0x004D2B80, the name of the CPU type ("Pentium" ...)

#define EH_BUFFER_SIZE 65536
#define NUM_CODE_BYTES 32
#define MAX_STACK_DUMP 1024

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
struct tImageHelpStruct {
		BOOL(WINAPI * SymCleanup)(HANDLE hProcess);
		BOOL(WINAPI * SymGetSymFromAddr)(HANDLE hProcess, DWORD dwAddr, PDWORD pdwdisplacementacement, PIMAGEHLP_SYMBOL Symbol);
		BOOL(WINAPI * SymInitialize)(HANDLE hProcess, PCSTR UserSearchPath, BOOL fInvadeProcess);
		DWORD(WINAPI * SymLoadModule)(HANDLE hProcess, HANDLE hFile, PCSTR ImageName, PCSTR ModuleName, DWORD BaseOfDll, DWORD SizeOfDll);
		DWORD(WINAPI * SymSetOptions)(DWORD SymOptions);
		BOOL(WINAPI * SymUnloadModule)(HANDLE hProcess, DWORD BaseOfDll);
	void * StackWalk;						// RA2: the walk also fills the three entry points the exception code never calls: the table has nine names
		void * SymFunctionTableAccess;
		void * SymGetModuleBase;
} ImageHelp = {								// RA2: 0x00865C20
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

static char const * ImagehelpFunctionNames[] =	// RA2: 0x007D6B00
{
		"SymCleanup",
		"SymGetSymFromAddr",
		"SymInitialize",
		"SymLoadModule",
		"SymSetOptions",
		"SymUnloadModule",
	"StackWalk",							// RA2: the three names after the first six are in the executable's table
		"SymFunctionTableAccess",
	"SymGetModuleBaseType",					// RA2: the executable spells it with the Type suffix, so this lookup finds nothing
		NULL,
};


/// <summary>
/// Handles the messages for the exception report dialog.
/// This routine is the dialog procedure used by Exception_Dialog. It fills the detail
/// field with the machine state text the exception handler gathered, and centers the
/// window over the main game window so the player cannot miss it.
/// </summary>
int CALLBACK Exception_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)	// RA2: 0x004B7BF0
{
	switch (message) {
		case WM_INITDIALOG:
			DebugString("WM_INITDIALOG\n");

			SetDlgItemText(window, IDC_EXCEPT_DETAILS, ExceptionText);
			SetFocus(window);
			if (MainWindow) {
				Center_Window_Within_Window(window, MainWindow);
			}
			ShowWindow(window, SW_NORMAL);
			Add_Audio_File_Window(window);	// RA2: the dialog is listed with the audio file windows
			return(0);

		case WM_CLOSE:
			EndDialog(window, IDC_EXCEPT_QUIT);
			Remove_Audio_File_Window(window);	// RA2: and removed again when it closes
			return(0);

		case WM_COMMAND:
			switch (LOWORD(wparam)) {
				case IDC_EXCEPT_SAVE:
					MessageBox(window, "Sorry, you can only attempt an emergency save if you were in the map editor when the exception occurred.\t", "Bummer", MB_ICONWARNING);
					return(0);

				case IDC_EXCEPT_QUIT:
					EndDialog(window, IDC_EXCEPT_QUIT);
					return(0);
			};
			break;

		case WM_MOVING:
			return(On_WM_MOVING(window, wparam, lparam));

		default:
			break;
	}

	return(0);
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
	int retval = 0;

	Audio_File_Flush();	// RA2: the audio file is flushed first

	switch (ExceptionRecursions) {

		default: {
			ShowCursor(TRUE);

			DLGTEMPLATE const * dialogt = (DLGTEMPLATE const *)Fetch_Resource(MAKEINTRESOURCE(IDD_EXCEPTION_SIMPLE), MAKEINTRESOURCE(5));
			if (dialogt != NULL) {
				DebugString("Resource found\n");
				retval = DialogBoxIndirectParam(ProgramInstance, dialogt, MainWindow, Exception_Proc, 0);
			} else {
				DebugString("Resource not found\n");
				retval = DialogBoxParam(ProgramInstance, MAKEINTRESOURCE(IDD_EXCEPTION_SIMPLE), MainWindow, Exception_Proc, 0);
			}
			ShowCursor(FALSE);
			return(retval);
		}

		case 3:
			ExitProcess(EXIT_SUCCESS);
			break;

		case 2:
			break;

		case 1:
			if (!AllocConsole()) {
				FreeConsole();
				AllocConsole();
			}
			_cprintf("Error - Recursive exception detected. Program halted.\r\n See EXCEPT.TXT file for details\r\r\n");	// RA2: the CRT console printf at 0x00796714, linked statically
			Sleep(4000);
			_getch();	// RA2: the CRT console getch at 0x007963F7
			FreeConsole();
			break;

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
static bool Any_Locked(void)
{
	if (VisibleSurface && VisibleSurface->Is_Locked() ||
		HiddenSurface && HiddenSurface->Is_Locked() ||
		CompositeSurface && CompositeSurface->Is_Locked() ||
		TileSurface && TileSurface->Is_Locked() ||
		SidebarSurface && SidebarSurface->Is_Locked() ||
		AlternateSurface && AlternateSurface->Is_Locked()) {
		return(true);
	}

	return(false);
}


/// <summary>
/// Adds the given text to the machine state dump buffer.
/// The text is dropped when it would not fit in the buffer.
/// </summary>
static void Add_Txt(char const * txt)
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
	static const unsigned int _codes[] = {
		EXCEPTION_ACCESS_VIOLATION,
		EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
		EXCEPTION_BREAKPOINT,
		EXCEPTION_DATATYPE_MISALIGNMENT,
		EXCEPTION_FLT_DENORMAL_OPERAND,
		EXCEPTION_FLT_DIVIDE_BY_ZERO,
		EXCEPTION_FLT_INEXACT_RESULT,
		EXCEPTION_FLT_INVALID_OPERATION,
		EXCEPTION_FLT_OVERFLOW,
		EXCEPTION_FLT_STACK_CHECK,
		EXCEPTION_FLT_UNDERFLOW,
		EXCEPTION_ILLEGAL_INSTRUCTION,
		EXCEPTION_IN_PAGE_ERROR,
		EXCEPTION_INT_DIVIDE_BY_ZERO,
		EXCEPTION_INT_OVERFLOW,
		EXCEPTION_INVALID_DISPOSITION,
		EXCEPTION_NONCONTINUABLE_EXCEPTION,
		EXCEPTION_PRIV_INSTRUCTION,
		EXCEPTION_SINGLE_STEP,
		EXCEPTION_STACK_OVERFLOW,
		0xffffffff
	};
	/*
	**	Information about each exception type.
	*/
	static char const * _code_txt[] = {
		"Error code: EXCEPTION_ACCESS_VIOLATION\r\r\nDescription: The thread tried to read from or write to a virtual address for which it does not have the appropriate access.",
		"Error code: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\r\r\nDescription: The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking.",
		"Error code: EXCEPTION_BREAKPOINT\r\r\nDescription: A breakpoint was encountered.",
		"Error code: EXCEPTION_DATATYPE_MISALIGNMENT\r\r\nDescription: The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on.",
		"Error code: EXCEPTION_FLT_DENORMAL_OPERAND\r\r\nDescription: One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value.",
		"Error code: EXCEPTION_FLT_DIVIDE_BY_ZERO\r\r\nDescription: The thread tried to divide a floating-point value by a floating-point divisor of zero.",
		"Error code: EXCEPTION_FLT_INEXACT_RESULT\r\r\nDescription: The result of a floating-point operation cannot be represented exactly as a decimal fraction.",
		"Error code: EXCEPTION_FLT_INVALID_OPERATION\r\r\nDescription: Some strange unknown floating point operation was attempted.",
		"Error code: EXCEPTION_FLT_OVERFLOW\r\r\nDescription: The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type.",
		"Error code: EXCEPTION_FLT_STACK_CHECK\r\r\nDescription: The stack overflowed or underflowed as the result of a floating-point operation.",
		"Error code: EXCEPTION_FLT_UNDERFLOW\r\r\nDescription:\tThe exponent of a floating-point operation is less than the magnitude allowed by the corresponding type.",
		"Error code: EXCEPTION_ILLEGAL_INSTRUCTION\r\r\nDescription:\tThe thread tried to execute an invalid instruction.",
		"Error code: EXCEPTION_IN_PAGE_ERROR\r\r\nDescription:\tThe thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.",
		"Error code: EXCEPTION_INT_DIVIDE_BY_ZERO\r\r\nDescription: The thread tried to divide an integer value by an integer divisor of zero.",
		"Error code: EXCEPTION_INT_OVERFLOW\r\r\nDescription: The result of an integer operation caused a carry out of the most significant bit of the result.",
		"Error code: EXCEPTION_INVALID_DISPOSITION\r\r\nDescription: An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception.",
		"Error code: EXCEPTION_NONCONTINUABLE_EXCEPTION\r\r\nDescription: The thread tried to continue execution after a noncontinuable exception occurred.",
		"Error code: EXCEPTION_PRIV_INSTRUCTION\r\r\nDescription: The thread tried to execute an instruction whose operation is not allowed in the current machine mode.",
		"Error code: EXCEPTION_SINGLE_STEP\r\r\nDescription: A trace trap or other single-instruction mechanism signaled that one instruction has been executed.",
		"Error code: EXCEPTION_STACK_OVERFLOW\r\r\nDescription: The thread used up its stack.",
		"Error code: ?????\r\r\nDescription: Unknown exception."
	};
	DebugString("Dump exception info\n");

	/*
	**	Scrap buffer for constructing dump strings
	*/
	char scrap[256];

	/*
	**	Clear out the dump buffer
	*/
	memset(ExceptionText, 0, sizeof(ExceptionText));

	/*
	**	Fix up the imagehelp function pointers since imagehlp.dll can't be statically linked.
	*/
	HINSTANCE imagehelp = LoadLibrary("IMAGEHLP.DLL");

	if (imagehelp != NULL) {
		DebugString("Found IMAGEHLP.DLL - linking to required functions\n");
		char const * function_name = NULL;
		unsigned int * fptr = (unsigned int *)&ImageHelp;
		int count = 0;

		do {
			function_name = ImagehelpFunctionNames[count];
			if (function_name) {
				*fptr = (unsigned int)GetProcAddress(imagehelp, function_name);
				fptr++;
				count++;
			}
		} while (function_name);
	} else {
		DebugString("Unable to load IMAGEHLP.DLL\n");
	}

	/*
	**	Retrieve the programs symbols if they are available
	*/
	if (ImageHelp.SymSetOptions != NULL) {
		ImageHelp.SymSetOptions(SYMOPT_DEFERRED_LOADS);
	}

	int symload = 0;
	int symbols_available = false;

	if (ImageHelp.SymInitialize != NULL && ImageHelp.SymInitialize(GetCurrentProcess(), NULL, false)) {
		DebugString("Symbols are available\r\n\n");	// RA2: the executable logs this
		symbols_available = true;
	}

	if (!symbols_available) {
		DebugString("SymInitialize failed with code %d - %s\n", GetLastError(), Last_Error_Text());
	} else {
		if (ImageHelp.SymSetOptions != NULL) {
			ImageHelp.SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
		}

		char module_name[MAX_PATH];	// RA2: the symbols are loaded for the running executable, whatever its name
		GetModuleFileName(NULL, module_name, sizeof(module_name));	// RA2: the symbols are loaded for the running executable

		if (ImageHelp.SymLoadModule != NULL) {
			symload = ImageHelp.SymLoadModule(GetCurrentProcess(), NULL, module_name, NULL, 0, 0);	// RA2: loaded under the executable's own file name, no module name
		}

		if (!symload) {
			DebugString ("SymLoad failed for module %s with code %d - %s\n", module_name, GetLastError(), Last_Error_Text());	// RA2: the message names the module
		}
	}

	struct : IMAGEHLP_SYMBOL {
			BYTE buffer[(256 - sizeof(IMAGEHLP_SYMBOL))];
	} symbol;
	DWORD displacement;
	IMAGEHLP_SYMBOL * symptr = (IMAGEHLP_SYMBOL *)&symbol;

	/*
	**	Get the exception address and the machine context at the time of the exception
	*/
	CONTEXT * context = e_info->ContextRecord;

	/*
	**	The following are set for access violation only
	*/
	int access_read_write = -1;
	unsigned int access_address = 0;

	if (e_info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
		DebugString("Exception is access violation\n");
		access_read_write = e_info->ExceptionRecord->ExceptionInformation[0];  // 0=read, 1=write
		access_address = e_info->ExceptionRecord->ExceptionInformation[1];
	} else {
		DebugString("Exception code is %d\n", e_info->ExceptionRecord->ExceptionCode);
	}

	/*
	**	Match the exception type with the error string and print it out
	*/
	int i;
	for (i = 0; _codes[i] != 0xffffffff; i++) {
		if (_codes[i] == e_info->ExceptionRecord->ExceptionCode) {
			DebugString("Found exception description\n");
			break;
		}
	}
	Add_Txt(_code_txt[i]);
	Add_Txt("\r\n");

	/*
	**	For access violations, print out the violation address and if it was read or write.
	*/
	if (e_info->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
		wsprintf(scrap, "Access address:%08X ", access_address);	// RA2: wsprintfA (the executable imports it) instead of sprintf
		Add_Txt(scrap);
		if (access_read_write) {
			Add_Txt("was written to.\r\n");
		} else {
			Add_Txt("was read from.\r\n");
		}
	}


	/*
	**	If symbols are available, print out the exception eip address and the name of the
	**	function it represents.
	*/
	memset(symptr, 0, sizeof(IMAGEHLP_SYMBOL));
	symptr->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
	symptr->MaxNameLength = sizeof(symbol.buffer);
	symptr->Size = 0;
	symptr->Address = context->Eip;

	if (!IsBadCodePtr((FARPROC)context->Eip)) {
		if (ImageHelp.SymGetSymFromAddr != NULL && ImageHelp.SymGetSymFromAddr(GetCurrentProcess(), context->Eip, &displacement, &symbol)) {
			wsprintf(scrap, "Exception occurred at %08X - %s + %08X\r\n", context->Eip, symptr->Name, displacement);	// RA2: wsprintfA (the executable imports it) instead of sprintf
		} else {
			DebugString ("Failed to get symbol for EIP\r\n");	// RA2: the executable logs this
			if (ImageHelp.SymGetSymFromAddr != NULL) {
				DebugString("SymGetSymFromAddr failed with code %d - %s\n", GetLastError(), Last_Error_Text());
			}
			wsprintf (scrap, "Exception occurred at %08X\r\n", context->Eip);	// RA2: wsprintfA (the executable imports it) instead of sprintf
		}
	} else {
		DebugString("context->Eip is bad code pointer\n");
	}

	Add_Txt(scrap);

	/*
	**	Add in the version info.
	*/
	wsprintf(scrap, "\r\nVersion %s\r\n", Version_Name());	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);

	wsprintf(scrap, "Internal Version %s\r\n", VerNum.Version_Name());	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);

	char buildinfo[128];
	buildinfo[0] = 0;
	Build_Date_String(buildinfo, sizeof(buildinfo));

	char build_number[128];
	char build_name[128];

	wsprintf(scrap, "Release Build: %s by %s - %s\r\n",	// RA2: wsprintfA (the executable imports it) instead of sprintf
		Build_Number_String(build_number, sizeof(build_number)),
		Build_By_String(build_name, sizeof(build_name)),
		buildinfo);

	Add_Txt(scrap);

	/*
	**	CPU type
	*/
	int cpu_type = 5;
	bool mmx = false;
	Get_CPU_Type(cpu_type, mmx, NULL, 0);
	wsprintf(scrap, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), mmx ? "Yes" : "No", &VendorID);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);


	Add_Txt("\r\nDetails:\r\n");

	DebugString("Register dump...\n");

	/*
	**	Dump the registers.
	*/
	wsprintf(scrap, "Eip:%08X\tEsp:%08X\tEbp:%08X\r\n", context->Eip, context->Esp, context->Ebp);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);
	wsprintf(scrap, "Eax:%08X\tEbx:%08X\tEcx:%08X\r\n", context->Eax, context->Ebx, context->Ecx);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);
	wsprintf(scrap, "Edx:%08X\tEsi:%08X\tEdi:%08X\r\n", context->Edx, context->Esi, context->Edi);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);
	wsprintf(scrap, "EFlags:%08X \r\n", context->EFlags);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);
	wsprintf(scrap, "CS:%04x  SS:%04x  DS:%04x  ES:%04x  FS:%04x  GS:%04x\r\n", context->SegCs, context->SegSs, context->SegDs, context->SegEs, context->SegFs, context->SegGs);	// RA2: wsprintfA (the executable imports it) instead of sprintf
	Add_Txt(scrap);

	/*
	**	Dump the bytes at EIP. This will make it easier to match the crash address with later versions of the game.
	*/
	DebugString("EIP bytes dump...\n");
	wsprintf(scrap, "\r\nBytes at CS:EIP (%08X)  : ", context->Eip);	// RA2: wsprintfA (the executable imports it) instead of sprintf

	unsigned char * eip_ptr = (unsigned char *)(context->Eip);
	char bytestr[NUM_CODE_BYTES];

	for (int c = 0; c < NUM_CODE_BYTES; c++) {
		if (IsBadReadPtr(eip_ptr, sizeof(*eip_ptr))) {
			lstrcat(scrap, "?? ");	// RA2: lstrcatA (imported) instead of strcat
		} else {
			wsprintf(bytestr, "%02X ", *eip_ptr);	// RA2: wsprintfA (the executable imports it) instead of sprintf
			lstrcat(scrap, bytestr);	// RA2: lstrcatA (imported) instead of strcat
		}
		eip_ptr++;
	}

	lstrcat(scrap, "\r\n\r\n");	// RA2: lstrcatA (imported) instead of strcat
	Add_Txt(scrap);

	/*
	**	Dump out the values on the stack.
	*/
	DebugString("Stack dump...\n");
	Add_Txt("Stack dump (* indicates possible code address) :\r\n");
	unsigned int * stackptr = (unsigned int *)context->Esp;

	for (int j = 0; j < MAX_STACK_DUMP; j++) {
		if (IsBadReadPtr(stackptr, sizeof(*stackptr))) {
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
			if (symbols_available) {
				symptr->SizeOfStruct = sizeof(symbol);
				symptr->MaxNameLength = 128;
				symptr->Size = 0;
				symptr->Address = *stackptr;

				if (ImageHelp.SymGetSymFromAddr != NULL && ImageHelp.SymGetSymFromAddr(GetCurrentProcess(), *stackptr, &displacement, symptr)) {
					char symbuf[160];
					wsprintf(symbuf, " - %s + %08X", symptr->Name, displacement);	// RA2: wsprintfA (the executable imports it) instead of sprintf
					lstrcat(scrap, symbuf);	// RA2: lstrcatA (imported) instead of strcat
				}
			} else {
				lstrcat (scrap, " *");	// RA2: lstrcatA (imported) instead of strcat
			}
			lstrcat(scrap, "\r\n");	// RA2: lstrcatA (imported) instead of strcat
		}
		Add_Txt(scrap);
		stackptr++;
	}

	/*
	**	Unload the symbols.
	*/
	if (symbols_available) {
		if (ImageHelp.SymCleanup != NULL) {
			ImageHelp.SymCleanup(GetCurrentProcess());
		}

		if (symload) {
			if (ImageHelp.SymUnloadModule != NULL) {
				ImageHelp.SymUnloadModule(GetCurrentProcess(), 0);
			}
		}
	}

	if (imagehelp != NULL) {
		FreeLibrary(imagehelp);
	}

	Add_Txt("\r\n\r\n");
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
	DebugString("Exception!\n");
	Audio_File_Flush();	// RA2: the audio file is flushed before anything else

	/*
	**	Track recursions because we need to know if something here is failing.
	*/
	ExceptionRecursions++;

	if (ExceptionRecursions == 0) {

		/*
		**	Create a dump of the exception info.
		*/
		Dump_Exception_Info(e_info);

		/*
		**	Log the machine state to disk
		*/
		HANDLE debug_file;
		DWORD actual;
		debug_file = CreateFile("except.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (debug_file != INVALID_HANDLE_VALUE) {
			WriteFile(debug_file, ExceptionText, lstrlen(ExceptionText), &actual, NULL);	// RA2: lstrlenA (imported) instead of strlen
			CloseHandle(debug_file);
		}
	}

	if (Any_Locked() && WinVersion.Is_Win9x() || GetCurrentThreadId() != (DWORD)MainThread) {
		DebugString("Can't bring up exception dialog due to Win16 mutex or threading issues\n");
		return(EXCEPTION_CONTINUE_SEARCH);
	}

	DebugString("Exception_Dialog()\n");
	if (Exception_Dialog() != IDC_EXCEPT_QUIT) {
		return(EXCEPTION_CONTINUE_SEARCH);
	}

	UnusedEHBool = false;

	if (ExceptionRecursions == 0) {
		ExceptionRecursions = -1;
	} else if (ExceptionRecursions == 2) {
		return(EXCEPTION_CONTINUE_SEARCH);
	}
	if (g_TopWindow) {
		OwnerDraw::End_Dialog(g_TopWindow);
	}

	return(EXCEPTION_EXECUTE_HANDLER);
}

/*
** Override the C library Pure Virtual Function Call error handler.
** The release build reports no error.
*/
int __cdecl _purecall(void) // RA2: 0x004B8960, Renegade's Except.cpp; the WWDEBUG int 3 branch is not in the release build
{
	int return_code = 0;
	return(return_code);
}
