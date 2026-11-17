// VectorClass<T> and DynamicVectorClass<T> declarations only (the layout and
// the member list; shared/vector_class.hpp documents them and holds the
// member definitions).
//
// A class with a container member includes this header instead of
// shared/vector_class.hpp, so that the member definitions a translation unit
// gets stay selected by that unit's own VECTOR_IMPLEMENT_* guards at its
// first inclusion of shared/vector_class.hpp (ScenarioClass's TypeList
// members, shared/id_source.hpp, which shared/abstract.hpp includes early).

#ifndef SHARED_VECTOR_CLASS_DECL_HPP
#define SHARED_VECTOR_CLASS_DECL_HPP

class NoInitClass;

template <class T>
class VectorClass {
public:
    VectorClass(NoInitClass const &noinit);
#ifdef RA2_VECTOR_SIGNED_SIZE
    // Signed size profile: Westwood's TibSun vector.h (the YR twin of WDTState() 0x0072BEF0
    // compiles exact with it; the unsigned form refuses the temporary's inline constructor).
    VectorClass(int size = 0, T const *array = 0);
#else
    VectorClass(unsigned size = 0, T const *array = 0);
#endif
    VectorClass(VectorClass<T> const &vector);
    virtual ~VectorClass();

#ifndef RA2_VECTOR_UNSIGNED_INDEX
    // Signed index as in Westwood's vector.h (TibSun vector.h, ra2-decomp vector.h); House004EF890 and
    // Place_Tiberium 0x47C3A0 byte proofs.
    T &operator[](int index);
    T const &operator[](int index) const;
#else
    // Migration holdout (docs/DECISIONS.md "Migration switch"): units not yet re-rolled to the signed
    // index define RA2_VECTOR_UNSIGNED_INDEX on their first row; listed in docs/HANDOFF.md.
    T &operator[](unsigned index);
    T const &operator[](unsigned index) const;
#endif
    int Length() const;

    // Not virtual in RA2: the vtables have no assignment slot.
    VectorClass<T> &operator=(VectorClass<T> const &vector);

    virtual bool operator==(VectorClass<T> const &vector) const;
    virtual bool Resize(int newsize, T const *array = 0);
    virtual void Clear();
    virtual int ID(T const *ptr);
    virtual int ID(T const &object);
    virtual T Fetch(int index) const;

protected:
    T *Vector;
    int VectorMax;
    bool IsValid;
    bool IsAllocated;
};

template <class T>
class DynamicVectorClass : public VectorClass<T> {
public:
    DynamicVectorClass(NoInitClass const &noinit);
    DynamicVectorClass(unsigned size = 0, T const *array = 0);

    int Count() const;
#ifdef VECTOR_IMPLEMENT_DYNAMIC_DELETE_ALL
    void Delete_All(); // actual TibSun vector.h889 capacity-preserving clear
#endif
    DynamicVectorClass<T> &operator=(DynamicVectorClass<T> const &rvalue);
    // Capacity increment for later growth (inlined into the map's hash-table
    // construction, MapClass::One_Time 0x005480D0).
    void Set_Growth_Step(int step);

    virtual bool Resize(int newsize, T const *array = 0);
    virtual void Clear();
#ifdef RA2_VECTOR_RESET_INLINE
    // Empties the list, keeping its size (inline; Explosion_Damage 0x0047E2C0; Scout765 form).
    void Reset() { int size = VectorMax; Clear(); Resize(size); }
#endif
    virtual int ID(T const *ptr);
    virtual int ID(T const &object);

    bool Add(T const &object);
    bool Add_Head(T const &object);
    bool Insert_After(T const &object, int index);   // inserts after the element at index (0x004F7270 for BaseNodeClass)
#ifdef VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER_INDEX
    bool Insert_After(int index, T const &object);   // the index-first overload (TibSun vector.h; always inlined)
#endif
    bool Add_After(T const &object, T const &after);
    bool Delete(T const &object);
    bool Delete(int index);

#ifdef RA2_VECTOR_PUBLIC_COUNT
public:
#else
protected:
#endif
    int ActiveCount;
    int GrowthStep;
};

#endif
