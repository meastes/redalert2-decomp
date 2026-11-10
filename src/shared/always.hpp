// Westwood's always.h min and max templates (TibSun always.h; the Westwood code
// uses MIN/MAX macros in its own sources and these templates where the windows.h
// macros are switched off). Do not include this after a windows.h that defines the
// min/max macros.
#ifndef RA2_SHARED_ALWAYS_HPP
#define RA2_SHARED_ALWAYS_HPP

#ifdef RA2_SDK_HEADERS
// RA2_SDK_HEADERS migration switch (docs/DECISIONS.md): always.h:91 switches off the windows.h min/max macros
// before any system header; then the shared flat prelude of system headers (shared/sdk_prelude.hpp).
#define NOMINMAX
#include "shared/sdk_prelude.hpp"
#endif

template <class T> T min(T a, T b)
{
    if (a < b) {
        return(a);
    } else {
        return(b);
    }
}

template <class T> T max(T a, T b)
{
    if (a > b) {
        return(a);
    } else {
        return(b);
    }
}

#endif
