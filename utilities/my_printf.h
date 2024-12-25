#ifndef MY_PRINTF_H
#define MY_PRINTF_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

// 自定义 printf 函数
void my_printf(const char *format, ...);


#ifdef __cplusplus
}
#endif

#endif // MY_PRINTF_H
