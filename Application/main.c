/**
 * @file main.c
 * @author qiming.qiao
 * @brief gd32vf103 移植 FreeRTOS
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2019
 *
 */
#include <stdio.h>
#include "gd32vf103.h"
#include "SYSVIEW_Serial_Conf.h"
#include "SEGGER_RTT.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_config.h"
#include "ringbuffer.h"
#include "littleshell.h"
#include "atomic.h"
#include "log.h"

RingBuffer uart_rx;

void uart_log_init(void)
{
    if(UARTLOG_PORT == USART0)
    {
        /* init uart recive ring buffer */
        static uint8_t uart_rx_buff[128] = {0};
        RingBuffer_Init(&uart_rx,uart_rx_buff,128);

        /* 初始化uart0 TX PA9 RX PA10 */
        rcu_periph_clock_enable(RCU_GPIOA);
        rcu_periph_clock_enable(RCU_USART0);
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
        gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
        usart_deinit(UARTLOG_PORT);
        usart_baudrate_set(UARTLOG_PORT, 115200U);
        usart_word_length_set(UARTLOG_PORT, USART_WL_8BIT);
        usart_stop_bit_set(UARTLOG_PORT, USART_STB_1BIT);
        usart_parity_config(UARTLOG_PORT, USART_PM_NONE);
        usart_hardware_flow_rts_config(UARTLOG_PORT, USART_RTS_DISABLE);
        usart_hardware_flow_cts_config(UARTLOG_PORT, USART_CTS_DISABLE);
        usart_receive_config(UARTLOG_PORT, USART_RECEIVE_ENABLE);
        usart_transmit_config(UARTLOG_PORT, USART_TRANSMIT_ENABLE);
        usart_enable(UARTLOG_PORT);
        /* uart0 interrupt enable */
        eclic_irq_enable(USART0_IRQn, 1, 0);
        usart_interrupt_enable(USART0, USART_INT_RBNE);
        usart_interrupt_disable(USART0, USART_INT_TBE);
    }
}



int p1 = 0;

void task1(void *p)
{
    int i = 0;
    for(i = 0;i < 6000000;i++)
    {
        atomic_add1(1, (unsigned int *)&p1);
        //p1++;
    }
    eclic_global_interrupt_disable();
    printf("task1 atomic add: %d \n", p1);
    eclic_global_interrupt_enable();

    for(;;)
    {
        gpio_bit_write(GPIOA, GPIO_PIN_1, (bit_status)(1-gpio_input_bit_get(GPIOA, GPIO_PIN_1)));
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_bit_write(GPIOA, GPIO_PIN_2, (bit_status)(1-gpio_input_bit_get(GPIOA, GPIO_PIN_2)));
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_bit_write(GPIOC, GPIO_PIN_13, (bit_status)(1-gpio_input_bit_get(GPIOC, GPIO_PIN_13)));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void task2(void *p)
{

    int i = 0;
    for(i = 0;i < 6000000;i++)
    {
        atomic_add1(-1, (unsigned int *)&p1);
        //p1--;
    }

    eclic_global_interrupt_disable();
    printf("task2 atomic add: %d \n", p1);
    eclic_global_interrupt_enable();

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }

    //char *taskStatus = (char *)pvPortMalloc( uxTaskGetNumberOfTasks() * sizeof( TaskStatus_t ) );
    //for(;;)
    //{
    //    vTaskList(taskStatus);
    //    printf("\nTaskName\tStatus\tPRI\tStack\tTaskNumber\n%s",taskStatus);
    //    printf("current tick is %ld\n",xTaskGetTickCount());
    //    vTaskDelay(pdMS_TO_TICKS(5000));
    //}
}

void show_version(void)
{
    const char *ascii_art = 
        "    __                                     _   __                \n"
        "   / /   ____  ____  ____ _____ _____     / | / /___ _____  ____ \n"
        "  / /   / __ \\/ __ \\/ __ `/ __ `/ __ \\   /  |/ / __ `/ __ \\/ __ \\\n"
        " / /___/ /_/ / / / / /_/ / /_/ / / / /  / /|  / /_/ / / / / /_/ /\n"
        "/_____/\\____/_/ /_/\\__, /\\__,_/_/ /_/  /_/ |_/\\__,_/_/ /_/\\____/ \n"
        "                  /____/                                         \n";

    printf("\n");
    PRINT_COLOR_FMT(GREEN,"%s",(char *)ascii_art);
    printf("\n");
    PRINT_COLOR_FMT(GREEN,"%s %s\n",__DATE__,__TIME__);
    printf("\n");

}

static __attribute__((noinline)) int my_memcpy(void * src,void *  dst,int len)
{
    unsigned int tmp = 0;
    unsigned int end = (char *)src + len;

    asm volatile (
            "1: lw %[tmp], (%[src])\n"
            "sw %[tmp], (%[dst])\n"
            "addi %[dst], %[dst], 4\n"
            "addi %[src], %[src], 4\n"
            "blt %[src], %[end], 1b"
            : [dst] "+r" (dst),[tmp] "+r" (tmp),[src] "+r" (src)
            : [end] "r" (end)
            : "memory");

    return len;
}


static int cmd_mymemcpy_test(int argc, char **argv)
{

    int src[8] = {0x11111111,0x22222222,0x33333333,0x44444444,
                  0x55555555,0x66666666,0x77777777,0x88888888};
    int dst[8] = {};

    printf("my memcpy test ret = %d\r\n",my_memcpy(src,dst,sizeof(src)));

    if(memcmp(src,dst,sizeof(src)) == 0)
        printf("my memcpy test ok.\r\n");

    int src1[32] = {0x11111111,0x22222222,0x33333333,0x44444444,
                    0x55555555,0x66666666,0x77777777,0x88888888};
    int dst1[32] = {};

    printf("my memcpy test ret = %d\r\n",my_memcpy(src1,dst1,sizeof(src1)));

    if(memcmp(src1,dst1,sizeof(src1)) == 0)
        printf("my memcpy test ok.\r\n");

    return 0;
}
LTSH_FUNCTION_EXPORT(cmd_mymemcpy_test, "my memcpy test");

int main(void)
{
    eclic_priority_group_set(ECLIC_PRIGROUP_LEVEL4_PRIO0); //四位优先级组全配置为lvl
    eclic_global_interrupt_enable();                       //使能全局中断

    #if CONFIG_SYSTEMVIEW_EN
    SEGGER_SYSVIEW_Conf();
    printf("Segger Sysview Control Block Detection Address is 0x%lx\n", (uint32_t)&_SEGGER_RTT);
    vSYSVIEWUARTInit();
    #endif

    #if UARTLOGEN
    uart_log_init();
    #endif
    show_version();
    /* 初始化led PA1/PA2/PC13 */
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_bit_reset(GPIOA, GPIO_PIN_2);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_bit_reset(GPIOC, GPIO_PIN_13);

    /*  */

    xTaskCreate(task1,"task1",521,NULL,2,NULL);
    xTaskCreate(task2,"task2",521,NULL,2,NULL);
    xTaskCreate(littleshell_main_entry,"shell",521,NULL,2,NULL);

    vTaskStartScheduler();
}
