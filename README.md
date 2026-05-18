# Autonomous Trumpet

This is the code for an automated trumpet project. It's written in C and built to run on an STM32F401RE microcontroller. 

Right now, the main focus is on motor control (check out the `motors` branch and launch files). 

## System Requirements
This system must physically actuate standard 18mm trumpet valves at frequencies up to 12 Hz. The electronics and firmware require an STM32 MCU capable of low-latency, multi-motor timing and processing real-time note sequencing via UART.

## Demo
https://github.com/user-attachments/assets/e8e194a6-651d-4421-bd73-05808591c485

## Hardware
* STM32 Nucleo-F401RE development board
* Motors and drivers (needs updating with exact parts used)

## How to use it
This project was set up using STM32CubeIDE.

1. Clone the repository: `git clone https://github.com/Khalil-Burns/autonomous-trumpet.git`
2. Open STM32CubeIDE.
3. Go to `File > Import > General > Existing Projects into Workspace`.
4. Select the folder you just cloned.
5. Plug in your STM32 board via USB, hit build, and run it.

## Structure
* `/Src/` - Main application code
* `motors.launch` - Launch configuration for debugging the motors
