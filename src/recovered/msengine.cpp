// msengine.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/msengine.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ms_engine.hpp"
#include "shared/ms_anim.hpp"
#include "shared/dsurface.hpp"
#define MS_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MS_MAX(a, b) (((a) > (b)) ? (a) : (b))


// Dynamic initializer of unknown_empty_rect (0x005B2BC0, _$E), the file-local
// empty Rect from shared/shape_file.hpp (0x00A71838 in this unit).


// MSEngine::MSEngine() (0x005B2BE0)
// Empty lists and room for 20 dirty rectangles.

MSEngine::MSEngine()
{
    RectCount = 0;
    Rects.Resize(20);
    Anims.Clear();
    Sfx.Clear();
}


// MSEngine::~MSEngine() (0x005B2C70)
// Deletes the sound effects and the animations it owns.

MSEngine::~MSEngine()
{
    int index;
    for (index = 0; index < Sfx.Count(); index++) {
        if (Sfx[index] != 0) {
            delete Sfx[index];
        }
    }
    Sfx.Clear();
    for (index = 0; index < Anims.Count(); index++) {
        if (Anims[index] != 0) {
            delete Anims[index];
        }
    }
    Anims.Clear();
    Rects.Clear();
}


// int MSEngine::Add_Anim(MSAnim *anim) (0x005B2D60)
// Appends an animation; its index, or -1 when null or the list is full.

int MSEngine::Add_Anim(MSAnim *anim)
{
    if (anim != 0 && Anims.Add(anim)) {
        return Anims.ID(anim);
    }
    return -1;
}


// void MSEngine::Replace_Anim(MSAnim *anim, MSAnim *old) (0x005B2DD0)
// Inserts anim after old (at index 0 when old is absent), then removes and
// deletes old.

void MSEngine::Replace_Anim(MSAnim *anim, MSAnim *old)
{
    Anims.Add_After(anim, old);
    Remove_Anim(old);
}


// void MSEngine::Remove_Anim(MSAnim *anim) (0x005B2EC0)
// Removes and deletes an animation in the list.

void MSEngine::Remove_Anim(MSAnim *anim)
{
    if (anim != 0 && Anims.ID(anim) != -1) {
        Anims.Delete(anim);
        delete anim;
    }
}


// Wait for an animation to disappear or finish (005B2F30).
void MSEngine::Wait_For(MSAnim *anim,unsigned time)
{
    CDTimerClass<SystemTimerClass> timer(time);
    while (true) {
        int index=Anims.ID(anim);
        if (index==-1) break;
        if (Anims[index]->Has_Finished()) break;
        if (time<=0) break;
        Run(1);
    }
}


// void MSEngine::Redraw() (0x005B2F90)
// Forces a full repaint of the hidden surface.

void MSEngine::Redraw()
{
    unknown_redraw_8523dc = true;
    Draw(unknown_surface_839954);
}


// void MSEngine::Draw(Surface *surface) (0x005B2FB0)
// After Redraw: clears the surface, centres the background on it and draws
// every animation. Then updates every animation, deleting finished ones,
// and collects the rectangles they changed.

