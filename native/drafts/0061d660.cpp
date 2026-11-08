// NON-EXACT DRAFT 0x61D660 FUN_0061d660
// entry 0x61D660; exact-match score 62.63 (misses_index best); plan "review: rewrite likely"
// origin lane: Prog441 (build/class-work/Prog441/w/best_0061d660_62.63.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
void UnknownPreview_0061D430::Unknown_0061D660()
{
    CellClass *cptr;

    Map.Unknown_0055A550();

    int minx = 10000;
    int maxx = 0;
    int miny = 10000;
    int originx = 10000;
    int maxy = 0;

    cptr = Map.Unknown_0055A490();
    while (cptr != 0) {
        if (Map.Unknown_0054AAB0(cptr->MapCoords)) {
            Point2D pixel = Cell_To_Preview_Pixel(cptr->MapCoords);
            if (pixel.X < minx) {
                originx = pixel.X;
                minx = pixel.X;
            }
            if (pixel.X > maxx) {
                maxx = pixel.X;
            }
            if (pixel.Y < miny) {
                miny = pixel.Y;
            }
            if (pixel.Y > maxy) {
                maxy = pixel.Y;
            }
        }
        cptr = Map.Unknown_0055A490();
    }

    int width = 2 * (maxx - minx);
    int height = maxy - miny;

    if (unknown_00 != 0) {
        delete unknown_00;
    }

    unknown_00 = new DSurface(width, height, true);
    unknown_00->Fill(0);

    Map.Unknown_0055A550();

    cptr = Map.Unknown_0055A490();

    unknown_00->Lock();
    while (cptr != 0) {
        Cell cell = cptr->Fetch_CellID();
        if (Map.Unknown_0054AAB0(cell)) {
            unsigned char unknown = 0;
            unsigned int lowcolor = Map[cell].Preview_Cell_Color(unknown, false);
            Point2D pixel = Cell_To_Preview_Pixel(cell);

            int x = 2 * (pixel.X - minx);
            int y = pixel.Y - miny;

            bool hasradarcolor = false;
            int level = 100;
            int radarcolor = 0;

            if (cptr->OverlayIndex != -1) {
                OverlayTypeClass *optr = OverlayTypes[cptr->OverlayIndex];
                RGBClass rgb = optr->unknown_func_5dd000(cptr->OverlayData);
                if (optr->Tiberium) {
                    level = 75;
                    hasradarcolor = true;
                    radarcolor = Build_Hicolor_Pixel(rgb.Get_Red(), rgb.Get_Green(), rgb.Get_Blue());
                    if (cptr->OverlayData < 2) {
                        level = 25;
                    } else if (cptr->OverlayData < 5) {
                        level = 50;
                    }
                    lowcolor = Blend_Hicolor_Pixel(radarcolor, lowcolor, level);
                } else {
                    if (Build_Hicolor_Pixel(rgb.Get_Red(), rgb.Get_Green(), rgb.Get_Blue()) != 0) {
                        hasradarcolor = true;
                    }
                }
            } else {
                TerrainClass *optr = (TerrainClass *)cptr->OccupierPtr;
                if (optr != 0 && optr->What_Am_I() == 0x24) {
                    hasradarcolor = true;
                    level = 75;
                    radarcolor = Build_Hicolor_Pixel(optr->Class->RadarColor.Get_Red(), optr->Class->RadarColor.Get_Green(), optr->Class->RadarColor.Get_Blue());
                }
            }
            int highcolor = lowcolor & 0x0000FFFF;
            lowcolor >>= 16;
            if (hasradarcolor) {
                if (level == 100) {
                    highcolor = radarcolor;
                    lowcolor = radarcolor;
                } else {
                    highcolor = Blend_Hicolor_Pixel(radarcolor, highcolor, level);
                    lowcolor = Blend_Hicolor_Pixel(radarcolor, lowcolor, level);
                }
            }

            unknown_00->Put_Pixel(Point2D(x, y), lowcolor, unknown_00->Get_Rect());
            unknown_00->Put_Pixel(Point2D(x + 1, y), highcolor, unknown_00->Get_Rect());

            minx = originx;
        }
        cptr = Map.Unknown_0055A490();
    }

    for (int i = 0; i < 8; i++) {
        if (unknown_id_source_a3d290->Unknown_00663240(i)) {
            Cell cell = unknown_id_source_a3d290->Get_Waypoint_Location(i);
            Point2D pixel = Cell_To_Preview_Pixel(cell);
            Rect rect;
            rect.X = 2 * (pixel.X - minx) - 1;
            rect.Y = pixel.Y - miny - 1;
            rect.Width = 4;
            rect.Height = 4;
            unknown_00->Fill_Rect(unknown_00->Get_Rect(), rect, Build_Hicolor_Pixel(240, 0, 0));
        }
    }

    unknown_00->Unlock();
}
// @ref ?Preview_Cell_Color@CellClass@@QAEIAAE_N@Z=004710d0
// @ref ?unknown_func_5dd000@OverlayTypeClass@@QBE?AVRGBClass@@I@Z=005dd000
