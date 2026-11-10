// void RadarClass::Complete_Radar_Refresh() (0x00634030)
// Redraws the whole radar: with no window, copies the surface at +0x121C
// onto the one at +0x1218 and finishes with 0x006324A0; otherwise draws
// every point of the radar surface.
#include "shared/radar.hpp"
// @ref ?MainWindow@@3PAUHWND__@@A=00b263d8
// @ref ?Render_Tracked_Objects@RadarClass@@QAEXXZ=006324a0
// @ref ?Plot_Radar_Pixel@RadarClass@@QAEXABUPoint2D@@@Z=00632000

void RadarClass::Complete_Radar_Refresh()
{
    if (!MainWindow) {
        unknown_1218->Copy_From(unknown_1218->Get_Rect(), *unknown_121c, unknown_121c->Get_Rect(), false, true);
        Render_Tracked_Objects();
    } else {
        int width = unknown_1218->Get_Width();
        int height = unknown_1218->Get_Height();
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Plot_Radar_Pixel(Point2D(x, y));
            }
        }
    }
}
