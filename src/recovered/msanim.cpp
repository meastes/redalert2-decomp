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


// ConvertClass *__fastcall unknown_load_drawer_5ac560(Wstring const &filename) (0x005AC560)
// Reads a 256-colour palette (6-bit components) into 0x00838710 and returns
// a new drawer for it; null without a name, file or data.

ConvertClass *__fastcall unknown_load_drawer_5ac560(Wstring const &filename)
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
        unknown_palette_838710[index] = RGBClass(palette[index * 3] << 2, palette[index * 3 + 1] << 2,
                                                 palette[index * 3 + 2] << 2);
    }
    operator delete(palette);
    return new ConvertClass(unknown_palette_838710, unknown_palette_838710, unknown_surface_839950, 1, false);
}


// MSAnim::~MSAnim() (0x005AC690)
// Out-of-line copy of the inline virtual destructor (stores the vtable).

MSAnim::~MSAnim()
{
}


// MSShapeAnim::MSShapeAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24, bool unknown_34, int flags) (0x005AC6A0)
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


// MSShapeAnim::~MSShapeAnim() (0x005AC720)
// Releases a shape this animation loaded itself.

MSShapeAnim::~MSShapeAnim()
{
    if (Shape != 0 && OwnsShape) {
        operator delete(Shape);
        Shape = 0;
        OwnsShape = false;
    }
}


// void MSShapeAnim::Set_Frame(int frame) (0x005AC760)
// Changes the frame, reports the old rectangle as dirty when the new one
// differs, and clamps to the last frame.

void MSShapeAnim::Set_Frame(int frame)
{
    Rect old = Get_Rect();
    Frame = frame;
    if (old != Get_Rect()) {
        unknown_surface_839954->Copy_From(old, *unknown_surface_839958, old, false, true);
    }
    if (Frame >= (unsigned)Shape->FrameCount) {
        Frame = Shape->FrameCount - 1;
    }
}


// void MSShapeAnim::Set_Start_Frame(int frame) (0x005AC810)

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


bool MSShapeAnim::Advance(Surface *surface, Rect *dirty)
{
    if (unknown_0C == true && Timer.Value()==0) {
        Timer=unknown_24;
        Rect old=Frame_Rect();
        old.X=X+old.X; old.Y=old.Y+Y;
        if (Flags & 0x200) { old.X-=Shape->Width/2; old.Y-=Shape->Height/2; }
        surface->Copy_From(old,*unknown_surface_839958,old,false,true);
        if (++Frame>EndFrame) {
            if (!unknown_34) { *dirty=old; return true; }
            Frame=StartFrame;
        }
        CC_Draw_Shape(surface,Drawer,Shape,Frame,Point2D(X,Y),surface->Get_Rect(),Flags|0x400,0,0,0,1000,0,0,Point2D(0,0));
        *dirty=Frame_Rect(); dirty->X=dirty->X+X; dirty->Y=dirty->Y+Y;
        if (Flags & 0x200) { dirty->X-=Shape->Width/2; dirty->Y-=Shape->Height/2; }
        *dirty=Union(*dirty,old);
        return false;
    }
    dirty->Set(0,0,0,0);
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


// Rect MSShapeAnim::Get_Rect() (0x005ACD00)
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


// MSFadeAnim::MSFadeAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24, int flags,
//     DynamicVectorClass<MSAnim *> *anims) (0x005ACDB0)

MSFadeAnim::MSFadeAnim(Wstring const &filename, int x, int y, ConvertClass *drawer, int unknown_24, int flags,
                       DynamicVectorClass<MSAnim *> *anims) :
    MSShapeAnim(filename, x, y, drawer, unknown_24, false, flags),
    unknown_40(0),
    Anims(anims)
{
}


// bool MSFadeAnim::Advance(Surface *surface, Rect *dirty) (0x005ACE30, slot 4)
// Every unknown_24 ticks restores the old frame's area, redraws the other
// animations of Anims over it, steps the frame (true with an empty area when
// the last frame is passed and unknown_34 is clear) and the fade level, draws
// the new frame and reports the union of the old and new areas.

bool MSFadeAnim::Advance(Surface *surface, Rect *dirty)
{
    if (unknown_0C == true && Timer.Value() == 0) {
        Timer = CDTimerClass<SystemTimerClass>(unknown_24);
        int flags;
        if ((unsigned)unknown_40 < 4) {
            flags = unknown_fade_flags_7e57f0[unknown_40] | Flags;
        } else {
            flags = Flags | unknown_fade_flags_7e57f0[3];
        }
        Rect old = Frame_Rect();
        old.X = X + old.X;
        old.Y = Y + old.Y;
        if (flags & 0x200) {
            old.X -= Shape->Width / 2;
            old.Y -= Shape->Height / 2;
        }
        surface->Copy_From(old, *unknown_surface_839958, old, false, true);
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
                dirty->Set(0, 0, 0, 0);
                return true;
            }
            Frame = StartFrame;
        }
        if ((unsigned)++unknown_40 < 4) {
            flags = unknown_fade_flags_7e57f0[unknown_40] | Flags;
        } else {
            flags = Flags | unknown_fade_flags_7e57f0[3];
        }
        CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(), flags, 0, 0, 0, 1000, 0, 0,
                      Point2D(0, 0));
        *dirty = Frame_Rect();
        dirty->X = X + dirty->X;
        dirty->Y = Y + dirty->Y;
        if (flags & 0x200) {
            dirty->X -= Shape->Width / 2;
            dirty->Y -= Shape->Height / 2;
        }
        *dirty = Union(*dirty, old);
        return false;
    }
    dirty->Set(0, 0, 0, 0);
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
        flags = unknown_fade_flags_7e57f0[unknown_40] | Flags;
    } else {
        flags = Flags | unknown_fade_flags_7e57f0[3];
    }
    CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(), flags, 0, 0, 0, 1000, 0, 0,
                  Point2D(0, 0));
}


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
extern "C" void* PTR_FUN_007a6d3c;
extern "C" void* PTR_FUN_007a6d64;

