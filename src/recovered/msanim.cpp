/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/msanim.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/msanim.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/msanim.cpp where both functions declare the same local types (names only).

// msanim.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/msanim.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ms_anim.hpp"
#include "shared/wstring.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/dsurface.hpp"
#include "shared/rect_intersect.hpp"
#include "shared/msfont.hpp"
#include "shared/audio.hpp"


// Dynamic initializer of unknown_empty_rect (0x005AC540, _$E), the file-local
// empty Rect from shared/shape_file.hpp (0x00A71808 in this unit).


/// <summary>
/// Creates a shape drawer from the palette file named.
/// The palette is loaded and installed as the game palette, then a drawer is built
/// against the visible surface so that the anims have something to render their shapes
/// with. Be aware that the game palette is replaced as a side effect.
/// </summary>
/// <param name="palette_name">The name of the palette file to load.</param>
/// <returns>Returns with a pointer to the drawer created. If the palette cannot be
/// loaded, then NULL is returned.</returns>
// (0x005AC560)
// Reads a 256-colour palette (6-bit components) into 0x00838710 and returns
// a new drawer for it; null without a name, file or data.

ConvertClass *__fastcall Create_Drawer(Wstring const &filename)
{
    if (filename == 0) {
        return 0;
    }
    CCFileClass file(filename);
    if (!file.Is_Available()) {
        return 0;
    }
    unsigned char *palette = (unsigned char *)Load_Alloc_Data(file);
    if (palette == 0) {
        return 0;
    }
    for (int index = 0; index < 256; index++) {
        CCPalette[index] = RGBClass(palette[index * 3] << 2, palette[index * 3 + 1] << 2,
                                                 palette[index * 3 + 2] << 2);
    }
    operator delete(palette);
    return new ConvertClass(CCPalette, CCPalette, VisibleSurface, 1, false);
}


/// <summary>
/// Destroys the anim.
/// </summary>
// (0x005AC690)
// Out-of-line copy of the inline virtual destructor (stores the vtable).

MSAnim::~MSAnim()
{
}


/// <summary>
/// Creates a shape anim from the shape file named.
/// The shape is fetched out of the mix file catalog where possible, and only loaded from
/// disk on its own account when the catalog does not carry it. The anim starts out set to
/// play every frame the shape holds.
/// </summary>
/// <param name="name">The name of the shape file to animate.</param>
/// <param name="drawer">The drawer to render the shape with.</param>
/// <param name="rate">The delay in game frames between animation frames.</param>
/// <param name="loop">Should the animation start over when it runs off the end?</param>
/// <param name="flags">The shape drawing flags to render with.</param>
// (0x005AC6A0)
// Loads (or retrieves) the shape and plays all of its frames.

MSShapeAnim::MSShapeAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24, bool unknown_34, int flags) :
    MSAnim(x, y),
    Drawer(drawer),
    unknown_24(unknown_24),
    Frame(0),
    unknown_34(unknown_34),
    Flags(flags),
    OwnsShape(false)
{
    Shape = unknown_load_shape_495310(filename, &OwnsShape);
    StartFrame = 0;
    EndFrame = Shape->FrameCount - 1;
}


/// <summary>
/// Destroys the shape anim.
/// The shape data is only freed when this anim loaded it itself. A shape borrowed from
/// the mix file catalog is left alone.
/// </summary>
// (0x005AC720)
// Releases a shape this animation loaded itself.

MSShapeAnim::~MSShapeAnim()
{
    if (Shape != 0 && OwnsShape) {
        operator delete(Shape);
        Shape = 0;
        OwnsShape = false;
    }
}


/// <summary>
/// Sets the frame the shape anim is displaying.
/// The area the shape occupies is restored from the backdrop first, so the frame being
/// left behind does not linger under the new one. A frame beyond the end of the shape is
/// clamped to the last one.
/// </summary>
/// <param name="frame">The shape frame to display.</param>
// (0x005AC760)
// Changes the frame, reports the old rectangle as dirty when the new one
// differs, and clamps to the last frame.

void MSShapeAnim::Set_Frame(int frame)
{
    Rect rect = Get_Rect();
    Frame = frame;
    if (rect != Get_Rect()) {
        HiddenSurface->Copy_From(rect, *AlternateSurface, rect, false, true);
    }
    if (Frame >= (unsigned)Shape->FrameCount) {
        Frame = Shape->FrameCount - 1;
    }
}


/// <summary>
/// Sets the first frame of the animation loop.
/// This is the frame a looping anim comes back to once it has run past its stop frame.
/// A frame beyond the end of the shape is clamped to the last one.
/// </summary>
/// <param name="frame">The frame the animation should start at.</param>
// (0x005AC810)

void MSShapeAnim::Set_Start_Frame(int frame)
{
    StartFrame = frame;
    if (StartFrame >= (unsigned)Shape->FrameCount) {
        StartFrame = Shape->FrameCount - 1;
    }
}


// void MSShapeAnim::Set_End_Frame(int frame) (0x005AC830)

void MSShapeAnim::Set_End_Frame(int frame)
{
    EndFrame = frame;
    if (EndFrame >= (unsigned)Shape->FrameCount) {
        EndFrame = Shape->FrameCount - 1;
    }
}


/// <summary>
/// Advances the shape anim by one frame.
/// The area the previous frame occupied is restored from the backdrop and the next frame
/// is drawn in its place. A non-looping anim reports itself finished once it runs past
/// its stop frame.
/// </summary>
/// <param name="rect">Filled in with the area disturbed, for the caller to update.</param>
/// <returns>bool; Has the anim finished and become ready for deletion?</returns>
bool MSShapeAnim::Advance(Surface *surface, Rect *rect)
{
    if (unknown_0C == true && Timer.Value()==0) {
        Timer=unknown_24;
        Rect draw_rect=Frame_Rect();
        draw_rect.X=X+draw_rect.X; draw_rect.Y=draw_rect.Y+Y;
        if (Flags & 0x200) { draw_rect.X-=Shape->Width/2; draw_rect.Y-=Shape->Height/2; }
        surface->Copy_From(draw_rect,*AlternateSurface,draw_rect,false,true);
        if (++Frame>EndFrame) {
            if (!unknown_34) { *rect=draw_rect; return true; }
            Frame=StartFrame;
        }
        CC_Draw_Shape(surface,Drawer,Shape,Frame,Point2D(X,Y),surface->Get_Rect(),Flags|0x400,0,0,0,1000,0,0,Point2D(0,0));
        *rect=Frame_Rect(); rect->X=rect->X+X; rect->Y=rect->Y+Y;
        if (Flags & 0x200) { rect->X-=Shape->Width/2; rect->Y-=Shape->Height/2; }
        *rect=Union(*rect,draw_rect);
        return false;
    }
    rect->Set(0,0,0,0);
    return false;
}


