#ifndef CLOCK_CONFIG_H
#define CLOCK_CONFIG_H

// Clock configuration parameters
#define SYSTEM_CLOCK_FREQ 16000000UL  // 16 MHz
#define APB1_CLOCK_FREQ SYSTEM_CLOCK_FREQ / 2
#define APB2_CLOCK_FREQ SYSTEM_CLOCK_FREQ / 2

void SystemClock_Config(void);

#endif // CLOCK_CONFIG_H