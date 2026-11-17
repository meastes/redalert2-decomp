// The CD a map or a file needs (RTTI: TypeList<DiskID>, an enum; -1 any disk, -2 the map file
// itself is present). Its own header as in the Westwood line (mischa85 include/diskid.hh).
#ifndef RA2_SHARED_DISKID_HPP
#define RA2_SHARED_DISKID_HPP

enum DiskID {
    DISK_ANY_UNKNOWN = -1
};

#endif
