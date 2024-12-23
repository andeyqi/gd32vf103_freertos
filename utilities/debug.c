#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define TRACE_DISPLAY_WIDTH          16

#define __OUTPUT_STREAM(__TYPE, __ADDR,__OFF,__SIZE, __FORMAT_STR, __BLANK)     \
    do {                                                                        \
        int8_t n = 0;                                                           \
        uint16_t line = 0;                                                      \
        uint16_t __Size = (__SIZE);                                             \
        __TYPE *pSrc = (__TYPE *)(__ADDR);                                      \
        uint8_t *pchSrc;                                                        \
                                                                                \
        printf("         ");                                                    \
        for (n = 0; n < (TRACE_DISPLAY_WIDTH / sizeof(__TYPE)); n++) {          \
            printf(__FORMAT_STR, (n*sizeof(__TYPE)));                           \
        }                                                                       \
        printf("\r\n");                                                         \
        while(__Size >= (TRACE_DISPLAY_WIDTH / sizeof(__TYPE))) {               \
            printf("%08x:",TRACE_DISPLAY_WIDTH*(line++));                       \
            for (n = 0; n < (TRACE_DISPLAY_WIDTH / sizeof(__TYPE)); n++) {      \
                printf(__FORMAT_STR, pSrc[n]);                                  \
            }                                                                   \
                                                                                \
            printf("\t");                                                       \
            pchSrc = (uint8_t *)pSrc;                                           \
                                                                                \
            for (n = 0; n < TRACE_DISPLAY_WIDTH; n++) {                         \
                char c = pchSrc[n];                                             \
                if (c >= 127 || c < 32) {                                       \
                    c = '.';                                                    \
                }                                                               \
                printf("%c", c);                                                \
            }                                                                   \
                                                                                \
            printf("\r\n");                                                     \
                                                                                \
            pSrc += (TRACE_DISPLAY_WIDTH / sizeof(__TYPE));                     \
            __Size -= (TRACE_DISPLAY_WIDTH / sizeof(__TYPE));                   \
        }                                                                       \
                                                                                \
        if (__Size > 0) {                                                       \
            printf("%08x:",TRACE_DISPLAY_WIDTH*(line));                         \
            for (n = 0; n < __Size; n++) {                                      \
                printf(__FORMAT_STR, pSrc[n]);                                  \
            }                                                                   \
            for (   n = 0;                                                      \
                    n < (TRACE_DISPLAY_WIDTH/sizeof(__TYPE) - __Size);          \
                    n++) {                                                      \
                printf(__BLANK);                                                \
            }                                                                   \
            printf("\t");                                                       \
            pchSrc = (uint8_t *)pSrc;                                           \
            for (n = 0; n < __Size * sizeof(__TYPE); n++) {                     \
                char c = pchSrc[n];                                             \
                if (c > 127 || c < 32) {                                        \
                    c = '.';                                                    \
                }                                                               \
                printf("%c", c);                                                \
            }                                                                   \
                                                                                \
            printf("\r\n");                                                     \
        }                                                                       \
    } while(0)


void trace_word_stream(uint32_t *pwStream, uint16_t hwSize,uint16_t osffset)
{
    __OUTPUT_STREAM(uint32_t, pwStream, osffset, hwSize, "%08X ", "         ");
}

void trace_hword_stream(uint16_t *phwStream, uint16_t hwSize,uint16_t osffset)
{
    __OUTPUT_STREAM(uint16_t, phwStream, osffset, hwSize, "%04X ", "     ");
}

void trace_byte_stream(uint8_t *pchStream, uint16_t hwSize,uint16_t osffset)
{
    __OUTPUT_STREAM(uint8_t, pchStream, osffset, hwSize, "%02X ", "   ");
}

