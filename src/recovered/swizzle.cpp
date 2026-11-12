/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// From mischa85/RedAlert2 dbca015 src/swizzle.cpp (matching RA2 1.006 decomp; TibSun swizzle.cpp);
// modified: the function bodies are this tree's accepted ones, and AddRef, Release, Save_Interface and
// Load_Interface are defined here rather than inline in the header (modules.json swizzle).

#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/swizzle.hpp"


SwizzleManagerClass Swizzler;


/// <summary>
/// Constructor for the pointer swizzle manager.
/// The request and pointer tables are given a generous growth step, since a save game
/// hands the swizzler thousands of pointers in a single pass.
/// </summary>
SwizzleManagerClass::SwizzleManagerClass()
{
    UnswizzledPointers.Set_Growth_Step(1000);
    SwizzledPointers.Set_Growth_Step(1000);
}


/// <summary>
/// Destructor for the pointer swizzle manager.
/// Any requests still pending are resolved on the way out.
/// </summary>
SwizzleManagerClass::~SwizzleManagerClass()
{
    Process_Tables();
}


/// <summary>
/// Resolves any outstanding pointer requests and starts over.
/// The load code calls this routine once every object has been read in and has announced
/// itself, so that the saved pointers become real addresses again.
/// </summary>
HRESULT __stdcall SwizzleManagerClass::Reset()
{
    Process_Tables();
    return S_OK;
}


/// <summary>
/// Registers a saved pointer to be resolved later.
/// The load code calls this routine for every pointer it reads back, since the value on
/// disk is a swizzle ID rather than an address. The request is remembered and the pointer
/// is cleared until the tables are processed and the real address is known.
/// </summary>
/// <param name="pointer">Pointer to the pointer that needs resolving.</param>
/// <returns>Returns with S_OK, or E_POINTER if no pointer was supplied.</returns>
HRESULT __stdcall SwizzleManagerClass::Swizzle(void **pointer)
{
    if (pointer == 0) {
        return E_POINTER;
    }
    if (*pointer == 0) {
        return S_OK;
    }
    UnswizzledPointers.Add(SwizzlePointerClass((LONG_PTR)*pointer, pointer));
    *pointer = 0;
    return S_OK;
}


/// <summary>
/// Announces the real address of a swizzle ID.
/// Objects call this routine as they are loaded, to say where they ended up. The requests
/// gathered by Swizzle are matched against these announcements when the tables are
/// processed.
/// </summary>
/// <param name="id">The swizzle ID the object was saved under.</param>
/// <param name="pointer">The address the object now resides at.</param>
HRESULT __stdcall SwizzleManagerClass::Here_I_Am(LONG_PTR id, void *pointer)
{
    SwizzlePointerClass entry;
    entry.Pointer = pointer;
    entry.ID = id;
    SwizzledPointers.Add(entry);
    return S_OK;
}


/// <summary>
/// Compares two swizzle entries by their ID.
/// This routine is the sort callback used on the request and pointer tables, so that the
/// two can be walked in step when the swizzler resolves them.
/// </summary>
/// <param name="ptr1">The first swizzle entry to compare.</param>
/// <param name="ptr2">The second swizzle entry to compare.</param>
/// <returns>Returns with -1, 0 or 1 according to whether the first entry's ID sorts
/// before, with, or after the second entry's ID.</returns>
static int __cdecl ptr_compare_func(void const *ptr1, void const *ptr2)
{
    LONG_PTR id1 = ((SwizzlePointerClass const *)ptr1)->ID;
    LONG_PTR id2 = ((SwizzlePointerClass const *)ptr2)->ID;
    if (id1 == id2) {
        return 0;
    }
    return id1 < id2 ? -1 : 1;
}


