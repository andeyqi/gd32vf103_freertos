int myatoi(const char* s)
{
    long int v=0;
    int sign=1;
    while ( *s == ' '  ||  (unsigned int)(*s - 9) < 5u) s++;
    switch (*s)
    {
    case '-':
        sign=-1;
    case '+':
        ++s;
    }
    while ((unsigned int) (*s - '0') < 10u)
    {
        v=v*10+*s-'0';
        ++s;
    }
    return sign==-1?-v:v;
}

int myatohex(const char* s)
{
    long int v=0;
    int sign=1;
    char s1;

    while ( *s == ' '  ||  (unsigned int)(*s - 9) < 5u) s++;
    switch (*s)
    {
    case '-':
        sign=-1;
    case '+':
        ++s;
    }
    while ((unsigned int) (*s - '0') < 10u || (unsigned int) (*s - 'a') < 6u || (unsigned int) (*s - 'A') < 6u )
    {
        s1 = '0';
        s1 += (unsigned int) (*s - '0') < 10u ? ((unsigned int)(*s - '0')):\
              (unsigned int) (*s - 'a') < 6u  ? ((unsigned int)(10+(*s - 'a'))):\
              ((unsigned int)(10+(*s - 'A')));
        v=v*16+s1-'0';
        ++s;
    }
    return sign==-1?-v:v;
}

long int myatol(const char* s) 
{
    long int v=0;
    int sign=0;
    while ( *s == ' '  ||  (unsigned int)(*s - 9) < 5u) ++s;
    switch (*s) 
    {
        case '-': sign=-1;
        case '+': ++s;
    }
    while ((unsigned int) (*s - '0') < 10u) 
    {
        v=v*10+*s-'0'; ++s;
    }
    return sign?-v:v;
}

#if defined(__CC_ARM)
__asm int my_ffs(int value)
{
    CMP     r0, #0x00
    BEQ     exit

    RBIT    r0, r0
    CLZ     r0, r0
    ADDS    r0, r0, #0x01

exit
    BX      lr
}
#elif defined(__clang__)
int my_ffs(int value)
{
    __asm volatile(
        "CMP     %1, #0x00            \n"
        "BEQ     1f                   \n"

        "RBIT    %1, %1               \n"
        "CLZ     %0, %1               \n"
        "ADDS    %0, %0, #0x01        \n"

        "1:                           \n"

        : "=r"(value)
        : "r"(value)
    );
    return value;
}
#elif defined(__IAR_SYSTEMS_ICC__)
int my_ffs(int value)
{
    if (value == 0) return value;

    asm("RBIT %0, %1" : "=r"(value) : "r"(value));
    asm("CLZ  %0, %1" : "=r"(value) : "r"(value));
    asm("ADDS %0, %1, #0x01" : "=r"(value) : "r"(value));

    return value;
}
#elif defined(__GNUC__)
int my_ffs(int value)
{
    return __builtin_ffs(value);
}
#endif

