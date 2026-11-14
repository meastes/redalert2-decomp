// ddraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ddraw.json carries the addresses and the rows that
// stay in their own files.


#define DIRECTDRAW_DECLARE_MODE_METHODS
#define RA2_PALETTE_CONSTRUCTORS
#define PALETTE_DECLARE_MEMBERS
#include "shared/palette_state.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_memory.hpp"
#include "shared/dsurface.hpp"
#include "shared/debug_printf.hpp"
#include "shared/ddraw_result.hpp"
#include "shared/directdraw.hpp"
#include "shared/win32_api.hpp"
#include "shared/crt_string.hpp"
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#include "shared/vector_class.hpp"
#include "shared/video_mode_size.hpp"


void __fastcall Fade_Palette_To(PaletteClass const &target, int time, void (*callback)())
{
    if (unknown_surface_839950->Bytes_Per_Pixel() != 1) return;
    CDTimerClass<SystemTimerClass> timer = time;
    PaletteClass original;
    memcpy(&original, &unknown_palette_00851cc8, sizeof(PaletteClass));
    PaletteClass goal = target;
    while (timer) {
        PaletteClass palette = original;
        int ratio = ((time - timer) * 256) / time;
        if (ratio >= 255) ratio = 255;
        palette.Adjust(ratio, goal);
        int holdtime = timer;
        palette.Set();
        if (callback) callback();
        while (timer == holdtime && holdtime != 0) {
            if (callback) callback();
        }
    }
    goal.Set();
}


extern bool direct_draw_overlap_enabled_007d6295;
extern char const color_fill_test_log_007d5a44[];
extern char const color_fill_enabled_007d5a40[];
extern char const color_fill_disabled_007d5a38[];
extern char const color_fill_result_format_007d5a34[];
extern char const overlap_test_log_007d5a0c[];
extern char const overlap_failed_007d5a00[];
extern char const overlap_passed_007d59fc[];
void __cdecl Video_Debug_Log_00496460(char const *format,...);
void __fastcall Probe_DirectDraw_Rendering_004957e0()
{
    DSurface surface(64,64,false);
    Debug_Printf(color_fill_test_log_007d5a44);
    unknown_flag_7d6294=true;
    surface.Fill(0);
    if (surface.Get_Pixel(Point2D(5,5))!=0) {
        unknown_flag_7d6294=false;
    } else {
        surface.Fill(1);
        if (surface.Get_Pixel(Point2D(5,5))!=1) unknown_flag_7d6294=false;
    }
    Video_Debug_Log_00496460(color_fill_result_format_007d5a34,
        unknown_flag_7d6294?color_fill_enabled_007d5a40:color_fill_disabled_007d5a38);
    Debug_Printf(overlap_test_log_007d5a0c);
    direct_draw_overlap_enabled_007d6295=true;
    surface.Fill(0);
    surface.Put_Pixel(Point2D(0,0),255);
    surface.Copy_From(Rect(0,1,64,63),surface,Rect(0,0,64,63),false,false);
    if (surface.Get_Pixel(Point2D(0,5))!=0) {
        Video_Debug_Log_00496460(overlap_failed_007d5a00);
        direct_draw_overlap_enabled_007d6295=false;
        return;
    }
    Video_Debug_Log_00496460(overlap_passed_007d59fc);
}


extern IDirectDraw *DirectDrawObject;
extern bool unknown_flag_851cb0;
extern char const direct_draw_prep_log_007d5a8c[];
extern char const direct_draw_create_failed_007d5a70[];
void __cdecl Prep_Direct_Draw()
{
    Debug_Printf(direct_draw_prep_log_007d5a8c);
    if (DirectDrawObject == 0) {
        HRESULT result = DirectDrawCreate(0, &DirectDrawObject, 0);
        Process_DD_Result(result, false);
        if (result == DD_OK) {
            if (unknown_flag_851cb0) result = DirectDrawObject->SetCooperativeLevel(MainWindow, 8);
            else result = DirectDrawObject->SetCooperativeLevel(MainWindow, 0x11);
            Process_DD_Result(result, false);
        } else {
            Debug_Printf(direct_draw_create_failed_007d5a70);
        }
    }
}


extern IDirectDraw *DirectDrawObject;
extern void (__fastcall *direct_draw_error_handler_008523ec)(HRESULT result);
extern HWND MainWindow;
extern char const direct_draw_error_format_007d59a8[];
extern char const direct_draw_error_title_007d599c[];
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
WIN32_IMPORT int WINAPI MessageBoxA(HWND window, char const *text, char const *caption, UINT type);
void __cdecl Release_DirectDraw_00495a60()
{
    char error[80];
    if (DirectDrawObject) {
        unsigned long result = DirectDrawObject->Release();
        if (result != 0) {
            if (direct_draw_error_handler_008523ec) direct_draw_error_handler_008523ec(result);
            else {
                sprintf(error,direct_draw_error_format_007d59a8,result);
                MessageBoxA(MainWindow,error,direct_draw_error_title_007d599c,0x30);
            }
        }
        DirectDrawObject = 0;
    }
}


