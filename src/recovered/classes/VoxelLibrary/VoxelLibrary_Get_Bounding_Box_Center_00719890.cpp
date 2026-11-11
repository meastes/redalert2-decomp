Vector3 VoxelLibrary::Get_Bounding_Box_Center(int section, int arg2)
{
    UnknownVoxelTailer *tailer = Get_Layer_Info(section, arg2);
    return (tailer->unknown_40[4] + tailer->unknown_40[2]) * 0.5f;
}
