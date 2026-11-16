/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/cpoint.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/cpoint.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/cpoint.cpp where both functions declare the same local types (names only).

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


/// <summary>
/// Fetches an interface pointer from the enumerator.
/// This routine serves IEnumConnections and IUnknown. A reference is taken on the
/// enumerator before the pointer is handed back.
/// </summary>
/// <param name="riid">The interface identifier being asked for.</param>
/// <param name="ppv">Pointer to fill in with the interface.</param>
/// <returns>Returns with S_OK if the interface was supplied, or E_NOINTERFACE if it is not
/// supported.</returns>
HRESULT __stdcall EnumConnectionsClass::QueryInterface(GUID const &riid,void **ppv) {
 if (!ppv) return E_POINTER;
 *ppv=0;
 if (!memcmp(&riid,&IID_IUnknown,sizeof(GUID))) *ppv=this;
 if (!memcmp(&riid,&IID_IEnumConnections,sizeof(GUID))) *ppv=this;
 if (*ppv) { AddRef(); return 0; }
 return E_NOINTERFACE;
}


unsigned long __stdcall EnumConnectionsClass::AddRef() { ++COMReferences_00a6eb14; return InterlockedIncrement(&RefCount); }


unsigned long __stdcall EnumConnectionsClass::Release() { --COMReferences_00a6eb14; unsigned long count=InterlockedDecrement(&RefCount); if (!count) delete this; return count; }


/// <summary>
/// Fetches the next batch of connections.
/// This routine is called by a client walking the sinks advised on a connection point.
/// Every sink handed back has been referenced on the client's behalf, so the client must
/// release them when it is done.
/// </summary>
/// <param name="cConnections">The number of connections asked for.</param>
/// <param name="rgpcd">Buffer to fill in with the connections fetched.</param>
/// <param name="pcFetched">Pointer to fill in with the number actually fetched. This may be
/// NULL only when a single connection was asked for.</param>
/// <returns>Returns with S_OK if any were fetched, or E_FAIL once the enumerator has run
/// dry.</returns>
HRESULT __stdcall EnumConnectionsClass::Next(unsigned long cConnections,tagCONNECTDATA *rgpcd,unsigned long *pcFetched) {
 if (!rgpcd) return E_POINTER;
 if (cConnections>1 && !pcFetched) return (HRESULT)0x80070057;
 if (pcFetched) *pcFetched=0;
 unsigned long count=0;
 for (unsigned long i=0; i<cConnections && Position<Connections.Count(); ++i) {
  Connections[Position].pUnk->AddRef();
  *rgpcd++=Connections[Position++]; ++count;
 }
 if (pcFetched) *pcFetched=count;
 return count ? 0 : (HRESULT)0x80004005;
}


HRESULT __stdcall EnumConnectionsClass::Skip(unsigned long cConnections) { Position+=cConnections; return 0; }


HRESULT __stdcall EnumConnectionsClass::Reset() { Position=0; return 0; }


/// <summary>
/// Creates a duplicate of this enumerator.
/// This routine hands the client an independent enumerator positioned exactly where this
/// one currently sits. The client owns the enumerator returned and must release it.
/// </summary>
/// <param name="ppEnum">Pointer to fill in with the clone.</param>
/// <returns>Returns with S_OK if the clone was created, or an error code otherwise.</returns>
HRESULT __stdcall EnumConnectionsClass::Clone(IEnumConnections **ppEnum) {
 if (!ppEnum) return E_POINTER;
 EnumConnectionsClass *enumerator=new EnumConnectionsClass(*this);
 if (!enumerator) return (HRESULT)0x8007000e;
 enumerator->RefCount=0;
 enumerator->AddRef(); *ppEnum=enumerator; return 0;
}


EnumConnectionsClass::EnumConnectionsClass(DynamicVectorClass<tagCONNECTDATA> const &data):Connections(data),Position(0),RefCount(0) { for (int i=0;i<Connections.Count();++i) Connections[i].pUnk->AddRef(); }


EnumConnectionsClass::EnumConnectionsClass(EnumConnectionsClass const &other):Connections(other.Connections),Position(other.Position),RefCount(0) { for (int i=0;i<Connections.Count();++i) Connections[i].pUnk->AddRef(); }


