// cpoint.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cpoint.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_ALL
#include "shared/shape_file.hpp"
#include "shared/connection_points.hpp"
#include "shared/abstract.hpp"
#include "shared/crt_memory.hpp"


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


// Emitted by the shared headers of this translation unit.


int __fastcall operator!=(tagCONNECTDATA const &a,tagCONNECTDATA const &b) { if (a.dwCookie==b.dwCookie && a.pUnk==b.pUnk) return 0; return 1; }


int __fastcall operator==(tagCONNECTDATA const &a,tagCONNECTDATA const &b) { if (a.dwCookie==b.dwCookie && a.pUnk==b.pUnk) return 1; return 0; }


HRESULT __stdcall EnumConnectionsClass::QueryInterface(GUID const &iid,void **object) {
 if (!object) return E_POINTER;
 *object=0;
 if (!memcmp(&iid,&IID_IUnknown,sizeof(GUID))) *object=this;
 if (!memcmp(&iid,&IID_IEnumConnections,sizeof(GUID))) *object=this;
 if (*object) { AddRef(); return 0; }
 return E_NOINTERFACE;
}


unsigned long __stdcall EnumConnectionsClass::AddRef() { ++COMReferences_00a6eb14; return InterlockedIncrement(&RefCount); }


unsigned long __stdcall EnumConnectionsClass::Release() { --COMReferences_00a6eb14; unsigned long count=InterlockedDecrement(&RefCount); if (!count) delete this; return count; }


HRESULT __stdcall EnumConnectionsClass::Next(unsigned long requested,tagCONNECTDATA *data,unsigned long *fetched) {
 if (!data) return E_POINTER;
 if (requested>1 && !fetched) return (HRESULT)0x80070057;
 if (fetched) *fetched=0;
 unsigned long count=0;
 for (unsigned long i=0; i<requested && Position<Connections.Count(); ++i) {
  Connections[Position].pUnk->AddRef();
  *data++=Connections[Position++]; ++count;
 }
 if (fetched) *fetched=count;
 return count ? 0 : (HRESULT)0x80004005;
}


HRESULT __stdcall EnumConnectionsClass::Skip(unsigned long count) { Position+=count; return 0; }


HRESULT __stdcall EnumConnectionsClass::Reset() { Position=0; return 0; }


HRESULT __stdcall EnumConnectionsClass::Clone(IEnumConnections **result) {
 if (!result) return E_POINTER;
 EnumConnectionsClass *copy=new EnumConnectionsClass(*this);
 if (!copy) return (HRESULT)0x8007000e;
 copy->RefCount=0;
 copy->AddRef(); *result=copy; return 0;
}


EnumConnectionsClass::EnumConnectionsClass(DynamicVectorClass<tagCONNECTDATA> const &data):Connections(data),Position(0),RefCount(0) { for (int i=0;i<Connections.Count();++i) Connections[i].pUnk->AddRef(); }


EnumConnectionsClass::EnumConnectionsClass(EnumConnectionsClass const &other):Connections(other.Connections),Position(other.Position),RefCount(0) { for (int i=0;i<Connections.Count();++i) Connections[i].pUnk->AddRef(); }


EnumConnectionsClass::~EnumConnectionsClass() { for (int i=0;i<Connections.Count();++i) Connections[i].pUnk->Release(); Connections.Clear(); }


HRESULT __stdcall ConnectionPointClass::QueryInterface(GUID const &iid,void **object) {
 if (!object) return E_POINTER;
 IUnknown *found=0;
 if (!memcmp(&iid,&IID_IConnectionPoint,sizeof(GUID)) || !memcmp(&iid,&IID_IUnknown,sizeof(GUID))) found=this;
 *object=found;
 if (found) { found->AddRef(); return 0; }
 return E_NOINTERFACE;
}


unsigned long __stdcall ConnectionPointClass::AddRef() { ++COMReferences_00a6eb14; return InterlockedIncrement(&RefCount); }


unsigned long __stdcall ConnectionPointClass::Release() { --COMReferences_00a6eb14; unsigned long count=InterlockedDecrement(&RefCount); if (!count) delete this; return count; }


HRESULT __stdcall ConnectionPointClass::GetConnectionInterface(GUID *iid) { if (!iid) return E_POINTER; *iid=InterfaceID; return 0; }


