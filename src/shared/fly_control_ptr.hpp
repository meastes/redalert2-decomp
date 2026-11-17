// The Fly unit's IFlyControl COM pointer instance, with no Win32 or game layouts.
// The interface is complete where the reference-management bodies are defined (shared/fly_locomotion.hpp).
// Member shapes follow comip's _com_ptr_t (operator->'s E_POINTER check, != as !==, GetIID, _AddRef, _Release).
#ifndef RA2_SHARED_FLY_CONTROL_PTR_HPP
#define RA2_SHARED_FLY_CONTROL_PTR_HPP
struct GUID;
struct IFlyControl;
class FootClass;
extern "C" GUID const unknown_uuid_7d80a8;
void __stdcall _com_issue_error(long result);
template<class Interface,GUID const *iid> class UnknownComPtr;
template<> class UnknownComPtr<IFlyControl,&unknown_uuid_7d80a8> {
public:
    UnknownComPtr() : Pointer(0) {}
    UnknownComPtr(UnknownComPtr const &other);
    ~UnknownComPtr();
    operator IFlyControl *() const { return Pointer; }
    IFlyControl *operator->() const {
        if (!Pointer) _com_issue_error((long)0x80004003);
        return Pointer;
    }
    IFlyControl *Checked() const {
        if (!Pointer) _com_issue_error((long)0x80004003);
        return Pointer;
    }
    void Release_Held();
    void Add_Held() { if (Pointer) Pointer->AddRef(); }
    static GUID const &GetIID() { return unknown_uuid_7d80a8; }
    UnknownComPtr(FootClass * const &other) : Pointer(0)
    {
        if (other) {
            long result=Query_From(other);
            if (result<0 && result!=(long)0x80004002) {
                _com_issue_error(result);
            }
        }
    }
    bool operator!=(int null_value) const { return !operator==(null_value); }
    template<class Other> long Query_Held(Other other) {
        IFlyControl *pointer;
        long result=other->QueryInterface(GetIID(),(void**)&pointer);
        if (result<0) {
            Release_Held();
            Pointer=0;
        } else {
            IFlyControl *attached=pointer;
            Release_Held();
            Pointer=attached;
        }
        return result;
    }
    void Attach(IFlyControl *pointer);
    UnknownComPtr &operator=(IFlyControl *pointer);
    long Query_From(FootClass * const &other);
    bool operator==(int null_value) const
    {
        if (null_value!=0) _com_issue_error((long)0x80004003);
        return Pointer==0;
    }
    IFlyControl *Pointer;
};
#endif
