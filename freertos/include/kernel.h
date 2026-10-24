#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include "kernel_config.h"

typedef void (*KernelTaskFunction_t)(void *argument);

typedef enum
{
    KERNEL_TASK_UNUSED = 0,
    KERNEL_TASK_READY,
    KERNEL_TASK_BLOCKED,
    KERNEL_TASK_SUSPENDED
} KernelTaskState_t;

typedef struct
{
    uint32_t *saved_stack_pointer;
    uint32_t *stack_base;
    uint32_t stack_size_words;

    KernelTaskFunction_t function;
    void *argument;

    uint32_t wake_tick;
    uint8_t priority;
    KernelTaskState_t state;
} KernelTask_t;

extern KernelTask_t kernel_tasks[KERNEL_MAX_TASKS];

/* The running task remains READY while it is eligible to execute. */
extern KernelTask_t * volatile kernel_current_task;

/* Internal scheduler helper; not an application API. */
KernelTask_t *Kernel_SelectNextTask(void);

void Kernel_Tick(void);
uint32_t Kernel_GetTick(void);
int Kernel_CreateTask(KernelTaskFunction_t function,
                      void *argument,
                      uint8_t priority,
                      uint32_t stack_size_words);


int Kernel_Init(void);

#endif