HRESULT __stdcall ConnectionPointClass::GetConnectionPointContainer(IConnectionPointContainer **container) { return Container->QueryInterface(IID_IConnectionPointContainer,(void **)container); }


HRESULT __stdcall ConnectionPointClass::Advise(IUnknown *sink,unsigned long *cookie) {
 if (!sink || !cookie) return E_POINTER;
 tagCONNECTDATA connection;
 connection.dwCookie=unknown_id_source_a3d290->Next_ID();
 if (FAILED(sink->QueryInterface(InterfaceID,(void **)&connection.pUnk))) return (HRESULT)0x80040202;
 if (!Connections.Add(connection)) return (HRESULT)0x80040201;
 *cookie=connection.dwCookie; return 0;
}


HRESULT __stdcall ConnectionPointClass::Unadvise(unsigned long cookie) {
 for (int i=0;i<Connections.Count();++i) if (Connections[i].dwCookie==cookie) { Connections.Delete(i); return 0; }
 return (HRESULT)0x80040200;
}


HRESULT __stdcall ConnectionPointClass::EnumConnections(IEnumConnections **result) {
 if (!result) return E_POINTER;
 EnumConnectionsClass *copy=new EnumConnectionsClass(Connections);
 if (!copy) return (HRESULT)0x8007000e;
 return copy->QueryInterface(IID_IEnumConnections,(void **)result);
}


ConnectionPointClass::ConnectionPointClass(GUID const &iid,IUnknown *container):InterfaceID(iid),RefCount(0),Container(container),Connections() { container->AddRef(); }


ConnectionPointClass::~ConnectionPointClass() { Container->Release(); Connections.Clear(); }


HRESULT __stdcall EnumConnectionPointsClass::QueryInterface(GUID const &iid,void **object) {
 if (!object) return E_POINTER;
 IUnknown *found=0;
 if (!memcmp(&iid,&IID_IEnumConnectionPoints,sizeof(GUID)) || !memcmp(&iid,&IID_IUnknown,sizeof(GUID))) found=this;
 *object=found;
 if (found) { AddRef(); return 0; }
 return E_NOINTERFACE;
}


unsigned long __stdcall EnumConnectionPointsClass::AddRef() { ++COMReferences_00a6eb14; return InterlockedIncrement(&RefCount); }


unsigned long __stdcall EnumConnectionPointsClass::Release() { --COMReferences_00a6eb14; unsigned long count=InterlockedDecrement(&RefCount); if (!count) delete this; return count; }


HRESULT __stdcall EnumConnectionPointsClass::Next(unsigned long requested,IConnectionPoint **data,unsigned long *fetched) {
 if (!data) return E_POINTER;
 if (requested>1 && !fetched) return E_POINTER;
 if (fetched) *fetched=0;
 unsigned long count=0;
 for (unsigned long i=0;i<requested && Position<Points.Count();++i) { Points[Position]->AddRef(); *data++=Points[Position++]; ++count; }
 if (fetched) *fetched=count;
 return count ? 0 : (HRESULT)0x80004005;
}


HRESULT __stdcall EnumConnectionPointsClass::Skip(unsigned long count) { Position+=count; return 0; }


HRESULT __stdcall EnumConnectionPointsClass::Reset() { Position=0; return 0; }


HRESULT __stdcall EnumConnectionPointsClass::Clone(IEnumConnectionPoints **result) {
 if (!result) return E_POINTER;
 EnumConnectionPointsClass *copy=new EnumConnectionPointsClass(*this);
 if (!copy) return (HRESULT)0x8007000e;
 copy->AddRef(); *result=copy; return 0;
}


EnumConnectionPointsClass::EnumConnectionPointsClass(DynamicVectorClass<IConnectionPoint *> const &data):Position(0),RefCount(0),Points(data) { for(int i=0;i<Points.Count();++i) Points[i]->AddRef(); }


EnumConnectionPointsClass::EnumConnectionPointsClass(EnumConnectionPointsClass const &other):Position(other.Position),RefCount(0),Points(other.Points) { for(int i=0;i<Points.Count();++i) Points[i]->AddRef(); }


EnumConnectionPointsClass::~EnumConnectionPointsClass() { for(int i=0;i<Points.Count();++i) Points[i]->Release(); Points.Clear(); }


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.


// Instantiated by the COM class definitions and shared vector template.