EnumConnectionsClass::~EnumConnectionsClass() { for (int i=0;i<Connections.Count();++i) Connections[i].pUnk->Release(); Connections.Clear(); }


/// <summary>
/// Fetches an interface pointer from the connection point.
/// This routine serves IConnectionPoint and IUnknown. A reference is taken on the
/// connection point before the pointer is handed back.
/// </summary>
/// <param name="riid">The interface identifier being asked for.</param>
/// <param name="ppv">Pointer to fill in with the interface.</param>
/// <returns>Returns with S_OK if the interface was supplied, or E_NOINTERFACE if it is not
/// supported.</returns>
HRESULT __stdcall ConnectionPointClass::QueryInterface(GUID const &riid,void **ppv) {
 if (!ppv) return E_POINTER;
 IUnknown *iface=0;
 if (!memcmp(&riid,&IID_IConnectionPoint,sizeof(GUID)) || !memcmp(&riid,&IID_IUnknown,sizeof(GUID))) iface=this;
 *ppv=iface;
 if (iface) { iface->AddRef(); return 0; }
 return E_NOINTERFACE;
}


unsigned long __stdcall ConnectionPointClass::AddRef() { ++COMReferences_00a6eb14; return InterlockedIncrement(&RefCount); }


unsigned long __stdcall ConnectionPointClass::Release() { --COMReferences_00a6eb14; unsigned long count=InterlockedDecrement(&RefCount); if (!count) delete this; return count; }


HRESULT __stdcall ConnectionPointClass::GetConnectionInterface(GUID *pIID) { if (!pIID) return E_POINTER; *pIID=InterfaceID; return 0; }


HRESULT __stdcall ConnectionPointClass::GetConnectionPointContainer(IConnectionPointContainer **ppCPC) { return Container->QueryInterface(IID_IConnectionPointContainer,(void **)ppCPC); }


/// <summary>
/// Connects a client sink to this connection point.
/// This routine is called by a client that wants to be notified of the events this
/// connection point publishes. The sink is asked for the connection interface and held
/// until the client unadvises it.
/// </summary>
/// <param name="pUnkSink">The client sink to connect.</param>
/// <param name="pdwCookie">Pointer to fill in with the cookie identifying the new
/// connection.</param>
/// <returns>Returns with S_OK if the sink was connected, or a connection error code
/// otherwise.</returns>
HRESULT __stdcall ConnectionPointClass::Advise(IUnknown *pUnkSink,unsigned long *pdwCookie) {
 if (!pUnkSink || !pdwCookie) return E_POINTER;
 tagCONNECTDATA connection;
 connection.dwCookie=Scen->Next_ID();
 if (FAILED(pUnkSink->QueryInterface(InterfaceID,(void **)&connection.pUnk))) return (HRESULT)0x80040202;
 if (!Connections.Add(connection)) return (HRESULT)0x80040201;
 *pdwCookie=connection.dwCookie; return 0;
}


/// <summary>
/// Breaks the connection identified by the cookie specified.
/// This routine is called by a client that no longer wants its sink notified.
/// </summary>
/// <param name="dwCookie">The cookie Advise handed out for the sink.</param>
/// <returns>Returns with S_OK if the connection was broken, or CONNECT_E_NOCONNECTION if
/// the cookie matches nothing.</returns>
HRESULT __stdcall ConnectionPointClass::Unadvise(unsigned long cookie) {
 for (int i=0;i<Connections.Count();++i) if (Connections[i].dwCookie==cookie) { Connections.Delete(i); return 0; }
 return (HRESULT)0x80040200;
}


/// <summary>
/// Creates an enumerator over the sinks advised on this connection point.
/// This routine is used by a client that wants to walk the connections currently
/// established. The client owns the enumerator returned and must release it.
/// </summary>
/// <param name="ppEnum">Pointer to fill in with the enumerator.</param>
/// <returns>Returns with S_OK if the enumerator was created, or an error code
/// otherwise.</returns>
HRESULT __stdcall ConnectionPointClass::EnumConnections(IEnumConnections **ppEnum) {
 if (!ppEnum) return E_POINTER;
 EnumConnectionsClass *enumerator=new EnumConnectionsClass(Connections);
 if (!enumerator) return (HRESULT)0x8007000e;
 return enumerator->QueryInterface(IID_IEnumConnections,(void **)ppEnum);
}


