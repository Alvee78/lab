# Microcontroller Lab Project

## ARM APSR Flag Detection using STM32F103C8T6


## Project Overview

This project demonstrates how to manually test and observe ARM processor status flags (APSR) using inline assembly on the STM32F103C8T6 microcontroller.

The program performs arithmetic and saturation operations, then reads the Application Program Status Register (APSR) to detect different flags and indicate their status using LEDs.


## Objectives

* Understand ARM Cortex-M3 status flags (NZCVQ)
* Learn how to use inline assembly in C
* Interface GPIO registers directly (bare-metal programming)
* Visualize processor flags using LEDs


## Development Environment

* Microcontroller: STM32F103C8T6 (Blue Pill)
* IDE: STM32CubeIDE
* Language: C with ARM Inline Assembly
* Programming Type: Bare-metal (no HAL/LL libraries used)



## Hardware Setup

* STM32F103C8T6 board
* LEDs connected to GPIOA pins:

  * PA8 → N flag
  * PA9 → Z flag
  * PA10 → C flag
  * PA11 → V flag
  * PA12 → Q flag


## Concept: APSR Flags

The ARM Cortex-M3 processor maintains condition flags inside the APSR register:

| Flag | Bit | Meaning         |
| ---- | --- | --------------- |
| N    | 31  | Negative result |
| Z    | 30  | Zero result     |
| C    | 29  | Carry / Borrow  |
| V    | 28  | Overflow        |
| Q    | 27  | Saturation      |



## Program Explanation

### 1. GPIO Configuration

* Enables clock for GPIOA
* Configures PA8–PA12 as output pins using direct register access

### 2. Flag Clearing

```c
MSR APSR_nzcvq, R0
```

All flags are cleared before testing.



### 3. Z Flag Test

```assembly
SUBS a, b
```

* When `a == b`, result = 0 → Z flag set
* LED on PA9 turns ON



### 4. N Flag Test

```assembly
5 - 10 → negative
```

* Negative result sets N flag
* LED on PA8 turns ON



### 5. C Flag Test

```assembly
0xFFFFFFFF + 1
```

* Generates carry
* LED on PA10 turns ON



### 6. V Flag Test (Overflow)

```assembly
2147483647 + 1
```

* Signed overflow occurs
* LED on PA11 turns ON



### 7. Q Flag Test (Saturation)

```assembly
SSAT R1, #7, R0
```

* Saturation triggers Q flag
* LED on PA12 turns ON



## Key Functions

### LED Control

```c
void led_on(int pin);
void led_off(int pin);
```

### GPIO Initialization

```c
void select_mode_output();
```

### Delay Function

Simple software delay using loop.



## How to Run

1. Open the project in STM32CubeIDE
2. Build the project
3. Flash it to STM32F103C8T6
4. Observe LEDs corresponding to each flag



## Expected Output

| Condition  | LED  | Result |
| ---------- | ---- | ------ |
| Z flag set | PA9  | ON     |
| N flag set | PA8  | ON     |
| C flag set | PA10 | ON     |
| V flag set | PA11 | ON     |
| Q flag set | PA12 | ON     |



## Learning Outcomes

* Direct register manipulation in STM32
* ARM assembly integration with C
* Understanding low-level CPU flags
* Debugging embedded systems without high-level libraries



## Notes

* This project uses bare-metal register access, not STM32 HAL
* Ensure correct LED wiring to avoid hardware damage
* APSR flags are updated only by specific instructions (e.g., ADDS, SUBS)


## Course Information

* Course: CSE3232 Microcontroller Programming Lab
* Level: Undergraduate (CSE)


## Conclusion

This project provides a hands-on understanding of how ARM processors handle arithmetic conditions internally and how those conditions can be observed externally using GPIO.
