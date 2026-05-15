# Autonomous Trumpet

This is the code for an automated trumpet project. It's written in C and built to run on an STM32F401RE microcontroller. 

Right now, the main focus is on motor control (check out the `motors` branch and launch files). 

# Demo
<video src="automatic_trumpet_demo.mp4" controls="controls" width="600" />

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