// void MSShapeAnim::Draw(Surface *surface, Rect const *clip) (0x005ACBB0, slot 5)
// Draws the current frame at X, Y unless clip is given and the frame's screen
// rectangle (Get_Rect's body, spelled out) lies outside it.

void MSShapeAnim::Draw(Surface *surface, Rect const *clip)
{
    Rect area(0, 0, 0, 0);
    if (clip != 0) {
        ShapeFileStruct *shape = Shape;
        area = shape->Get_Frame_Rect(Frame);
        area.X = X + area.X;
        area.Y = Y + area.Y;
        if (Flags & 0x200) {
            area.X -= shape->Width / 2;
            area.Y -= shape->Height / 2;
        }
        area = clip->Intersect(area);
        if (!area.Is_Valid()) {
            return;
        }
    }
    if (unknown_0C) {
        CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(), Flags | 0x400, 0, 0, 0, 1000,
                      0, 0, Point2D(0, 0));
    }
}


/// <summary>
/// Fetches the area the shape anim currently occupies.
/// </summary>
/// <returns>Returns with the bounding rectangle of the frame on display.</returns>
// (0x005ACD00)
// Screen rectangle of the current frame; centred on X/Y with flag 0x200.

Rect MSShapeAnim::Get_Rect()
{
    ShapeFileStruct *shape = Shape;
    Rect rect = shape->Get_Frame_Rect(Frame);
    rect.X = X + rect.X;
    rect.Y = Y + rect.Y;
    if (Flags & 0x200) {
        rect.X -= shape->Width / 2;
        rect.Y -= shape->Height / 2;
    }
    return rect;
}


/// <summary>
/// Creates a shape anim that fades itself in as it plays.
/// The shape is drawn through the translucency stages as the animation runs, so it
/// appears to materialize rather than simply arrive. Since it is see-through while it
/// fades, the siblings behind it have to be redrawn as it goes.
/// </summary>
/// <param name="name">The name of the shape file to animate.</param>
/// <param name="drawer">The drawer to render the shape with.</param>
/// <param name="rate">The delay in game frames between animation frames.</param>
/// <param name="flags">The shape drawing flags to render with.</param>
/// <param name="vector">The list of sibling anims to repair over this one.</param>
// (0x005ACDB0)
// MSFadeAnim::MSFadeAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24, int flags,
//     DynamicVectorClass<MSAnim *> *anims) (0x005ACDB0)

MSFadeAnim::MSFadeAnim(Wstring const &name, int x, int y, ConvertClass *drawer, int rate, int flags,
                       DynamicVectorClass<MSAnim *> *vector) :
    MSShapeAnim(name, x, y, drawer, rate, false, flags),
    unknown_40(0),
    Anims(vector)
{
}


/// <summary>
/// Advances the fading shape anim by one frame.
/// The area the previous frame occupied is restored from the backdrop and every sibling
/// anim overlapping it is asked to repair itself, before the next frame goes down at
/// whatever translucency the fade has reached.
/// </summary>
/// <param name="rect">Filled in with the area disturbed, for the caller to update.</param>
/// <returns>bool; Has the anim finished and become ready for deletion?</returns>
// (0x005ACE30, slot 4)
// Every unknown_24 ticks restores the old frame's area, redraws the other
// animations of Anims over it, steps the frame (true with an empty area when
// the last frame is passed and unknown_34 is clear) and the fade level, draws
// the new frame and reports the union of the old and new areas.

bool MSFadeAnim::Advance(Surface *surface, Rect *rect)
{
    if (unknown_0C == true && Timer.Value() == 0) {
        Timer = CDTimerClass<SystemTimerClass>(unknown_24);
        int flags;
        if ((unsigned)unknown_40 < 4) {
            flags = FadeStages[unknown_40] | Flags;
        } else {
            flags = Flags | FadeStages[3];
        }
        Rect old = Frame_Rect();
        old.X = X + old.X;
        old.Y = Y + old.Y;
        if (flags & 0x200) {
            old.X -= Shape->Width / 2;
            old.Y -= Shape->Height / 2;
        }
        surface->Copy_From(old, *AlternateSurface, old, false, true);
        if (Anims != 0) {
            int index = Anims->ID(this);
            for (int i = 0; i < Anims->Count(); i++) {
                if (i != index) {
                    (*Anims)[i]->Draw(surface, &old);
                }
            }
        }
        if (++Frame > EndFrame) {
            if (!unknown_34) {
                rect->Set(0, 0, 0, 0);
                return true;
            }
            Frame = StartFrame;
        }
        if ((unsigned)++unknown_40 < 4) {
            flags = FadeStages[unknown_40] | Flags;
        } else {
            flags = Flags | FadeStages[3];
        }
        CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(), flags, 0, 0, 0, 1000, 0, 0,
                      Point2D(0, 0));
        *rect = Frame_Rect();
        rect->X = X + rect->X;
        rect->Y = Y + rect->Y;
        if (flags & 0x200) {
            rect->X -= Shape->Width / 2;
            rect->Y -= Shape->Height / 2;
        }
        *rect = Union(*rect, old);
        return false;
    }
    rect->Set(0, 0, 0, 0);
    return false;
}


// void MSFadeAnim::Draw(Surface *surface, Rect const *clip) (0x005AD210, slot 5)
// As MSShapeAnim::Draw, with the translucency flags of fade level unknown_40
// (the last level past 3) and no unknown_0C test.

void MSFadeAnim::Draw(Surface *surface, Rect const *clip)
{
    Rect area(0, 0, 0, 0);
    if (clip != 0) {
        ShapeFileStruct *shape = Shape;
        area = shape->Get_Frame_Rect(Frame);
        area.X = X + area.X;
        area.Y = Y + area.Y;
        if (Flags & 0x200) {
            area.X -= shape->Width / 2;
            area.Y -= shape->Height / 2;
        }
        area = clip->Intersect(area);
        if (!area.Is_Valid()) {
            return;
        }
    }
    int flags;
    if ((unsigned)unknown_40 < 4) {
        flags = FadeStages[unknown_40] | Flags;
    } else {
        flags = Flags | FadeStages[3];
    }
    CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(), flags, 0, 0, 0, 1000, 0, 0,
                  Point2D(0, 0));
}


/// <summary>
/// Creates an anim that fades a single shape frame into place.
/// Unlike its fading parent the shape does not animate -- one frame is faded up and then
/// stamped onto the backdrop, so it becomes part of the scene rather than something drawn
/// over it every pass.
/// </summary>
/// <param name="name">The name of the shape file to draw from.</param>
/// <param name="drawer">The drawer to render the shape with.</param>
/// <param name="rate">The delay in game frames between fade stages.</param>
/// <param name="vector">The list of sibling anims to repair over this one.</param>
/// <param name="persistent">Should the anim stay alive once the fade is done?</param>
/// <param name="frame">The shape frame to fade into place.</param>
// (0x005AD370)
// MSOverlayAnim::MSOverlayAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24,
//     DynamicVectorClass<MSAnim *> *anims, bool unknown_48, int frame, bool unknown9) (0x005AD370)

