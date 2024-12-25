#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "utilities.h"

// 辅助函数：整数转字符串
void int_to_str(long long num, char *str, int base) {
    char digits[] = "0123456789";
    int i = 0, is_negative = 0;

    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }

    do {
        str[i++] = digits[num % base];
        num /= base;
    } while (num > 0);

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // 反转字符串
    for (int j = 0; j < i / 2; j++) {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

// 辅助函数：浮点数转字符串
void float_to_str(double num, char *str, int precision) {
    long long int_part = (long long)num;
    double frac_part = fabs(num - int_part);
    char int_buffer[32], frac_buffer[32];

    // 转换整数部分
    int_to_str(int_part, int_buffer, 10);

    // 转换小数部分
    for (int i = 0; i < precision; i++) {
        frac_part *= 10;
    }
    int_to_str((long long)frac_part, frac_buffer, 10);

    // 合并结果
    sprintf(str, "%s.%0*d", int_buffer, precision, (int)myatol(frac_buffer));
}

// 打印填充字符串
void print_padded(const char *str, int width, int left_align) {
    int len = strlen(str);
    if (left_align) {
        printf("%s", str);
        for (int i = 0; i < width - len; i++) {
            putchar(' ');
        }
    } else {
        for (int i = 0; i < width - len; i++) {
            putchar(' ');
        }
        printf("%s", str);
    }
}

// 自定义 printf 实现
void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    const char *p = format;
    char buffer[256];
    while (*p) {
        if (*p == '%' && *(p + 1)) {
            p++;  // 跳过 '%'

            // 解析对齐方式和宽度
            int left_align = 0, width = 0;
            if (*p == '-') {
                left_align = 1;
                p++;
            }
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }

            // 处理格式化符
            switch (*p) {
                case 's': { // 字符串
                    const char *str = va_arg(args, const char *);
                    snprintf(buffer, sizeof(buffer), "%s", str);
                    print_padded(buffer, width, left_align);
                    break;
                }
                case 'l': { // 长整型，支持 %lld
                    if (*(p + 1) == 'l') {
                        p++;
                        long long value = va_arg(args, long long);
                        int_to_str(value, buffer, 10);
                        print_padded(buffer, width, left_align);
                    }
                    break;
                }
                case 'f': { // 浮点数
                    double value = va_arg(args, double);
                    float_to_str(value, buffer, 6);  // 默认 6 位小数
                    print_padded(buffer, width, left_align);
                    break;
                }
                case '%': { // 百分号
                    putchar('%');
                    break;
                }
                default: {
                    putchar('%');
                    putchar(*p);
                    break;
                }
            }
        } else {
            putchar(*p);
        }
        p++;
    }

    va_end(args);
}