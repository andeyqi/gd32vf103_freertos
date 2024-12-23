#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool perfc_port_init_system_timer(bool bIsTimeOccupied)
{
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
    return true;
}

int64_t perfc_port_get_system_timer_top(void)
{
    /* the top value of the counting */
    return 0xFFFFFFFF;
}

int64_t perfc_port_get_system_timer_elapsed(void)
{
    return (int64_t)0;
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