extern IDirectDraw *DirectDrawObject;
extern char const video_memory_actual_log_007d5af8[];
extern char const video_memory_display_log_007d5ac8[];
extern char const video_memory_adjusted_log_007d5aa0[];
DWORD __cdecl Get_Video_Memory()
{
    HRESULT result;
    DDCAPS video_capabilities;
    DDSURFACEDESC desc;
    bool created = false;
    if (DirectDrawObject == 0) {
        result = DirectDrawCreate(0, &DirectDrawObject, 0);
        Process_DD_Result(result, false);
        if (result == DD_OK) {
            created = true;
        } else {
            DirectDrawObject = 0;
        }
    }
    DWORD video_memory = 0;
    if (DirectDrawObject) {
        video_capabilities.dwSize = sizeof(video_capabilities);
        if (DD_OK == DirectDrawObject->GetCaps(&video_capabilities, 0)) {
            video_memory = video_capabilities.dwVidMemTotal;
            Debug_Printf(video_memory_actual_log_007d5af8, video_memory, video_memory >> 20);
            memset(&desc, 0, sizeof(desc));
            desc.dwSize = sizeof(desc);
            if (DirectDrawObject->GetDisplayMode(&desc) == 0) {
                Debug_Printf(video_memory_display_log_007d5ac8, desc.dwWidth, desc.dwHeight, desc.ddpfPixelFormat.dwRGBBitCount, desc.lPitch);
                video_memory += desc.dwWidth * desc.dwHeight * (desc.ddpfPixelFormat.dwRGBBitCount >> 3);
            }
            video_memory += 512 * 256;
            video_memory &= 0xfff00000;
            Debug_Printf(video_memory_adjusted_log_007d5aa0, video_memory, video_memory >> 20);
        }
    }
    if (created) {
        if (DirectDrawObject != 0) {
            result = DirectDrawObject->Release();
            Process_DD_Result(result, false);
            DirectDrawObject = 0;
        }
    }
    return video_memory;
}


extern IDirectDraw *DirectDrawObject;
extern int direct_draw_width_008523e0, direct_draw_height_008523e4, direct_draw_depth_008523e8;
extern char const set_display_mode_log_007d5b7c[];
extern char const set_display_mode_failed_007d5b64[];
extern char const create_palette_log_007d5b54[];
extern char const create_palette_failed_007d5b3c[];
extern char const display_mode_set_log_007d5b28[];
void __fastcall Probe_DirectDraw_Rendering_004957e0();
void __cdecl Prep_Direct_Draw();
bool __fastcall Set_Video_Mode(HWND, int w, int h, int bits_per_pixel)
{
    HRESULT result;
    Prep_Direct_Draw();
    Debug_Printf(set_display_mode_log_007d5b7c, w, h, bits_per_pixel);
    result = DirectDrawObject->SetDisplayMode(w, h, bits_per_pixel);
    if (result != DD_OK) {
        Debug_Printf(set_display_mode_failed_007d5b64);
        return false;
    }
    direct_draw_width_008523e0 = w;
    direct_draw_height_008523e4 = h;
    direct_draw_depth_008523e8 = bits_per_pixel;
    if (bits_per_pixel == 8) {
        Debug_Printf(create_palette_log_007d5b54);
        result = DirectDrawObject->CreatePalette(0x44, unknown_palette_entries_00851fcc, (void **)&unknown_palette_00851fc8, 0);
        Process_DD_Result(result, false);
        if (result != DD_OK) {
            Debug_Printf(create_palette_failed_007d5b3c);
            return false;
        }
    }
    Probe_DirectDraw_Rendering_004957e0();
    Debug_Printf(display_mode_set_log_007d5b28);
    return true;
}


extern IDirectDraw *DirectDrawObject;
extern void (__fastcall *direct_draw_error_handler_008523ec)(HRESULT result);
extern HWND MainWindow;
extern char const direct_draw_error_format_007d59a8[];
extern char const direct_draw_error_title_007d599c[];
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
WIN32_IMPORT int WINAPI MessageBoxA(HWND window, char const *text, char const *caption, UINT type);
extern int direct_draw_width_008523e0,direct_draw_height_008523e4,direct_draw_depth_008523e8;
void __cdecl Restore_Video_Mode_00495e90()
{
    char error[80];
    if (DirectDrawObject) {
        HRESULT result=DirectDrawObject->RestoreDisplayMode();
        if (result!=0) {
            if (direct_draw_error_handler_008523ec) direct_draw_error_handler_008523ec(result);
            else {
                sprintf(error,direct_draw_error_format_007d59a8,result);
                MessageBoxA(MainWindow,error,direct_draw_error_title_007d599c,0x30);
            }
        }
        direct_draw_width_008523e0=0;
        direct_draw_height_008523e4=0;
        direct_draw_depth_008523e8=0;
        unsigned long releaseCount=DirectDrawObject->Release();
        if (releaseCount!=0) {
            if (direct_draw_error_handler_008523ec) direct_draw_error_handler_008523ec(releaseCount);
            else {
                sprintf(error,direct_draw_error_format_007d59a8,releaseCount);
                MessageBoxA(MainWindow,error,direct_draw_error_title_007d599c,0x30);
            }
        }
        DirectDrawObject=0;
    }
}


