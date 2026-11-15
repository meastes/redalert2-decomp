/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/cstream.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// cstream.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cstream.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cstream.hpp"
#include "shared/lzo.hpp"


/// <summary>
/// Creates a compressing stream object.
/// This routine prepares the working buffers that the LZO codec needs. The object starts
/// out with no storage stream of its own to compress through.
/// </summary>
/// <remarks>Call Link_Stream to attach a storage stream before reading or writing.</remarks>
CStreamClass::CStreamClass() : RefCount(0), unknown_10(false), unknown_11(false), unknown_14(0), unknown_18(new char[65536]), unknown_1c(new char[65536]), unknown_20(new char[65536])
{
    unknown_24 = 65535;
}


/// <summary>
/// Destroys the compressing stream object.
/// Any storage stream still attached is unlinked first, so that whatever is left in the
/// work buffer is compressed out rather than lost.
/// </summary>
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


/// <summary>
/// Takes out a reference on this stream object.
/// </summary>
/// <returns>Returns with the new reference count.</returns>
// (0x00494910)
// Counts a live COM object and a reference (IUnknown slot 1).

unsigned long __stdcall CStreamClass::AddRef()
{
    unknown_com_objects_a6eb14++;
    return InterlockedIncrement(&RefCount);
}


/// <summary>
/// Releases a reference to this stream object.
/// This routine will destroy the object once the last outstanding reference has been
/// given up.
/// </summary>
/// <returns>Returns with the number of references that remain.</returns>
// (0x00494930)
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
/// <summary>
/// Fetches an alternate interface to this stream object.
/// The IUnknown, IStream and ILinkStream interfaces are the ones supported. A successful
/// query takes out a reference on this object for the caller.
/// </summary>
/// <param name="riid">The identifier of the interface being asked for.</param>
/// <param name="ppvObject">Pointer to the interface pointer to fill in.</param>
/// <returns>Returns with S_OK, or E_NOINTERFACE if the interface is not supported.</returns>
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


/// <summary>
/// Attaches the storage stream this object compresses through.
/// Use this routine to bind the compressor to the real stream that the compressed blocks
/// will be written to or read back from. Only one stream may be attached at a time.
/// </summary>
/// <param name="stream">Pointer to the object to fetch the storage stream from.</param>
/// <returns>Returns with S_OK, or E_FAIL if a stream is already attached.</returns>
HRESULT __stdcall CStreamClass::Link_Stream(IUnknown *stream)
{
    if (!stream) return E_POINTER;
    if (Stream) return E_FAIL;
    Stream = stream;
    return S_OK;
}


/// <summary>
/// Detaches the underlying storage stream from this object.
/// Anything still sitting in the work buffer is compressed out first and the storage
/// stream is committed, so the caller gets back a complete stream.
/// </summary>
/// <param name="stream">Pointer to fill in with the released stream, or NULL if the caller
/// does not want it.</param>
/// <returns>Returns with S_OK, or E_FAIL if no stream was attached. A commit failure is
/// returned as it stands.</returns>
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


/// <summary>
/// Reads and decompresses data from the underlying stream.
/// This routine pulls whole compressed blocks out of the storage stream and doles out
/// pieces of the decompressed result until the caller's request has been satisfied.
/// </summary>
/// <param name="pv">Pointer to the buffer to fill with the data read.</param>
/// <param name="cb">The number of bytes to read.</param>
/// <param name="pcbRead">Pointer to fill in with the number of bytes read, or NULL if the
/// count is not wanted.</param>
/// <returns>Returns with S_OK, or an error code if the data could not be read.</returns>
/// <remarks>A stream that is being written cannot also be read.</remarks>
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


/// <summary>
/// Compresses data out to the underlying stream.
/// This routine gathers the caller's data into a work buffer and hands it to the
/// compressor a block at a time, so what reaches the storage stream is a run of
/// compressed blocks rather than the raw bytes.
/// </summary>
/// <param name="pv">Pointer to the data to write.</param>
/// <param name="cb">The number of bytes to write.</param>
/// <param name="pcbWritten">Pointer to fill in with the number of bytes accepted, or NULL
/// if the count is not wanted.</param>
/// <returns>Returns with S_OK, or an error code if the data could not be written.</returns>
/// <remarks>A stream that is being read cannot also be written. The trailing partial
/// block does not reach the storage stream until the object is flushed or unlinked.</remarks>
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


