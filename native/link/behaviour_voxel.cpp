// The voxel span drawer table (0x007F9DE0) for the native build (Scout1091,
// native M8). The original stores code addresses there; the native call site
// (port/fixups.json, 0x00719940) asks this function for the drawer. Never
// original code or bytes: the table is read from the user's game.exe at run
// time and each address is matched to the recovered drawer at that address.
struct VoxelFuncArgumentStruct;
typedef void (*VoxelDrawer)(VoxelFuncArgumentStruct *parameters);

void Draw_Voxel_Regular_Normals(VoxelFuncArgumentStruct *parameters);                   // 0x0071A180
void Draw_Voxel_Reverse_Normals(VoxelFuncArgumentStruct *parameters);                   // 0x0071A330
void Draw_Voxel_Regular_Normals_ZBuffer(VoxelFuncArgumentStruct *parameters);           // 0x0071A4D0
void Draw_Voxel_Reverse_Normals_ZBuffer(VoxelFuncArgumentStruct *parameters);           // 0x0071A710
void Draw_Voxel_Regular_Normals_Lighting(VoxelFuncArgumentStruct *parameters);          // 0x0071A950
void Draw_Voxel_Reverse_Normals_Lighting(VoxelFuncArgumentStruct *parameters);          // 0x0071AB40
void Draw_Voxel_Regular_Normals_ZBuffer_Lighting(VoxelFuncArgumentStruct *parameters);  // 0x0071AD30
void Draw_Voxel_Reverse_Normals_ZBuffer_Lighting(VoxelFuncArgumentStruct *parameters);  // 0x0071AFA0
void Draw_Voxel_Regular(VoxelFuncArgumentStruct *parameters);                           // 0x0071B220
void Draw_Voxel_Reverse(VoxelFuncArgumentStruct *parameters);                           // 0x0071B3E0
void Draw_Voxel_Regular_ZBuffer(VoxelFuncArgumentStruct *parameters);                   // 0x0071B5A0
void Draw_Voxel_Reverse_ZBuffer(VoxelFuncArgumentStruct *parameters);                   // 0x0071B7E0

namespace ra2port { bool original_bytes(unsigned int va, void *out, unsigned long n); }
extern "C" void ra2port_progress(const char *milestone, const char *detail);

namespace {
struct DrawerAt { unsigned int address; VoxelDrawer drawer; };
DrawerAt const drawers[] = {
    {0x0071A180, Draw_Voxel_Regular_Normals},
    {0x0071A330, Draw_Voxel_Reverse_Normals},
    {0x0071A4D0, Draw_Voxel_Regular_Normals_ZBuffer},
    {0x0071A710, Draw_Voxel_Reverse_Normals_ZBuffer},
    {0x0071A950, Draw_Voxel_Regular_Normals_Lighting},
    {0x0071AB40, Draw_Voxel_Reverse_Normals_Lighting},
    {0x0071AD30, Draw_Voxel_Regular_Normals_ZBuffer_Lighting},
    {0x0071AFA0, Draw_Voxel_Reverse_Normals_ZBuffer_Lighting},
    {0x0071B220, Draw_Voxel_Regular},
    {0x0071B3E0, Draw_Voxel_Reverse},
    {0x0071B5A0, Draw_Voxel_Regular_ZBuffer},
    {0x0071B7E0, Draw_Voxel_Reverse_ZBuffer},
};
unsigned int const table_va = 0x007F9DE0;   // unknown_drawers_7f9de0, 32 entries
int const table_entries = 32;

VoxelDrawer Drawer_At(unsigned int address)
{
    for (DrawerAt const &d : drawers) {
        if (d.address == address) return d.drawer;
    }
    return 0;
}

void Missing_Drawer(VoxelFuncArgumentStruct *)
{
}
}  // namespace

VoxelDrawer Native_Voxel_Drawer_7f9de0(int index)
{
    static VoxelDrawer table[table_entries];
    static bool built = false;
    if (!built) {
        built = true;
        for (int i = 0; i < table_entries; ++i) {
            unsigned int address = 0;
            ra2port::original_bytes(table_va + 4 * i, &address, 4);
            VoxelDrawer drawer = Drawer_At(address);
            if (drawer == 0 && i < table_entries / 2) {
                // A handwritten (winasm.asm) drawer: the C drawer of the same
                // index in the table's second half.
                unsigned int same = 0;
                ra2port::original_bytes(table_va + 4 * (i + table_entries / 2), &same, 4);
                drawer = Drawer_At(same);
            }
            if (drawer == 0) {
                ra2port_progress("voxel-drawer", "an entry of 0x007F9DE0 names no recovered drawer");
                drawer = Missing_Drawer;
            }
            table[i] = drawer;
        }
    }
    return index >= 0 && index < table_entries ? table[index] : Missing_Drawer;
}
