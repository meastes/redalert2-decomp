// @ref ??_H@YGXPAXIHP6EX0@Z@Z=0040aed0
#include "shared/unknown_pair_first.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/unknown_voxel_palette.hpp"
#include "shared/matrix3d.hpp"
// @ref ?VoxelRGBColors@@3PAUVxlRGB@@A=00ae3008
// @ref ?VoxelPaletteTranslateTable@@3PAEA=00af4608
struct VxlRGB { unsigned char Red,Green,Blue; };
extern VxlRGB VoxelRGBColors[256];
extern unsigned char VoxelPaletteTranslateTable[];
struct VxlFileHeader { char Name[16]; unsigned PaletteCount; int LayerCount, LayerInfoCount, DataSize; };
struct VxlLayerHeader { char Name[16]; int InfoIndex, Unused1; unsigned char Unused2; };
struct VxlLayerInfo { int StartOffset, EndOffset, DataOffset; float Scale; float Transform[12]; float MinBounds[3], MaxBounds[3]; unsigned char XSize,YSize,ZSize,NormalType; };
struct VxlMatrixData { float Data[12]; };
int VoxelLibrary::Read_File(FileClass *file, int load_palette)
{
    unsigned i = 0;
    Clear();
    if (!file->Open(1)) return 0;
    VxlLayerInfo layer_info;
    VxlFileHeader header;
    VxlLayerHeader layer_header;
    file->Read(&header, sizeof(header));
    LayerCount = header.LayerCount;
    LayerInfoCount = header.LayerInfoCount;
    DataSize = header.DataSize;
    LayerHeaders = new UnknownVoxelSection[LayerCount];
    LayerInfos = new UnknownVoxelTailer[LayerInfoCount];
    Data = new unsigned char[DataSize];
    if (!LayerHeaders || !LayerInfos || !Data) {
        Clear(); file->Close(); return 0;
    }
    if (load_palette) {
        VoxelPaletteLibrary palette((unsigned char *)VoxelRGBColors,VoxelPaletteTranslateTable);
        unsigned char first, last;
        if (file->Read(&first,1)!=1) { file->Close(); Clear(); return 0; }
        if (file->Read(&last,1)!=1) { file->Close(); Clear(); return 0; }
        palette.Header.unknown_00=first;
        palette.Header.unknown_04=last;
        for (unsigned char *green=&VoxelRGBColors[0].Green;green<&VoxelRGBColors[256].Green;green+=3) {
            unsigned channel=0;
            file->Read(green+channel++-1,1); file->Read(green+channel++-1,1); file->Read(green+channel-1,1);
        }
        if (header.PaletteCount>1) file->Seek(768*header.PaletteCount-768,1);
        palette.Calculate_Lookup_Table(0,0);
    } else file->Seek(770*header.PaletteCount,1);
    for (i=0;i<(unsigned)LayerCount;++i) {

        if (file->Read(&layer_header,sizeof(layer_header))!=sizeof(layer_header)) { Clear(); file->Close(); return 0; }
        LayerHeaders[i].unknown_00=layer_header.InfoIndex;
        LayerHeaders[i].unknown_04=layer_header.Unused1;
        *(unsigned char *)&LayerHeaders[i].unknown_08=layer_header.Unused2;
    }
    if (file->Read(Data,DataSize)!=DataSize) { Clear(); file->Close(); return 0; }
    for (i=0;i<LayerInfoCount;++i) {

        if (file->Read(&layer_info,sizeof(layer_info))!=sizeof(layer_info)) { Clear(); file->Close(); return 0; }
        LayerInfos[i].unknown_a0=layer_info.XSize;
        LayerInfos[i].unknown_a1=layer_info.YSize;
        LayerInfos[i].unknown_a2=layer_info.ZSize;
        LayerInfos[i].unknown_a3=layer_info.NormalType;
        LayerInfos[i].unknown_0c=layer_info.Scale;
        *(VxlMatrixData *)LayerInfos[i].unknown_10 = (VxlMatrixData &)Matrix3D(layer_info.Transform);
        Vector3 min; min.X=layer_info.MinBounds[0]; min.Y=layer_info.MinBounds[1]; min.Z=layer_info.MinBounds[2];
        Vector3 max; max=*(Vector3 const *)layer_info.MaxBounds;
        LayerInfos[i].unknown_40[0]=Vector3(max.X,max.Y,min.Z);
        LayerInfos[i].unknown_40[1]=Vector3(max.X,min.Y,min.Z);
        LayerInfos[i].unknown_40[2]=Vector3(min.X,min.Y,min.Z);
        LayerInfos[i].unknown_40[3]=Vector3(min.X,max.Y,min.Z);
        LayerInfos[i].unknown_40[4]=Vector3(max.X,max.Y,max.Z);
        LayerInfos[i].unknown_40[5]=Vector3(max.X,min.Y,max.Z);
        LayerInfos[i].unknown_40[6]=Vector3(min.X,min.Y,max.Z);
        LayerInfos[i].unknown_40[7]=Vector3(min.X,max.Y,max.Z);
        LayerInfos[i].unknown_00=(int *)((unsigned char *)Data+layer_info.StartOffset);
        LayerInfos[i].unknown_04=(int *)((unsigned char *)Data+layer_info.EndOffset);
        LayerInfos[i].unknown_08=(unsigned char *)Data+layer_info.DataOffset;
    }
    file->Close(); return 1;
}
