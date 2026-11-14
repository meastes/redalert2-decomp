// animfile.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/animfile.json carries the addresses and the rows that
// stay in their own files.


#include "shared/anim_file.hpp"
#include "shared/crt_memory.hpp"


// AnimFile::AnimFile() (0x00423F90)
// An empty player: no file, a cleared palette, no frame surface or page
// buffer, no current frame or page.

AnimFile::AnimFile() :
    File(0),
    Loaded(false),
    OpenedFile(false),
    Frame(0),
    PageData(0),
    CurrentFrame(-1),
    CurrentPage(-1)
{
}


// AnimFile::~AnimFile() (0x00423FF0)
// Closes the player (the file when this object opened it, the frame surface
// and the page buffer).

AnimFile::~AnimFile()
{
    AnimFile::Close();
}


// 0x00424060: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_00424060(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00424060 {

unsigned int  FUN_00424060(void)
{
    return 0xb00;
}

}  // namespace seed_00424060


// bool AnimFile::Open(FileClass *file) (0x00424070)
// Remembers the file, opens it for reading when it is not open (and then
// closes it again itself) and reads the header.

bool AnimFile::Open(FileClass *file)
{
    File = file;
    if (file != 0) {
        if (!file->Is_Open()) {
            File->Open(FILE_RIGHTS_READ);
            OpenedFile = true;
        }
        return Initialize();
    }
    return false;
}


// void AnimFile::Close() (0x004240B0)

void AnimFile::Close()
{
    if (OpenedFile) {
        File->Close();
        OpenedFile = false;
    }
    File = 0;
    if (Frame != 0) {
        delete Frame;
        Frame = 0;
    }
    if (PageData != 0) {
        delete PageData;
        PageData = 0;
    }
    Loaded = false;
}


// bool AnimFile::Initialize() (0x00424110)
// Reads the animation file's header, color map (stored as 4-byte entries
// in blue, green, red order) and large page table, rejects a file Validate
// does not accept (the player closes itself), and allocates the frame
// surface and the page buffer. Returns whether the player is loaded.

bool AnimFile::Initialize()
{
    if (!Loaded && File != 0) {
        Loaded = true;
        memset(&Header, 0, sizeof(Header));
        File->Read(&Header, sizeof(Header));
        memset(Buffer, 0, sizeof(Buffer));
        File->Read(Buffer, sizeof(Buffer));
        File->Seek(0, FILE_SEEK_CURRENT);
        File->Read(ColorMap, sizeof(ColorMap));
        for (int index = 0; index < 256; index++) {
            Palette[index % 256] = RGBClass(ColorMap[index * 4 + 2], ColorMap[index * 4 + 1], ColorMap[index * 4]);
        }
        memset(Pages, 0, sizeof(Pages));
        File->Read(Pages, sizeof(Pages));
        if (!Validate()) {
            Close();
            return false;
        }
        Frame = new BSurface(Header.Width, Header.Height, 1);
        PageData = (unsigned char *)operator new(0x10000);
        memset(PageData, 0, 0x10000);
    }
    return Loaded;
}


// bool AnimFile::Validate() (0x004242B0)
// Whether the header describes a supported animation: "LPF " / "ANIM",
// 256 pages at most, 320x200, a plain bitmap, and every page of at most 256
// records.

bool AnimFile::Validate()
{
    if (Header.ID == 0x2046504c && Header.ContentType == 0x4d494e41 && Header.MaxPages == 0x100
        && Header.RecordCount <= 0xffff && Header.MaxRecordsPerPage <= 0x100 && Header.TableOffset == 0x500
        && Header.Width == 0x140 && Header.Height == 200 && Header.CompressionType <= 1
        && Header.BitmapType == 1 && Header.OtherRecordsPerFrame == 0 && Header.PixelType == 0) {
        for (int page = 0; page < Header.PageCount; page++) {
            if (Pages[page].RecordCount > 0x100) {
                return false;
            }
        }
        return true;
    }
    return false;
}


