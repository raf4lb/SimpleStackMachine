# Virtual Machine Project

This project implements a Virtual Machine (VM) with support for a variety of features including basic data types, function calls, recursion, hardware manipulation, and asynchronous task management. The VM is capable of executing concurrent tasks, supporting task messaging, and handling imports, delays, and memory usage.

## Features

The VM currently supports the following features:

Data Types:
- Unsigned 16-bit integers (u16)
- Signed 16-bit integers (i16)
- 32-bit floating-point numbers (f32)
- Arrays
- Constants
- Local Variables

Control Flow:
- Loops
- Branching

Functionality:
- Function Calls
- Recursion

Memory Management:
- Memory Allocation
- Memory Usage tracking

Control:
- Delay support (delay)
- Hardware manipulation (Arduino-specific)

Output:
- Formatted print

Asynchronous Tasks:
- Support for concurrent tasks
- Async tasks can send messages to each other

### Future Feature (Planned):

Messaging between tasks on different hosts via Internet sockets (currently under development)

## Architecture

The VM is designed with a custom Instruction Set Architecture (ISA) tailored for simplicity, portability, and concurrency. It supports asynchronous task execution and inter-task messaging, making it capable of running concurrent code efficiently—even on resource-constrained devices.

One of the key strengths of this VM is its lightweight footprint:

- On an Arduino Uno (2KB SRAM), the VM runs with just 178 bytes of RAM.

- This memory usage is customizable — you can adjust the VM stack size to fit different resource requirements or application needs.

This minimal footprint allows the VM to operate effectively on microcontrollers with very limited memory, while still supporting advanced features like async tasks, recursion, and messaging.

## Setup and Installation

Clone the repository.

Navigate to the src folder to view the source code.

The VM can run on multiple platforms, including Arduino, macOS, and Windows. Some examples may only work on Arduino Due due to hardware manipulation.

## Requirements

To run and compile the project, the following software/tools are required:

### 1. Python 3.11

The project requires Python 3.11 or higher to run the compiler.

### 2. Compilers for Target Platforms

Depending on the platform you wish to compile for, you'll need the appropriate compiler:

### macOS:

clang is required for compiling the code on macOS.

### Windows:

gcc is required for compiling code on Windows.

### Arduino Uno R3:

avr-gcc is required for compiling code for the Arduino Uno R3.

Install it by setting up the Arduino IDE, or directly install avr-gcc via package managers like brew on macOS.

## Build and Run

You can build and run programs on different target platforms using the provided Makefile. The build command has the following format:

`make <target_platform> PROGRAM_FILE=programs/<program.rfl>`

### Available Target Platforms

`arduino` — Build for Arduino boards (e.g., Arduino Uno, Arduino Due)

`macosx` — Build for macOS systems

`windows` — Build for Windows systems

### Output

The compiled program is self-contained with no external dependencies and will be placed in the build folder.

### Running the Program

On macOS and Windows, simply run the compiled executable from the command line:

`$ ./build/<target_platform>`

On Arduino, follow your usual procedure to flash the compiled binary onto the device or use the following [Arduino Emulator](https://wokwi.com/projects/434796710305388545).

#### Example Usage

To build and run a program named example.rfl on macOS:

```
$ make macosx PROGRAM_FILE=example.rfl
$ ./build/macosx
```

## Code Examples

Example programs are located in the programs/ folder. Most examples include inline comments explaining how they work. Comments in the source code use the ; character.

- programs/hello_world.rfl
- programs/datatypes.rfl
- programs/array.rfl
- programs/local_variables.rfl
- programs/recursion.rfl
- programs/fibonacci.rfl
- programs/async_tasks.rfl

## Running on Emulator

You can run the compiled Arduino program on an emulator by following these steps:

Go to the Wokwi emulator project page:
https://wokwi.com/projects/434796710305388545

Upload the compiled `arduino.hex` file:

- Press F1 to open the command palette.

- Select Upload Firmware and Start Simulation...

- Choose the .hex file located in the project root folder: `build/arduino.hex`

The simulation will start, and you can observe the program running on the virtual Arduino board.

### Example: Async Task Communication (Recommended for Emulator)

A great program to test running on the emulator is the Async Task Communication Example:

`$ make arduino PROGRAM_FILE=programs/async_tasks_intercom.rfl`

The main task spawns an asynchronous task (task_1) and then enters a loop where it continuously monitors a push button.

task_1 runs concurrently: it periodically prints the VM’s current memory usage and waits for incoming messages.

When the push button is pressed, the main task sends a message to task_1.

Upon receiving the message, task_1 toggles an LED connected to pin B5.

This example demonstrates:

- Concurrent execution of async tasks

- Task-to-task messaging

- Hardware interaction (button and LED control)

It’s ideal for verifying async task support and messaging functionality on the Wokwi emulator or actual Arduino hardware.

## Compiler Implementation

The compiler was developed in Python 3.11 primarily for rapid prototyping and ease of experimentation. It currently serves as the toolchain's first-stage compiler, translating a low-level assembly-like language into the VM's custom bytecode format, which is then executed by the virtual machine.

## Current State

The VM does not yet have a high-level language — all programs must be written in its low-level assembly language, which closely resembles the VM’s custom instruction set.

The Python compiler is responsible for:

- Parsing and validating the assembly code.

- Converting it into the correct bytecode instructions.

- Handling labels, constants, memory allocation, and task declarations.

## Limitations & Future Improvements

While functional, the compiler is still early-stage and has significant room for improvement, like code optimizations, performance, error handling, improved syntax error messages, modularity and extensibility.

## Bootstrapping Vision

In the long term, the goal is to design and implement a higher-level language for the VM. Once this language is mature enough, the compiler itself will be rewritten in that high-level language, following the classic bootstrapping approach.

## License and Usage

⚠️ This project is proprietary.

All source code, tools, and documentation included in this repository are the intellectual property of the author and are provided for reference and personal use only.

Reproduction, redistribution, modification, or commercial use is not permitted without explicit written permission from the author.

Do not copy, publish, or reuse any part of the codebase in other projects—whether open source or commercial—without prior authorization.

If you’re interested in collaboration, licensing, or have questions about usage, please contact the author directly.