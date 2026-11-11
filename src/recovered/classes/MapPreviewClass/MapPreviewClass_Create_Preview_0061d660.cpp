// void MapPreviewClass::Create_Preview() (0x0061D660; TibSun preview.cpp Create_Preview)
// Renders the preview from the loaded map: the first pass finds the pixel bounding box of the
// cells inside the map diamond, the second paints each cell's two preview pixels (tinted by a
// tiberium overlay or a terrain object's radar colour), the last marks the first eight waypoints
// with red squares. RA2 differs from TibSun in three places the original fixes: there is no
// originx/originy (minx is spilled to its home and reloaded after each painted cell because
// `level` evicts it from ebp), the painted cell's position is kept as the Point2D offset from the
// bounding box with its two-pixel area as a Rect, and the waypoint square is built in place.
// Names of the callees are the unit's current ones: Unknown_0054AAB0 is TibSun In_Radar,
// unknown_func_5dd000 is OverlayTypeClass::Get_Radar_Color, Put_Pixel with a clip rectangle is
// TibSun Put_Pixel_Clip, 0x24 is RTTI_TERRAIN.

void UnknownPreview_0061D430::Unknown_0061D660()
{
    CellClass *cptr;

    Map.Reset_Iterator();

    int minx = 10000;
    int miny = 10000;
    int maxx = 0;
    int maxy = 0;

    cptr = Map.Iterate();
    while (cptr != 0) {
        if (Map.Unknown_0054AAB0(cptr->MapCoords)) {
            Point2D pixel = Cell_To_Preview_Pixel(cptr->MapCoords);
            if (pixel.X < minx) {
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
        cptr = Map.Iterate();
    }

    int width = 2 * (maxx - minx);
    int height = maxy - miny;

    if (unknown_00 != 0) {
        delete unknown_00;
    }

    unknown_00 = new DSurface(width, height, true);
    unknown_00->Fill(0);

    Map.Reset_Iterator();

    cptr = Map.Iterate();

    unknown_00->Lock();
    while (cptr != 0) {
        Cell cell = cptr->Fetch_CellID();
        if (Map.Unknown_0054AAB0(cell)) {
            unsigned char unknown = 0;
            unsigned int lowcolor = Map[cell].Preview_Cell_Color(unknown, false);
            Point2D pixel = Cell_To_Preview_Pixel(cell);

            // The cell's two preview pixels, relative to the bounding box.
            Point2D offset = pixel - Point2D(minx, miny);
            Rect area(2 * offset.X, offset.Y, 2, 1);

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
                    // Other overlays flag the cell but never set radarcolor: it paints black.
                    if (Build_Hicolor_Pixel(rgb.Get_Red(), rgb.Get_Green(), rgb.Get_Blue()) != 0) {
                        hasradarcolor = true;
                    }
                }
            } else {
                TerrainClass *optr = (TerrainClass *)cptr->Cell_Occupier();
                if (optr != 0 && optr->What_Am_I() == 0x24) {
                    hasradarcolor = true;
                    level = 75;
                    radarcolor = Build_Hicolor_Pixel(optr->Class->RadarColor.Get_Red(), optr->Class->RadarColor.Get_Green(), optr->Class->RadarColor.Get_Blue());
                }
            }
            // The cell's low and high preview colours arrive packed into one value.
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

            unknown_00->Put_Pixel(Point2D(area.X, area.Y), lowcolor, unknown_00->Get_Rect());
            unknown_00->Put_Pixel(Point2D(area.X + 1, area.Y), highcolor, unknown_00->Get_Rect());
        }
        cptr = Map.Iterate();
    }

    for (int i = 0; i < 8; i++) {
        if (unknown_id_source_a3d290->Unknown_00663240(i)) {
            Cell cell = unknown_id_source_a3d290->Get_Waypoint_Location(i);
            Point2D pixel = Cell_To_Preview_Pixel(cell);
            pixel = pixel - Point2D(minx, miny);
            Rect rect;
            rect.X = 2 * pixel.X - 1;
            rect.Y = pixel.Y - 1;
            rect.Width = 4;
            rect.Height = 4;
            unknown_00->Fill_Rect(unknown_00->Get_Rect(), rect, Build_Hicolor_Pixel(240, 0, 0));
        }
    }

    unknown_00->Unlock();
}