/// <summary>
/// Moves the file pointer of the underlying stream.
/// </summary>
/// <returns>Returns with S_OK, or E_FAIL if a transfer is already under way.</returns>
/// <remarks>Seeking is refused once reading or writing has begun, since the compressor
/// keeps state that a seek would invalidate.</remarks>
// (0x00494DE0)
// IStream slot 5: refused (E_FAIL) while either state flag is set,
// otherwise forwarded to the inner stream.

HRESULT __stdcall CStreamClass::Seek(LARGE_INTEGER move, unsigned long origin, ULARGE_INTEGER *position)
{
    if (unknown_10 || unknown_11) {
        return E_FAIL;
    }
    return Stream->Seek(move, origin, position);
}


/// <summary>
/// Sets the size of the underlying stream.
/// </summary>
/// <returns>Returns with S_OK, or E_FAIL if a transfer is already under way.</returns>
/// <remarks>Resizing is refused once reading or writing has begun.</remarks>
// (0x00494E30)
// IStream slot 6: refused (E_FAIL) while either state flag is set,
// otherwise forwarded to the inner stream.

HRESULT __stdcall CStreamClass::SetSize(ULARGE_INTEGER size)
{
    if (unknown_10 || unknown_11) {
        return E_FAIL;
    }
    return Stream->SetSize(size);
}


/// <summary>
/// Copies data from this stream over to another stream.
/// The request is handed straight to the underlying stream, so it is the compressed
/// bytes that get copied rather than the data they stand for.
/// </summary>
/// <returns>Returns with the result of the underlying stream's copy request.</returns>
// (0x00494E80)
// IStream slot 7: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::CopyTo(IStream *stream, ULARGE_INTEGER size, ULARGE_INTEGER *read, ULARGE_INTEGER *written)
{
    return Stream->CopyTo(stream, size, read, written);
}


/// <summary>
/// Commits any pending changes to the underlying stream.
/// </summary>
/// <returns>Returns with the result of the underlying stream's commit request.</returns>
// (0x00494EC0)
// IStream slot 8: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Commit(unsigned long flags)
{
    return Stream->Commit(flags);
}


/// <summary>
/// Discards any uncommitted changes to the stream.
/// </summary>
/// <returns>Returns with the result of the underlying stream's revert request.</returns>
// (0x00494EF0)
// IStream slot 9: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Revert()
{
    return Stream->Revert();
}


/// <summary>
/// Locks a byte range of the underlying stream.
/// </summary>
/// <returns>Returns with the result of the underlying stream's lock request.</returns>
// (0x00494F20)
// IStream slot 10: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::LockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, unsigned long type)
{
    return Stream->LockRegion(offset, size, type);
}


/// <summary>
/// Releases a lock on a byte range of the underlying stream.
/// </summary>
/// <returns>Returns with the result of the underlying stream's unlock request.</returns>
// (0x00494F60)
// IStream slot 11: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::UnlockRegion(ULARGE_INTEGER offset, ULARGE_INTEGER size, unsigned long type)
{
    return Stream->UnlockRegion(offset, size, type);
}


/// <summary>
/// Fetches the statistics of the underlying stream.
/// </summary>
/// <returns>Returns with the result of the underlying stream's stat request.</returns>
// (0x00494FA0)
// IStream slot 12: forwards to the held inner stream (E_POINTER through
// _com_issue_error when none is held).

HRESULT __stdcall CStreamClass::Stat(STATSTG *stat, unsigned long flags)
{
    return Stream->Stat(stat, flags);
}


/// <summary>
/// Creates a second stream object over the same storage.
/// The clone is made by the underlying stream, so it is a plain stream rather than a
/// compressing one.
/// </summary>
/// <returns>Returns with the result of the underlying stream's clone request.</returns>
// (0x00494FD0)
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
