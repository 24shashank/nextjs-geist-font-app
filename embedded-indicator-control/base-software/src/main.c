#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "gpio_driver.h"
#include "pwm_driver.h"
#include "uart_driver.h"
#include "timer_driver.h"

// System states
typedef enum {
    STATE_OFF = 0,
    STATE_LEFT,
    STATE_RIGHT,
    STATE_HAZARD
} system_state_t;

// Global variables
volatile system_state_t current_state = STATE_OFF;
volatile uint32_t button_press_time_left = 0;
volatile uint32_t button_press_time_right = 0;
volatile uint8_t led_toggle_counter = 0;
volatile uint8_t led_state_left = 0;
volatile uint8_t led_state_right = 0;

// Function prototypes
void system_init(void);
void scheduler_task(void);
void handle_button_events(void);
void update_leds(void);
void send_status_uart(void);

int main(void) {
    system_init();
    uart_send_log("MAIN", "System initialized");
    
    // Enable global interrupts
    sei();
    
    // Main loop
    while(1) {
        // All processing done in scheduler callback
    }
    
    return 0;
}

// System initialization
void system_init(void) {
    gpio_init();
    pwm_init();
    uart_init();
    timer_init();
    
    // Register scheduler callback
    timer_register_callback(scheduler_task);
    timer_start();
    
    uart_send_log("INIT", "GPIO, PWM, UART, Timer initialized");
}

// Scheduler task (called every 100ms)
void scheduler_task(void) {
    static uint8_t counter_300ms = 0;
    
    handle_button_events();
    
    // 300ms timing for LED toggle
    if(++counter_300ms >= 3) {
        counter_300ms = 0;
        update_leds();
    }
    
    // Send status every 1 second
    static uint8_t status_counter = 0;
    if(++status_counter >= 10) {
        status_counter = 0;
        send_status_uart();
    }
}

// Handle button press events
void handle_button_events(void) {
    static bool left_button_prev = false;
    static bool right_button_prev = false;
    
    bool left_button = gpio_read_button(GPIO_PIN_LEFT_BUTTON);
    bool right_button = gpio_read_button(GPIO_PIN_RIGHT_BUTTON);
    
    // Left button handling
    if(left_button && !left_button_prev) {
        button_press_time_left = timer_get_elapsed();
    } else if(!left_button && left_button_prev) {
        uint32_t press_duration = timer_get_elapsed() - button_press_time_left;
        if(press_duration >= 1000) {  // 1 second press
            switch(current_state) {
                case STATE_OFF:
                    current_state = STATE_LEFT;
                    uart_send_log("BUTTON", "Left indicator ON");
                    break;
                case STATE_LEFT:
                    current_state = STATE_OFF;
                    uart_send_log("BUTTON", "Left indicator OFF");
                    break;
                case STATE_RIGHT:
                    current_state = STATE_LEFT;
                    uart_send_log("BUTTON", "Right OFF, Left ON");
                    break;
                case STATE_HAZARD:
                    current_state = STATE_OFF;
                    uart_send_log("BUTTON", "Hazard OFF");
                    break;
            }
        }
    }
    
    // Right button handling
    if(right_button && !right_button_prev) {
        button_press_time_right = timer_get_elapsed();
    } else if(!right_button && right_button_prev) {
        uint32_t press_duration = timer_get_elapsed() - button_press_time_right;
        if(press_duration >= 1000) {  // 1 second press
            switch(current_state) {
                case STATE_OFF:
                    current_state = STATE_RIGHT;
                    uart_send_log("BUTTON", "Right indicator ON");
                    break;
                case STATE_RIGHT:
                    current_state = STATE_OFF;
                    uart_send_log("BUTTON", "Right indicator OFF");
                    break;
                case STATE_LEFT:
                    current_state = STATE_RIGHT;
                    uart_send_log("BUTTON", "Left OFF, Right ON");
                    break;
                case STATE_HAZARD:
                    current_state = STATE_OFF;
                    uart_send_log("BUTTON", "Hazard OFF");
                    break;
            }
        }
    }
    
    // Hazard detection (both buttons pressed)
    if(left_button && right_button) {
        static bool hazard_detected = false;
        static uint32_t hazard_start_time = 0;
        
        if(!hazard_detected) {
            hazard_start_time = timer_get_elapsed();
            hazard_detected = true;
        } else if((timer_get_elapsed() - hazard_start_time) >= 1000) {
            if(current_state != STATE_HAZARD) {
                current_state = STATE_HAZARD;
                uart_send_log("BUTTON", "Hazard light ON");
            }
        }
    }
    
    left_button_prev = left_button;
    right_button_prev = right_button;
}

// Update LED states based on current state
void update_leds(void) {
    switch(current_state) {
        case STATE_OFF:
            pwm_set_duty_cycle(PWM_CHANNEL_LEFT, 0);
            pwm_set_duty_cycle(PWM_CHANNEL_RIGHT, 0);
            led_state_left = 0;
            led_state_right = 0;
            break;
            
        case STATE_LEFT:
            led_state_left = !led_state_left;
            pwm_set_duty_cycle(PWM_CHANNEL_LEFT, led_state_left ? 100 : 0);
            pwm_set_duty_cycle(PWM_CHANNEL_RIGHT, 0);
            break;
            
        case STATE_RIGHT:
            led_state_right = !led_state_right;
            pwm_set_duty_cycle(PWM_CHANNEL_LEFT, 0);
            pwm_set_duty_cycle(PWM_CHANNEL_RIGHT, led_state_right ? 100 : 0);
            break;
            
        case STATE_HAZARD:
            led_state_left = !led_state_left;
            led_state_right = led_state_left;
            pwm_set_duty_cycle(PWM_CHANNEL_LEFT, led_state_left ? 100 : 0);
            pwm_set_duty_cycle(PWM_CHANNEL_RIGHT, led_state_right ? 100 : 0);
            break;
    }
}

// Send status over UART
void send_status_uart(void) {
    char status_msg[64];
    snprintf(status_msg, sizeof(status_msg), 
             "State: %d, Left: %d, Right: %d", 
             current_state, led_state_left, led_state_right);
    uart_send_log("STATUS", status_msg);
}
