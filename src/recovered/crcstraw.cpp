// crcstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/crcstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crc_stream.hpp"


int UnknownCRCStraw::Get(void *buffer,int length)
{
    if(buffer && length>=1) {
        int received=Straw::Get(buffer,length);
        Engine(buffer,received);
        return received;
    }
    return 0;
}


unsigned long UnknownCRCStraw::Value_004942d0()
{
    return Engine.Value();
}
