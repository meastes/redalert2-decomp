VoxelLibrary::VoxelLibrary(FileClass *file, int arg2)
{
    unknown_00 = false;
    unknown_04 = 0;
    unknown_08 = 0;
    unknown_0c = 0;
    unknown_10 = 0;
    unknown_14 = 0;
    unknown_18 = 0;
    if (Read_File(file, arg2) == 0) {
        unknown_00 = true;
    }
}