MSOverlayAnim::MSOverlayAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24,
                             DynamicVectorClass<MSAnim *> *anims, bool unknown_48, int frame, bool unknown9) :
    MSFadeAnim(filename, x, y, drawer, unknown_24, 0, anims),
    unknown_48(unknown_48)
{
    Frame = frame;
    if (!unknown9) {
        unknown_40 = 4;
    }
}


// 0x005ad410: function not yet identified
//
// Owner: MSShapeAnim, stores the MSShapeAnim vtable (constructor or destructor) (from RTTI/vtables).
// Body: 51 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_005ad410(undefined4 *param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005ad410 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* MSAnim_VTable_007a6d3c;
extern "C" void* MSShapeAnim_VTable_007a6d64;

void __fastcall FUN_005ad410(unsigned int *object_words)
{
    *object_words = (unsigned int)(&MSShapeAnim_VTable_007a6d64);
    if ((object_words[7] != 0) && (*(char *)(object_words + 0xf) != '\0')) {
        operator_delete_00781d3b((unsigned int)(object_words[7]));
        object_words[7] = 0;
        *(unsigned char *)(object_words + 0xf) = 0;
    }
    *object_words = (unsigned int)(&MSAnim_VTable_007a6d3c);
    return;
}

}  // namespace seed_005ad410


/// <summary>
/// Advances the overlay's fade by one stage.
/// Each pass draws the frame a little more solidly and asks the sibling anims to repair
/// themselves over it. Once the fade is done the frame is stamped onto the backdrop, so
/// it survives every restore that follows.
/// </summary>
/// <param name="rect">Filled in with the area disturbed, for the caller to update.</param>
/// <returns>bool; Has the anim finished and become ready for deletion?</returns>
// (0x005AD450, slot 4)
// While the fade level is below 4, every unknown_24 ticks redraws the current
// frame at the next fade level over the restored background and the other
// animations of Anims, reporting its area; at level 4 the frame is also drawn
// into the background surface. Returns !unknown_48.

