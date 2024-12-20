#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "riscv_encoding.h"
#include "n200_func.h"
#include "littleshell.h"

unsigned int cpuinfo(char argc,char ** argv)
{
    char str[20];
    uint8_t offset = 0;
    uint32_t misa;
    misa = read_csr(misa);

    if((misa&0xc0000000) == (0x01<<30))
    {
        str[0] = 'R';
        str[1] = 'v';
        str[2] = '3';
        str[3] = '2';
        offset += 4;
    }
    else if((misa&0xc0000000) == (0x02<<30))
    {
        str[0] = 'R';
        str[1] = 'v';
        str[2] = '6';
        str[3] = '4';
        offset += 4;
    }
    else
    {
        str[0] = 'R';
        str[1] = 'v';
        str[2] = '1';
        str[3] = '2';
        str[4] = '8';
        offset += 5;
    }

    for(int i = 0; i < 26;i++)
    {
        if((0x01<<i)&misa)
        {
            str[offset] = 'A' + i;
            offset++;
        }
    }

    str[offset] = '\0';

    printf("ISA : %s\n",str);
    return 1;
}
LTSH_FUNCTION_EXPORT(cpuinfo,"show cpu info");


unsigned int get_csr(char argc,char ** argv)
{
	uint32_t intattr = 0;
	printf("mtvec = %08x\n",read_csr(mtvec));
	for(uint32_t i =0;i < 87;i++)
	{
		intattr = eclic_get_intattr(i);
		printf("intattr[%d] = %08x\n",i,intattr);
	}
	return 0;
}
LTSH_FUNCTION_EXPORT(get_csr,"show csr register");

