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

unsigned int myatohexuint(const char* s)
{
    unsigned int v=0;
    char s1;

    while ( *s == ' '  ||  (unsigned int)(*s - 9) < 5u) s++;
    switch (*s)
    {
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
    return v;
}


long long int myatol(const char* s) 
{
    long long int v=0;
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


