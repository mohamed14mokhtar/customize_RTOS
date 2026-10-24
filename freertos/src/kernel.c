#include "kernel.h"
#include "kernel_config.h"
#include "kernel_port.h"
#include <stddef.h>

KernelTask_t kernel_tasks[KERNEL_MAX_TASKS] = {0};

static uint32_t kernel_task_stacks[KERNEL_MAX_TASKS]
                                  [KERNEL_STACK_SIZE_WORDS]
                                  __attribute__((aligned(8)));
static volatile uint32_t kernel_tick_count = 0U;

KernelTask_t * volatile kernel_current_task = NULL;



/*
 * Startup-only API for this milestone:
 * call from main before starting the scheduler, never from an ISR.
 *
 * Returns task index on success, or -1 on failure.
 * Our priority convention: larger number = higher task priority.
 */
int Kernel_CreateTask(KernelTaskFunction_t function,
                      void *argument,
                      uint8_t priority,
                      uint32_t stack_size_words)
{
    if ((function == NULL) ||
        (stack_size_words < 32U) ||
        (stack_size_words > KERNEL_STACK_SIZE_WORDS) ||
        ((stack_size_words & 1U) != 0U))
    {
        return -1;
    }

    for (uint32_t i = 0U; i < KERNEL_MAX_TASKS; i++)
    {
        KernelTask_t *task = &kernel_tasks[i];

        if (task->state != KERNEL_TASK_UNUSED)
        {
            continue;
        }

        uint32_t *saved_sp = KernelPort_InitTaskStack(
            kernel_task_stacks[i],
            stack_size_words,
            function,
            argument
        );

        if (saved_sp == NULL)
        {
            return -1; /* Stack initialization failed. */
        }

        task->saved_stack_pointer = saved_sp;
        task->stack_base = kernel_task_stacks[i];
        task->stack_size_words = stack_size_words;

        task->function = function;
        task->argument = argument;
        task->priority = priority;
        task->wake_tick = 0U;

        /* Publish READY only after all fields are initialized. */
        task->state = KERNEL_TASK_READY;

        return (int)i;
    }

    return -1; /* All task slots are occupied. */
}

void Kernel_Tick(void)
{
    kernel_tick_count++;
}

uint32_t Kernel_GetTick(void)
{
    return kernel_tick_count;
}

/*
 * Select a READY task without changing kernel_current_task.
 *
 * Call before scheduler startup, or later from the context-switch
 * handler while kernel task-state updates are protected.
 */
KernelTask_t *Kernel_SelectNextTask(void)
{
    KernelTask_t *selected = NULL;
    KernelTask_t *current = kernel_current_task;
    uint32_t start_index = 0U;

    /*
     * Start after the current task so equal-priority tasks
     * take turns when a scheduling decision occurs.
     */
    for (uint32_t i = 0U; i < KERNEL_MAX_TASKS; i++)
    {
        if (current == &kernel_tasks[i])
        {
            start_index = (i + 1U) % KERNEL_MAX_TASKS;
            break;
        }
    }

    for (uint32_t offset = 0U;
         offset < KERNEL_MAX_TASKS;
         offset++)
    {
        uint32_t index =
            (start_index + offset) % KERNEL_MAX_TASKS;

        KernelTask_t *candidate = &kernel_tasks[index];

        if (candidate->state != KERNEL_TASK_READY)
        {
            continue;
        }

        if ((selected == NULL) ||
            (candidate->priority > selected->priority))
        {
            selected = candidate;
        }
    }

    return selected;
}

static void Kernel_IdleTask(void *argument)
{
    (void)argument;

    for (;;)
    {
        /*
         * Always ready.
         * Busy-loop initially; low-power sleep will be added later.
         * Preemption will allow higher-priority tasks to run.
         */
    }
}

int Kernel_Init(void)
{
    /*
     * Startup-only: call once, before creating application tasks.
     * Reject initialization if any task slot is already occupied.
     */
    for (uint32_t i = 0U; i < KERNEL_MAX_TASKS; i++)
    {
        if (kernel_tasks[i].state != KERNEL_TASK_UNUSED)
        {
            return -1;
        }
    }

    kernel_tick_count = 0U;
    kernel_current_task = NULL;

    int idle_id = Kernel_CreateTask(
        Kernel_IdleTask,
        NULL,
        0U,
        KERNEL_STACK_SIZE_WORDS
    );

    return (idle_id >= 0) ? 0 : -1;
}



