VoxelLibrary::VoxelLibrary(FileClass *file, int arg2)
{
    LoadFailed = false;
    LayerCount = 0;
    LayerInfoCount = 0;
    DataSize = 0;
    LayerHeaders = 0;
    LayerInfos = 0;
    Data = 0;
    if (Read_File(file, arg2) == 0) {
        LoadFailed = true;
    }
}
