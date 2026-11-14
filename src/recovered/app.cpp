// app.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/app.json carries the addresses and the rows that
// stay in their own files.


#include "shared/tiberian_sun_class_factory.hpp"
#include "shared/application.hpp"
#include "shared/swizzle.hpp"


class Leaf_426b00 { public: void get(); };
void Leaf_426b00::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; *(unsigned int*)(0x84c7dc) = 0x79c3e4; }


// HRESULT __stdcall TiberianSunClassFactory::QueryInterface(GUID const &iid, void **object) -- 0x00426B10
// IUnknown slot 0: answers IClassFactory and IUnknown with the factory itself (AddRef).

HRESULT __stdcall TiberianSunClassFactory::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    if (iid == IID_IClassFactory || iid == IID_IUnknown_7afce0) {
        *object = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}


// HRESULT __stdcall TiberianSunClassFactory::CreateInstance(IUnknown *outer, GUID const &iid, void **object) -- 0x00426B70
// The scripting application's class factory: no aggregation; IApplication and
// IUnknown ask the application object at 0x0084C7DC, the third interface the
// map at 0x008324E0; an object whose query fails is deleted.

HRESULT __stdcall TiberianSunClassFactory::CreateInstance(IUnknown *outer, GUID const &iid, void **object)
{
    if (object == 0) {
        return E_INVALIDARG;
    }
    *object = 0;
    if (outer != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    HRESULT result = E_NOINTERFACE;
    if (iid == IID_IApplication_79c3d0 || iid == IID_IUnknown_7afce0) {
        IUnknown *application = &unknown_application_object_84c7dc;
        if (application == 0) {
            return E_OUTOFMEMORY;
        }
        result = application->QueryInterface(iid, object);
        if (result < 0) {
            delete application;
        }
    }
    if (iid == unknown_iid_7a2c28) {
        IUnknown *map = (IUnknown *)&unknown_map_object_8324e0;
        if (map == 0) {
            return E_OUTOFMEMORY;
        }
        result = map->QueryInterface(iid, object);
        if (result < 0) {
            delete map;
        }
    }
    return result;
}


// HRESULT __stdcall ApplicationClass::QueryInterface(GUID const &iid, void **object) -- 0x00426C60
// IUnknown slot 0: answers IApplication and IUnknown with itself (AddRef),
// and a third interface (0x007A2C28) with the map object at 0x008324E0
// (no AddRef).

HRESULT __stdcall ApplicationClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    if (iid == IID_IApplication_79c3d0 || iid == IID_IUnknown_7afce0) {
        *object = this;
        AddRef();
        return S_OK;
    }
    if (iid == unknown_iid_7a2c28) {
        *object = &unknown_map_object_8324e0;
        return S_OK;
    }
    return E_NOINTERFACE;
}


// unsigned long __stdcall ApplicationClass::AddRef() -- 0x00426CE0
// IUnknown slot 1: the application object is static; no reference count.

unsigned long __stdcall ApplicationClass::AddRef()
{
    return 1;
}


// unsigned long __stdcall ApplicationClass::Release() -- 0x00426CF0
// IUnknown slot 2: the application object is static; never deleted.

unsigned long __stdcall ApplicationClass::Release()
{
    return 1;
}


// HRESULT __stdcall ApplicationClass::App_Interface(void **object) -- 0x00426D00
// IApplication slot: the object itself as IApplication (a QueryInterface
// for IID 0x0079C3D0).

HRESULT __stdcall ApplicationClass::App_Interface(void **object)
{
    return QueryInterface(IID_IApplication_79c3d0, object);
}


// HRESULT __stdcall ApplicationClass::get_FullName(BSTR *name) -- 0x00426D20
// IApplication slot: returns a new BSTR of L"Tiberian Sun" (E_POINTER without an
// out pointer).

HRESULT __stdcall ApplicationClass::get_FullName(BSTR *name)
{
    if (name == 0) {
        return E_POINTER;
    }
    *name = SysAllocString(L"Tiberian Sun");
    return S_OK;
}


// HRESULT __stdcall ApplicationClass::get_Name(BSTR *name) -- 0x00426D50
// IApplication slot: returns a new BSTR of L"Sun" (E_POINTER without an
// out pointer).

HRESULT __stdcall ApplicationClass::get_Name(BSTR *name)
{
    if (name == 0) {
        return E_POINTER;
    }
    *name = SysAllocString(L"Sun");
    return S_OK;
}


// HRESULT __stdcall ApplicationClass::Application_Interface(void **object) -- 0x00426D80
// IApplication slot: the object itself as IApplication (a QueryInterface
// for IID 0x0079C3D0).

HRESULT __stdcall ApplicationClass::Application_Interface(void **object)
{
    return QueryInterface(IID_IApplication_79c3d0, object);
}


// long __stdcall ApplicationClass::Quit() -- 0x00426DA0
// IApplication slot 5: always 0.

long __stdcall ApplicationClass::Quit()
{
    return 0;
}


// HRESULT __stdcall ApplicationClass::get_ScenarioName(BSTR *name) -- 0x00426DB0
// IApplication slot: the scenario name (+0x83C of the scenario at 0x00A3D290) as a new BSTR.

HRESULT __stdcall ApplicationClass::get_ScenarioName(BSTR *name)
{
    if (name == 0) {
        return E_POINTER;
    }
    wchar_t text[32];
    MultiByteToWideChar(0, 1, unknown_id_source_a3d290->FileName, -1, text, 32);
    *name = SysAllocString(text);
    return S_OK;
}


// HRESULT __stdcall ApplicationClass::get_FrameCount(long *frame) -- 0x00426E00
// IApplication slot: the scenario's elapsed-time timer value (+0x614).

HRESULT __stdcall ApplicationClass::get_FrameCount(long *frame)
{
    if (frame == 0) {
        return E_POINTER;
    }
    *frame = unknown_id_source_a3d290->Elapsed_Time().Time();
    return S_OK;
}


// HRESULT ApplicationClass::get_Swizzle_Interface(ISwizzle **swizzle) (0x00426E50): the save-game
// swizzler's ISwizzle interface (name from mischa85/RedAlert2 src/app.cpp).

HRESULT __stdcall ApplicationClass::get_Swizzle_Interface(ISwizzle **swizzle)
{
    return Swizzler.QueryInterface(IID_ISwizzle, (void **)swizzle);
}
