// void Dump_Exception_004b7df0(_EXCEPTION_POINTERS *information) (0x004B7DF0)
// TS except.cpp Dump_Exception_Info: clears the 64K exception text and fills it with the exception
// description, the faulting symbol (IMAGEHLP.DLL linked at run time), the version and build lines,
// the CPU, a register dump, the 32 code bytes at EIP and a 1024-entry stack dump. RA2 loads the
// symbols of its own module file (GetModuleFileNameA) and formats with wsprintfA/lstrcatA.
#include "shared/win32_api.hpp"
#include "shared/debug_printf.hpp"
#include "shared/cpu_info.hpp"
#include "shared/win_stub.hpp"
#include "shared/lobby.hpp"
#include "shared/crt_memory.hpp"
typedef HINSTANCE HMODULE;
typedef int (WINAPI *FARPROC)();
WIN32_IMPORT HMODULE WINAPI LoadLibraryA(LPCSTR name);
WIN32_IMPORT FARPROC WINAPI GetProcAddress(HMODULE module, LPCSTR name);
WIN32_IMPORT BOOL WINAPI FreeLibrary(HMODULE module);
WIN32_IMPORT HANDLE WINAPI GetCurrentProcess();
WIN32_IMPORT BOOL WINAPI IsBadReadPtr(void const *pointer, unsigned int size);
WIN32_IMPORT BOOL WINAPI IsBadCodePtr(FARPROC pointer);

struct _EXCEPTION_RECORD {
    DWORD ExceptionCode;                    // +0x00
    DWORD ExceptionFlags;                   // +0x04
    _EXCEPTION_RECORD *ExceptionRecord;     // +0x08
    void *ExceptionAddress;                 // +0x0C
    DWORD NumberParameters;                 // +0x10
    DWORD ExceptionInformation[15];         // +0x14 (access violation: 0 read / 1 write, address)
};
struct _CONTEXT {
    DWORD ContextFlags;                     // +0x00
    DWORD Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;     // +0x04
    unsigned char FloatSave[0x70];          // +0x1C FLOATING_SAVE_AREA
    DWORD SegGs, SegFs, SegEs, SegDs;       // +0x8C
    DWORD Edi, Esi, Ebx, Edx, Ecx, Eax;     // +0x9C
    DWORD Ebp, Eip, SegCs, EFlags, Esp, SegSs;  // +0xB4
};
struct _EXCEPTION_POINTERS {
    _EXCEPTION_RECORD *ExceptionRecord;
    _CONTEXT *ContextRecord;
};
struct _IMAGEHLP_SYMBOL {
    DWORD SizeOfStruct;                     // +0x00
    DWORD Address;                          // +0x04
    DWORD Size;                             // +0x08
    DWORD Flags;                            // +0x0C
    DWORD MaxNameLength;                    // +0x10
    char Name[1];                           // +0x14
};

// The IMAGEHLP.DLL entry points, filled in the order of their names (0x00865C20; names 0x007D6B00).
struct ImageHelpStruct {
    BOOL (WINAPI *SymCleanup)(HANDLE process);
    BOOL (WINAPI *SymGetSymFromAddr)(HANDLE process, DWORD address, DWORD *displacement, _IMAGEHLP_SYMBOL *symbol);
    BOOL (WINAPI *SymInitialize)(HANDLE process, char const *search_path, BOOL invade);
    DWORD (WINAPI *SymLoadModule)(HANDLE process, HANDLE file, char const *image, char const *module, DWORD base, DWORD size);
    DWORD (WINAPI *SymSetOptions)(DWORD options);
    BOOL (WINAPI *SymUnloadModule)(HANDLE process, DWORD base);
};
extern ImageHelpStruct ImageHelp_00865c20;
extern char const *ImagehelpFunctionNames_007d6b00[];
// The exception codes (0xFFFFFFFF ends them; 0x007A0E48) and their descriptions (0x007D6B28).
extern unsigned int const exception_codes_007a0e48[];
extern char const *exception_code_text_007d6b28[];
extern char exception_text_00855ba8[];

void __fastcall Dump_Exception_004b7df0(_EXCEPTION_POINTERS *information);

// Appends text to the exception text while it fits (TS Add_Txt).
static inline void Add_Txt(char const *text)
{
    if (lstrlenA(exception_text_00855ba8) + lstrlenA(text) < 0xFFFF) {
        lstrcatA(exception_text_00855ba8, text);
    }
}

