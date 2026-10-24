#include "kernel_port.h"

#include <stddef.h>
#include <stdint.h>

#define KERNEL_INITIAL_XPSR          0x01000000UL
#define KERNEL_INITIAL_EXC_RETURN    0xFFFFFFFDUL

static void Kernel_TaskExitError(void)
{
    /* Tasks must not return. Replace this trap with kernel error handling later. */
    for (;;)
    {
    }
}

uint32_t *KernelPort_InitTaskStack(uint32_t *stack_base,
                                   uint32_t stack_size_words,
                                   KernelTaskFunction_t function,
                                   void *argument)
{
    uint32_t *stack_pointer;

    if ((stack_base == NULL) ||
    (function == NULL) ||
    (stack_size_words < 32U) ||
    ((stack_size_words & 1U) != 0U) ||
    (((uintptr_t)stack_base & 7U) != 0U))
    {
        return NULL;
    }

    /* The stack grows down. Start one word past its highest valid element. */
    stack_pointer = stack_base + stack_size_words;

    /*
     * Initial hardware exception frame, restored when the task first starts:
     * R0, R1, R2, R3, R12, LR, PC, xPSR.
     */
    *(--stack_pointer) = KERNEL_INITIAL_XPSR;
    *(--stack_pointer) = ((uint32_t)(uintptr_t)function) & ~1UL; /* PC */
    *(--stack_pointer) = ((uint32_t)(uintptr_t)Kernel_TaskExitError) | 1UL; /* Task LR */
    *(--stack_pointer) = 0U;                    /* R12 */
    *(--stack_pointer) = 0U;                    /* R3 */
    *(--stack_pointer) = 0U;                    /* R2 */
    *(--stack_pointer) = 0U;                    /* R1 */
    *(--stack_pointer) = (uint32_t)(uintptr_t)argument; /* R0 */

    /*
     * Software-saved context expected by the planned PendSV handler:
     * R4-R11, then EXC_RETURN.
     */
    *(--stack_pointer) = KERNEL_INITIAL_EXC_RETURN;
    *(--stack_pointer) = 0U; /* R11 */
    *(--stack_pointer) = 0U; /* R10 */
    *(--stack_pointer) = 0U; /* R9 */
    *(--stack_pointer) = 0U; /* R8 */
    *(--stack_pointer) = 0U; /* R7 */
    *(--stack_pointer) = 0U; /* R6 */
    *(--stack_pointer) = 0U; /* R5 */
    *(--stack_pointer) = 0U; /* R4 */

    return stack_pointer;
}