extern IDirectDraw *DirectDrawObject;
extern char const video_memory_log_007d5b98[];
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
DWORD __cdecl Get_Free_Video_Memory_00495f60()
{
    DDCAPS caps;
    char buffer[256];
    if (DirectDrawObject) {
        caps.dwSize=sizeof(caps);
        if (DirectDrawObject->GetCaps(&caps,0)==0) {
            sprintf(buffer,video_memory_log_007d5b98,caps.dwVidMemFree);
            return caps.dwVidMemFree;
        }
    }
    return 0;
}


extern IDirectDraw *DirectDrawObject;
extern void (__fastcall *direct_draw_error_handler_008523ec)(HRESULT result);
extern HWND MainWindow;
extern char const direct_draw_error_format_007d59a8[];
extern char const direct_draw_error_title_007d599c[];
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
WIN32_IMPORT int WINAPI MessageBoxA(HWND window, char const *text, char const *caption, UINT type);
unsigned int __cdecl Get_DirectDraw_Capabilities_00495fc0()
{
    char error[80];
    DDCAPS caps;
    if (!DirectDrawObject) return 0;
    memset(&caps,0,sizeof(caps));
    caps.dwSize=sizeof(caps);
    HRESULT result=DirectDrawObject->GetCaps(&caps,0);
    if (result!=0) {
        if (direct_draw_error_handler_008523ec) direct_draw_error_handler_008523ec(result);
        else {
            sprintf(error,direct_draw_error_format_007d59a8,result);
            MessageBoxA(MainWindow,error,direct_draw_error_title_007d599c,0x30);
        }
        return 0;
    }
    unsigned int flags=0;
    if (caps.dwCaps&0x40) flags=1;
    if (caps.dwCaps&0x80) flags|=2;
    if (caps.dwCaps&0x10000) flags|=4;
    if (caps.dwCaps&0x8000000) flags|=8;
    if (caps.dwCaps&0x4000000) flags|=16;
    if (caps.dwCaps&0x2000000) flags|=32;
    if (caps.dwCaps&0x200) flags|=64;
    return flags;
}


extern IDirectDraw *DirectDrawObject;
extern void (__fastcall *direct_draw_error_handler_008523ec)(HRESULT result);
extern HWND MainWindow;
extern bool direct_draw_wait_enabled_007d5998;
extern char const direct_draw_error_format_007d59a8[];
extern char const direct_draw_error_title_007d599c[];
extern "C" int __cdecl sprintf(char *buffer,char const *format,...);
WIN32_IMPORT int WINAPI MessageBoxA(HWND window,char const *text,char const *caption,UINT type);
void __cdecl Wait_For_Vertical_Blank_004960a0()
{
    char error[80];
    if (direct_draw_wait_enabled_007d5998) {
        HRESULT result=DirectDrawObject->WaitForVerticalBlank(1,0);
        if (result==(HRESULT)0x80004001L) {
            direct_draw_wait_enabled_007d5998=false;
            return;
        }
        if (result!=0) {
            if (direct_draw_error_handler_008523ec) direct_draw_error_handler_008523ec(result);
            else {
                sprintf(error,direct_draw_error_format_007d59a8,result);
                MessageBoxA(MainWindow,error,direct_draw_error_title_007d599c,0x30);
            }
        }
    }
}


extern IDirectDrawSurface *primary_ddraw_surface_008523d8;
HRESULT __cdecl Wait_For_Surface_Blit_004961d0()
{
    HRESULT result;
    do {
        result=primary_ddraw_surface_008523d8->GetBltStatus(2);
    } while (result!=0 && result!=DDERR_SURFACELOST);
    return result;
}


HRESULT __stdcall Collect_Display_Mode_004961f0(DDSURFACEDESC *description,void *context)
{
    DDSURFACEDESC *copy=new DDSURFACEDESC;
    *copy=*description;
    ((DynamicVectorClass<DDSURFACEDESC *> *)context)->Add(copy);
    return 1;
}


