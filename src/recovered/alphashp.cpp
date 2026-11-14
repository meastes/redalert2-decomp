// alphashp.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/alphashp.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x0041E970 and the
// unit's AlphaShapeClass list; the other drafts of this plan are emitted by this file.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class AlphaShapeClass;

// The unit's list of AlphaShapeClass objects (0x0083C648; name is a hypothesis).
DynamicVectorClass<AlphaShapeClass *> unknown_AlphaShapeClass_list_0083c648;


// Dynamic initializer (0x0041E990, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041E9C0, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041E9E0, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EA00, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EA20, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EA40, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EA60, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EAA0, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EAD0, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EB00, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EB30, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EB40, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EB60, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// Dynamic initializer (0x0041EBA0, _$E) emitted by HeaderStatics_empty_rect_init_0041e970.cpp; no source of its own.


// AlphaShapeClass::AlphaShapeClass(ObjectClass *object, int x, int y) (0x0041EBE0)
// The owner type's alpha image placed at x, y; joins both lists.
#include "shared/alpha_shape.hpp"
#include "shared/object.hpp"
#include "shared/object_type.hpp"
#include "shared/object_lists.hpp"

AlphaShapeClass::AlphaShapeClass(ObjectClass *object, int x, int y) :
    Owner(object),
    Image(0),
    unknown_3c(false)
{
    ShapeFileStruct *image = (ShapeFileStruct *)object->Class_Of()->unknown_ac;
    Image = image;
    Bounds = Rect(x, y, image->Get_Width(), image->Get_Height());
    unknown_AlphaShapeClass_list_0083c648.Add(this);
    unknown_AbstractClass_list_00ac0d60.Add(this);
    if (!unknown_alpha_table_ready_84c684) {
        Init_Alpha_Table();
    }
}


// AlphaShapeClass::AlphaShapeClass() (0x0041ED70)
// An empty shape for the loader; joins both lists.

AlphaShapeClass::AlphaShapeClass() :
    Owner(0),
    Bounds(0, 0, 0, 0),
    Image(0),
    unknown_3c(false)
{
    unknown_AlphaShapeClass_list_0083c648.Add(this);
    unknown_AbstractClass_list_00ac0d60.Add(this);
    if (!unknown_alpha_table_ready_84c684) {
        Init_Alpha_Table();
    }
}


// AlphaShapeClass::AlphaShapeClass(NoInitClass const &noinit) (0x0041EED0)

AlphaShapeClass::AlphaShapeClass(NoInitClass const &noinit) :
    AbstractClass(noinit)
{
}


// AlphaShapeClass::~AlphaShapeClass() (0x0041EF00)
// Leaves both lists.

AlphaShapeClass::~AlphaShapeClass()
{
    unknown_AlphaShapeClass_list_0083c648.Delete(this);
    unknown_AbstractClass_list_00ac0d60.Delete(this);
}


// HRESULT AlphaShapeClass::GetClassID(GUID *clsid) (0x0041EFC0, slot 3)

HRESULT __stdcall AlphaShapeClass::GetClassID(GUID *clsid)
{
    if (!clsid) {
        return E_POINTER;
    }
    *clsid = CLSID_AlphaShapeClass;
    return S_OK;
}


// int AlphaShapeClass::Kind_Of() const (0x0041F000, slot 11)

int AlphaShapeClass::Kind_Of() const
{
    return 0x3E;
}


// int AlphaShapeClass::Size_Of() const (0x0041F010, slot 12)

int AlphaShapeClass::Size_Of() const
{
    return sizeof(AlphaShapeClass);
}


// void AlphaShapeClass::Compute_CRC(WWCRCEngine &crc) const (0x0041F020, slot 13)

void AlphaShapeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc((long)Bounds.X);
    crc((long)Bounds.Y);
    crc((long)Bounds.Width);
    crc((long)Bounds.Height);
}


