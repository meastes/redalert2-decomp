// int ConnectionClass::Discard_Undeliverable_Packets() (0x00481550, slot 6)
// Always 0 here (IPXGlobalConnClass overrides it).
#include "shared/connection.hpp"

int ConnectionClass::Discard_Undeliverable_Packets()
{
    return 0;
}
