int FactoryClass::Count_Type_004B9E40(TechnoTypeClass *type) const
{
    int total = 0;
    if (Object && Object->Techno_Type_Class() == type) total = 1;
    int count = Queue.Count();
    if (count > 0) {
        TechnoTypeClass *const *next = &Queue[0];
        do {
            if (*next == type) ++total;
            ++next;
        } while (--count);
    }
    return total;
}
