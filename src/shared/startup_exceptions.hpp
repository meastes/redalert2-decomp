// Exact original SEH shims: scopefilter00690891 and shutdownfilter0068E270.
struct _CONTEXT;
struct _EXCEPTION_RECORD {
    unsigned long ExceptionCode;
    unsigned long ExceptionFlags;
    _EXCEPTION_RECORD *ExceptionRecord;
    void *ExceptionAddress;
    unsigned long NumberParameters;
    unsigned long ExceptionInformation[15];
};
struct _EXCEPTION_POINTERS {
    _EXCEPTION_RECORD *ExceptionRecord;
    _CONTEXT *ContextRecord;
};
extern "C" unsigned long __cdecl _exception_code();
extern "C" void *__cdecl _exception_info();
#define GetExceptionCode() _exception_code()
#define GetExceptionInformation() _exception_info()
int __fastcall Handle_Exception_004b87f0(unsigned int code, _EXCEPTION_POINTERS *information);
// Existing callback shim requires independent typed migration from genericowner.
int __fastcall Top_Level_Exception_Filter_0068e270(_EXCEPTION_POINTERS *information);
typedef long (__stdcall *StartupTopLevelFilter)(_EXCEPTION_POINTERS *information);
extern "C" __declspec(dllimport) StartupTopLevelFilter __stdcall SetUnhandledExceptionFilter(StartupTopLevelFilter filter);
