# Vehicle Indicator Control System

## Overview
Embedded software solution for vehicle indicator control with mobile BLE interface.

## Architecture
- **Base Software**: C drivers (UART, GPIO, PWM, Timer)
- **Application Software**: MATLAB Simulink models
- **Mobile Interface**: BLE protocol for remote control

## Project Structure
```
embedded-indicator-control/
├── base-software/
│   ├── drivers/
│   ├── middleware/
│   └── scheduler/
├── application-software/
│   ├── simulink-models/
│   └── generated-code/
├── ble-mobile-control/
├── documentation/
└── testing/
```

## Features
- Left/Right indicator control
- Hazard light activation
- 300ms LED toggle timing
- UART logging
- BLE mobile control
- 100ms task scheduler

## Hardware Requirements
- Microcontroller Kit
- UART TTL adapter
- 2 Push Buttons (Left/Right)
- 2 LEDs (Left/Right indicators)
- Jumper wires
