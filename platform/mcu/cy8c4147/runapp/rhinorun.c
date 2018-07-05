/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */
#include "project.h"
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>

#define DEMO_TASK_STACKSIZE    256 //256*cpu_stack_t = 1024byte
#define DEMO_TASK_PRIORITY     20

static ktask_t demo_task_obj;
cpu_stack_t demo_task_buf[DEMO_TASK_STACKSIZE];

int default_UART_Init(void);
void PendSV_Handler(void);

void SysTick_IRQ(void)
{
    krhino_intrpt_enter();
    krhino_tick_proc();
    krhino_intrpt_exit();	
}

void demo_task(void *arg)
{
    (void)arg;
    
    int count = 0;
    
    default_UART_Init();

    printf("demo_task here!\n");
    printf("rhino memory is %d!\n", krhino_global_space_get());

    while (1)
    {
        printf("hello world! count %d\n", count++);

        //sleep 1 second
        krhino_task_sleep(RHINO_CONFIG_TICKS_PER_SECOND);
    };
}

int main(void)
{
    krhino_init();
   
    CySysTickInit();
    CySysTickSetReload(CYDEV_BCLK__SYSCLK__HZ/RHINO_CONFIG_TICKS_PER_SECOND);
    CyIntSetSysVector(CY_INT_PEND_SV_IRQN, PendSV_Handler);
    CySysTickEnable();
    CySysTickSetCallback(0, SysTick_IRQ);

    krhino_task_create(&demo_task_obj, "demo_task", 0,DEMO_TASK_PRIORITY, 
        50, demo_task_buf, DEMO_TASK_STACKSIZE, demo_task, 1);
    
    krhino_start();
    
    return 0;
}

