// long ThemeClass::Save(IStream *stream) -- 0x006E8310 (name hypothesis)
// Purpose: writes the playing (+0x04) and suspended (+0x08) themes to a
// save stream, stopping after a failed write.
// Uses the real shared ThemeClass declaration and returns the last write result.
#include "shared/com_persist.hpp"



#define THEME_SAVELOAD
#include "shared/theme.hpp"

long ThemeClass::Save(IStream *stream)
{
    HRESULT result = stream->Write(&Score, sizeof(Score), 0);
    if (SUCCEEDED(result)) {
        return stream->Write(&Pending, sizeof(Pending), 0);
    }
    return result;
}