// int AnimFile::Get_Frame_Count() (0x00424370)
// The frame count; one less when the file ends with a delta back to the
// first frame.

int AnimFile::Get_Frame_Count()
{
    int count = Header.FrameCount;
    if (Header.HasLastDelta) {
        count--;
    }
    return count;
}


// int AnimFile::Get_Width() (0x00424390)

int AnimFile::Get_Width()
{
    return Header.Width;
}


// int AnimFile::Get_Height() (0x004243A0)

int AnimFile::Get_Height()
{
    return Header.Height;
}


// void *AnimFile::Get_Frame(int frame) (0x004243B0, slot 3)
// Decodes frames into the frame surface until it shows the wanted one:
// going back restarts from a cleared surface unless the file loops through
// its last delta back to frame 0; past the last record it wraps to 0.

void *AnimFile::Get_Frame(int frame)
{
    if (!Loaded) {
        return 0;
    }
    if (CurrentFrame > frame) {
        if (Header.HasLastDelta && Header.LastDeltaValid) {
            if (CurrentFrame != (int)Header.RecordCount || frame != 0) {
                Frame->Fill(0);
                CurrentFrame = -1;
            }
        } else {
            Frame->Fill(0);
            CurrentFrame = -1;
        }
    }
    if (CurrentFrame != frame) {
        void *buffer = Frame->Lock();
        int size = Frame->Get_Height() * Frame->Get_Width() * Frame->Bytes_Per_Pixel();
        while (CurrentFrame != frame) {
            CurrentFrame++;
            void *data = Frame_Data(CurrentFrame);
            if (data != 0) {
                Decode_Frame((unsigned char *)data, Frame_Size(CurrentFrame), (unsigned char *)buffer, size);
            }
            if (CurrentFrame > Header.FrameCount) {
                CurrentFrame = 0;
            }
        }
        Frame->Unlock();
    }
    return Frame;
}


// int AnimFile::Page_Size(int page) const (0x004246F0)
// The bytes of a large page: its data, the record size table and the header.

int AnimFile::Page_Size(int page) const
{
    return Pages[page].ByteCount + Pages[page].RecordCount * 2 + 8;
}


// bool AnimFile::Load_Page(int page) (0x00424720)
// Reads a large page into the page buffer unless it is the current one.

bool AnimFile::Load_Page(int page)
{
    if (Loaded) {
        if (CurrentPage != page) {
            CurrentPage = page;
            File->Seek(page * 0x10000 + 0xb00, FILE_SEEK_START);
            if (File->Read(PageData, Page_Size(page)) != Page_Size(page)) {
                return false;
            }
        }
        return true;
    }
    return false;
}


// int AnimFile::Find_Page(int frame) const (0x004247A0)
// The large page holding a frame; -1 when there is none.

int AnimFile::Find_Page(int frame) const
{
    if (frame < Header.FrameCount) {
        for (int page = 0; page < Header.PageCount; page++) {
            if (Pages[page].FirstRecord <= frame && Pages[page].RecordCount + Pages[page].FirstRecord > frame) {
                return page;
            }
        }
    }
    return -1;
}


// void *AnimFile::Frame_Data(int frame) (0x004247F0)
// The compressed data of a frame inside its large page (loaded first): past
// the page header and size table, after the sizes of the earlier records.

void *AnimFile::Frame_Data(int frame)
{
    int page = Find_Page(frame);
    if (page != -1) {
        Load_Page(page);
        int first = Pages[page].FirstRecord;
        int offset = Pages[page].RecordCount * 2 + 8;
        unsigned short *sizes = (unsigned short *)(PageData + 8);
        frame -= first;
        while (frame > 0) {
            offset += *sizes++;
            frame--;
        }
        return PageData + offset + 2;
    }
    return 0;
}