/// <summary>
/// Resolves every outstanding pointer request.
/// This is the heart of the swizzler. The requests registered by Swizzle are matched
/// against the addresses announced by Here_I_Am, and each real address is written into the
/// pointer that asked for it. Both tables are emptied once the matching is done.
/// </summary>
/// <remarks>Every ID handed to Swizzle must have been announced by Here_I_Am. An unmatched
/// request is fatal and this routine will deliberately fault rather than leave a dangling
/// pointer in the game.</remarks>
void SwizzleManagerClass::Process_Tables()
{
    if (UnswizzledPointers.Count() != 0) {
        if (SwizzledPointers.Count() > 0) {
            qsort(&SwizzledPointers[0], SwizzledPointers.Count(), sizeof(SwizzlePointerClass), ptr_compare_func);
        }
        if (UnswizzledPointers.Count() > 0) {
            qsort(&UnswizzledPointers[0], UnswizzledPointers.Count(), sizeof(SwizzlePointerClass), ptr_compare_func);
        }
        SwizzlePointerClass *sptr = &SwizzledPointers[0];
        int scount = SwizzledPointers.Count();
        int ucount = UnswizzledPointers.Count();
        SwizzlePointerClass *uptr = &UnswizzledPointers[0];
        while (ucount > 0) {
            if (uptr->ID == sptr->ID) {
                *(void **)uptr->Pointer = sptr->Pointer;
                uptr++;
                ucount--;
            } else {
                if (uptr->ID > sptr->ID) {
                    do {
                        sptr++;
                        scount--;
                    } while (uptr->ID > sptr->ID);
                }
                if (uptr->ID != sptr->ID) {
                    volatile int zero = 0;
                    zero = 1 / zero;
                }
            }
        }
        UnswizzledPointers.Clear();
        SwizzledPointers.Clear();
    }
}


/// <summary>
/// Fetches the room a swizzled pointer takes up in a save file.
/// The persistence code calls this routine to size the space a pointer will need once it
/// has been reduced to a swizzle ID.
/// </summary>
/// <param name="size">Pointer to the variable to fill in with the byte count.</param>
/// <returns>Returns with S_OK, or E_POINTER if no destination was supplied.</returns>
HRESULT __stdcall SwizzleManagerClass::Get_Save_Size(int *size)
{
    if (size == 0) {
        return E_POINTER;
    }
    *size = 4;
    return S_OK;
}


/// <summary>
/// Fetches the requested interface from the swizzle manager.
/// Only the swizzler's own interface is supported. The manager is a single global object
/// rather than a reference counted one, so the caller never really owns what it gets back.
/// </summary>
/// <param name="riid">The interface identifier being asked for.</param>
/// <param name="ppvObject">Pointer to the variable to fill in with the interface pointer.</param>
/// <returns>Returns with S_OK, E_POINTER if no destination was supplied, or E_NOINTERFACE
/// if the interface asked for is not one the manager provides.</returns>
HRESULT __stdcall SwizzleManagerClass::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    if (iid == IID_ISwizzle || iid == IID_IUnknown_7afce0) {
        *object = this;
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}


/// <summary>
/// Fetches the swizzle ID that stands in for an object.
/// The save code calls this routine to turn a live pointer into the value that will be
/// written to the file in its place. An object's current address serves as its ID.
/// </summary>
/// <param name="pointer">The object to fetch the ID of.</param>
/// <param name="id">Pointer to the variable to fill in with the ID.</param>
/// <returns>Returns with S_OK, or E_POINTER if the object or the destination was missing.</returns>
HRESULT __stdcall SwizzleManagerClass::Fetch_Swizzle_ID(void *pointer, LONG_PTR *id)
{
    if (pointer != 0 && id != 0) {
        *id = (LONG_PTR)pointer;
        return S_OK;
    }
    return E_POINTER;
}

/*
**	The COM housekeeping slots (inline in the template's swizzle.h; out of line here).
*/
unsigned long __stdcall SwizzleManagerClass::AddRef()
{
    return 1;
}

unsigned long __stdcall SwizzleManagerClass::Release()
{
    return 1;
}

HRESULT __stdcall SwizzleManagerClass::Save_Interface(IStream *stream, IUnknown *object)
{
    return E_NOTIMPL;
}

HRESULT __stdcall SwizzleManagerClass::Load_Interface(IStream *stream, GUID *riid, void **object)
{
    return E_NOTIMPL;
}


/*
**	The unit also emits the atexit destructor of Swizzler (0x0069F9D0) and, for the tables'
**	vtables, VectorClass<SwizzlePointerClass>'s constructor and the scalar deleting destructors of
**	VectorClass and DynamicVectorClass<SwizzlePointerClass> (0x006A0130-0x006A01D0).
*/
