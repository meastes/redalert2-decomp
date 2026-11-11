// 32-bit handles for pointers that the recovered code keeps in an int field of a class whose native
// layout is pinned (link/check_real_offsets_*.cpp), so the field cannot become pointer-sized
// (Scout1246, 64/32 audit). First use: ObjectTypeClass +0xAC, the AlphaImage shape
// (MixFileClass::Retrieve in Read_INI 0x005D7810 and Load 0x005D7C30, read back as ShapeFileStruct *
// by AlphaShapeClass 0x0041EBE0 and 0x0041F5E0). 0 stays 0, so the field's zero test is unchanged.
// The pointers are long-lived (mix-file data), so the table only grows.
namespace {
const int kPointerBase = 0x50000000;
const int kPointerMax = 4096;
void *g_pointers[kPointerMax];
int g_pointer_count = 0;
}
int Native_Pointer_Handle(void const *pointer)
{
    if (!pointer) return 0;
    for (int i = 0; i < g_pointer_count; ++i)
        if (g_pointers[i] == pointer) return kPointerBase + i;
    if (g_pointer_count == kPointerMax) return 0;
    g_pointers[g_pointer_count] = (void *)pointer;
    return kPointerBase + g_pointer_count++;
}
void *Native_Pointer_From_Handle(int handle)
{
    int i = handle - kPointerBase;
    return i >= 0 && i < g_pointer_count ? g_pointers[i] : 0;
}
