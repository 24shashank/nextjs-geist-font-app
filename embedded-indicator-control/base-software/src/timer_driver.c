#include "timer_driver.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint32_t timer_count = 0;
static void (*timer_callback)(void) = NULL;

// Initialize timer
void timer_init(void) {
    // Configure Timer1 for 100ms interrupts
    TCCR1A = 0;
    TCCR1B = (1 << WGM12);  // CTC mode
    
    // Set compare value for 100ms at 16MHz
    OCR1A = 15624;
    
    // Enable compare interrupt
    TIMSK1 = (1 << OCIE1A);
}

// Start timer
void timer_start(void) {
    // Start timer with prescaler 1024
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

// Stop timer
void timer_stop(void) {
    TCCR1B &= ~((1 << CS12) | (1 << CS10) | (1 << CS11));
}

// Get elapsed time in milliseconds
uint32_t timer_get_elapsed(void) {
    return timer_count * TIMER_INTERVAL_MS;
}

// Register callback function
void timer_register_callback(void (*callback)(void)) {
    timer_callback = callback;
}

// Timer1 compare interrupt
ISR(TIMER1_COMPA_vect) {
    timer_count++;
    if(timer_callback != NULL) {
        timer_callback();
    }
}
