#ifndef CONFIG_H
#define CONFIG_H

#define __PERF_CNT_USE_RTOS__
#define __PERFC_CFG_PORTING_INCLUDE__  "perfc_port_riscv_cycle.h"

extern void __freertos_evr_on_create_task(void *ptTCB);
extern void __freertos_evr_on_task_switched_out (void *ptTCB);
extern void __freertos_evr_on_task_switched_in(void *ptTCB, unsigned int uxTopPriority) ;

#   define traceTASK_SWITCHED_OUT()                                             \
        __freertos_evr_on_task_switched_out(pxCurrentTCB)
#   define traceTASK_SWITCHED_IN()                                              \
        __freertos_evr_on_task_switched_in(pxCurrentTCB, uxTopReadyPriority)
#   define traceTASK_CREATE( pxNewTCB )                                        \
        __freertos_evr_on_create_task((void*)pxNewTCB)

#define __perf_counter_printf__  printf

#define TASK_TRACE_SWITCH_LOG         0

#endif /* CONFIG_H */