// int AnimFile::Frame_Size(int frame) (0x004248F0)
// The size of a frame's record data (its size table entry less the
// 2-byte header); 0 when the frame is not in the file.

int AnimFile::Frame_Size(int frame)
{
    unsigned size = 0;
    int page = Find_Page(frame);
    if (page != -1) {
        Load_Page(page);
        unsigned short *sizes = (unsigned short *)(PageData + 8);
        frame -= Pages[page].FirstRecord;
        if (frame >= 0) {
            int remaining = frame + 1;
            do {
                size = *sizes++;
                if (size >= 2) {
                    size -= 2;
                }
                remaining--;
            } while (remaining != 0);
        }
    }
    return size;
}


// bool AnimFile::Decode_Frame(void *source, int size, void *dest, int destsize) (0x004249F0):
// lays one frame record over dest. The record starts with its body type (short): 0 is an
// uncompressed copy (refused unless it fills dest exactly), 1 a run/skip/dump stream that
// must end in the stop code 80 00 00; any other type is refused. An empty record is no change.

bool AnimFile::Decode_Frame(void *source, int size, void *dest, int destsize)
{
    int body;
    unsigned char *dst;
    bool decoded;

    if (size == 0) {
        return true;
    }

    unsigned char *src;
    signed char count;
    int word_count;
    int pixel;

    body = *(short *)source;
    size -= sizeof(short);
    source = (unsigned char *)source + sizeof(short);
    dst = (unsigned char *)dest;

    if (size == 0) {
        return true;
    }

    decoded = true;

    switch (body) {
        case 0:
            if (size != destsize) {
                decoded = false;
            } else {
                memcpy(dst, source, size);
            }
            break;

        case 1:
            src = (unsigned char *)source + size - 3;
            if (size < 3 || src[0] != 0x80 || src[1] != 0 || src[2] != 0) {
                decoded = false;
                break;
            }

            src = (unsigned char *)source;
            {
            next_op:
                count = *src++;
                if (count > 0) {
                    goto short_dump;
                }
                if (count == 0) {
                    goto short_run;
                }
                count -= (signed char)0x80;
                if (count == 0) {
                    goto long_op;
                }

                // short skip
                dst += count;
                goto next_op;

            short_dump:
                memcpy(dst, src, count);
                dst += count;
                src += count;
                goto next_op;

            short_run:
                word_count = *(unsigned char *)src;
                src += sizeof(unsigned char);
                pixel = *src;
                src += sizeof(unsigned char);
                memset(dst, pixel, word_count);
                dst += word_count;
                goto next_op;

            long_op:
                word_count = *(unsigned short *)src;
                src += sizeof(unsigned short);
                if ((signed short)word_count <= 0) {
                    goto not_long_skip;
                }

                // long skip
                dst += word_count;
                goto next_op;

            not_long_skip:
                if (word_count == 0) {
                    goto stop;
                }
                word_count -= 0x8000;
                if (word_count >= 0x4000) {
                    goto long_run;
                }

                // long dump
                memcpy(dst, src, word_count);
                dst += word_count;
                src += word_count;
                goto next_op;

            long_run:
                word_count -= 0x4000;
                pixel = *src;
                src += sizeof(unsigned char);
                memset(dst, pixel, word_count);
                dst += word_count;
                goto next_op;

            stop:
                ;
            }
            break;

        default:
            decoded = false;
            break;
    }

    return decoded;
}


// void *Animate::`scalar deleting destructor'(unsigned int) (0x00424B90)
// Compiler-emitted from shared/anim_file.hpp in this unit; no source of its own.


// RGBClass *AnimFile::Get_Palette() (0x00424BB0)
// Compiler-emitted from shared/anim_file.hpp in this unit; no source of its own.


// void *AnimFile::`scalar deleting destructor'(unsigned int) (0x00424BC0)
// Compiler-emitted from shared/anim_file.hpp in this unit with the
// destructor expanded; no source of its own.
