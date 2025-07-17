#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

// Timer configuration
#define TIMER_INTERVAL_MS 100  // 100ms scheduler
#define TIMER_PRESCALER 1024

// Function prototypes
void timer_init(void);
void timer_start(void);
void timer_stop(void);
uint32_t timer_get_elapsed(void);
void timer_register_callback(void (*callback)(void));

#endif // TIMER_DRIVER_H