bool MSOverlayAnim::Advance(Surface *surface, Rect *rect)
{
    if (unknown_0C == true && Timer.Value() == 0) {
        Timer = CDTimerClass<SystemTimerClass>(unknown_24);
        if ((unsigned)unknown_40 < 4) {
            *rect = Frame_Rect();
            rect->X = X + rect->X;
            rect->Y = Y + rect->Y;
            if (Flags & 0x200) {
                rect->X -= Shape->Width / 2;
                rect->Y -= Shape->Height / 2;
            }
            surface->Copy_From(*rect, *AlternateSurface, *rect, false, true);
            unsigned level = (unsigned)unknown_40 < 3 ? unknown_40 : 3;
            CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(),
                          FadeStages[level] | Flags, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (Anims != 0) {
                int index = Anims->ID(this);
                for (int i = 0; i < Anims->Count(); i++) {
                    if (i != index) {
                        (*Anims)[i]->Draw(surface, rect);
                    }
                }
            }
            if ((unsigned)++unknown_40 >= 4) {
                CC_Draw_Shape(AlternateSurface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(),
                              Flags | 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            return !unknown_48;
        }
        return false;
    }
    rect->Set(0, 0, 0, 0);
    return false;
}


// void MSOverlayAnim::Draw(Surface *surface, Rect const *clip) (0x005AD6B0, slot 5)
// As MSFadeAnim::Draw while unknown_0C is set, the fade level capped at 3.

void MSOverlayAnim::Draw(Surface *surface, Rect const *clip)
{
    Rect area(0, 0, 0, 0);
    if (unknown_0C) {
        ShapeFileStruct *shape = Shape;
        area = shape->Get_Frame_Rect(Frame);
        area.X = X + area.X;
        area.Y = Y + area.Y;
        if (Flags & 0x200) {
            area.X -= shape->Width / 2;
            area.Y -= shape->Height / 2;
        }
        if (clip != 0) {
            area = clip->Intersect(area);
            if (!area.Is_Valid()) {
                return;
            }
        }
        unsigned level = (unsigned)unknown_40 < 3 ? unknown_40 : 3;
        CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(),
                      FadeStages[level] | Flags, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


/// <summary>
/// Restarts the overlay's fade from the beginning.
/// </summary>
// (0x005AD810)

void MSOverlayAnim::Restart()
{
    unknown_40 = 0;
}


/// <summary>
/// Sets the shape frame the overlay displays.
/// The fade starts over from the beginning, so the new frame appears the same way the
/// old one did.
/// </summary>
/// <param name="frame">The shape frame to fade into place, or a negative value to draw
/// nothing at all.</param>
// (0x005AD820)

void MSOverlayAnim::Set_Frame(unsigned frame)
{
    unknown_40 = 0;
    Frame = frame;
}


/// <summary>
/// Stamps the overlay onto the backdrop surface.
/// This routine is used to make the overlay part of the scene, so that whatever is drawn
/// over it afterwards can be repaired without losing it.
/// </summary>
// (0x005AD840, slot 8)
// Draws the current frame into the background surface at its fade level
// (while active).

void MSOverlayAnim::Restore(int rect)
{
    unsigned fading_stage = (unsigned)unknown_40 < 3 ? unknown_40 : 3;
    if (unknown_0C) {
        CC_Draw_Shape(AlternateSurface, Drawer, Shape, Frame, Point2D(X, Y), AlternateSurface->Get_Rect(),
                      FadeStages[fading_stage] | Flags, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


// 0x005ad8c0: function not yet identified
//
// Owner: MSOverlayAnim, virtual slot 7 (vtable +0x1c) (from RTTI/vtables).
// Body: 22 bytes (frozen inventory bound).
// Referenced from 1 site (data).
/// <summary>
/// Has the overlay finished fading into place?
/// </summary>
/// <returns>bool; Is the fade over, or is there nothing to fade?</returns>
// Ghidra prototype: undefined4 __fastcall Has_Finished(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005ad8c0 {

unsigned int __fastcall Has_Finished(int animation_address)
{
    if ((*(char *)(animation_address + 0xc) != '\0') && (*(unsigned int *)(animation_address + 0x40) < 4)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_005ad8c0


/// <summary>
/// Creates an anim that plays a Bink movie within the rectangle specified.
/// The movie is opened at once and placed at the top left corner of the rectangle. An
/// empty name gives an anim that plays nothing.
/// </summary>
/// <param name="name">The name of the movie file to play.</param>
/// <param name="surface">The surface the movie frames are drawn to.</param>
/// <param name="vector">The anim list whose other anims are redrawn over each new frame.</param>
/// <param name="rect">The screen area the movie occupies.</param>
/// <param name="held">Should the anim stay on the list once the movie has ended?</param>
// (0x005AD8E0)
// Opens the movie (when a name is given) and places it at the rectangle.

MSBinkAnim::MSBinkAnim(Wstring const &name, Surface *surface, DynamicVectorClass<MSAnim *> *anims,
                       Rect const &rect, bool unknown_38) :
    MSAnim(rect.X, rect.Y),
    Movie(0),
    Target(surface),
    MovieRect(rect),
    Anims(anims),
    unknown_38(unknown_38)
{
    if (name.Length() > 0) {
        Movie = new BinkClass(name.Peek_Buffer(), surface);
        Movie->Set_Position(rect.X, rect.Y);
    }
}


/// <summary>
/// Destroys the movie anim, closing the movie.
/// </summary>
// (0x005AD9A0)

MSBinkAnim::~MSBinkAnim()
{
    delete Movie;
}


/// <summary>
/// Fetches the screen rectangle this movie occupies.
/// </summary>
/// <returns>Returns with the bounding rectangle the movie was created with.</returns>
// (0x005AD9D0, slot 6)

Rect MSBinkAnim::Get_Rect()
{
    return MovieRect;
}


/// <summary>
/// Checks to see if the movie has run out of frames.
/// </summary>
/// <returns>bool; Has the movie ended (or was there no movie at all)?</returns>
// (0x005ADA00, slot 7)

bool MSBinkAnim::Has_Finished()
{
    return Movie == 0 || Movie->Is_Finished();
}


/// <summary>
/// Shows the next frame of the movie once it is due.
/// When a new frame has been drawn, the movie area is copied to the surface and the
/// other anims of the list are redrawn over it.
/// </summary>
// (0x005ADA20, slot 4)
// Shows each new movie frame: draws it, reports the movie rectangle and
// redraws every other animation of the list inside it. True once the movie
// finished unless unknown_38.

bool MSBinkAnim::Advance(Surface *surface, Rect *rect)
{
    if (surface == 0) {
        return false;
    }
    if (Movie == 0) {
        return true;
    }
    if (!Movie->Is_Finished() && Movie->Render_Frame()) {
        Draw(surface, 0);
        *rect = MovieRect;
        int my_id = Anims->ID(this);
        for (int i = 0; i < Anims->Count(); i++) {
            if (i != my_id) {
                (*Anims)[i]->Draw(surface, &MovieRect);
            }
        }
    }
    return Movie->Is_Finished() && !unknown_38;
}


// void MSBinkAnim::Draw(Surface *surface, Rect const *clip) (0x005ADAF0, slot 5)
// Copies the movie rectangle from the background surface onto `surface`.
// The overlap with `clip` is computed but not used.

void MSBinkAnim::Draw(Surface *surface, Rect const *clip)
{
    if (clip != 0) {
        Rect area = clip->Intersect(MovieRect);
    }
    surface->Copy_From(MovieRect, *AlternateSurface, MovieRect, false, true);
}


/// <summary>
/// Restores the movie area. Nothing need be done, as the next frame covers it anyway.
/// </summary>
// (0x005ADB90, slot 8)

void MSBinkAnim::Restore(int rect)
{
}


/// <summary>
/// Creates an anim that plays a VQA movie.
/// The movie is centered on the surface handed over and plays at the current sound
/// volume. A PCX picture sharing the movie's name is loaded alongside it where one
/// exists, and serves as the still image the movie leaves behind when it ends.
/// </summary>
/// <param name="name">The name of the movie file to play.</param>
/// <param name="surface">The surface the movie is played onto.</param>
/// <param name="vector">The list of sibling anims to repair over the movie.</param>
/// <param name="persistent">Should the anim stay alive after the movie has ended?</param>
// (0x005ADBA0)
// Opens the movie into `surface` (source centred, destination `dest`) and
// loads "<name up to '.'>.PCX" when present.

MSVQAnim::MSVQAnim(Wstring const &name, Surface *surface, DynamicVectorClass<MSAnim *> *anims, Rect const &dest,
                   bool unknown_2c) :
    Movie(0),
    Target(surface),
    Anims(anims),
    Image(0),
    unknown_2c(unknown_2c),
    Done(false)
{
    if (name != 0 && surface != 0) {
        Movie = Movie_Create(name.Peek_Buffer(), surface, Rect(0, 0, 0, 0), Rect(0, 0, 0, 0),
                                     (int)(unknown_volume_a40b44 * 255.0), 0);
        if (Movie != 0) {
            Movie->Source = Rect((surface->Get_Width() - VideoModeWidth) / 2,
                                 (surface->Get_Height() - VideoModeHeight) / 2, VideoModeWidth, VideoModeHeight);
            Movie->Dest = dest;
        }
        Wstring base;
        name.Get_Token(0, ".", base);
        CCFileClass file(base + ".PCX");
        if (file.Is_Available()) {
            Image = Read_PCX_File(&file, 0, 0, 0);
        }
    }
}


/// <summary>
/// RA2
/// </summary>
// (0x005ADD90)
// As 0x005ADBA0 for a 640x400 movie centred in `surface` and on the screen.

MSVQAnim::MSVQAnim(Wstring const &name, Surface *surface, DynamicVectorClass<MSAnim *> *anims, bool unknown_2c) :
    Movie(0),
    Target(surface),
    Anims(anims),
    Image(0),
    unknown_2c(unknown_2c),
    Done(false)
{
    if (name != 0 && surface != 0) {
        Movie = Movie_Create(name.Peek_Buffer(), surface, Rect(0, 0, 0, 0), Rect(0, 0, 0, 0),
                                     (int)(unknown_volume_a40b44 * 255.0), 0);
        if (Movie != 0) {
            Movie->Source = Rect((surface->Get_Width() - 640) / 2, (surface->Get_Height() - 400) / 2, 640, 400);
            Movie->Dest = Rect((HiddenSurface->Get_Width() - 640) / 2,
                               (HiddenSurface->Get_Height() - 400) / 2, 640, 400);
        }
        Wstring base;
        name.Get_Token(0, ".", base);
        CCFileClass file(base + ".PCX");
        if (file.Is_Available()) {
            Image = Read_PCX_File(&file, 0, 0, 0);
        }
    }
}


/// <summary>
/// Destroys the movie anim.
/// The movie is shut down and the still image loaded alongside it is released.
/// </summary>
// (0x005ADF90)

MSVQAnim::~MSVQAnim()
{
    delete Image;
    if (Movie != 0) {
        VQA_Movie_Close(Movie);
        delete Movie;
    }
}


/// <summary>
/// Advances the movie by one frame.
/// Every frame that arrives is put up on the surface and the sibling anims are asked to
/// repair themselves over it. When the movie runs out, the still image loaded alongside
/// it takes its place so the screen is not left blank.
/// </summary>
/// <param name="rect">Filled in with the area disturbed, for the caller to update.</param>
/// <returns>bool; Has the anim finished and become ready for deletion?</returns>
// (0x005ADFE0, slot 4)
// Shows each new movie frame and, when the movie ends, the picture; every
// other animation of the list is redrawn over the area. Without a movie the
// picture is shown once. True when done unless unknown_2c.

bool MSVQAnim::Advance(Surface *surface, Rect *dirty)
{
    int index;
    int i;
    if (Movie != 0) {
        bool finished = false;
        if (!Done) {
            if (Movie_Advance_Frame(Movie, &finished) == true) {
                Draw(surface, 0);
                *dirty = Movie->Dest;
                index = Anims->ID(this);
                for (i = 0; i < Anims->Count(); i++) {
                    if (i != index) {
                        (*Anims)[i]->Draw(surface, &Movie->Dest);
                    }
                }
            }
            if (finished == true && Image != 0) {
                Target->Copy_From(Movie->Source, *Image, Image->Get_Rect(), false, true);
                Draw(surface, 0);
                *dirty = Movie->Dest;
                index = Anims->ID(this);
                for (i = 0; i < Anims->Count(); i++) {
                    if (i != index) {
                        (*Anims)[i]->Draw(surface, &Movie->Dest);
                    }
                }
            }
            if (finished) {
                Done = finished;
            }
        }
    } else {
        if (!Done && Image != 0) {
            Rect rect = Image->Get_Rect();
            Target->Copy_From(rect, *Image, rect, false, true);
            Draw(surface, 0);
            *dirty = rect;
            index = Anims->ID(this);
            for (i = 0; i < Anims->Count(); i++) {
                if (i != index) {
                    (*Anims)[i]->Draw(surface, &rect);
                }
            }
        }
        Done = true;
    }
    return Done && !unknown_2c;
}


// void MSVQAnim::Draw(Surface *surface, Rect const *clip) (0x005AE230, slot 5)
// Copies the movie area from the background surface onto `surface` (while
// playing) unless it lies outside `clip`.

void MSVQAnim::Draw(Surface *surface, Rect const *clip)
{
    if (Movie != 0 && !Done) {
        if (clip != 0) {
            Rect area = clip->Intersect(Movie->Dest);
            if (!area.Is_Valid()) {
                return;
            }
        }
        surface->Copy_From(Movie->Source, *AlternateSurface, Movie->Dest, false, true);
    }
}


/// <summary>
/// Restores the still image the finished movie left behind.
/// This routine is used to put the movie's parting picture back onto the backdrop after
/// something else has drawn over it.
/// </summary>
// (0x005AE390, slot 8)
// Copies the picture into the target again once the movie is done.

void MSVQAnim::Restore(int rect)
{
    if (Done && Movie != 0 && Image != 0) {
        Target->Copy_From(Movie->Source, *Image, Image->Get_Rect(), false, true);
    }
}


/// <summary>
/// Fetches the area the movie occupies on the screen.
/// </summary>
/// <returns>Returns with the movie's screen rectangle. An empty rectangle is returned
/// when there is no movie to play.</returns>
// (0x005AE3E0, slot 6)
// The movie's destination rectangle, or an empty one without a movie.

Rect MSVQAnim::Get_Rect()
{
    static Rect empty(0, 0, 0, 0);
    return Movie != 0 ? Movie->Dest : empty;
}


// _$E (0x005AE460): the empty atexit thunk the compiler registers for the
// function-local static rectangle of MSVQAnim::Get_Rect (0x005AE3E0); no
// separate source text.


/// <summary>
/// Has the movie finished playing?
/// </summary>
/// <returns>bool; Has the movie run to its end?</returns>
// (0x005AE470, slot 7)

bool MSVQAnim::Has_Finished()
{
    return Done;
}


/// <summary>
/// RA2
/// </summary>
// (0x005AE480)
// Unicode text shown in a screen rectangle; positioned at the rectangle's corner.
// Slots 4 and 5 are near misses, not in this unit's plan yet:

MSBitPrintAnim::MSBitPrintAnim(WideString const &text, Rect const &rect, bool unknown_31) :
    MSAnim(rect.X, rect.Y),
    Text(text),
    TextRect(rect),
    unknown_30(false),
    unknown_31(unknown_31)
{
}


/// <summary>
/// RA2
/// </summary>
// (0x005AE4F0)

MSBitPrintAnim::~MSBitPrintAnim()
{
}


// void MSBitPrintAnim::Set_Unknown_30(bool value) (0x005AE510)

void MSBitPrintAnim::Set_Unknown_30(bool value)
{
    unknown_30 = value;
}


/// <summary>
/// RA2
/// </summary>
// (0x005AE520, slot 6)

Rect MSBitPrintAnim::Get_Rect()
{
    return TextRect;
}


/// <summary>
/// RA2
/// </summary>
// (0x005AE550, slot 7)

bool MSBitPrintAnim::Has_Finished()
{
    return unknown_30;
}


/// <summary>
/// RA2
/// </summary>
// (0x005AE560, slot 4)
// Prints the text clipped to TextRect with the bitmap font and reports
// TextRect as the area to refresh; done once unknown_30 is set.

bool MSBitPrintAnim::Advance(Surface *surface, Rect *rect)
{
    if (unknown_bit_printer_84e9d0_ptr != 0 && unknown_bit_font_84e9e8_ptr != 0) {
        RECT box;
        box.left = TextRect.X;
        box.top = TextRect.Y;
        box.right = TextRect.X + TextRect.Width - 1;
        box.bottom = TextRect.Height + TextRect.Y - 1;
        unknown_bit_font_84e9e8_ptr->Set_Clip_431460(&box);
        int unknown8 = unknown_31 != 0;
        unknown_bit_printer_84e9d0_ptr->Print_432490(unknown_bit_font_84e9e8_ptr, surface, Text.Get(), TextRect.X,
                                                     TextRect.Y, TextRect.Width, TextRect.Height, unknown8, 0, 0);
    }
    *rect = TextRect;
    return !unknown_30;
}


// void MSBitPrintAnim::Draw(Surface *surface, Rect const *clip) (0x005AE620, slot 5)
// Prints the text with the bitmap font, clipped to clip (TextRect when 0).

void MSBitPrintAnim::Draw(Surface *surface, Rect const *clip)
{
    if (unknown_bit_printer_84e9d0_ptr != 0 && unknown_bit_font_84e9e8_ptr != 0) {
        if (clip == 0) {
            clip = &TextRect;
        }
        RECT box;
        box.left = clip->X;
        box.top = clip->Y;
        box.right = clip->X + clip->Width - 1;
        box.bottom = clip->Height + clip->Y - 1;
        unknown_bit_font_84e9e8_ptr->Set_Clip_431460(&box);
        int unknown8 = unknown_31 != 0;
        unknown_bit_printer_84e9d0_ptr->Print_432490(unknown_bit_font_84e9e8_ptr, surface, Text.Get(), TextRect.X,
                                                     TextRect.Y, TextRect.Width, TextRect.Height, unknown8, 0, 0);
    }
}


/// <summary>
/// Creates a text printing anim from a string table entry.
/// The text is typed out a character at a time within the area specified. Use this
/// version when the words come from the game's string table rather than from code.
/// </summary>
/// <param name="text">The string table identifier of the text to print.</param>
/// <param name="font">The font to print the text with.</param>
/// <param name="rect">The area to center and clip the text within.</param>
/// <param name="start_delay">The delay in game frames before printing begins.</param>
/// <param name="print_delay">The delay in game frames between characters.</param>
/// <param name="fade_effect">Should the characters fade in as they are printed?</param>
/// <param name="center">Should the text be centered within its area?</param>
// (0x005AE6C0)
// MSPrintAnim::MSPrintAnim(WideString const &text, int x, int y, MSFont *font, Rect const &rect,
//     unsigned position, int char_delay, bool unknown_30, bool centered) (0x005AE6C0)
// Fits TextRect to the text in rect and, when centred, works out the first
// line's centring offset.

MSPrintAnim::MSPrintAnim(WideString const &text, int x, int y, MSFont *font, Rect const &rect, unsigned position,
                         int char_delay, bool unknown_30, bool centered) :
    MSAnim(x, y),
    Text(text),
    LineStart(0),
    Position(position),
    unknown_28(0),
    CharDelay(char_delay),
    unknown_30(unknown_30),
    Font(font),
    unknown_48(false),
    Centered(centered),
    unknown_4C(0)
{
    Timer = CDTimerClass<SystemTimerClass>(position);
    Set_Rect(rect);
    if (centered) {
        int spacing = Font->Width;
        int width = Line_Width(Text);
        unknown_4C = (TextRect.Width - width - spacing) / 2;
    }
}


void MSPrintAnim::Set_Rect(Rect const &rect)
{
    if (rect.Is_Valid()) {
        Measure_Text();
        X += (rect.Width - TextRect.Width) / 2;
        int line_height=Font->Height;
        Y += (rect.Height - (rect.Height / line_height) * line_height) / 2;
        TextRect.X += X;
        TextRect.Y += Y;
        TextRect = rect.Intersect(TextRect);
        X += Font->Width / 2;
    } else {
        Measure_Text();
        TextRect.X = X;
        TextRect.Y = Y;
    }
}


// int __fastcall unknown_wrap_text_5ae9a0(WideString &text, MSFont *font, int width) (0x005AE9A0)
// Word wrap: each line ends at its last space, which becomes '\n'; a line
// without one is cut after a '-' or where it runs out of room (a '\n' is
// inserted). Returns the widest line.

int __fastcall unknown_wrap_text_5ae9a0(WideString &text, MSFont *font, int width)
{
    int widest = 0;
    if (text == 0 || font == 0 || width <= font->Width) {
        return 0;
    }
    width -= font->Width;
    unsigned i = 0;
    while (text.Get_Char(i) != 0) {
        unsigned start = i;
        int line_width = 0;
        while (line_width < width && text.Get_Char(i) != 0 && text.Get_Char(i) != '\n' && text.Get_Char(i) != '@') {
            line_width += font->Char_Width(text.Get_Char(i++));
        }
        if (line_width >= width) {
            while (text.Get_Char(i) != 0 && text.Get_Char(i) != ' ' && text.Get_Char(i) != '\n') {
                if (i == start) {
                    break;
                }
                line_width -= font->Char_Width(text.Get_Char(--i));
            }
        }
        if (i == start) {
            while (line_width < width && text.Get_Char(i) != 0 && text.Get_Char(i) != ' ' && text.Get_Char(i) != '\n'
                   && text.Get_Char(i) != '@' && text.Get_Char(i) != '-') {
                line_width += font->Char_Width(text.Get_Char(i++));
            }
            if (line_width < width) {
                WideString first(text);
                first.Truncate(++i);
                WideString rest(text);
                rest.Remove(0, i);
                text = first + WideString(L"\n") + rest;
            } else {
                line_width -= font->Char_Width(text.Get_Char(i));
                WideString first(text);
                first.Truncate(i);
                WideString rest(text);
                rest.Remove(0, i);
                text = first + WideString(L"\n") + rest;
            }
        }
        if (line_width > widest) {
            widest = line_width;
        }
        if (text.Get_Char(i) != 0) {
            text.Set_Char('\n', i++);
        }
    }
    return widest;
}


// int __fastcall unknown_paginate_text_5aece0(WideString &text, MSFont *font, int height) (0x005AECE0)
// Counts the pages of already wrapped text: a running total of line heights that reaches height starts a new
// page, marked with a form feed (0x0C) in the text. As in the original, the position only advances past a line's
// end when that end is not '\n' (name and meaning are hypotheses).

int __fastcall unknown_paginate_text_5aece0(WideString &text, MSFont *font, int height)
{
    if (text == 0 || font == 0 || height < font->Height) {
        return 0;
    }
    unsigned i = 0;
    int pages = 0;
    int y = 0;
    while (text.Get_Char(i) != 0) {
        while (text.Get_Char(i) != 0 && text.Get_Char(i) != '\n') {
            i++;
        }
        if (text.Get_Char(i) != '\n') {
            y += font->Height;
            if (y >= height) {
                y = 0;
                pages++;
                text.Set_Char(0x0C, i);
            }
            i++;
        }
    }
    return pages;
}


/// <summary>
/// Types out one more character of the text.
/// This routine prints the string a character at a time, trailing a short fade behind
/// the newest ones so the text appears to arrive over a teletype. When the text runs
/// past the bottom of its rectangle the topmost line is dropped and printing carries
/// on below.
/// </summary>
/// <param name="rect">Filled in with the region the text occupies.</param>
/// <returns>bool; Should this anim be removed from the list?</returns>
// (0x005AEDA0, slot 4)
// Types the next character once the per-character delay has run out: redraws
// the visible lines up to it (centring each line when Centered), scrolls to
// the next line when the text runs past the bottom of TextRect, and reports
// TextRect as the area to refresh. Once the text is finished it returns
// !unknown_48 after three more calls.

bool MSPrintAnim::Advance(Surface *surface, Rect *rect)
{
    rect->X = 0;
    rect->Y = 0;
    rect->Width = 0;
    rect->Height = 0;
    if (Timer.Value() != 0) {
        return false;
    }
    if (unknown_0C == true) {
        Timer = CDTimerClass<SystemTimerClass>(CharDelay);
        if (Position > Text.Length() && unknown_28 > 2) {
            return !unknown_48;
        }
        if (unknown_30 == true) {
            surface->Copy_From(TextRect, *AlternateSurface, TextRect, false, true);
        }
        int x = X;
        bool centered = Centered;
        int y = Y;
        for (unsigned index = LineStart; index <= Position; index++) {
            unsigned short ch = Text.unknown_char_770400(index);
            if (ch == 0) {
                unknown_28++;
                break;
            }
            if (centered) {
                WideString line(Text);
                line.Remove(0, index);
                int spacing = Font->Width;
                int width = Line_Width(line);
                int offset = (TextRect.Width - width - spacing) / 2;
                centered = false;
                x += offset;
                unknown_4C = offset;
            }
            if (ch == '\n') {
                x = X;
                centered = Centered;
                int height = Font->Height;
                y += height;
                if (height - Y + y > TextRect.Height) {
                    unsigned next = LineStart;
                    while (Text.Get_Char(next) != 0 && Text.Get_Char(next) != '\n') {
                        next++;
                    }
                    if (Text.Get_Char(next) == '\n') {
                        next++;
                    }
                    LineStart = next;
                    break;
                }
                continue;
            }
            unsigned style = (unknown_30 && Position - index < 2) ? Position - index : 2;
            Font->Print_Char(surface, ch, x, y, style, 1);
            x += Font->Char_Width(ch);
        }
        *rect = TextRect;
        Position++;
    }
    return false;
}


// void MSPrintAnim::Draw(Surface *surface, Rect const *clip) (0x005AF030, slot 5)
// Draws the typed characters from LineStart on (unless TextRect lies outside
// clip), stopping at the bottom of the surface.
// The inline ::Intersect (rect_intersect.hpp, visible from MSBinkAnim::Draw on)
// stays out of line here (0x0041FDF0) because the Line_Width site later in the
// body is still pending when the nested site is decided (depth-first rule,
// docs/VC6_MATCHING_NOTES.md).

void MSPrintAnim::Draw(Surface *surface, Rect const *clip)
{
    if (clip != 0) {
        Rect area = clip->Intersect(TextRect);
        if (!area.Is_Valid()) {
            return;
        }
    }
    int x = X;
    bool centered = Centered;
    int y = Y;
    unsigned end = (Position < Text.Length()) ? Position : Text.Length();
    for (unsigned index = LineStart; index < end; index++) {
        if (Text.Get_Char(index) == '\n') {
            x = X;
            centered = Centered;
            int height = Font->Height;
            y += height;
            if (height + y >= surface->Get_Height()) {
                return;
            }
        } else {
            if (centered) {
                WideString line(Text);
                line.Remove(0, index);
                int spacing = Font->Width;
                int width = Line_Width(line);
                int offset = (TextRect.Width - width - spacing) / 2;
                centered = false;
                x += offset;
                unknown_4C = offset;
            }
            Font->Print_Char(surface, Text.Get_Char(index), x, y, 2, 1);
            x += Font->Char_Width(Text.Get_Char(index));
        }
    }
}


// bool MSPrintAnim::Unknown_5af210(Surface *surface, Rect *dirty) (0x005AF210)
// Slot 4's line-at-a-time variant (no caller, not in the vtable): every
// CharDelay ticks draws the line from LineStart at unknown_50 in fade step
// unknown_24 (0..2, stepping while unknown_30) and reports its area; after the
// last step moves to the next line with a "bleep1". True once the text is done.

bool MSPrintAnim::Unknown_5af210(Surface *surface, Rect *dirty)
{
    if (Timer.Value() == 0 && unknown_0C == true) {
        Timer = CDTimerClass<SystemTimerClass>(CharDelay);
        if (LineStart >= Text.Length()) {
            Audio_Event_Play_Named("bleep1", unknown_volume_a40b44 * 64.0f);
            return true;
        }
        if (unknown_30 == true && unknown_54.Is_Valid()) {
            surface->Copy_From(unknown_54, *AlternateSurface, unknown_54, false, true);
        }
        int x = X;
        if (Centered) {
            x += unknown_4C;
        }
        unsigned index = LineStart;
        unsigned short ch = Text.Get_Char(index++);
        while (ch != '\n' && ch != 0) {
            Font->Print_Char(surface, ch, x, unknown_50, Position, 0);
            x += Font->Char_Width(ch);
            ch = Text.Get_Char(index++);
        }
        dirty->Set(TextRect.X, unknown_50, TextRect.Width, Font->Height);
        if (Position == 2) {
            unknown_54.Set(0, 0, 0, 0);
            LineStart = index;
            if (ch == '\n') {
                Audio_Event_Play_Named("bleep1", unknown_volume_a40b44 * 64.0f);
                unknown_50 += Font->Height;
            }
        } else {
            unknown_54 = *dirty;
        }
        if (unknown_30 == true) {
            if (++Position > 2) {
                Position = 0;
            }
        }
        return false;
    }
    dirty->Set(0, 0, 0, 0);
    return false;
}


/// <summary>
/// Fetches the screen rectangle the text prints within.
/// </summary>
/// <returns>Returns with the bounding rectangle the text was fitted to.</returns>
// (0x005AF400, slot 6)

Rect MSPrintAnim::Get_Rect()
{
    return TextRect;
}


/// <summary>
/// Sets whether the text is a transient message.
/// A transient anim reports itself finished once its string has been printed, which
/// lets the owning screen decide when to retire it.
/// </summary>
// (0x005AF430)

void MSPrintAnim::Set_Transient(bool transient)
{
    if (transient != unknown_48) {
        unknown_48 = transient;
    }
}


/// <summary>
/// Has the text finished printing itself?
/// Only a transient anim ever reports itself finished, and only once the whole string
/// has been typed out and the trailing fade has run its course.
/// </summary>
/// <returns>bool; Is the anim done with the screen?</returns>
// (0x005AF450, slot 7)

bool MSPrintAnim::Has_Finished()
{
    return unknown_48 && Position > Text.Length() && unknown_28 > 2;
}


int MSPrintAnim::Line_Width(WideString const &line)
{
    int i = 0;
    int width = 0;
    while (line.Get_Char(i) != 0 && line.Get_Char(i) != '\n') {
        width += Font->Char_Width(line.Get_Char(i));
        i++;
    }
    return width;
}


void MSPrintAnim::Draw_Completed_005af4f0(Surface *surface, Rect const *clip)
{
    if (clip != 0) {
        Rect area=clip->Intersect(TextRect);
        if (!area.Is_Valid()) return;
    }
    int x=X;
    int y=Y;
    unsigned end=LineStart;
    for (unsigned index=0;index<end;++index) {
        if (Text.Get_Char(index)=='\n') {
            x=X;
            int height=Font->Get_Font_Height();
            y+=height;
            if (y+height>=surface->Get_Height()) return;
        } else {
            Font->Print_Char(surface,Text.Get_Char(index),x,y,2,1);
            x+=Font->Char_Width(Text.Get_Char(index));
        }
    }
}


/// <summary>
/// Creates a PCX image anim centered on the screen.
/// The artwork is loaded from the file matching the name given and placed in the
/// middle of the alternate surface.
/// </summary>
/// <param name="name">Name of the artwork file; the extension is replaced with ".PCX".</param>
/// <param name="vector">The anim list this anim belongs to.</param>
// (0x005AF5F0)
// Loads "<name up to the first '.'>.PCX" and centres it on the background surface.

MSPCXAnim::MSPCXAnim(Wstring const &name, DynamicVectorClass<MSAnim *> *anims, bool unknown_28) :
    Background(AlternateSurface),
    Anims(anims),
    Image(0),
    unknown_28(unknown_28),
    Done(false),
    PCXRect(0, 0, 0, 0)
{
    unknown_0C = true;
    if (name != 0) {
        Wstring base;
        name.Get_Token(0, ".", base);
        CCFileClass file(base + ".PCX");
        if (file.Is_Available()) {
            Image = Read_PCX_File(&file, 0, 0, 0);
            if (Image != 0) {
                PCXRect = Image->Get_Rect();
                Rect screen = Background->Get_Rect();
                PCXRect.X += (screen.Width - PCXRect.Width) / 2;
                PCXRect.Y += (screen.Height - PCXRect.Height) / 2;
            }
        }
    }
}


/// <summary>
/// Creates a PCX image anim at an explicit screen position.
/// The artwork is loaded from the file matching the name given. Use this constructor
/// when the image must land somewhere other than the middle of the screen.
/// </summary>
/// <param name="name">Name of the artwork file; the extension is replaced with ".PCX".</param>
/// <param name="vector">The anim list this anim belongs to.</param>
// (0x005AF790)
// Loads "<name up to the first '.'>.PCX" and places it at `position`.
// Assumption: the trailing Background->Get_Rect() whose result is unused is
// in the original (the virtual call is made).

MSPCXAnim::MSPCXAnim(Wstring const &name, DynamicVectorClass<MSAnim *> *anims, Point2D const &position,
                     bool unknown_28) :
    Background(AlternateSurface),
    Anims(anims),
    Image(0),
    unknown_28(unknown_28),
    Done(false),
    PCXRect(0, 0, 0, 0)
{
    unknown_0C = true;
    if (name != 0) {
        Wstring base;
        name.Get_Token(0, ".", base);
        CCFileClass file(base + ".PCX");
        if (file.Is_Available()) {
            Image = Read_PCX_File(&file, 0, 0, 0);
            if (Image != 0) {
                PCXRect = Image->Get_Rect();
                PCXRect.X = position.X;
                PCXRect.Y = position.Y;
                Background->Get_Rect();
            }
        }
    }
}


/// <summary>
/// Destroys the anim and frees the image it loaded.
/// </summary>
// (0x005AF8F0)

MSPCXAnim::~MSPCXAnim()
{
    delete Image;
}


/// <summary>
/// Brings up the next line of text.
/// Where the base class types the string out a character at a time, this routine puts
/// up a whole line at once and fades it in. A bleep sounds as each line lands, and
/// again when the last of the string has been printed.
/// </summary>
/// <param name="rect">Filled in with the region the current line occupies.</param>
/// <returns>bool; Has the whole string been printed?</returns>
// (0x005AF910, slot 4)
// The first time (while active): copies the picture into the background,
// draws it onto `surface`, reports its rectangle and redraws every other
// animation of the list inside it. True once shown unless unknown_28.

bool MSPCXAnim::Advance(Surface *surface, Rect *rect)
{
    if (!Done) {
        if (unknown_0C) {
            if (Image != 0) {
                Background->Copy_From(PCXRect, *Image, Image->Get_Rect(), false, true);
                Draw(surface, 0);
                Rect rect2 = PCXRect;
                *rect = rect2;
                int my_id = Anims->ID(this);
                for (int i = 0; i < Anims->Count(); i++) {
                    if (i != my_id) {
                        (*Anims)[i]->Draw(surface, &rect2);
                    }
                }
            }
            Done = true;
        }
    }
    return Done && !unknown_28;
}


// void MSPCXAnim::Draw(Surface *surface, Rect const *clip) (0x005AFA00, slot 5)
// Copies the picture onto `surface` (while active and not yet shown) unless
// it lies outside `clip`.

void MSPCXAnim::Draw(Surface *surface, Rect const *clip)
{
    if (Image != 0 && !Done && unknown_0C) {
        if (clip != 0) {
            Rect area = clip->Intersect(PCXRect);
            if (!area.Is_Valid()) {
                return;
            }
        }
        surface->Copy_From(PCXRect, *Image, Image->Get_Rect(), false, true);
    }
}


/// <summary>
/// Restores the PCX image to the backing surface.
/// This routine is called when something else has covered the artwork and the screen
/// needs it put back.
/// </summary>
// (0x005AFB80, slot 8)
// Copies the picture into the background again (while active).

void MSPCXAnim::Restore(int unknown1)
{
    if (Image != 0 && unknown_0C) {
        Background->Copy_From(PCXRect, *Image, Image->Get_Rect(), false, true);
    }
}


/// <summary>
/// Fetches the screen rectangle this image occupies.
/// </summary>
/// <returns>Returns with the destination rectangle of the PCX image.</returns>
// (0x005AFBC0, slot 6)

Rect MSPCXAnim::Get_Rect()
{
    return PCXRect;
}


/// <summary>
/// Determines if the image has been put up on the screen.
/// </summary>
/// <returns>bool; Has the image been drawn?</returns>
// (0x005AFBF0, slot 7)

bool MSPCXAnim::Has_Finished()
{
    return Done;
}


// void MSAnim::Set_Unknown_0C(bool value) (0x005AFC00), inline in the header


// void MSAnim::Pause() (0x005AFC10): stops the countdown timer; inline in the header


// void MSAnim::Resume() (0x005AFC60): restarts the countdown timer; inline in the header


// bool MSAnim::Has_Finished() (0x005AFC80): false; inline in the header


// void MSAnim::Restore(int) (0x005AFC90): empty; inline in the header


// MSAnim scalar deleting destructor (0x005AFCA0), compiler generated


// MSShapeAnim scalar deleting destructor (0x005AFCC0), compiler generated (destructor inlined)


// MSFadeAnim scalar deleting destructor (0x005AFD10), compiler generated (implicit destructor)


// MSOverlayAnim scalar deleting destructor (0x005AFD60), compiler generated (implicit destructor)


// MSBinkAnim scalar deleting destructor (0x005AFDB0): compiler-generated
// from the virtual destructor; no source of its own.


// MSVQAnim scalar deleting destructor (0x005AFDF0): compiler-generated
// from the virtual destructor; no source of its own.


// MSBitPrintAnim scalar deleting destructor (0x005AFE50): compiler-generated
// from the virtual destructor; no source of its own.


// MSPrintAnim scalar deleting destructor (0x005AFE80): compiler-generated
// from the implicit virtual destructor; no source of its own.


// MSPCXAnim scalar deleting destructor (0x005AFEB0): compiler-generated
// from the virtual destructor; no source of its own.
