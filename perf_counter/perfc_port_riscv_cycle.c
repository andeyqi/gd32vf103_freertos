#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "riscv_encoding.h"

bool perfc_port_init_system_timer(bool bIsTimeOccupied)
{
    /* enable risc-v cpu cycle counter */
    asm volatile ("csrci 0x320, 0x5");
    return true;
}

uint32_t perfc_port_get_system_timer_freq(void)
{
    extern uint32_t SystemCoreClock;

    /* return the system timer frequency */
    return SystemCoreClock;
}

bool perfc_port_is_system_timer_ovf_pending(void)
{
    /* whether the system timer overflow is pending */
    return false;
}

int64_t perfc_port_get_system_timer_top(void)
{
    /* the 64bit cycle counter is not overflow */
    return 0;
}

int64_t perfc_port_get_system_timer_elapsed(void)
{
    int64_t counter = 0,tmp = 0;
    counter = read_csr(cycle);
    tmp = read_csr(0xc80);
    counter |= tmp<<32u;

    return (int64_t)counter;
}

void perfc_port_clear_system_timer_ovf_pending(void)
{

}

void perfc_port_stop_system_timer_counting(void)
{
    /* stop the system timer */
}

void perfc_port_clear_system_timer_counter(void)
{
    /* clear the system timer counter */
}
