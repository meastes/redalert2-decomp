#include "shared/unknown_pair_second.hpp"
// @ref ?Read_File_0059ebc0@UnknownPairSecond@@QAEHPAVFileClass@@@Z=0059ebc0

UnknownPairSecond::UnknownPairSecond(FileClass *file)
{
    unknown_00 = false;
    unknown_section_count = 0;
    unknown_frame_count = 0;
    unknown_transforms = 0;
    if (Read_File_0059ebc0(file) == 0) {
        unknown_00 = true;
    }
}
