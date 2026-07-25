# STM32 UART-Controlled PWM LED Brightness Controller

A bare-metal-informed embedded project on the STM32F401RE (Nucleo board) that controls the brightness of an external LED in real time via serial commands. Type a number from 0–100 into a serial terminal, and the LED adjusts to that brightness level using hardware PWM.

## Demo



https://github.com/user-attachments/assets/dd85e9e7-0cac-46f7-adbc-0a77120b0cd5



## What it does

The user sends a brightness value (0–100) over UART from a PC terminal. The microcontroller parses the incoming characters, converts them to a numeric value, and sets the PWM duty cycle on a timer channel driving an external LED. The board sends a confirmation message back to the terminal.

## Two implementations

This project is built two ways to demonstrate both practical and low-level understanding:

- **`hal-version/`** — Built with STM32 HAL and CubeMX. The production-style approach.
- **`bare-metal-version/`** — Built by writing directly to hardware registers (RCC, GPIO, TIM2, USART2), with no HAL library. Every peripheral is configured by hand using the reference manual: clock enables, GPIO alternate-function mapping, timer PWM mode via CCMR1, and UART receive by polling the RXNE flag.

The bare-metal version demonstrates understanding of what HAL does underneath — memory-mapped registers, clock gating, alternate function selection, and PWM generation at the register level.

## Hardware used

- STM32 Nucleo-F401RE development board
- External LED (breadboard)
- 220Ω current-limiting resistor
- Jumper wires + breadboard
- USB cable (also provides the serial connection via the onboard ST-Link virtual COM port)

## How it works

**PWM generation:** TIM2 is configured in PWM mode on channel 2 (pin PA1). The timer runs from an 84 MHz system clock (it might be different depending on the MCU), divided by a prescaler of 84 to 1 MHz, with a counter period of 100 — giving a ~10 kHz PWM frequency (flicker-free) where the duty cycle value maps directly to a brightness percentage.

**UART input:** USART2 (115200 baud) receives characters over the ST-Link virtual COM port. Digits are assembled into a number; pressing Enter applies it. The value is written to the timer's compare register via `__HAL_TIM_SET_COMPARE()`, changing brightness instantly.

**Key concept:** Brightness is controlled by pulse-width modulation — rapidly switching the LED on and off, where the ratio of on-time (duty cycle) determines perceived brightness. Since the counter period is 100, the input number *is* the duty cycle percentage.

## Peripherals & concepts demonstrated

- Timer configuration and PWM generation
- UART serial communication (receive and transmit)
- ASCII-to-integer parsing of serial input
- Register-level understanding of clock, prescaler, and compare registers
- Hardware wiring and current-limiting for external components

## Build & run

1. Open the `.ioc` file in STM32CubeIDE to view/regenerate the peripheral configuration
2. Build and flash to a Nucleo-F401RE
3. Wire an LED + 220Ω resistor from PA1 to GND
4. Open a serial terminal (PuTTY etc.) at 115200 baud on the board's COM port
5. Type a number 0–100 and press Enter

## What I learned

This was my first ever project in embedded systems , I really liked to learn about what exactly controls the brightness of an LED in real-world situations. I learnt how two CPU's communicate with each other 
using the communication protocol of UART (Universal Asynchronous Receiver Transmitter). I learnt how bit manipulation exactly works and how data in the form of bits is tramsmitted from one CPU to another in UART and even a 
small delay in the timing of transferring the bits can cause to re-send the entire data back again which tells me that precise timing is a very important part of embedded systems. I learnt how to configure the PWM timer and UART in STM32CubeMX and
I learnt the importance of hardware compenents isolation as while I was working on the project , it turned out that I was using a dead LED instead of a working one which made me kind of confused during the process and made me think if there was a problem in the code or the configuration itself or not 
(which it obviously wasn't cause I was using a dead LED). 
