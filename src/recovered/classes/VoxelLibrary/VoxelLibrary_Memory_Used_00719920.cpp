int VoxelLibrary::Memory_Used()
{
    return LayerCount * sizeof(UnknownVoxelSection) + LayerInfoCount * sizeof(UnknownVoxelTailer) + sizeof(VoxelLibrary) + DataSize;
}
