int VoxelLibrary::Memory_Used()
{
    return unknown_04 * sizeof(UnknownVoxelSection) + unknown_08 * sizeof(UnknownVoxelTailer) + sizeof(VoxelLibrary) + unknown_0c;
}
