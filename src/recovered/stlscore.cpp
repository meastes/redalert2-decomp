// stlscore.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/stlscore.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/id_source.hpp"
#include "shared/campaign_score.hpp"
#include "shared/movie_globals.hpp"
#include "shared/rect.hpp"
#include "shared/unknown_globals.hpp"


// Header-static dynamic initializers of the unit at 0x0069A450: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0069A450-0x0069A631); the other
// drafts of this plan are emitted by these includes.
// The unit expands the vector accessors inline (Score_Dialog_Proc_0069A7D0).


// Dynamic initializer (0x0069A470, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A4A0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A4C0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A4E0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A500, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A520, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A540, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A580, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A5B0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A5E0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A610, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


// Dynamic initializer (0x0069A620, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0069a450.cpp; no source of its own.


class Leaf_69a640 { public: void* get(); };
void* Leaf_69a640::get() { return this; }


// void UnknownScore_0069A640::Presentation_0069A650() (0x0069A650)
// Loads the score art, clears the screen behind the dialog, runs the score
// dialog 0x103 (procedure 0x0069A7D0) and clears the screen again.

// Clears the screen area behind the score dialog (0x006F3BF0; hypothesis).
void __fastcall Unknown_6F3BF0(Surface *surface, Rect const &window);
// The score dialog's procedure (0x0069A7D0; not recovered).
int __stdcall Score_Dialog_Proc_0069A7D0(HWND__ *dialog, unsigned int message, unsigned int wparam, long lparam);

void UnknownScore_0069A640::Presentation_0069A650()
{
    Unknown_Load_Art_Once_006F2710(unknown_id_source_a3d290->unknown_2239 == false);
    Rect rect(0, 0, unknown_8523e0, unknown_8523e4);
    Unknown_6F3BF0(unknown_surface_839954, rect);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    unknown_dialog_box_5eab60(0x103, Score_Dialog_Proc_0069A7D0, 0);
    Unknown_6F3BF0(unknown_surface_839954, rect);
    Unknown_004E0C90(true, unknown_surface_839954, 0);
    Unknown_Free_Art_Flagged_006F2750();
}
