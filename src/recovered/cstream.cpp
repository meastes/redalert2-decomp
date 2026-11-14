// cstream.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cstream.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cstream.hpp"
#include "shared/lzo.hpp"


CStreamClass::CStreamClass() : RefCount(0), unknown_10(false), unknown_11(false), unknown_14(0), unknown_18(new char[65536]), unknown_1c(new char[65536]), unknown_20(new char[65536])
{
    unknown_24 = 65535;
}


CStreamClass::~CStreamClass()
{
    if(Stream!=0) Unlink_Stream(0);
    delete [] (char *)unknown_20;
    unknown_20=0;
    delete [] (char *)unknown_18;
    unknown_18=0;
    delete [] (char *)unknown_1c;
    unknown_1c=0;
}


// unsigned long __stdcall CStreamClass::AddRef() -- 0x00494910
// Counts a live COM object and a reference (IUnknown slot 1).

unsigned long __stdcall CStreamClass::AddRef()
{
    unknown_com_objects_a6eb14++;
    return InterlockedIncrement(&RefCount);
}


// unsigned long __stdcall CStreamClass::Release() -- 0x00494930
// Drops a reference; the last one deletes the stream (IUnknown slot 2).

unsigned long __stdcall CStreamClass::Release()
{
    unknown_com_objects_a6eb14--;
    long count = InterlockedDecrement(&RefCount);
    if (count == 0) {
        delete this;
    }
    return count;
}


#ifndef E_NOINTERFACE
#define E_NOINTERFACE ((HRESULT)0x80004002L)
#endif
extern "C" GUID const IID_IStream_7afd40;
extern "C" GUID const IID_ILinkStream_7a1aa0;
HRESULT __stdcall CStreamClass::QueryInterface(GUID const &iid, void **object)
{
    if (!object) return E_POINTER;
    *object = 0;
    if (iid == IID_IUnknown_7afce0) *object = static_cast<IStream *>(this);
    if (iid == IID_IStream_7afd40) *object = static_cast<IStream *>(this);
    if (iid == IID_ILinkStream_7a1aa0) *object = static_cast<ILinkStream *>(this);
    if (!*object) return E_NOINTERFACE;
    AddRef();
    return S_OK;
}


HRESULT __stdcall CStreamClass::Link_Stream(IUnknown *stream)
{
    if (!stream) return E_POINTER;
    if (Stream) return E_FAIL;
    Stream = stream;
    return S_OK;
}


HRESULT __stdcall CStreamClass::Unlink_Stream(IUnknown **stream)
{
    unknown_flush_pending();
    if (!Stream) return E_FAIL;
    if (stream) {
        Stream->AddRef();
        *stream = Stream;
    }
    HRESULT result = Stream->Commit(0);
    if (SUCCEEDED(result)) {
        Stream.Release();
        result = S_OK;
    }
    return result;
}


HRESULT __stdcall CStreamClass::Read(void *buffer, unsigned long size, unsigned long *read)
{
    int remaining = size;
    if (!buffer) return E_POINTER;
    if ((long)size < 0) return (HRESULT)0x80070057L;
    if (!Stream) return E_FAIL;
    if (unknown_11) return E_FAIL;
    unknown_10 = true;
    if (read) *read = 0;
    while (remaining > 0) {
        if (unknown_14 > 0) {
            int count = remaining < unknown_14 ? remaining : unknown_14;
            memmove(buffer, (char *)unknown_18 + unknown_28 - unknown_14, count);
            buffer = (char *)buffer + count;
            remaining -= count;
            unknown_14 -= count;
        }
        if (!remaining) break;
        unsigned long received = 0;
        HRESULT result = Stream->Read(&unknown_24, 8, &received);
        if (FAILED(result)) return result;
        if (received != 8) return E_FAIL;
        result = Stream->Read(unknown_1c, unknown_24, &received);
        if (FAILED(result)) return result;
        if (received != unknown_24) return E_FAIL;
        unsigned int decompressed_size = 65536;
        lzo1x_decompress((unsigned char const *)unknown_1c, unknown_24, (unsigned char *)unknown_18, &decompressed_size, 0);
        unknown_14 = unknown_28;
    }
    if (read) *read = size;
    return S_OK;
}


HRESULT __stdcall CStreamClass::Write(void const *buffer, unsigned long size, unsigned long *written)
{
    char const *input = (char const *)buffer;
    int remaining = size;
    if (!input) return E_POINTER;
    if ((long)size < 0) return (HRESULT)0x80070057L;
    if (!Stream) return E_FAIL;
    if (unknown_10) return E_FAIL;
    unknown_11 = true;
    if (size != 0) {
        if (written) *written = 0;
        if (unknown_14 > 0) {
            int count = (int)size < 65536 - unknown_14 ? size : 65536 - unknown_14;
            memmove((char *)unknown_18 + unknown_14, input, count);
            int buffered = unknown_14 + count;
            input += count;
            remaining = size - count;
            unknown_14 = buffered;
            if (unknown_14 == 65536) {
                HRESULT result = unknown_flush_495000(unknown_18, 65536);
                if (FAILED(result)) return result;
                unknown_14 = 0;
            }
        }
        for (; remaining >= 65536; remaining -= 65536) {
            HRESULT result = unknown_flush_495000((void *)input, 65536);
            if (FAILED(result)) return result;
            input += 65536;
        }
        if (remaining > 0) {
            memmove(unknown_18, input, remaining);
            unknown_14 = remaining;
        }
        if (written) *written = size;
    }
    return S_OK;
}


