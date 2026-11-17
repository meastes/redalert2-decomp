#ifndef RA2_SHARED_UNKNOWN_CRC_STREAMS_HPP
#define RA2_SHARED_UNKNOWN_CRC_STREAMS_HPP
#include "shared/pipe.hpp"
#include "shared/straw.hpp"
#include "shared/ini_class.hpp"
// CRCPipe and CRCStraw (names: mischa85 names.csv, TibSun crcpipe.h/crcstraw.h); uninstantiated here (no RTTI copy).
// Same-ECX qualified calls establish the Pipe/Straw prefix; inheritance is
// a hypothesis without RTTI. State offsets+C/+10/+14 identify the
// real CRCEngine prefix. No allocation, constructor or full-size claim here.
class CRCPipe : public Pipe {
public:
    virtual int Put(void const *source,int length);
    unsigned long Result();
    CRCEngine Engine;
};
class CRCStraw : public Straw {
public:
    virtual int Get(void *buffer,int length);
    unsigned long Result();
    CRCEngine Engine;
};
#endif
