// HRESULT __fastcall Speech_Save(IStream *stream) -- 0x00716790
// Purpose: writes the two four-character markers 'SxoV' and 'ExoV'
// (0x566F7853, 0x566F7845) to a save stream, stopping after a failed
// write; returns the last write's result (SaveLoad Put_All 0x006558B0 tests it).
// Assumptions: what the markers bracket (EVA voice data?) is a guess.
#include "shared/com_persist.hpp"

HRESULT __fastcall Speech_Save(IStream *stream)
{
    int marker = 0x566F7853;
    HRESULT result = stream->Write(&marker, sizeof(marker), 0);
    if (SUCCEEDED(result)) {
        marker = 0x566F7845;
        result = stream->Write(&marker, sizeof(marker), 0);
    }
    return result;
}