ConnectionPointClass::ConnectionPointClass(GUID const &riid,IUnknown *obj):InterfaceID(riid),RefCount(0),Container(obj),Connections() { obj->AddRef(); }


ConnectionPointClass::~ConnectionPointClass() { Container->Release(); Connections.Clear(); }


/// <summary>
/// Fetches an interface pointer from the enumerator.
/// This routine serves IEnumConnectionPoints and IUnknown. A reference is taken on the
/// enumerator before the pointer is handed back.
/// </summary>
/// <param name="riid">The interface identifier being asked for.</param>
/// <param name="ppv">Pointer to fill in with the interface.</param>
/// <returns>Returns with S_OK if the interface was supplied, or E_NOINTERFACE if it is not
/// supported.</returns>
HRESULT __stdcall EnumConnectionPointsClass::QueryInterface(GUID const &riid,void **ppv) {
 if (!ppv) return E_POINTER;
 IUnknown *iface=0;
 if (!memcmp(&riid,&IID_IEnumConnectionPoints,sizeof(GUID)) || !memcmp(&riid,&IID_IUnknown,sizeof(GUID))) iface=this;
 *ppv=iface;
 if (iface) { AddRef(); return 0; }
 return E_NOINTERFACE;
}


unsigned long __stdcall EnumConnectionPointsClass::AddRef() { ++COMReferences_00a6eb14; return InterlockedIncrement(&RefCount); }


unsigned long __stdcall EnumConnectionPointsClass::Release() { --COMReferences_00a6eb14; unsigned long count=InterlockedDecrement(&RefCount); if (!count) delete this; return count; }


/// <summary>
/// Fetches the next batch of connection points.
/// This routine is called by a client walking the connection points a container supports.
/// Every connection point handed back has been referenced on the client's behalf, so the
/// client must release them when it is done.
/// </summary>
/// <param name="cConnections">The number of connection points asked for.</param>
/// <param name="rgpcn">Buffer to fill in with the connection points fetched.</param>
/// <param name="pcFetched">Pointer to fill in with the number actually fetched. This may be
/// NULL only when a single connection point was asked for.</param>
/// <returns>Returns with S_OK if any were fetched, or E_FAIL once the enumerator has run
/// dry.</returns>
HRESULT __stdcall EnumConnectionPointsClass::Next(unsigned long cConnections,IConnectionPoint **rgpcn,unsigned long *pcFetched) {
 if (!rgpcn) return E_POINTER;
 if (cConnections>1 && !pcFetched) return E_POINTER;
 if (pcFetched) *pcFetched=0;
 unsigned long count=0;
 for (unsigned long i=0;i<cConnections && Position<Points.Count();++i) { Points[Position]->AddRef(); *rgpcn++=Points[Position++]; ++count; }
 if (pcFetched) *pcFetched=count;
 return count ? 0 : (HRESULT)0x80004005;
}


HRESULT __stdcall EnumConnectionPointsClass::Skip(unsigned long cConnections) { Position+=cConnections; return 0; }


HRESULT __stdcall EnumConnectionPointsClass::Reset() { Position=0; return 0; }


/// <summary>
/// Creates a duplicate of this enumerator.
/// This routine hands the client an independent enumerator positioned exactly where this
/// one currently sits. The client owns the enumerator returned and must release it.
/// </summary>
/// <param name="ppEnum">Pointer to fill in with the clone.</param>
/// <returns>Returns with S_OK if the clone was created, or an error code otherwise.</returns>
HRESULT __stdcall EnumConnectionPointsClass::Clone(IEnumConnectionPoints **ppEnum) {
 if (!ppEnum) return E_POINTER;
 EnumConnectionPointsClass *enumerator=new EnumConnectionPointsClass(*this);
 if (!enumerator) return (HRESULT)0x8007000e;
 enumerator->AddRef(); *ppEnum=enumerator; return 0;
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
