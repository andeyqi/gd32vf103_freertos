#include "littleshell.h"
#include "utilities.h"
#include "debug.h"


unsigned int hexdump1(char argc,char ** argv)
{
    uint32_t addr = 0,len = 0,wide = 1;

    if( argc < 3)
        return 1;

    if(argc == 4)
    {
        wide = myatoi(argv[3]);
    }

    if(wide != 1 && wide != 2 && wide !=4 )
        return 2;

    if((argv[1][0] == '0') && (argv[1][1] == 'x' || argv[1][1] == 'X'))
        addr = myatohex(&argv[1][2]);
    else
        addr = myatoi(argv[1]);

    if((argv[2][0] == '0') && (argv[2][1] == 'x' || argv[2][1] == 'X'))
        len = myatohex(&argv[2][2]);
    else
        len = myatoi(argv[2]);

    if(wide == 1)
        trace_byte_stream((uint8_t *)addr,len,addr);

    if(wide == 2)
        trace_hword_stream((uint16_t *)addr,len,addr);

    if(wide == 4)
        trace_word_stream((uint32_t *)addr,len,addr);

    return 0;
}
LTSH_FUNCTION_EXPORT(hexdump1,"dump data with start addr and len");


unsigned int hexdump2(char argc,char ** argv)
{
    int addr1 = 0,addr2 = 0,wide=1;

    if( argc < 3)
        return 1;

    if(argc == 4)
    {
        wide = myatoi(argv[3]);
    }

    if(wide != 1 && wide != 2 && wide !=4 )
        return 2;


    if((argv[1][0] == '0') && (argv[1][1] == 'x' || argv[1][1] == 'X'))
        addr1 = myatohex(&argv[1][2]);
    else
        addr1 = myatoi(argv[1]);

    if((argv[2][0] == '0') && (argv[2][1] == 'x' || argv[2][1] == 'X'))
        addr2 = myatohex(&argv[2][2]);
    else
        addr2 = myatoi(argv[2]);

    if(wide == 1)
        trace_byte_stream((uint8_t *)addr1,addr2 - addr1 + 1,addr1);

    if(wide == 2)
        trace_hword_stream((uint16_t *)addr1,(addr2 - addr1 + 1)/2,addr1);

    if(wide == 4)
        trace_word_stream((uint32_t *)addr1,(addr2 - addr1 + 1)/4,addr1);

    return 0;

}
LTSH_FUNCTION_EXPORT(hexdump2,"dump data with start addr and end addr");

unsigned int hexwrite(char argc,char ** argv)
{
    int addr,value;

    if(argc != 3)
        return 1;

    if((argv[1][0] == '0') && (argv[1][1] == 'x' || argv[1][1] == 'X'))
        addr = myatohex(&argv[1][2]);
    else
        addr = myatoi(argv[1]);

    if((argv[2][0] == '0') && (argv[2][1] == 'x' || argv[2][1] == 'X'))
        value = myatohex(&argv[2][2]);
    else
        value = myatoi(argv[2]);

    *((uint32_t *)addr) = value;

    return 0;
}
LTSH_FUNCTION_EXPORT(hexwrite,"write value to address");