int __cdecl Compare_Video_Mode_Size_00496260(void const *left,void const *right)
{
    VideoModeSize a=*(VideoModeSize const *)left;
    VideoModeSize b=*(VideoModeSize const *)right;
    if (b.Width>a.Width) return -1;
    if (b.Width<a.Width) return 1;
    if (a.Height<b.Height) return -1;
    return b.Height<a.Height;
}


extern IDirectDraw *DirectDrawObject;
HRESULT __stdcall Collect_Display_Mode_004961f0(DDSURFACEDESC *description,void *context);
int __cdecl Compare_Video_Mode_Size_00496260(void const *left,void const *right);
extern "C" void __cdecl qsort(void *base,unsigned int count,unsigned int size,int (__cdecl *compare)(void const *,void const *));
// The real derived constructor body, with the base constructor external in this unit.
template <class T>
DynamicVectorClass<T>::DynamicVectorClass(unsigned size,T const *array)
    : VectorClass<T>(size,array)
{
    GrowthStep=10;
    ActiveCount=0;
}
VideoModeSize *__fastcall Get_Display_Mode_Sizes_004962a0(DWORD minimumWidth,DWORD minimumHeight,
    DWORD maximumWidth,DWORD maximumHeight,DWORD bitDepth)
{
    if (!DirectDrawObject) return 0;
    DynamicVectorClass<DDSURFACEDESC *> descriptions;
    if (DirectDrawObject->EnumDisplayModes(0,0,&descriptions,Collect_Display_Mode_004961f0)==0 && descriptions.Count()!=0) {
        int *result=new int[descriptions.Count()*2+1];
        int *next=result;
        memset(result,0,descriptions.Count()*8+4);
        int count=0;
        for (int i=0;i<descriptions.Count();++i) {
            DDSURFACEDESC *description=descriptions[i];
            if (description->dwWidth<=maximumWidth && description->dwHeight<=maximumHeight &&
                description->dwWidth>=minimumWidth && description->dwHeight>=minimumHeight &&
                description->ddpfPixelFormat.dwRGBBitCount==bitDepth) {
                *next++=(int)description->dwWidth;
                *next++=(int)description->dwHeight;
                ++count;
            }
            delete descriptions[i];
        }
        if (next==result) {
            delete[] result;
            return 0;
        }
        qsort(result,count,8,Compare_Video_Mode_Size_00496260);
        return (VideoModeSize *)result;
    }
    return 0;
}


class Leaf_4955b0 { public: void get(); };
void Leaf_4955b0::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(0x851cb8) = zero_value; *(unsigned int*)(0x851cbc) = zero_value; *(unsigned int*)(0x851cc0) = zero_value; *(unsigned int*)(0x851cc4) = zero_value; }


void PaletteClass::Set()
{
    if (this != &unknown_palette_00851cc8)
        memmove(&unknown_palette_00851cc8, this, sizeof(PaletteClass));
    IDirectDrawSurface *surface;
    if (DirectDrawObject && (surface = unknown_palette_surface_008523d8)) {
        unsigned char const *color = (unsigned char const *)Palette;
        for (int i = 0; i < 256; ++i) {
            int red = *color++;
            int green = *color++;
            int blue = *color++;
            unknown_palette_entries_00851fcc[i].peRed = (unsigned char)red;
            unknown_palette_entries_00851fcc[i].peGreen = (unsigned char)green;
            unknown_palette_entries_00851fcc[i].peBlue = (unsigned char)blue;
        }
        if (unknown_palette_00851fc8) {
            if (!unknown_palette_attached_008523d4) {
                surface->SetPalette(unknown_palette_00851fc8);
                unknown_palette_attached_008523d4 = true;
            }
            unknown_palette_00851fc8->SetEntries(0, 0, 256, unknown_palette_entries_00851fcc);
        }
    }
}


class Leaf_496460 { public: void get(); };
void Leaf_496460::get() {  }


extern "C" void __cdecl release_496470(void*);
class Cleanup_496470 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_496470::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_496470(pointer); items=0; } owns=0; count=0; }


extern "C" unsigned char vtable_496730[]; extern "C" void __cdecl release_buffer_496730(void*); extern "C" void __cdecl release_object_496730(void*);
class Leaf_496730 { public: void* get(unsigned int flags); };
void* Leaf_496730::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_496730; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_496730((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_496730(this); return this; }


extern "C" unsigned char vtable_496780[]; extern "C" void __cdecl release_buffer_496780(void*); extern "C" void __cdecl release_object_496780(void*);
class Leaf_496780 { public: void* get(unsigned int flags); };
void* Leaf_496780::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_496780; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_496780((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_496780(this); return this; }


extern "C" unsigned char table_4967d0[]; extern "C" void* __cdecl storage_4967d0(unsigned int);
class Storage_4967d0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4967d0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4967d0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4967d0(requested << 2); owns=1; } return this; }
