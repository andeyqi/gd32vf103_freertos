#ifndef __LITTLE_SHELL_H__
#define __LITTLE_SHELL_H__

#include <stdarg.h>
#include <stdint.h>

#if defined (__ICCARM__)
	#define SECTION(x)                  @ x
	#define RT_USED                     __root
#elif defined (__GNUC__)
    #define SECTION(x)                  __attribute__((section(x)))
    #define RT_USED                     __attribute__((used))
#endif

#define MAX_CLI_ARGS_BUF_LEN    80
#define MAX_CLI_ARGS_NUM        10

#define MAX_HISTROY_NUMS        5
#define LTSHELL_USING_HISTORY
#define LTSHELL_USING_SHOW_HELP 1

struct littleshell_arg {
    int argc;
    char *argv[MAX_CLI_ARGS_NUM];
    char _buf[MAX_CLI_ARGS_BUF_LEN];
};

enum input_stat
{
    WAIT_NORMAL,
    WAIT_SPEC_KEY,
    WAIT_FUNC_KEY,
};

struct little_shell
{
    enum input_stat stat;
#ifdef  LTSHELL_USING_HISTORY
    uint16_t current_history;
    uint16_t history_count;
    char cmd_history[MAX_HISTROY_NUMS][MAX_CLI_ARGS_BUF_LEN];
#endif  
    char line[MAX_CLI_ARGS_BUF_LEN];
    uint16_t line_position;
    uint16_t line_curpos;
};
    
typedef unsigned int (*syscall_func)(char argc,char ** argv);
extern void littleshell_main_entry(void *pvParameters);

/* system call table */
struct littleshell_syscall
{
    const char*     name;       /* the name of system call */
    const char*     desc;       /* description of system call */
    syscall_func func;          /* the function address of system call */
};

#define LTSH_FUNCTION_EXPORT_CMD(name, cmd, desc)                      \
    const char __fsym_##cmd##_name[] SECTION(".rodata.name") = #cmd;    \
    const char __fsym_##cmd##_desc[] SECTION(".rodata.name") = #desc;   \
    RT_USED const struct littleshell_syscall __fsym_##cmd SECTION("FSymTab")= \
    {                           \
        __fsym_##cmd##_name,    \
        __fsym_##cmd##_desc,    \
        (syscall_func)&name     \
    };
    
#define LTSH_FUNCTION_EXPORT(name, desc)   \
    LTSH_FUNCTION_EXPORT_CMD(name, name, desc)


#endif /* end of __LITTLE_SHELL_H__ */
