#include "kernel.h"
#include "kernel_config.h"

KernelTask_t kernel_tasks[KERNEL_MAX_TASKS] = {0};

static uint32_t kernel_task_stacks[KERNEL_MAX_TASKS]
                                  [KERNEL_STACK_SIZE_WORDS]
                                  __attribute__((aligned(8)));
static volatile uint32_t kernel_tick_count = 0U;



void Kernel_Tick(void)
{
    kernel_tick_count++;
}

uint32_t Kernel_GetTick(void)
{
    return kernel_tick_count;
}

