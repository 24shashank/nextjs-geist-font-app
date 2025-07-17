#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>

// UART configuration
#define UART_BAUD_RATE 9600
#define UART_BUFFER_SIZE 64

// Function prototypes
void uart_init(void);
void uart_send_char(char c);
void uart_send_string(const char* str);
void uart_send_data(const uint8_t* data, uint16_t length);
char uart_receive_char(void);
bool uart_data_available(void);
void uart_send_log(const char* module, const char* message);

#endif // UART_DRIVER_H
