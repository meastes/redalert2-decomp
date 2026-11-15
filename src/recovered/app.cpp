/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/app.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/app.cpp where both define the function (names only).

// app.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/app.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializers of the unit at 0x00426910: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00426910-0x00426AF1); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x00426930, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426960, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426980, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x004269A0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x004269C0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x004269E0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426A00, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426A40, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426A70, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426AA0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426AD0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.


// Dynamic initializer (0x00426AE0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00426910.cpp; no source of its own.




#include "shared/tiberian_sun_class_factory.hpp"
#include "shared/application.hpp"
#include "shared/swizzle.hpp"


class Leaf_426b00 { public: void get(); };
void Leaf_426b00::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int unused_result; unsigned int unused_value; *(unsigned int*)(0x84c7dc) = 0x79c3e4; }


/// <summary>
/// Fetches the requested interface from the class factory.
/// The factory hands out nothing but its own creation interface -- the objects it makes
/// are reached through CreateInstance.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied, or E_NOINTERFACE if the
/// factory does not implement it.</returns>
// (0x00426B10)
// IUnknown slot 0: answers IClassFactory and IUnknown with the factory itself (AddRef).

HRESULT __stdcall TiberianSunClassFactory::QueryInterface(GUID const &riid, void **ppvObject)
{
    if (ppvObject == 0) {
        return E_POINTER;
    }
    if (riid == IID_IClassFactory || riid == IID_IUnknown_7afce0) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}


/// <summary>
/// Creates an object of the class this factory serves.
/// This routine is the entry point a scripting client uses to reach the game, handing out
/// either the application or the game map interface.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied. Otherwise a COM error code
/// describing the failure is returned.</returns>
/// <remarks>Aggregation is not supported, so no outer unknown may be supplied.</remarks>
// (0x00426B70)
// The scripting application's class factory: no aggregation; IApplication and
// IUnknown ask the application object at 0x0084C7DC, the third interface the
// map at 0x008324E0; an object whose query fails is deleted.

HRESULT __stdcall TiberianSunClassFactory::CreateInstance(IUnknown *pUnkOuter, GUID const &riid, void **ppvObject)
{
    if (ppvObject == 0) {
        return E_INVALIDARG;
    }
    *ppvObject = 0;
    if (pUnkOuter != 0) {
        return CLASS_E_NOAGGREGATION;
    }
    HRESULT result = E_NOINTERFACE;
    if (riid == IID_IApplication_79c3d0 || riid == IID_IUnknown_7afce0) {
        IUnknown *application = &ApplicationInstance;
        if (application == 0) {
            return E_OUTOFMEMORY;
        }
        result = application->QueryInterface(riid, ppvObject);
        if (result < 0) {
            delete application;
        }
    }
    if (riid == unknown_iid_7a2c28) {
        IUnknown *map = (IUnknown *)&unknown_map_object_8324e0;
        if (map == 0) {
            return E_OUTOFMEMORY;
        }
        result = map->QueryInterface(riid, ppvObject);
        if (result < 0) {
            delete map;
        }
    }
    return result;
}


/// <summary>
/// Fetches the requested interface from the application object.
/// The application serves the game map interface as well as its own, so that a client
/// only needs to get hold of the application in order to reach the map.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied, or E_NOINTERFACE if the
/// application does not serve it.</returns>
// (0x00426C60)
// IUnknown slot 0: answers IApplication and IUnknown with itself (AddRef),
// and a third interface (0x007A2C28) with the map object at 0x008324E0
// (no AddRef).

HRESULT __stdcall ApplicationClass::QueryInterface(GUID const &riid, void **ppvObject)
{
    if (ppvObject == 0) {
        return E_POINTER;
    }
    if (riid == IID_IApplication_79c3d0 || riid == IID_IUnknown_7afce0) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    if (riid == unknown_iid_7a2c28) {
        *ppvObject = &unknown_map_object_8324e0;
        return S_OK;
    }
    return E_NOINTERFACE;
}


/// <summary>
/// Adds a reference to the application object.
/// The application is a global rather than a heap object, so it is never really reference
/// counted.
/// </summary>
/// <returns>Returns with the reference count, which is always one.</returns>
// (0x00426CE0)
// IUnknown slot 1: the application object is static; no reference count.

