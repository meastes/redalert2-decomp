UnknownVoxelTailer *VoxelLibrary::Get_Layer_Info(int section, int arg2)
{
    return &LayerInfos[Get_Layer(section)->unknown_00 + arg2];
}
