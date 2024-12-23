#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
	uint8_t intattr = 0;
	printf("mtvec = %08lx\n",read_csr(mtvec));
	for(uint8_t i =0;i < 87;i++)
	{
		intattr = eclic_get_intattr(i);
		printf("intattr[%u] = %x\n",i,intattr);
	}
	return 0;
}
LTSH_FUNCTION_EXPORT(get_csr,"show csr register");


#define MCOUNTINHIBIT    0x320  //自定义寄存器用于控制计数器的开启和关闭

unsigned int mcount(char argc,char ** argv)
{
    uint32_t countinhibit = 0;
    if(argc == 1)
    {
        countinhibit = read_csr(mucounteren); //读取 mcycle 寄存器
        printf("MCOUNT = 0x%08lx \n",countinhibit);
        printf("IR %s\n",((countinhibit & 0x01U << 2U) ? "OFF" : "ON"));
        printf("CY %s\n",((countinhibit & 0x01U << 0U) ? "OFF" : "ON"));
        printf("cycle %lu\n",read_csr(cycle));
        printf("cycleh %ld\n",read_csr(0xc80));
    }

    if(argc == 2)
    {

        if(strcmp(argv[1],"off") == 0)
            asm volatile ("csrsi 0x320, 0x5");

        if(strcmp(argv[1],"on") == 0)
            asm volatile ("csrci 0x320, 0x5");

    }


	return 0;
}
LTSH_FUNCTION_EXPORT(mcount,"show mcountinhibit register");
