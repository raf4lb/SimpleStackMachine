# Simple Virtual Stack Machine
**Disclaimer**: despite being a project that works, it is still under development to add new features and improvements.

### Description
This is a simple virtual stack machine implemented in pure python. It has its own assembly language, compiler and the current instructions supported are:

##### Stack operations
- Push literal: push a value to the stack;
  - PSHL value
- Push: push a value from an address to the stack;
  - PSH address or $variable_name
- Pop: pop a value from the stack;
  - POP
- Pop address: pop a value from the stack and stores it in the address;
  - POPA address or $variable_name
- Top: print the value stored at the top of the stack;
  - TOP
##### Arithmetic operations
- Addition: add the two last values of the stack and stores the result at the top of the stack;
  - ADD
- Subtracttion: subtract the two last values of the stack and stores the result at the top of the stack;
  - SUB
- Multiplication: multiply the two last values of the stack and stores the result at the top of the stack;
  - MUL
- Division: divide the two last values of the stack and stores the result at the top of the stack;
  - DIV
##### Comparations
- Compare if equal: compares whether the penultimate value is equal to the last value and pushes the result in the stack;
  - CPE
- Compare if less: compares whether the penultimate value is less that the last value and pushes the result in the stack;
  - CPL
- Compare if greater: compares whether the penultimate value is greater that the last value and pushes the result in the stack;
  - CPG
- Compare if less equal: compares whether the penultimate value is less equal that the last value and pushes the result in the stack;
  - CPLE
- Compare if greater equal: compares whether the penultimate value is greater equal that the last value and pushes the result in the stack;
  - CPGE

##### Loop and Branching operations
- Jump: jump to address;
  - JMP address or .label_name
- Pop Jump If False: jump to the address or label if the value of the top of the stack is false;
  - PJIF address or .label_name

##### Other instructions
- Variable declaration: variables can be used in push (PSH) pop address (POPA) instructions;
  - VAR variable_name
- Label declaration: labels can be used in jump instructions (JMP, PJIF);
  - .label_name
- Halt: end the program;
  - HLT
- Delay (milliseconds): wait for milliseconds;
  - DLY milliseconds

### Requirements
 - Python 3.10;
 
### Installing
Clone this repo.

### Executing Programs
To execute a program in the VM, run the following command in the project root folder:
```
python main.py <path_to_program.rfl>
```
There are some commented programs examples in the ```programs``` folder. Example:
```
python main.py programs/for.rfl
```

### Project Structure
- ```programs``` folder contains program examples in the VM assembly code;
- ```alu.py``` contains the class that represents the arithmetic logic unit;
- ```compiler.py``` contains the compiler responsible for converting assembly code to VM bytecode;
- ```isa.py``` contains all VM instructions that can be easily extented using the ```Instruction``` abstract class;
- ```main.py``` contains the main script to compile and run the VM bytecode;
- ```memory.py``` contains classes related to memory elements like memory and stack;
- ```processor.py``` contains the class that represents the processor.

### Related Projects
There is an implementation of this VM in C, so the same assembly code can be executed even in baremetal microcontrollers like arduino with support to PIN manipulations (see atmega328p branch):

https://github.com/raf4lb/RFLVirtualStackMachineC
