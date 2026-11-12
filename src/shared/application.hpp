// ApplicationClass (RTTI; vtable 0x0079C3E4, bases IApplication and
// IUnknown): the scripting application object, the global at 0x0084C7DC
// (constructor 0x00426B00 stores only its vtable). Slot order from the
// vtable; slot names past IUnknown from TibSun app.h.
#ifndef RA2_SHARED_APPLICATION_HPP
#define RA2_SHARED_APPLICATION_HPP

#include "shared/com_persist.hpp"
#include "shared/system_timer.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/abstract.hpp"   // unknown_id_source_a3d290 (the scenario)

#ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif
typedef wchar_t *BSTR;

#ifndef E_POINTER
#define E_POINTER ((HRESULT)0x80004003L)
#endif
#ifndef E_NOINTERFACE
#define E_NOINTERFACE ((HRESULT)0x80004002L)
#endif

extern "C" __declspec(dllimport) BSTR __stdcall SysAllocString(wchar_t const *text);


extern "C" GUID const IID_IApplication_79c3d0;   // 0x0079C3D0 (hypothesis)
// A third interface ID QueryInterface answers with the object at 0x008324E0
// (the map/screen global); not identified.
extern "C" GUID const unknown_iid_7a2c28;         // 0x007A2C28
extern char unknown_map_object_8324e0;            // 0x008324E0 (only its address is used)

struct ISwizzle;

class __declspec(novtable) IApplication : public IUnknown {
public:
    virtual HRESULT __stdcall get_FullName(BSTR *name) = 0;                      // 0x00426D20
    virtual HRESULT __stdcall get_Name(BSTR *name) = 0;                          // 0x00426D50
    virtual long __stdcall Quit() = 0;                                           // 0x00426DA0
    virtual HRESULT __stdcall get_ScenarioName(BSTR *name) = 0;                  // 0x00426DB0
    virtual HRESULT __stdcall get_FrameCount(long *frame) = 0;                   // 0x00426E00
    virtual HRESULT __stdcall get_Swizzle_Interface(ISwizzle **swizzle) = 0;     // 0x00426E50 (the save-game swizzler)
    virtual HRESULT __stdcall App_Interface(void **object) = 0;                  // 0x00426D00
    virtual HRESULT __stdcall Application_Interface(void **object) = 0;          // 0x00426D80
};

class ApplicationClass : public IApplication {
public:
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);     // 0x00426C60
    virtual unsigned long __stdcall AddRef();                                    // 0x00426CE0
    virtual unsigned long __stdcall Release();                                   // 0x00426CF0
    virtual HRESULT __stdcall get_FullName(BSTR *name);
    virtual HRESULT __stdcall get_Name(BSTR *name);
    virtual long __stdcall Quit();
    virtual HRESULT __stdcall get_ScenarioName(BSTR *name);
    virtual HRESULT __stdcall get_FrameCount(long *frame);
    virtual HRESULT __stdcall get_Swizzle_Interface(ISwizzle **swizzle);
    virtual HRESULT __stdcall App_Interface(void **object);
    virtual HRESULT __stdcall Application_Interface(void **object);
};

#endif
