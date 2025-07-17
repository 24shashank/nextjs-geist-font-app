#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <stdint.h>

// PWM channel definitions
#define PWM_CHANNEL_LEFT    0
#define PWM_CHANNEL_RIGHT   1

// PWM configuration
typedef struct {
    uint8_t channel;
    uint8_t pin;
    uint16_t frequency;
    uint8_t duty_cycle;
} pwm_config_t;

// Function prototypes
void pwm_init(void);
void pwm_set_duty_cycle(uint8_t channel, uint8_t duty_cycle);
void pwm_start(uint8_t channel);
void pwm_stop(uint8_t channel);
void pwm_toggle(uint8_t channel);

#endif // PWM_DRIVER_H