void MSEngine::Draw(Surface *surface)
{
    int index;
    if (unknown_redraw_8523dc == true) {
        unknown_redraw_8523dc = false;
        surface->Fill(0);
        Rect rect = unknown_surface_839958->Get_Rect();
        rect.X = (surface->Get_Width() - rect.Width) / 2;
        rect.Y = (surface->Get_Height() - rect.Height) / 2;
        surface->Copy_From(unknown_surface_839958->Get_Rect(), *unknown_surface_839958, rect, false, true);
        for (index = 0; index < Anims.Count(); index++) {
            Anims[index]->Draw(surface, 0);
        }
        RectCount = 0;
        Add_Rect(surface->Get_Rect());
        Unknown_Slot1(surface);
    }
    for (index = 0; index < Anims.Count(); index++) {
        Rect dirty;
        if (Anims[index]->Advance(surface, &dirty) == true) {
            delete Anims[index];
            Anims.Delete(index);
        }
        Add_Rect(dirty);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// void MSEngine::Add_Rect(Rect const &rect) (0x005B3110)
// Adopted from ra2-decomp MSEngine::Add_Update_Rect (YR 0x005D1FD0).
// Merges rect into the dirty list: the first overlapping entry is removed
// and the union added again; otherwise rect is appended while there is
// room, else the entry whose union with rect has the smallest area is
// replaced by that union.


static inline bool Overlaps(Rect const &rect1, Rect const &rect2)
{
    bool overlapping = false;
    if (rect1.X < rect2.X) {
        if (rect1.X + rect1.Width >= rect2.X) {
            overlapping = true;
        }
    } else {
        if (rect2.X + rect2.Width >= rect1.X) {
            overlapping = true;
        }
    }

    if (rect1.Y < rect2.Y) {
        if (rect1.Y + rect1.Height < rect2.Y) {
            overlapping = false;
        }
    } else {
        if (rect2.Y + rect2.Height < rect1.Y) {
            overlapping = false;
        }
    }
    return overlapping;
}

void MSEngine::Add_Rect(Rect const &rect)
{
    int index;

    if (!rect.Is_Valid()) {
        return;
    }

    for (index = 0; index < RectCount; index++) {
        Rect const &update_rect = Rects[index];
        if (Overlaps(rect, update_rect) == true) {
            int rect_right = rect.X + rect.Width;
            int rect_bottom = rect.Y + rect.Height;
            int i_rect_right = update_rect.X + update_rect.Width;
            int i_rect_bottom = update_rect.Y + update_rect.Height;

            Rect new_rect;
            new_rect.X = MS_MIN(rect.X, update_rect.X);
            new_rect.Y = MS_MIN(rect.Y, update_rect.Y);
            new_rect.Width = rect_right > i_rect_right ? rect_right - new_rect.X : i_rect_right - new_rect.X;
            new_rect.Height = rect_bottom > i_rect_bottom ? rect_bottom - new_rect.Y : i_rect_bottom - new_rect.Y;

            RectCount--;
            for (; index < RectCount; index++) {
                Rects[index] = Rects[index + 1];
            }

            Add_Rect(new_rect);
            return;
        }
    }

    if (RectCount < Rects.Length()) {
        Rects[RectCount] = rect;
        RectCount++;
        return;
    }

    unsigned min_size = 0xFFFFFFFF;
    Rect min_rect(0, 0, 0, 0);
    int min_id = 0;

    for (index = 0; index < RectCount; index++) {
        Rect const &entry = Rects[index];
        int entry_left = entry.X;
        int entry_right = entry_left + entry.Width;
        int entry_top = entry.Y;
        int entry_bottom = entry_top + entry.Height;

        int left = MS_MIN(rect.X, entry_left);
        int top = MS_MIN(rect.Y, entry_top);
        int rect_right = rect.X + rect.Width;
        int width = rect_right > entry_right ? rect_right - left : entry_right - left;
        int bottom = MS_MAX(rect.Y + rect.Height, entry_bottom);
        unsigned area = width * (bottom - top);

        if (area < min_size) {
            min_size = area;
            min_id = index;
            min_rect.Set(left, top, width, bottom - top);
        }
    }

    Rects[min_id] = min_rect;
}


// void MSEngine::Blit_Rects(Surface *surface) (0x005B33A0)
// Copies every dirty rectangle of surface to the screen surface at the
// window's client origin, with the mouse drawn over it; empties the list.

void MSEngine::Blit_Rects(Surface *surface)
{
    if (RectCount > 0) {
        RECT client;
        GetClientRect(MainWindow, &client);
        ClientToScreen(MainWindow, (POINT *)&client);
        if (WWMouse != 0) {
            WWMouse->Draw_Mouse(surface);
        }
        for (int index = 0; index < RectCount; index++) {
            Rect dest = Rects[index];
            dest.X += client.left;
            dest.Y += client.top;
            unknown_surface_839950->Copy_From(dest, *surface, Rects[index], false, true);
        }
        RectCount = 0;
        if (WWMouse != 0) {
            WWMouse->Erase_Mouse(surface);
        }
    }
}


// void MSEngine::Blit_Rect(Surface *surface, Rect const &rect) (0x005B3490)
// Copies one rectangle of surface to the screen surface (no direct callers).

void MSEngine::Blit_Rect(Surface *surface, Rect const &rect)
{
    if (rect.Width > 0 && rect.Height > 0) {
        RECT client;
        GetClientRect(MainWindow, &client);
        ClientToScreen(MainWindow, (POINT *)&client);
        if (WWMouse != 0) {
            WWMouse->Draw_Mouse(surface);
        }
        Rect dest = rect;
        dest.X += client.left;
        dest.Y += client.top;
        unknown_surface_839950->Copy_From(dest, *surface, rect, false, true);
        if (WWMouse != 0) {
            WWMouse->Erase_Mouse(surface);
        }
    }
}


// Advance the animation engine for the requested number of ticks (005B3550).
void MSEngine::Run(int frames)
{
    CDTimerClass<SystemTimerClass> timer(1);
    do {
        do {
            unknown_481fb0();
            Unknown_Slot2();
            unknown_4961d0();
            Draw(unknown_surface_839954);
            Blit_Rects(unknown_surface_839954);
            unknown_5b5ee0();
            if (!unknown_flag_a40d28) {
                timer.Stop();
                Check_Focus();
                timer.Start();
            }
            Sleep(0);
        } while (timer.Value()>0);
        timer=1;
        --frames;
    } while (frames!=0);
}


// void MSEngine::Check_Focus() (0x005B3670)
// While the application is inactive: pauses every animation and sleeps in
// 500 ms steps, then resumes them.

void MSEngine::Check_Focus()
{
    if (unknown_flag_a40d28 != true) {
        Debug_Printf("MSEngine - Pausing animations\n");
        int index;
        for (index = 0; index < Anims.Count(); index++) {
            Anims[index]->Pause();
        }
        while (!unknown_flag_a40d28) {
            Debug_Printf("MSEngine - Sleeping\n");
            Sleep(500);
            unknown_5b5ee0();
        }
        Debug_Printf("MSEngine - Resuming animations\n");
        for (index = 0; index < Anims.Count(); index++) {
            Anims[index]->Resume();
        }
    }
}


// bool MSEngine::Add_Sfx(char const *name, int unknown2) (0x005B3710)
// Adds a named sound effect (no direct callers).

bool MSEngine::Add_Sfx(char const *name, int unknown2)
{
    if (name != 0 && unknown2 != 0) {
        MSSfx *sfx = new MSSfx(name, unknown2);
        return Sfx.Add(sfx);
    }
    return false;
}


// void MSEngine::Play_Sfx(char const *name, int volume) (0x005B3790)
// Plays the named sound effect (case-insensitive match) at volume scaled by the sound effect volume; nothing
// when name is null or sound is unavailable. No direct callers (the inventory has no row; the bound is a linear
// sweep). The name read through a local gives the original's registers (this in ebx, the entry in edi).

void MSEngine::Play_Sfx(char const *name, int volume)
{
    if (name != 0 && unknown_check_406f30()) {
        for (int i = 0; i < Sfx.Count(); i++) {
            MSSfx *sfx = Sfx[i];
            char const *sfx_name = sfx->Name;
            if (_stricmp(name, sfx_name) == 0) {
                sfx->Play(volume * unknown_volume_a40b44);
                return;
            }
        }
    }
}


// void MSEngine::Broadcast(int unknown1) (0x005B3800)
// Passes a value to slot 8 of every animation.

void MSEngine::Broadcast(int unknown1)
{
    for (int index = 0; index < Anims.Count(); index++) {
        Anims[index]->Restore(unknown1);
    }
}


// void MSEngine::Unknown_Slot2() (0x005B3830)
// Per-frame hook; empty in the base class.

void MSEngine::Unknown_Slot2()
{
}


// MSEngine scalar deleting destructor (0x005B3840), compiler generated; calls the destructor.


// DynamicVectorClass_MSAnim_ptr_Clear (0x005B3860): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSAnim_ptr_ID_pointer (0x005B3890): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_Fetch (0x005B38B0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSSfx_ptr_Clear (0x005B38C0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSSfx_ptr_ID_pointer (0x005B38F0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_Fetch (0x005B3910): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_operator_equal (0x005B3920): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_Resize (0x005B3960): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_Clear (0x005B3A10): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_ID_object (0x005B3A40): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_ID_pointer (0x005B3A80): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSAnim_ptr_Resize (0x005B3AA0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSAnim_ptr_ID_object (0x005B3B50): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_operator_equal (0x005B3B80): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_Resize (0x005B3BC0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_Clear (0x005B3C70): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_ID_object (0x005B3CA0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_ID_pointer (0x005B3CE0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSSfx_ptr_Resize (0x005B3D00): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSSfx_ptr_ID_object (0x005B3DB0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSAnim_ptr_scalar_deleting_destructor (0x005B3DE0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSAnim_ptr_scalar_deleting_destructor (0x005B3E30): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// VectorClass_MSSfx_ptr_scalar_deleting_destructor (0x005B3E80): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.


// DynamicVectorClass_MSSfx_ptr_scalar_deleting_destructor (0x005B3ED0): instantiated in MSEngine.cpp from shared/vector_class.hpp
// (the engine's animation and sound effect lists); no source of its own.
