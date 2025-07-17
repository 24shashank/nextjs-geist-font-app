#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// GPIO pin definitions
#define GPIO_PIN_LEFT_BUTTON    0
#define GPIO_PIN_RIGHT_BUTTON   1
#define GPIO_PIN_LEFT_LED       2
#define GPIO_PIN_RIGHT_LED      3

// GPIO modes
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_PWM = 2
} gpio_mode_t;

// GPIO states
typedef enum {
    GPIO_STATE_LOW = 0,
    GPIO_STATE_HIGH = 1
} gpio_state_t;

// Function prototypes
void gpio_init(void);
void gpio_set_mode(uint8_t pin, gpio_mode_t mode);
void gpio_write(uint8_t pin, gpio_state_t state);
gpio_state_t gpio_read(uint8_t pin);
bool gpio_read_button(uint8_t pin);
void gpio_enable_interrupt(uint8_t pin);

#endif // GPIO_DRIVER_H
