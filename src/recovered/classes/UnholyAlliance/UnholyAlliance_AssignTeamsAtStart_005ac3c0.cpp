// bool UnholyAlliance::AssignTeamsAtStart() -- 0x005AC3C0
// Slot 6: switches the option at 0x00A3D2B8 on; true.

bool UnholyAlliance::AssignTeamsAtStart()
{
    unknown_flag_a3d2b8 = true;
    return true;
}
