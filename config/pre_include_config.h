
#ifndef CONFIG_H
#define CONFIG_H

#define __PERF_CNT_USE_RTOS__
#define __PERFC_CFG_PORTING_INCLUDE__  "perfc_port_riscv_cycle.h"

extern void __freertos_evr_on_create_task(void *ptTCB);
#   define traceTASK_CREATE( pxNewTCB )                                        \
        __freertos_evr_on_create_task((void*)pxNewTCB)

#define __perf_counter_printf__  printf


#endif /* CONFIG_H */