unsigned long __stdcall ApplicationClass::AddRef()
{
    return 1;
}


/// <summary>
/// Releases a reference to the application object.
/// The application is a global that lives for the duration of the game, so releasing it
/// never destroys anything.
/// </summary>
/// <returns>Returns with the reference count, which is always one.</returns>
// (0x00426CF0)
// IUnknown slot 2: the application object is static; never deleted.

unsigned long __stdcall ApplicationClass::Release()
{
    return 1;
}


/// <summary>
/// Fetches the application interface.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied.</returns>
// (0x00426D00)
// IApplication slot: the object itself as IApplication (a QueryInterface
// for IID 0x0079C3D0).

HRESULT __stdcall ApplicationClass::App_Interface(void **pVal)
{
    return QueryInterface(IID_IApplication_79c3d0, pVal);
}


/// <summary>
/// Fetches the full name of the application.
/// This routine is used by a scripting client that wishes to display which game it is
/// talking to.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination string was supplied.</returns>
/// <remarks>The caller takes ownership of the string returned and must free it.</remarks>
// (0x00426D20)
// IApplication slot: returns a new BSTR of L"Tiberian Sun" (E_POINTER without an
// out pointer).

HRESULT __stdcall ApplicationClass::get_FullName(BSTR *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = SysAllocString(L"Tiberian Sun");
    return S_OK;
}


/// <summary>
/// Fetches the short name of the application.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination string was supplied.</returns>
/// <remarks>The caller takes ownership of the string returned and must free it.</remarks>
// (0x00426D50)
// IApplication slot: returns a new BSTR of L"Sun" (E_POINTER without an
// out pointer).

HRESULT __stdcall ApplicationClass::get_Name(BSTR *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = SysAllocString(L"Sun");
    return S_OK;
}


/// <summary>
/// Fetches the application interface.
/// This is the property named alias of App_Interface, provided so that a scripting client
/// can reach the application under either name.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied.</returns>
// (0x00426D80)
// IApplication slot: the object itself as IApplication (a QueryInterface
// for IID 0x0079C3D0).

HRESULT __stdcall ApplicationClass::Application_Interface(void **pVal)
{
    return QueryInterface(IID_IApplication_79c3d0, pVal);
}


/// <summary>
/// Handles a request from a client to quit the game.
/// </summary>
/// <remarks>The request is accepted, but the game is not actually shut down by it.</remarks>
// (0x00426DA0)
// IApplication slot 5: always 0.

long __stdcall ApplicationClass::Quit()
{
    return 0;
}


/// <summary>
/// Fetches the name of the scenario currently being played.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination string was supplied.</returns>
/// <remarks>The caller takes ownership of the string returned and must free it.</remarks>
// (0x00426DB0)
// IApplication slot: the scenario name (+0x83C of the scenario at 0x00A3D290) as a new BSTR.

HRESULT __stdcall ApplicationClass::get_ScenarioName(BSTR *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    wchar_t text[32];
    MultiByteToWideChar(0, 1, unknown_id_source_a3d290->FileName, -1, text, 32);
    *retval = SysAllocString(text);
    return S_OK;
}


/// <summary>
/// Fetches the elapsed game time of the scenario in progress.
/// </summary>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
// (0x00426E00)
// IApplication slot: the scenario's elapsed-time timer value (+0x614).

HRESULT __stdcall ApplicationClass::get_FrameCount(long *retval)
{
    if (retval == 0) {
        return E_POINTER;
    }
    *retval = unknown_id_source_a3d290->Elapsed_Time().Time();
    return S_OK;
}


/// <summary>
/// Fetches the pointer swizzler interface.
/// This routine hands a client access to the save game pointer remapper, so that it can
/// resolve the pointers held in a saved game.
/// </summary>
/// <returns>Returns with S_OK if the interface was supplied.</returns>
// (0x00426E50)
// swizzler's ISwizzle interface (name from mischa85/RedAlert2 src/app.cpp).

HRESULT __stdcall ApplicationClass::get_Swizzle_Interface(ISwizzle **pVal)
{
    return Swizzler.QueryInterface(IID_ISwizzle, (void **)pVal);
}
