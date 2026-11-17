// The COM class factory template the game instantiates once per creatable
// class (TClassFactory<T>, RTTI names such as TClassFactory<WalkLocomotionClass>).
// Each instantiation has the five IClassFactory methods in one block:
// QueryInterface, AddRef, Release, CreateInstance, LockServer (0x180-byte
// stride from 0x006914F0). Only CreateInstance depends on T.
//
// Members (TibSun code/classfactory.h lines 35-121, the same bodies): the
// constructor, QueryInterface, AddRef, Release and LockServer are defined
// here, so the one unit that creates the factories (startup.cpp, Register
// classes: `new TClassFactory<T>`) emits them with the vtable, as the
// original's startup object did. CreateInstance is declared only: its body
// (TibSun's `new T()`) needs T's own declaration, which the startup unit's
// headers do not yet carry for the 74 registered classes, so each class's
// CreateInstance stays an explicit specialization in its own file.
#ifndef RA2_SHARED_CLASS_FACTORY_HPP
#define RA2_SHARED_CLASS_FACTORY_HPP

#include "shared/com_persist.hpp"

#ifndef E_INVALIDARG
#define E_INVALIDARG ((HRESULT)0x80070057L)
#endif
#ifndef E_OUTOFMEMORY
#define E_OUTOFMEMORY ((HRESULT)0x8007000EL)
#endif
#ifndef CLASS_E_NOAGGREGATION
#define CLASS_E_NOAGGREGATION ((HRESULT)0x80040110L)
#endif

extern "C" GUID const IID_IClassFactory;      // 0x007AFCF0 {00000001-0000-0000-C000-000000000046}

struct __declspec(novtable) IClassFactory : public IUnknown {
    virtual HRESULT __stdcall CreateInstance(IUnknown *outer, GUID const &iid, void **object) = 0;
    virtual HRESULT __stdcall LockServer(int lock) = 0;
};

template <class T>
class TClassFactory : public IClassFactory {
public:
    TClassFactory();
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual HRESULT __stdcall CreateInstance(IUnknown *outer, GUID const &iid, void **object);
    virtual HRESULT __stdcall LockServer(int lock);

private:
    long references;                    // +0x04
};

template <class T>
TClassFactory<T>::TClassFactory()
    : references(0)
{
}

template <class T>
HRESULT __stdcall TClassFactory<T>::QueryInterface(GUID const &iid, void **object)
{
    if (object == 0) {
        return E_POINTER;
    }
    *object = 0;
    if (iid == IID_IUnknown_7afce0 || iid == IID_IClassFactory) {
        *object = this;
    }
    if (*object == 0) {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

template <class T>
unsigned long __stdcall TClassFactory<T>::AddRef()
{
    return InterlockedIncrement(&references);
}

template <class T>
unsigned long __stdcall TClassFactory<T>::Release()
{
    long remaining = InterlockedDecrement(&references);
    if (remaining == 0) {
        delete this;
    }
    return remaining;
}

template <class T>
HRESULT __stdcall TClassFactory<T>::LockServer(int lock)
{
    if (lock) {
        references++;
    } else {
        references--;
    }
    return S_OK;
}

#endif
