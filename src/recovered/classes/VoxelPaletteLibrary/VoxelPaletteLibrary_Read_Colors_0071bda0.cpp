int VoxelPaletteLibrary::Read_Colors(FileClass *file)
{
    int failed = 0;
    file->Open(1);
    if (file->Read(Colors, 0x300) != 0x300) {
        failed = 1;
    }
    file->Close();
    return failed;
}
