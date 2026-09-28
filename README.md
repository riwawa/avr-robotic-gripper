# AVR Robotic Gripper

A bare-metal AVR project for controlling and calibrating a robotic gripper using the ATmega328P.

The project started with direct servo control through Timer1 PWM and evolved into a calibrated gripper with defined open and closed positions.

## Current features

- Bare-metal AVR C
- Timer1 PWM servo control
- Servo calibration
- Smooth gripper movement
- Defined mechanical limits
- UART-based calibration tool
- No Arduino Servo library

## Calibrated positions

```c
#define GRIPPER_OPEN   2728
#define GRIPPER_CLOSED 4628
```

Measured values:

- Open: `1364 us` pulse, ~`77°`
- Closed: `2314 us` pulse, ~`163°`

These values correspond to the real mechanical limits of the assembled gripper.

## Project structure

```text
avr-robotic-gripper/
├── main.c
├── recalibracao.c
├── Makefile
└── README.md
```

`main.c` contains the gripper control logic.

`recalibracao.c` is a UART-based calibration utility used to manually adjust and inspect servo positions.

## What I am learning

This project is helping me understand:

- AVR registers
- GPIO
- Timer1
- PWM
- servo timing
- hardware calibration
- UART communication
- abstraction in embedded C
- direct hardware control without Arduino libraries

## Next steps

- UART commands for gripper control
- non-blocking movement
- state machine
- object detection
- sensor feedback
- fault handling

## Hardware

- ATmega328P / Arduino-compatible board
- servo motor
- robotic gripper kit
- USB-serial connection

## Build

```bash
make
make flash
```

Built with `avr-gcc`, `avr-objcopy` and `avrdude`.

