// crcpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/crcpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crc_stream.hpp"


int UnknownCRCPipe::Put(void const *source,int length)
{
    Engine(source,length);
    return Pipe::Put(source,length);
}


unsigned long UnknownCRCPipe::Value_00494240()
{
    return Engine.Value();
}
