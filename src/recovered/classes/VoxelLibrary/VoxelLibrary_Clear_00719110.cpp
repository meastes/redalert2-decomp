void VoxelLibrary::Clear()
{
    if (LayerHeaders != 0) {
        operator delete(LayerHeaders);
    }
    LayerHeaders = 0;
    if (LayerInfos != 0) {
        operator delete(LayerInfos);
    }
    LayerInfos = 0;
    if (Data != 0) {
        operator delete(Data);
    }
    Data = 0;
}