void __fastcall FUN_005ad410(unsigned int *object_words)
{
    *object_words = (unsigned int)(&PTR_FUN_007a6d64);
    if ((object_words[7] != 0) && (*(char *)(object_words + 0xf) != '\0')) {
        operator_delete_00781d3b((unsigned int)(object_words[7]));
        object_words[7] = 0;
        *(unsigned char *)(object_words + 0xf) = 0;
    }
    *object_words = (unsigned int)(&PTR_FUN_007a6d3c);
    return;
}

}  // namespace seed_005ad410


// bool MSOverlayAnim::Advance(Surface *surface, Rect *dirty) (0x005AD450, slot 4)
// While the fade level is below 4, every unknown_24 ticks redraws the current
// frame at the next fade level over the restored background and the other
// animations of Anims, reporting its area; at level 4 the frame is also drawn
// into the background surface. Returns !unknown_48.

bool MSOverlayAnim::Advance(Surface *surface, Rect *dirty)
{
    if (unknown_0C == true && Timer.Value() == 0) {
        Timer = CDTimerClass<SystemTimerClass>(unknown_24);
        if ((unsigned)unknown_40 < 4) {
            *dirty = Frame_Rect();
            dirty->X = X + dirty->X;
            dirty->Y = Y + dirty->Y;
            if (Flags & 0x200) {
                dirty->X -= Shape->Width / 2;
                dirty->Y -= Shape->Height / 2;
            }
            surface->Copy_From(*dirty, *unknown_surface_839958, *dirty, false, true);
            unsigned level = (unsigned)unknown_40 < 3 ? unknown_40 : 3;
            CC_Draw_Shape(surface, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(),
                          unknown_fade_flags_7e57f0[level] | Flags, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            if (Anims != 0) {
                int index = Anims->ID(this);
                for (int i = 0; i < Anims->Count(); i++) {
                    if (i != index) {
                        (*Anims)[i]->Draw(surface, dirty);
                    }
                }
            }
            if ((unsigned)++unknown_40 >= 4) {
                CC_Draw_Shape(unknown_surface_839958, Drawer, Shape, Frame, Point2D(X, Y), surface->Get_Rect(),
                              Flags | 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            return !unknown_48;
        }
        return false;
    }
    dirty->Set(0, 0, 0, 0);
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
                      unknown_fade_flags_7e57f0[level] | Flags, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


// void MSOverlayAnim::Restart() (0x005AD810): restarts the fade at the current frame.

void MSOverlayAnim::Restart()
{
    unknown_40 = 0;
}


// void MSOverlayAnim::Set_Frame(unsigned frame) (0x005AD820): restarts the fade at a new frame.

void MSOverlayAnim::Set_Frame(unsigned frame)
{
    unknown_40 = 0;
    Frame = frame;
}


// void MSOverlayAnim::Restore(int unknown1) (0x005AD840, slot 8)
// Draws the current frame into the background surface at its fade level
// (while active).

void MSOverlayAnim::Restore(int unknown1)
{
    unsigned level = (unsigned)unknown_40 < 3 ? unknown_40 : 3;
    if (unknown_0C) {
        CC_Draw_Shape(unknown_surface_839958, Drawer, Shape, Frame, Point2D(X, Y), unknown_surface_839958->Get_Rect(),
                      unknown_fade_flags_7e57f0[level] | Flags, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
}


// 0x005ad8c0: function not yet identified
//
// Owner: MSOverlayAnim, virtual slot 7 (vtable +0x1c) (from RTTI/vtables).
// Body: 22 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __fastcall FUN_005ad8c0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005ad8c0 {

unsigned int __fastcall FUN_005ad8c0(int animation_address)
{
    if ((*(char *)(animation_address + 0xc) != '\0') && (*(unsigned int *)(animation_address + 0x40) < 4)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_005ad8c0


// MSBinkAnim::MSBinkAnim(Wstring const &name, Surface *surface, DynamicVectorClass<MSAnim *> *anims, Rect const &rect, bool unknown_38) (0x005AD8E0)
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


// MSBinkAnim::~MSBinkAnim() (0x005AD9A0)

MSBinkAnim::~MSBinkAnim()
{
    delete Movie;
}


// Rect MSBinkAnim::Get_Rect() (0x005AD9D0, slot 6)

Rect MSBinkAnim::Get_Rect()
{
    return MovieRect;
}


// bool MSBinkAnim::Has_Finished() (0x005ADA00, slot 7): no movie, or it finished.

bool MSBinkAnim::Has_Finished()
{
    return Movie == 0 || Movie->Is_Finished();
}


// bool MSBinkAnim::Advance(Surface *surface, Rect *dirty) (0x005ADA20, slot 4)
// Shows each new movie frame: draws it, reports the movie rectangle and
// redraws every other animation of the list inside it. True once the movie
// finished unless unknown_38.

bool MSBinkAnim::Advance(Surface *surface, Rect *dirty)
{
    if (surface == 0) {
        return false;
    }
    if (Movie == 0) {
        return true;
    }
    if (!Movie->Is_Finished() && Movie->Render_Frame()) {
        Draw(surface, 0);
        *dirty = MovieRect;
        int index = Anims->ID(this);
        for (int i = 0; i < Anims->Count(); i++) {
            if (i != index) {
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
    surface->Copy_From(MovieRect, *unknown_surface_839958, MovieRect, false, true);
}


// void MSBinkAnim::Restore(int unknown1) (0x005ADB90, slot 8): nothing.

void MSBinkAnim::Restore(int unknown1)
{
}


// MSVQAnim::MSVQAnim(Wstring const &name, Surface *surface, DynamicVectorClass<MSAnim *> *anims, Rect const &dest, bool unknown_2c) (0x005ADBA0)
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
        Movie = VQA_Unknown_005A1010(name.Peek_Buffer(), surface, Rect(0, 0, 0, 0), Rect(0, 0, 0, 0),
                                     (int)(unknown_volume_a40b44 * 255.0), 0);
        if (Movie != 0) {
            Movie->Source = Rect((surface->Get_Width() - unknown_8523e0) / 2,
                                 (surface->Get_Height() - unknown_8523e4) / 2, unknown_8523e0, unknown_8523e4);
            Movie->Dest = dest;
        }
        Wstring base;
        name.Get_Token(0, ".", base);
        CCFileClass file(base + ".PCX");
        if (file.Is_Available()) {
            Image = unknown_load_pcx_60d110(&file, 0, 0, 0);
        }
    }
}


// MSVQAnim::MSVQAnim(Wstring const &name, Surface *surface, DynamicVectorClass<MSAnim *> *anims, bool unknown_2c) (0x005ADD90)
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
        Movie = VQA_Unknown_005A1010(name.Peek_Buffer(), surface, Rect(0, 0, 0, 0), Rect(0, 0, 0, 0),
                                     (int)(unknown_volume_a40b44 * 255.0), 0);
        if (Movie != 0) {
            Movie->Source = Rect((surface->Get_Width() - 640) / 2, (surface->Get_Height() - 400) / 2, 640, 400);
            Movie->Dest = Rect((unknown_surface_839954->Get_Width() - 640) / 2,
                               (unknown_surface_839954->Get_Height() - 400) / 2, 640, 400);
        }
        Wstring base;
        name.Get_Token(0, ".", base);
        CCFileClass file(base + ".PCX");
        if (file.Is_Available()) {
            Image = unknown_load_pcx_60d110(&file, 0, 0, 0);
        }
    }
}


// MSVQAnim::~MSVQAnim() (0x005ADF90)

MSVQAnim::~MSVQAnim()
{
    delete Image;
    if (Movie != 0) {
        VQA_Movie_Close(Movie);
        delete Movie;
    }
}


// bool MSVQAnim::Advance(Surface *surface, Rect *dirty) (0x005ADFE0, slot 4)
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
            if (VQA_Movie_Unknown_005A15F0(Movie, &finished) == true) {
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
        surface->Copy_From(Movie->Source, *unknown_surface_839958, Movie->Dest, false, true);
    }
}


// void MSVQAnim::Restore(int unknown1) (0x005AE390, slot 8)
// Copies the picture into the target again once the movie is done.

void MSVQAnim::Restore(int unknown1)
{
    if (Done && Movie != 0 && Image != 0) {
        Target->Copy_From(Movie->Source, *Image, Image->Get_Rect(), false, true);
    }
}


// Rect MSVQAnim::Get_Rect() (0x005AE3E0, slot 6)
// The movie's destination rectangle, or an empty one without a movie.

Rect MSVQAnim::Get_Rect()
{
    static Rect empty(0, 0, 0, 0);
    return Movie != 0 ? Movie->Dest : empty;
}


// _$E (0x005AE460): the empty atexit thunk the compiler registers for the
// function-local static rectangle of MSVQAnim::Get_Rect (0x005AE3E0); no
// separate source text.


// bool MSVQAnim::Has_Finished() (0x005AE470, slot 7): the movie is done.

bool MSVQAnim::Has_Finished()
{
    return Done;
}


// MSBitPrintAnim::MSBitPrintAnim(WideString const &text, Rect const &rect, bool unknown_31) (0x005AE480)
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


// MSBitPrintAnim::~MSBitPrintAnim() (0x005AE4F0)

MSBitPrintAnim::~MSBitPrintAnim()
{
}


// void MSBitPrintAnim::Set_Unknown_30(bool value) (0x005AE510)

void MSBitPrintAnim::Set_Unknown_30(bool value)
{
    unknown_30 = value;
}


// Rect MSBitPrintAnim::Get_Rect() (0x005AE520, slot 6)

Rect MSBitPrintAnim::Get_Rect()
{
    return TextRect;
}


// bool MSBitPrintAnim::Has_Finished() (0x005AE550, slot 7)

bool MSBitPrintAnim::Has_Finished()
{
    return unknown_30;
}


// bool MSBitPrintAnim::Advance(Surface *surface, Rect *dirty) (0x005AE560, slot 4)
// Prints the text clipped to TextRect with the bitmap font and reports
// TextRect as the area to refresh; done once unknown_30 is set.

bool MSBitPrintAnim::Advance(Surface *surface, Rect *dirty)
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
    *dirty = TextRect;
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


// bool MSPrintAnim::Advance(Surface *surface, Rect *dirty) (0x005AEDA0, slot 4)
// Types the next character once the per-character delay has run out: redraws
// the visible lines up to it (centring each line when Centered), scrolls to
// the next line when the text runs past the bottom of TextRect, and reports
// TextRect as the area to refresh. Once the text is finished it returns
// !unknown_48 after three more calls.

bool MSPrintAnim::Advance(Surface *surface, Rect *dirty)
{
    dirty->X = 0;
    dirty->Y = 0;
    dirty->Width = 0;
    dirty->Height = 0;
    if (Timer.Value() != 0) {
        return false;
    }
    if (unknown_0C == true) {
        Timer = CDTimerClass<SystemTimerClass>(CharDelay);
        if (Position > Text.Length() && unknown_28 > 2) {
            return !unknown_48;
        }
        if (unknown_30 == true) {
            surface->Copy_From(TextRect, *unknown_surface_839958, TextRect, false, true);
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
        *dirty = TextRect;
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
            surface->Copy_From(unknown_54, *unknown_surface_839958, unknown_54, false, true);
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


// Rect MSPrintAnim::Get_Rect() (0x005AF400, slot 6)

Rect MSPrintAnim::Get_Rect()
{
    return TextRect;
}


// void MSPrintAnim::Set_Transient(bool transient) (0x005AF430): sets unknown_48 when it differs.

void MSPrintAnim::Set_Transient(bool transient)
{
    if (transient != unknown_48) {
        unknown_48 = transient;
    }
}


// bool MSPrintAnim::Has_Finished() (0x005AF450, slot 7): finished (as slot 4 sees it) and unknown_48 set

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


// MSPCXAnim::MSPCXAnim(Wstring const &name, DynamicVectorClass<MSAnim *> *anims, bool unknown_28) (0x005AF5F0)
// Loads "<name up to the first '.'>.PCX" and centres it on the background surface.

MSPCXAnim::MSPCXAnim(Wstring const &name, DynamicVectorClass<MSAnim *> *anims, bool unknown_28) :
    Background(unknown_surface_839958),
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
            Image = unknown_load_pcx_60d110(&file, 0, 0, 0);
            if (Image != 0) {
                PCXRect = Image->Get_Rect();
                Rect screen = Background->Get_Rect();
                PCXRect.X += (screen.Width - PCXRect.Width) / 2;
                PCXRect.Y += (screen.Height - PCXRect.Height) / 2;
            }
        }
    }
}


// MSPCXAnim::MSPCXAnim(Wstring const &name, DynamicVectorClass<MSAnim *> *anims, Point2D const &position, bool unknown_28) (0x005AF790)
// Loads "<name up to the first '.'>.PCX" and places it at `position`.
// Assumption: the trailing Background->Get_Rect() whose result is unused is
// in the original (the virtual call is made).

MSPCXAnim::MSPCXAnim(Wstring const &name, DynamicVectorClass<MSAnim *> *anims, Point2D const &position,
                     bool unknown_28) :
    Background(unknown_surface_839958),
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
            Image = unknown_load_pcx_60d110(&file, 0, 0, 0);
            if (Image != 0) {
                PCXRect = Image->Get_Rect();
                PCXRect.X = position.X;
                PCXRect.Y = position.Y;
                Background->Get_Rect();
            }
        }
    }
}


// MSPCXAnim::~MSPCXAnim() (0x005AF8F0)

MSPCXAnim::~MSPCXAnim()
{
    delete Image;
}


// bool MSPCXAnim::Advance(Surface *surface, Rect *dirty) (0x005AF910, slot 4)
// The first time (while active): copies the picture into the background,
// draws it onto `surface`, reports its rectangle and redraws every other
// animation of the list inside it. True once shown unless unknown_28.

bool MSPCXAnim::Advance(Surface *surface, Rect *dirty)
{
    if (!Done) {
        if (unknown_0C) {
            if (Image != 0) {
                Background->Copy_From(PCXRect, *Image, Image->Get_Rect(), false, true);
                Draw(surface, 0);
                Rect area = PCXRect;
                *dirty = area;
                int index = Anims->ID(this);
                for (int i = 0; i < Anims->Count(); i++) {
                    if (i != index) {
                        (*Anims)[i]->Draw(surface, &area);
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


// void MSPCXAnim::Restore(int unknown1) (0x005AFB80, slot 8)
// Copies the picture into the background again (while active).

void MSPCXAnim::Restore(int unknown1)
{
    if (Image != 0 && unknown_0C) {
        Background->Copy_From(PCXRect, *Image, Image->Get_Rect(), false, true);
    }
}


// Rect MSPCXAnim::Get_Rect() (0x005AFBC0, slot 6)

Rect MSPCXAnim::Get_Rect()
{
    return PCXRect;
}


// bool MSPCXAnim::Has_Finished() (0x005AFBF0, slot 7): the picture was shown.

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
