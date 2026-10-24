#ifndef KERNEL_PORT_H
#define KERNEL_PORT_H   

#include <stdint.h>
#include "kernel.h"

uint32_t *KernelPort_InitTaskStack(
    uint32_t *stack_base,
    uint32_t stack_size_words,
    KernelTaskFunction_t function,
    void *argument);

#endif