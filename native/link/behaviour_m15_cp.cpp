// Behaviour row (Scout1125, native M15; NOT EXACT, native/behaviour_rewrites.json):
// the connection-point enumerator HouseClass::EnumConnectionPoints 0x004ED340
// allocates (0x24 bytes, constructor 0x00492E20, unrecovered). HouseClass::
// Adjust_Drain walks it when a house's power changes (a power plant destroyed:
// reached in the win run at frame ~19,000). Written as the recovered twin
// EnumConnectionsClass (0x004924B0 constructor, 0x00492210 Release, Next):
// a copy of the points (each AddRef'd), a position and a reference count;
// Next hands out AddRef'd points and fails (E_FAIL) when none is left, which
// ends Adjust_Drain's `while (Next(...) >= 0)` loop.
#include "shared/com_persist.hpp"
#include "shared/connection_points.hpp"


namespace {
bool Same(GUID const &a, GUID const &b)
{
    unsigned char const *x = (unsigned char const *)&a, *y = (unsigned char const *)&b;
    for (int i = 0; i < 16; ++i) if (x[i] != y[i]) return false;
    return true;
}

class Native_Enum_Points : public IEnumConnectionPoints {
public:
    explicit Native_Enum_Points(DynamicVectorClass<IConnectionPoint *> &points)
        : Points(0), Count(points.Count()), Position(0), RefCount(0)
    {
        if (Count > 0) {
            Points = new IConnectionPoint *[Count];
            for (int i = 0; i < Count; ++i) {
                Points[i] = points[i];
                if (Points[i]) Points[i]->AddRef();
            }
        }
    }
    virtual HRESULT __stdcall QueryInterface(GUID const &iid, void **object)
    {
        if (object == 0) return E_POINTER;
        if (Same(iid, IID_IEnumConnectionPoints) || Same(iid, IID_IUnknown_7afce0)) {
            *object = this;
            AddRef();
            return S_OK;
        }
        *object = 0;
        return (HRESULT)0x80004002;
    }
    virtual RA2_ULONG __stdcall AddRef() { ++COMReferences_00a6eb14; return ++RefCount; }
    virtual RA2_ULONG __stdcall Release()
    {
        --COMReferences_00a6eb14;
        int count = --RefCount;
        if (count == 0) {
            for (int i = 0; i < Count; ++i) if (Points[i]) Points[i]->Release();
            delete[] Points;
            this->~Native_Enum_Points();
            ::operator delete((void *)this);
        }
        return count;
    }
    virtual HRESULT __stdcall Next(RA2_ULONG requested, IConnectionPoint **out, RA2_ULONG *fetched)
    {
        if (out == 0) return E_POINTER;
        if (requested > 1 && fetched == 0) return (HRESULT)0x80070057;
        RA2_ULONG count = 0;
        for (RA2_ULONG i = 0; i < requested && Position < Count; ++i) {
            IConnectionPoint *point = Points[Position++];
            if (point) point->AddRef();
            *out++ = point;
            ++count;
        }
        if (fetched) *fetched = count;
        return count ? S_OK : (HRESULT)0x80004005;
    }
    virtual HRESULT __stdcall Skip(RA2_ULONG count)
    {
        Position += (int)count;
        if (Position > Count) { Position = Count; return S_FALSE; }
        return S_OK;
    }
    virtual HRESULT __stdcall Reset() { Position = 0; return S_OK; }
    virtual HRESULT __stdcall Clone(IEnumConnectionPoints **out)
    {
        if (out) *out = 0;
        return (HRESULT)0x80004001;    // E_NOTIMPL (no caller clones it)
    }
    IConnectionPoint **Points;
    int Count;
    int Position;
    int RefCount;
};
}  // namespace

// The house allocates 0x24 bytes natively sized as its in-file class (vptr + 0x20 = 0x28 on arm64).
static_assert(sizeof(Native_Enum_Points) <= 0x28, "fits the house's allocation");
extern "C" void *ra2_m15_enum_points(void *self, DynamicVectorClass<IConnectionPoint *> &points)
    __asm__("__ZN27UnknownEnumConnectionPointsC1ER18DynamicVectorClassIP16IConnectionPointE");
void *ra2_m15_enum_points(void *self, DynamicVectorClass<IConnectionPoint *> &points)
{
    return new (self) Native_Enum_Points(points);
}
