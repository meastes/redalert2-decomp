// The Win32 rectangle record under its tag name (left, top, right, bottom),
// as the bitmap font and the tooltips use it.
#ifndef RA2_SHARED_TAG_RECT_HPP
#define RA2_SHARED_TAG_RECT_HPP

#ifndef RA2_SDK_HEADERS    // a converted unit: windef.h tagRECT (pinned SDK set)
struct tagRECT {
    long left;
    long top;
    long right;
    long bottom;
};
#endif

#endif
