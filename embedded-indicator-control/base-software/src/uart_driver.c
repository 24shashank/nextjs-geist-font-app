#include "uart_driver.h"
#include <avr/io.h>
#include <string.h>
#include <stdio.h>

// Initialize UART
void uart_init(void) {
    // Set baud rate for 9600 at 16MHz
    UBRR0H = 0;
    UBRR0L = 103;
    
    // Enable transmitter and receiver
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Send single character
void uart_send_char(char c) {
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

// Send string
void uart_send_string(const char* str) {
    while(*str) {
        uart_send_char(*str++);
    }
}

// Send data buffer
void uart_send_data(const uint8_t* data, uint16_t length) {
    for(uint16_t i = 0; i < length; i++) {
        uart_send_char(data[i]);
    }
}

// Receive character
char uart_receive_char(void) {
    while(!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// Check if data is available
bool uart_data_available(void) {
    return (UCSR0A & (1 << RXC0));
}

// Send formatted log message
void uart_send_log(const char* module, const char* message) {
    char buffer[UART_BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "[%s] %s\r\n", module, message);
    uart_send_string(buffer);
}
