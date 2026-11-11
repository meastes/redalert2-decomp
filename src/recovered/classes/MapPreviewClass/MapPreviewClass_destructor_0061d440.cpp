MapPreviewClass::~MapPreviewClass()
{
    if (unknown_00 != 0) {
        delete unknown_00;
        unknown_00 = 0;
    }
}
