// The original's irtti.h (IRTTITypeInfo; TibSun code/irtti.h, mischa85 include/irtti.h): line 12 includes
// <comdef.h>, so a unit reaching it sees the COM support classes and, when WIN32_LEAN_AND_MEAN is not yet
// defined, the full windows.h. RA2_SDK_HEADERS migration switch only (docs/DECISIONS.md "SDK headers for a
// unit"): an unconverted unit sees nothing here. Angle includes are allowed only in the canonical.SDK_WRAPPERS
// headers, inside the switch, naming files of the pinned set (manifests/toolchain.json sdk_headers).
#ifndef RA2_SHARED_IRTTI_HPP
#define RA2_SHARED_IRTTI_HPP
#ifdef RA2_SDK_HEADERS
#include <comdef.h>
#endif
#endif
