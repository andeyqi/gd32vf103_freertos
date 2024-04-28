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


void task1(void *p)
{
    for(;;)
    {
        gpio_bit_write(GPIOA, GPIO_PIN_1, (bit_status)(1-gpio_input_bit_get(GPIOA, GPIO_PIN_1)));
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void task2(void *p)
{
    char *taskStatus = (char *)pvPortMalloc( uxTaskGetNumberOfTasks() * sizeof( TaskStatus_t ) );
    for(;;)
    {
        vTaskList(taskStatus);
        printf("\nTaskName\tStatus\tPRI\tStack\tTaskNumber\n%s",taskStatus);
        printf("current tick is %ld\n",xTaskGetTickCount());
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
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
    printf("%s",ascii_art);
    printf("%s %s\n",__DATE__,__TIME__);
    printf("\n");

}

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
    /* 初始化led PA7 */
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_bit_reset(GPIOA, GPIO_PIN_1);

    xTaskCreate(task1,"task1",521,NULL,2,NULL);
    xTaskCreate(littleshell_main_entry,"task2",521,NULL,2,NULL);

    vTaskStartScheduler();
}
