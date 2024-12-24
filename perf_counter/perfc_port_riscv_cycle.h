#ifndef PERFC_PORT_RISCV_CYCLE_H
#define PERFC_PORT_RISCV_CYCLE_H

#include "n200_func.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#ifndef __perfc_sync_barrier__
#   define __perfc_sync_barrier__(...)         do {__DSB();__ISB();} while(0)
#endif

/*============================ TYPES =========================================*/
typedef uint32_t perfc_global_interrupt_status_t;

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

__STATIC_INLINE
perfc_global_interrupt_status_t perfc_port_disable_global_interrupt(void)
{
    __disable_irq();
    return 0;
}

__STATIC_INLINE
void perfc_port_resume_global_interrupt(perfc_global_interrupt_status_t tStatus)
{
    __enable_irq();
}

#endif /* PERFC_PORT_RISCV_CYCLE_H */