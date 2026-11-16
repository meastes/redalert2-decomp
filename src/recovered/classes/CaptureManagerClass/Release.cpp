// void CaptureManagerClass::Release_4681e0() (0x004681E0)
// Returns the held object to the house it came from (slot 222) and forgets
// both; Capture 0x00467FF0 starts with the same steps.

void CaptureManagerClass::Release_4681e0()
{
    if (Unit != 0) {
        Unit->unknown_234 = 0;
        Unit->Captured(unknown_28, true);
        Unit = 0;
        unknown_28 = 0;
    }
}
