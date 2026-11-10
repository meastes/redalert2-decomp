// NON-EXACT DRAFT 0x6A7E80 FUN_006a7e80
// entry 0x6A7E80; exact-match score 86.26 (misses_index best); plan "draft, verify"
// origin lane: Scout778 (build/class-work/Scout778/keep/px_8626.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Tactical pixel effects (0x006A7E80): the glints over ore and gem cells.
#include "shared/tactical.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/cell_class.hpp"
#include "shared/frame_rate.hpp"
#include "shared/pixel_fx.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/win32_api.hpp"
unsigned int Unknown_0053D840();
extern unsigned long unknown_last_time_abfd70;
// @ref ?unknown_last_time_abfd70@@3KA=00abfd70
// @ref ?Unknown_0053D840@@YAIXZ=0053d840

void Tactical::Draw_Pixel_Effects(Rect const &area, Rect const &cliprect)
{
    if (unknown_a6eb1c < Unknown_0053D840()) {
        return;
    }
    if (unknown_surface_83995c->Bytes_Per_Pixel() != 2) {
        return;
    }
    if (unknown_a40b2c == 0) {
        return;
    }
    char *pixels = (char *)unknown_surface_83995c->Lock();
    if (pixels == 0) {
        return;
    }
    DWORD now = timeGetTime();
    unsigned int frames = now - unknown_last_time_abfd70;
    unknown_last_time_abfd70 = now;
    if (frames > 1000) {
        frames = 1000;
    }
    int pitch = unknown_surface_83995c->Stride();
    Point2D lepton = Unknown_006A2900(Point2D(area.Top_Left()) + Point2D(TacPixelX, TacPixelY) - unknown_rect_839630.Top_Left());
    Cell origin(lepton.X / 256, lepton.Y / 256);
    Cell base = origin;
    int rows = area.Height / 15 + 17;
    int columns = area.Width / 60 + 4;
    base.X -= 2;
    Cell scan;
    for (int y = 0; y < rows; ++y) {
        Cell step(y / 2, (y + 1) / 2);
        scan = base + step;
        for (int x = columns; x > 0; --x) {
            if (Map.Unknown_0054AAB0(scan)) {
                CellClass &cell = Map[scan];
                int value = cell.Get_Tiberium_Value();
                if (cell.Flags_12c & 0x10 && !cell.unknown_47cb50()
                    && (cell.unknown_ec == 2 || value != 0)
                    && cell.OccupierPtr == 0
                    && !(cell.unknown_140 & 0x1000)) {
                    PixelFXClass *effect = (PixelFXClass *)cell.unknown_fc;
                    int type = value != 0;
                    if (effect == 0) {
                        effect = new PixelFXClass(type);
                        cell.unknown_fc = (UnknownCellOwned *)effect;
                    } else if (effect->Phase >= 0x2000) {
                        effect->Init(type);
                    }
                    if (effect != 0 && effect->Reduce_Life(frames)) {
                        Rect rect = cell.Cell_Render_Rect();
                        Point2D point(rect.Width / 2, rect.Height / 2);
                        point.X += rect.X;
                        point.Y += rect.Y;
                        point.X += effect->XOffset;
                        point.Y += effect->YOffset;
                        if (point.X >= cliprect.X && point.X < cliprect.Width + cliprect.X
                            && point.Y >= cliprect.Y && point.Y < cliprect.Height + cliprect.Y) {
                            effect->Update(frames);
                            *(unsigned short *)(pixels + pitch * point.Y + point.X * 2) = (unsigned short)RGB_To_Pixel(effect->Current.Red, effect->Current.Green, effect->Current.Blue);
                        }
                    }
                }
            }
            scan += Cell(1, -1);
        }
    }
    unknown_surface_83995c->Unlock();
}