// HRESULT AlphaShapeClass::Load(IStream *stream) (0x0041F060, slot 5)
// The image is reloaded from the owner when drawn.

HRESULT __stdcall AlphaShapeClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) AlphaShapeClass(NoInitClass());
        Swizzle_Pointer(Owner);
        Image = 0;
    }
    return result;
}


// HRESULT AlphaShapeClass::Save(IStream *stream, BOOL clear_dirty) (0x0041F0C0, slot 6)

HRESULT __stdcall AlphaShapeClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    return S_OK;
}


// void AlphaShapeClass::Detach(AbstractClass *target, bool all) (0x0041F0F0, slot 10)

void AlphaShapeClass::Detach(AbstractClass *target, bool all)
{
    if (target == (AbstractClass *)Owner) {
        unknown_3c = true;
    }
}


// void AlphaShapeClass::Update_All() (0x0041F110)
// Builds the alpha table once and deletes the shapes whose owner went away.

void AlphaShapeClass::Update_All()
{
    if (!unknown_alpha_table_ready_84c684) {
        Init_Alpha_Table();
    }
    for (int i = unknown_AlphaShapeClass_list_0083c648.Count() - 1; i >= 0; i--) {
        if (unknown_AlphaShapeClass_list_0083c648[i]->unknown_3c) {
            delete unknown_AlphaShapeClass_list_0083c648[i];
        }
    }
}


// Not exact (RA2_NON_MATCHING): AlphaShapeClass::Draw_In_Area at 0x0041F1C0, best score 41.92, 2026-11-13; differs: 312 instructions in the original, 313 in the draft (shape 90.24)
#ifdef RA2_NON_MATCHING
#include "shared/rect.hpp"
inline Rect &operator+=(Rect &rect, Point2D const &point) { rect.X += point.X; rect.Y += point.Y; return rect; }
// void AlphaShapeClass::Draw_In_Area(Point2D const &point, Rect const &cliprect) (0x0041F1C0)
// Blends every live shape into the alpha (depth) buffer, limited to the
// diamond of the tile at point (60x30 pixels).
#include "shared/alpha_shape_list.hpp"
#include "shared/blit.hpp"
#include "shared/display.hpp"
#include "shared/tactical.hpp"

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

