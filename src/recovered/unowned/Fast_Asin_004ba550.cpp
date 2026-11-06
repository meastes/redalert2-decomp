float __cdecl Fast_Asin_004ba550(double value)
{
    float number = (float)value;
    int index = (int)((number + 1.0f) * -2048.0f);
    return unknown_asin_table_80bdd4[-index];
}