// HRESULT __stdcall CStreamClass::Seek(LARGE_INTEGER, unsigned long, ULARGE_INTEGER *) -- 0x00494DE0
// IStream slot 5: refused (E_FAIL) while either state flag is set,
// otherwise forwarded to the inner stream.

HRESULT __stdcall CStreamClass::Seek(LARGE_INTEGER move, unsigned long origin, ULARGE_INTEGER *position)
{
    if (unknown_10 || unknown_11) {
        return E_FAIL;
    }
    return Stream->Seek(move, origin, position);
}


// HRESULT __stdcall CStreamClass::SetSize(ULARGE_INTEGER) -- 0x00494E30
// IStream slot 6: refused (E_FAIL) while either state flag is set,
// otherwise forwarded to the inner stream.

HRESULT __stdcall CStreamClass::SetSize(ULARGE_INTEGER size)
{
    if (unknown_10 || unknown_11) {
        return E_FAIL;
    }
    return Stream->SetSize(size);
}


// HRESULT __stdcall CStreamClass::CopyTo(IStream *stream, ULARGE_INTEGER size, ULARGE_INTEGER *read, ULARGE_INTEGER *written) -- 0x00494E80
// IStream slot 7: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::CopyTo(IStream *stream, ULARGE_INTEGER size, ULARGE_INTEGER *read, ULARGE_INTEGER *written)
{
    return Stream->CopyTo(stream, size, read, written);
}


// HRESULT __stdcall CStreamClass::Commit(unsigned long flags) -- 0x00494EC0
// IStream slot 8: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Commit(unsigned long flags)
{
    return Stream->Commit(flags);
}


// HRESULT __stdcall CStreamClass::Revert() -- 0x00494EF0
// IStream slot 9: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Revert()
{
    return Stream->Revert();
}


// HRESULT __stdcall CStreamClass::LockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, unsigned long type) -- 0x00494F20
// IStream slot 10: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::LockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, unsigned long type)
{
    return Stream->LockRegion(offset, size, type);
}


// HRESULT __stdcall CStreamClass::UnlockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, unsigned long type) -- 0x00494F60
// IStream slot 11: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::UnlockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, unsigned long type)
{
    return Stream->UnlockRegion(offset, size, type);
}


// HRESULT __stdcall CStreamClass::Stat(STATSTG *stat, unsigned long flags) -- 0x00494FA0
// IStream slot 12: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Stat(STATSTG *stat, unsigned long flags)
{
    return Stream->Stat(stat, flags);
}


// HRESULT __stdcall CStreamClass::Clone(IStream **stream) -- 0x00494FD0
// IStream slot 13: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Clone(IStream **stream)
{
    return Stream->Clone(stream);
}


HRESULT CStreamClass::unknown_flush_495000(void *buffer, int size)
{
    unsigned int compressed_size = size;
    lzo1x_1_compress((unsigned char const *)buffer, size, (unsigned char *)unknown_1c, &compressed_size, unknown_20);
    unknown_28 = 65536;
    unknown_24 = compressed_size;
    unsigned long written = 0;
    HRESULT result = Stream->Write(&unknown_24, 8, &written);
    if (FAILED(result)) return result;
    if (written != 8) return E_FAIL;
    result = Stream->Write(unknown_1c, compressed_size, &written);
    if (FAILED(result)) return result;
    return written == compressed_size ? S_OK : E_FAIL;
}


HRESULT CStreamClass::unknown_flush_pending()
{
    if (unknown_11 && unknown_14 > 0) {
        if (Stream==0) return E_FAIL;
        if(unknown_14>0) return unknown_flush_495000(unknown_18, unknown_14);
    }
    return S_OK;
}


// VC6 emits this scalar deleting destructor from the real CStreamClass destructor definition in this unit.


// Original operation returns 0/1 in EAX; callers test AL. Exact original
// C++ return spelling is unproved; unsigned char records the byte predicate.
unsigned char UnknownStreamPtr::operator==(int null_value) const
{
    if(null_value!=0) _com_issue_error(E_POINTER);
    return Pointer==0;
}


// Compiler-generated ILinkStream this adjustment, emitted with CStreamClass vtables.


// Compiler-generated ILinkStream this adjustment, emitted with CStreamClass vtables.


// Compiler-generated ILinkStream this adjustment, emitted with CStreamClass vtables.