void __fastcall AlphaShapeClass::Draw_In_Area(Point2D const &point, Rect const &cliprect)
{
#define _s "\x20"
#define _X "\xDB"
    static unsigned char const _tilemask[] = {
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s
        _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s
        _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s
        _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X
        _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s
        _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s
        _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
        _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _X _X _X _X _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s _s
    };
#undef _s
#undef _X

    for (int index = 0; index < unknown_AlphaShapeClass_list_0083c648.Count(); index++) {
        if (!unknown_AlphaShapeClass_list_0083c648[index]->unknown_3c) {
            Rect r1 = unknown_AlphaShapeClass_list_0083c648[index]->Bounds;
            r1 -= Point2D(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
            r1 += Point2D(unknown_rect_839630.X, unknown_rect_839630.Y);
            Rect r2 = Intersect(r1, cliprect);
            Rect r3(point.X, point.Y, 60, 30);
            Rect r4 = Intersect(r2, r3);
            if (r4.Is_Valid()) {
                int dx = r4.X - r3.X;
                int dy = r4.Y - r3.Y;
                ShapeFileStruct const *shape = unknown_AlphaShapeClass_list_0083c648[index]->Image;
                Rect shape_rect = shape->Get_Frame_Rect(0);
                int top = MAX(r4.Y, shape_rect.Y + r1.Y);
                int src_y = top - shape_rect.Y - r1.Y;
                int bottom = MIN(r4.Y + r4.Height, shape_rect.Y + shape_rect.Height + r1.Y);
                int left = MAX(r4.X, shape_rect.X + r1.X);
                int src_x = left - shape_rect.X - r1.X;
                int right = MIN(r4.X + r4.Width, shape_rect.X + shape_rect.Width + r1.X);
                int shape_skip = shape_rect.Width - right + shape_rect.X + src_x + r1.X;
                int alpha_skip = AlphaBuffer->Width - right + left;
                int mask_skip = 60 - r4.Width;
                unsigned char const *maskptr = &_tilemask[60 * dy + dx];
                unsigned char *shapedata = (unsigned char *)shape->Get_Frame_Data(0);
                unsigned short *alphaptr = AlphaBuffer->Get_Buffer_Offset(Point2D(left, top - unknown_rect_839630.Y));
                unsigned char *shapeptr = &shapedata[src_x + src_y * shape_rect.Width];
                if (&alphaptr[right - left + (bottom - top) * AlphaBuffer->Width + 2] >= (unsigned short *)AlphaBuffer->BufferTail) {
                    for (int y = top; y < bottom; y++) {
                        for (int x = left; x < right; x++) {
                            unsigned char pixel = *shapeptr++;
                            if (*maskptr++ != ' ') {
                                *alphaptr = unknown_alpha_table_83c670[pixel * 256 + *alphaptr];
                            }
                            alphaptr++;
                            alphaptr = AlphaBuffer->Wrap_Pointer(alphaptr);
                        }
                        shapeptr += shape_skip;
                        maskptr += mask_skip;
                        alphaptr += alpha_skip;
                        alphaptr = AlphaBuffer->Wrap_Pointer(alphaptr);
                    }
                } else {
                    for (int y = top; y < bottom; y++) {
                        for (int x = left; x < right; x++) {
                            unsigned char pixel = *shapeptr++;
                            if (*maskptr++ != ' ') {
                                *alphaptr = unknown_alpha_table_83c670[pixel * 256 + *alphaptr];
                            }
                            ++alphaptr;
                        }
                        shapeptr += shape_skip;
                        alphaptr += alpha_skip;
                        maskptr += mask_skip;
                    }
                }
            }
        }
    }
}
#endif

// Not exact (RA2_NON_MATCHING): AlphaShapeClass::Draw_All at 0x0041F5E0, best score 27.34, 2026-11-13; differs: 274 instructions in the original, 282 in the draft (shape 82.37)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x41F5E0 AlphaShapeClass::Draw_All
// entry 0x41F5E0; exact-match score 34.72 (misses_index best); plan "draft, verify"
// origin lane: Scout1000 (build/class-work/Scout1000/w/a2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): defined under its callers' name Unknown_0041F5E0 (Tactical 0x006A7820) instead of the overlay's AlphaShapeClass::Draw_All declaration; marked NATIVE.
// void AlphaShapeClass::Draw_All(Rect const &cliprect) (0x0041F5E0)
// Blends every shape into the alpha (depth) buffer; a shape without its
// image fetches it from the owner's type first.
void *Native_Pointer_From_Handle(int handle);   // NATIVE: native/link/native_pointer_handles.cpp

#ifndef MIN
#endif
#ifndef MAX
#endif

void __fastcall Unknown_0041F5E0(Rect const &cliprect)   // NATIVE: the callers' name (overlay ov3 declared AlphaShapeClass::Draw_All)
{
    for (int index = 0; index < unknown_AlphaShapeClass_list_0083c648.Count(); index++) {
        {
            Rect r1 = unknown_AlphaShapeClass_list_0083c648[index]->Bounds;
            r1 -= Point2D(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
            r1.X += unknown_rect_839630.X;   // NATIVE: the overlay's Rect += Point2D (RA2_RECT_POINT_INLINE): moves the corner
            r1.Y += unknown_rect_839630.Y;   // NATIVE
            Rect clip = cliprect;
            Rect r2 = Intersect(r1, clip);
            if (r2.Is_Valid()) {
                int x1 = r1.X;
                int y1 = r1.Y;
                ShapeFileStruct const *shape = unknown_AlphaShapeClass_list_0083c648[index]->Image;
                if (shape == 0) {
                    unknown_AlphaShapeClass_list_0083c648[index]->Image = (ShapeFileStruct *)Native_Pointer_From_Handle(unknown_AlphaShapeClass_list_0083c648[index]->Owner->Class_Of()->unknown_ac);   // NATIVE: 32-bit handle (Scout1246)
                    shape = unknown_AlphaShapeClass_list_0083c648[index]->Image;
                }
                Rect shape_rect = shape->Get_Frame_Rect(0);
                int top = MAX(r2.Y, shape_rect.Y + y1);
                int src_y = top - shape_rect.Y - y1;
                int bottom = MIN(clip.Y + clip.Height, shape_rect.Y + shape_rect.Height + y1);
                int left = MAX(r2.X, shape_rect.X + x1);
                int src_x = left - shape_rect.X - x1;
                int right = MIN(clip.X + clip.Width, shape_rect.X + shape_rect.Width + x1);
                int shape_skip = shape_rect.Width - right + shape_rect.X + x1 + src_x;
                int alpha_skip = AlphaBuffer->Width - right + left;
                unsigned char *shapedata = (unsigned char *)shape->Get_Frame_Data(0);
                unsigned short *alphaptr = AlphaBuffer->Get_Buffer_Offset(Point2D(left - unknown_rect_839630.X, top - unknown_rect_839630.Y));
                unsigned char *shapeptr = &shapedata[src_x + src_y * shape_rect.Width];
                if (&alphaptr[right - left + (bottom - top) * AlphaBuffer->Width + 2] >= (unsigned short *)AlphaBuffer->BufferTail) {
                    for (int y = top; y < bottom; y++) {
                        for (int x = left; x < right; x++) {
                            unsigned char pixel = *shapeptr++;
                            *alphaptr = unknown_alpha_table_83c670[pixel * 256 + *alphaptr];
                            alphaptr++;
                            alphaptr = AlphaBuffer->Wrap_Pointer(alphaptr);
                        }
                        shapeptr += shape_skip;
                        alphaptr += alpha_skip;
                        alphaptr = AlphaBuffer->Wrap_Pointer(alphaptr);
                    }
                } else {
                    for (int y = top; y < bottom; y++) {
                        for (int x = left; x < right; x++) {
                            unsigned char pixel = *shapeptr++;
                            *alphaptr = unknown_alpha_table_83c670[pixel * 256 + *alphaptr];
                            ++alphaptr;
                        }
                        shapeptr += shape_skip;
                        alphaptr += alpha_skip;
                    }
                }
            }
        }
    }
}
#endif

// void AlphaShapeClass::Init_Alpha_Table() (0x0041F950)
// Each entry is the product of its two index bytes over 127, clamped to a byte.

#define ALPHA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define ALPHA_MAX(a, b) ((a) > (b) ? (a) : (b))

void AlphaShapeClass::Init_Alpha_Table()
{
    unknown_alpha_table_ready_84c684 = true;
    for (int i = 0; i < 256 * 256; i++) {
        int low = i % 256;
        int high = i / 256;
        int value = ALPHA_MAX(0, ALPHA_MIN(255, low * high / 127));
        unknown_alpha_table_83c670[i] = value;
    }
}


// void *AlphaShapeClass::`scalar deleting destructor'(unsigned int flags) (0x0041F9C0)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


extern "C" unsigned char vtable_41fd50[]; extern "C" void __cdecl release_buffer_41fd50(void*); extern "C" void __cdecl release_object_41fd50(void*);
class Leaf_41fd50 { public: void* get(unsigned int flags); };
void* Leaf_41fd50::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_41fd50; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_41fd50((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_41fd50(this); return this; }


extern "C" unsigned char vtable_41fda0[]; extern "C" void __cdecl release_buffer_41fda0(void*); extern "C" void __cdecl release_object_41fda0(void*);
class Leaf_41fda0 { public: void* get(unsigned int flags); };
void* Leaf_41fda0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_41fda0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_41fda0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_41fda0(this); return this; }
