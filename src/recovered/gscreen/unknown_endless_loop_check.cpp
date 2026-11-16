// unknown_endless_loop_check_004e12a0 (0x004E12A0): no name evidence; named by what it does

extern unsigned int  unknown_endless_loop_004e1290(void);

bool  unknown_endless_loop_check_004e12a0(void)
{
    char result_byte;

    result_byte = (char)(unknown_endless_loop_004e1290());
    return result_byte == '\0';
}

