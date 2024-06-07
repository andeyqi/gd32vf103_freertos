#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

// ANSI escape codes for colors and reset
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLD    "\033[1m"       /* Bold */
#define UNDERLINE "\033[4m"     /* Underline */

// Macro functions for colored printing
#define PRINT_COLOR(color, text) printf(color text RESET )
#define PRINT_COLOR_FMT(color, fmt, ...) printf(color fmt RESET , ##__VA_ARGS__)

// Specific color macros
#define PRINT_RED(text) PRINT_COLOR(RED, text)
#define PRINT_GREEN(text) PRINT_COLOR(GREEN, text)
#define PRINT_YELLOW(text) PRINT_COLOR(YELLOW, text)
#define PRINT_BLUE(text) PRINT_COLOR(BLUE, text)
#define PRINT_MAGENTA(text) PRINT_COLOR(MAGENTA, text)
#define PRINT_CYAN(text) PRINT_COLOR(CYAN, text)
#define PRINT_WHITE(text) PRINT_COLOR(WHITE, text)
#define PRINT_BOLD(text) PRINT_COLOR(BOLD, text)
#define PRINT_UNDERLINE(text) PRINT_COLOR(UNDERLINE, text)

#endif /* end of __LOG_H__ */