void __fastcall Dump_Exception_004b7df0(_EXCEPTION_POINTERS *information)
{
    Debug_Printf("Dump exception info\n");

    char scrap[256];

    memset(exception_text_00855ba8, 0, 0x10000);

    HMODULE imagehelp = LoadLibraryA("IMAGEHLP.DLL");
    if (imagehelp != 0) {
        Debug_Printf("Found IMAGEHLP.DLL - linking to required functions\n");
        char const *function_name = 0;
        unsigned int *fptr = (unsigned int *)&ImageHelp_00865c20;
        int count = 0;

        do {
            function_name = ImagehelpFunctionNames_007d6b00[count];
            if (function_name) {
                *fptr = (unsigned int)GetProcAddress(imagehelp, function_name);
                fptr++;
                count++;
            }
        } while (function_name);
    } else {
        Debug_Printf("Unable to load IMAGEHLP.DLL\n");
    }

    if (ImageHelp_00865c20.SymSetOptions != 0) {
        ImageHelp_00865c20.SymSetOptions(4);
    }

    int symload = 0;
    int symbols_available = false;

    if (ImageHelp_00865c20.SymInitialize != 0 && ImageHelp_00865c20.SymInitialize(GetCurrentProcess(), 0, false)) {
        Debug_Printf("Symbols are available\r\n\n");
        symbols_available = true;
    }

    if (!symbols_available) {
        Debug_Printf("SymInitialize failed with code %d - %s\n", GetLastError(), 0);
    } else {
        if (ImageHelp_00865c20.SymSetOptions != 0) {
            ImageHelp_00865c20.SymSetOptions(6);
        }

        char module_name[260];
        GetModuleFileNameA(0, module_name, sizeof(module_name));

        if (ImageHelp_00865c20.SymLoadModule != 0) {
            symload = ImageHelp_00865c20.SymLoadModule(GetCurrentProcess(), 0, module_name, 0, 0, 0);
        }

        if (!symload) {
            Debug_Printf("SymLoad failed for module %s with code %d - %s\n", module_name, GetLastError(), 0);
        }
    }

    struct {
        _IMAGEHLP_SYMBOL symbol;
        unsigned char buffer[256 - sizeof(_IMAGEHLP_SYMBOL)];
    } symbol;
    DWORD displacement;
    _IMAGEHLP_SYMBOL *symptr = (_IMAGEHLP_SYMBOL *)&symbol;

    _CONTEXT *context = information->ContextRecord;

    int access_read_write = -1;
    unsigned int access_address = 0;

    if (information->ExceptionRecord->ExceptionCode == 0xC0000005) {
        Debug_Printf("Exception is access violation\n");
        access_read_write = information->ExceptionRecord->ExceptionInformation[0];
        access_address = information->ExceptionRecord->ExceptionInformation[1];
    } else {
        Debug_Printf("Exception code is %d\n", information->ExceptionRecord->ExceptionCode);
    }

    int i;
    for (i = 0; exception_codes_007a0e48[i] != 0xFFFFFFFF; i++) {
        if (exception_codes_007a0e48[i] == information->ExceptionRecord->ExceptionCode) {
            Debug_Printf("Found exception description\n");
            break;
        }
    }
    Add_Txt(exception_code_text_007d6b28[i]);
    Add_Txt("\r\n");

    if (information->ExceptionRecord->ExceptionCode == 0xC0000005) {
        wsprintfA(scrap, "Access address:%08X ", access_address);
        Add_Txt(scrap);
        if (access_read_write) {
            Add_Txt("was written to.\r\n");
        } else {
            Add_Txt("was read from.\r\n");
        }
    }

    memset(symptr, 0, sizeof(_IMAGEHLP_SYMBOL));
    symptr->SizeOfStruct = sizeof(_IMAGEHLP_SYMBOL);
    symptr->MaxNameLength = sizeof(symbol.buffer);
    symptr->Size = 0;
    symptr->Address = context->Eip;

    if (!IsBadCodePtr((FARPROC)context->Eip)) {
        if (ImageHelp_00865c20.SymGetSymFromAddr != 0
            && ImageHelp_00865c20.SymGetSymFromAddr(GetCurrentProcess(), context->Eip, &displacement, symptr)) {
            wsprintfA(scrap, "Exception occurred at %08X - %s + %08X\r\n", context->Eip, symptr->Name, displacement);
        } else {
            Debug_Printf("Failed to get symbol for EIP\r\n");
            if (ImageHelp_00865c20.SymGetSymFromAddr != 0) {
                Debug_Printf("SymGetSymFromAddr failed with code %d - %s\n", GetLastError(), 0);
            }
            wsprintfA(scrap, "Exception occurred at %08X\r\n", context->Eip);
        }
    } else {
        Debug_Printf("context->Eip is bad code pointer\n");
    }
    Add_Txt(scrap);

    wsprintfA(scrap, "\r\nVersion %s\r\n", Version_Name());
    Add_Txt(scrap);

    wsprintfA(scrap, "Internal Version %s\r\n", VerNum.Version_Name());
    Add_Txt(scrap);

    char buildinfo[128];
    buildinfo[0] = 0;
    Build_Time_Text(buildinfo, sizeof(buildinfo));

    char build_number[128];
    char build_name[128];
    wsprintfA(scrap, "Release Build: %s by %s - %s\r\n", Build_Number_String(build_number, sizeof(build_number)),
              Build_By_String_00737b00(build_name, sizeof(build_name)), buildinfo);
    Add_Txt(scrap);

    int cpu_type = 5;
    unsigned char mmx = false;
    Get_CPU_Info(&cpu_type, &mmx, 0, 0);
    wsprintfA(scrap, "CPU %s, MMX %s, Vendor: %s\r\n", Get_CPU_Type_Name(), mmx ? "Yes" : "No", VendorID);
    Add_Txt(scrap);

    Add_Txt("\r\nDetails:\r\n");

    Debug_Printf("Register dump...\n");
    wsprintfA(scrap, "Eip:%08X\tEsp:%08X\tEbp:%08X\r\n", context->Eip, context->Esp, context->Ebp);
    Add_Txt(scrap);
    wsprintfA(scrap, "Eax:%08X\tEbx:%08X\tEcx:%08X\r\n", context->Eax, context->Ebx, context->Ecx);
    Add_Txt(scrap);
    wsprintfA(scrap, "Edx:%08X\tEsi:%08X\tEdi:%08X\r\n", context->Edx, context->Esi, context->Edi);
    Add_Txt(scrap);
    wsprintfA(scrap, "EFlags:%08X \r\n", context->EFlags);
    Add_Txt(scrap);
    wsprintfA(scrap, "CS:%04x  SS:%04x  DS:%04x  ES:%04x  FS:%04x  GS:%04x\r\n", context->SegCs, context->SegSs,
              context->SegDs, context->SegEs, context->SegFs, context->SegGs);
    Add_Txt(scrap);

    Debug_Printf("EIP bytes dump...\n");
    wsprintfA(scrap, "\r\nBytes at CS:EIP (%08X)  : ", context->Eip);

    unsigned char *eip_ptr = (unsigned char *)(context->Eip);
    char bytestr[32];
    for (int c = 0; c < 32; c++) {
        if (IsBadReadPtr(eip_ptr, sizeof(*eip_ptr))) {
            lstrcatA(scrap, "?? ");
        } else {
            wsprintfA(bytestr, "%02X ", *eip_ptr);
            lstrcatA(scrap, bytestr);
        }
        eip_ptr++;
    }
    lstrcatA(scrap, "\r\n\r\n");
    Add_Txt(scrap);

    Debug_Printf("Stack dump...\n");
    Add_Txt("Stack dump (* indicates possible code address) :\r\n");

    unsigned int *stackptr = (unsigned int *)context->Esp;
    for (int j = 0; j < 1024; j++) {
        if (IsBadReadPtr(stackptr, sizeof(*stackptr))) {
            wsprintfA(scrap, "%08X: ", stackptr);
            lstrcatA(scrap, "????????\r\n");
        } else if (IsBadCodePtr((FARPROC)*stackptr)) {
            wsprintfA(scrap, "%08X: %08X ", stackptr, *stackptr);
            lstrcatA(scrap, "DATA_PTR\r\n");
        } else {
            wsprintfA(scrap, "%08X: %08X", stackptr, *stackptr);
            if (symbols_available) {
                symptr->SizeOfStruct = sizeof(symbol);
                symptr->MaxNameLength = 128;
                symptr->Size = 0;
                symptr->Address = *stackptr;
                if (ImageHelp_00865c20.SymGetSymFromAddr != 0
                    && ImageHelp_00865c20.SymGetSymFromAddr(GetCurrentProcess(), *stackptr, &displacement, symptr)) {
                    char symbuf[160];
                    wsprintfA(symbuf, " - %s + %08X", symptr->Name, displacement);
                    lstrcatA(scrap, symbuf);
                }
            } else {
                lstrcatA(scrap, " *");
            }
            lstrcatA(scrap, "\r\n");
        }
        Add_Txt(scrap);
        stackptr++;
    }

    if (symbols_available) {
        if (ImageHelp_00865c20.SymCleanup != 0) {
            ImageHelp_00865c20.SymCleanup(GetCurrentProcess());
        }
        if (symload) {
            if (ImageHelp_00865c20.SymUnloadModule != 0) {
                ImageHelp_00865c20.SymUnloadModule(GetCurrentProcess(), 0);
            }
        }
    }

    if (imagehelp != 0) {
        FreeLibrary(imagehelp);
    }

    Add_Txt("\r\n\r\n");
}
