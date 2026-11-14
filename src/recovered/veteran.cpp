// veteran.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/veteran.json carries the addresses and the rows that
// stay in their own files.


#define MIN(a,b) (((a)<(b))?(a):(b))


// Gains value / (cost * VeteranRatio) experience, capped at VeteranCap (0x00713360; TS Made_A_Kill).
// The original loads the cap as an FP rvalue (fld, then fld again for the store), which a
// double-returning inline accessor reproduces; reading the member directly copies it as two dwords.
inline double Veteran_Cap() { return Rules->VeteranCap; }
void VeterancyClass::Made_A_Kill(int cost, int value)
{
    Experience += value / (cost * Rules->VeteranRatio);
    Experience = MIN(Experience, Veteran_Cap());
}


bool VeterancyClass::Is_Veteran() const
{
    return Experience >= 1 && Experience < 2;
}


bool VeterancyClass::Is_Rookie() const
{
    return Experience >= 0 && Experience < 1;
}


bool VeterancyClass::Is_Dumbass() const
{
    return Experience < 0;
}


bool VeterancyClass::Is_Elite() const
{
    return Experience >= 2;
}


// The rank: 0 elite, 1 veteran, 2 rookie (0x00713440).
int VeterancyClass::Remaining_Levels() const
{
    if (Experience >= 2) {
        return 0;
    }
    return Experience >= 1 ? 1 : 2;
}


// The multiplier for value at this experience: 1 + value * level above zero, a fraction
// below it (0x00713470).
double VeterancyClass::Modify(double value)
{
    double lvl = Experience;
    lvl = floor(lvl);

    if (lvl < -0.01) {
        return -1.0 / (lvl - 1.0);
    }
    if (lvl > 0.01) {
        return 1.0 + (value * lvl);
    }
    return 1.0;
}


void VeterancyClass::Set_Dumbass(bool set)
{
    if (set) {
        Experience = -.25;
    } else {
        Experience = 0;
    }
}


void VeterancyClass::Set_Rookie(bool set)
{
    Experience = 0;
}


void VeterancyClass::Set_Veteran(bool set)
{
    if (set) {
        Experience = 1;
    } else {
        Experience = 0;
    }
}


void VeterancyClass::Set_Elite(bool set)
{
    if (set) {
        Experience = 2;
    } else {
        Experience = 0;
    }
}


int VeterancyClass::To_Integer() const
{
    return int(Experience * 100);
}


void VeterancyClass::From_Integer(int value)
{
    Experience = value / 100.0;
}
