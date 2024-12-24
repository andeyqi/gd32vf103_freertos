// See LICENSE file for licence details

#ifndef N200_FUNC_H
#define N200_FUNC_H


#include <stddef.h>
#include "n200_timer.h"
#include "n200_eclic.h"
#include "nmsis_compiler.h"

#define	ECLIC_GROUP_LEVEL0_PRIO4	0
#define	ECLIC_GROUP_LEVEL1_PRIO3	1
#define	ECLIC_GROUP_LEVEL2_PRIO2	2
#define	ECLIC_GROUP_LEVEL3_PRIO1	3
#define	ECLIC_GROUP_LEVEL4_PRIO0	4

void pmp_open_all_space();

void switch_m2u_mode();

uint32_t get_mtime_freq();

uint32_t mtime_lo(void);

uint32_t mtime_hi(void);

uint64_t get_mtime_value();

uint64_t get_instret_value();

uint64_t get_cycle_value();

uint32_t get_cpu_freq();

uint32_t __attribute__((noinline)) measure_cpu_freq(size_t n);


///////////////////////////////////////////////////////////////////
/////// ECLIC relevant functions
///////
void eclic_init ( uint32_t num_irq );
uint64_t get_timer_value();
void eclic_enable_interrupt (uint32_t source);
void eclic_disable_interrupt (uint32_t source);

void eclic_set_pending(uint32_t source);
void eclic_clear_pending(uint32_t source);

void eclic_set_intctrl (uint32_t source, uint8_t intctrl);
uint8_t eclic_get_intctrl  (uint32_t source);

void eclic_set_intattr (uint32_t source, uint8_t intattr);
uint8_t eclic_get_intattr  (uint32_t source);

void eclic_set_cliccfg (uint8_t cliccfg);
uint8_t eclic_get_cliccfg ();

void eclic_set_mth (uint8_t mth);
uint8_t eclic_get_mth();

//sets nlbits
void eclic_set_nlbits(uint8_t nlbits);


//get nlbits
uint8_t eclic_get_nlbits();

void eclic_set_irq_lvl(uint32_t source, uint8_t lvl);
uint8_t eclic_get_irq_lvl(uint32_t source);

void eclic_set_irq_lvl_abs(uint32_t source, uint8_t lvl_abs);
uint8_t eclic_get_irq_lvl_abs(uint32_t source);

uint8_t eclic_set_irq_priority(uint32_t source, uint8_t priority);
uint8_t eclic_get_irq_priority(uint32_t source);

void eclic_mode_enable();

void eclic_set_vmode(uint32_t source);
void eclic_set_nonvmode(uint32_t source);

void eclic_set_level_trig(uint32_t source);
void eclic_set_posedge_trig(uint32_t source);
void eclic_set_negedge_trig(uint32_t source);


///** \brief  Wait For Interrupt
//
//    Wait For Interrupt is a hint instruction that suspends execution
//    until one of a number of events occurs.
// */
__attribute__( ( always_inline ) ) static inline void __WFI(void) {
	__asm volatile ("wfi");
}
//
//
/** \brief  Wait For Event

    Wait For Event is a hint instruction that permits the processor to enter
    a low-power state until one of a number of events occurs.
 */
__attribute__( ( always_inline ) ) static inline  void __WFE(void) {
	__asm volatile ("csrs 0x810, 0x1");
	__asm volatile ("wfi");
	__asm volatile ("csrc 0x810, 0x1");
}

#define __STR(s)                #s
#define STRINGIFY(s)            __STR(s)

/** \brief Type of Control and Status Register(CSR), depends on the XLEN defined in RISC-V */
typedef unsigned long rv_csr_t;

/**
 * \brief CSR operation Macro for csrs instruction.
 * \details
 * Set csr register to be csr_content | val
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrs instruction
 */
#define __RV_CSR_SET(csr, val)                                  \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrs " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "rK"(__v)                                \
                     : "memory");                               \
    })

/**
 * \brief CSR operation Macro for csrc instruction.
 * \details
 * Set csr register to be csr_content & ~val
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrc instruction
 */
#define __RV_CSR_CLEAR(csr, val)                                \
    ({                                                          \
        rv_csr_t __v = (rv_csr_t)(val);                         \
        __ASM volatile("csrc " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "rK"(__v)                                \
                     : "memory");                               \
    })

/**
 * \brief   Enable IRQ Interrupts
 * \details Enables IRQ interrupts by setting the MIE-bit in the MSTATUS Register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __enable_irq(void)
{
    __RV_CSR_SET(CSR_MSTATUS, MSTATUS_MIE);
}

/**
 * \brief   Disable IRQ Interrupts
 * \details Disables IRQ interrupts by clearing the MIE-bit in the MSTATUS Register.
 * \remarks
 *          Can only be executed in Privileged modes.
 */
__STATIC_FORCEINLINE void __disable_irq(void)
{
    __RV_CSR_CLEAR(CSR_MSTATUS, MSTATUS_MIE);
}


/* ===== ARM Compatiable Functions ===== */
/**
 * \defgroup NMSIS_Core_ARMCompatiable_Functions   ARM Compatiable Functions
 * \ingroup  NMSIS_Core
 * \brief    A few functions that compatiable with ARM CMSIS-Core.
 * \details
 *
 * Here we provided a few functions that compatiable with ARM CMSIS-Core,
 * mostly used in the DSP and NN library.
 * @{
 */

/**
 * \brief Execute fence instruction, p -> pred, s -> succ
 * \details
 * the FENCE instruction ensures that all memory accesses from instructions preceding
 * the fence in program order (the `predecessor set`) appear earlier in the global memory order than
 * memory accesses from instructions appearing after the fence in program order (the `successor set`).
 * For details, please refer to The RISC-V Instruction Set Manual
 * \param p     predecessor set, such as iorw, rw, r, w
 * \param s     successor set, such as iorw, rw, r, w
 **/
#define __FENCE(p, s) __ASM volatile ("fence " #p "," #s : : : "memory")

/** \brief Read & Write Memory barrier */
#define __RWMB()        __FENCE(iorw,iorw)

/** \brief Instruction Synchronization Barrier, compatiable with ARM */
#define __ISB()                             __RWMB()

/** \brief Data Synchronization Barrier, compatiable with ARM */
#define __DSB()                             __RWMB()

/** \brief Data Memory Barrier, compatiable with ARM */
#define __DMB()                             __RWMB()

#endif
