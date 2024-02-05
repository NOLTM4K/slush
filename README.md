# slush: A Custom Shell Implementation

## Overview
Slush is a custom shell designed to enhance understanding of operating system processes and shell interaction. It allows users to execute commands and manage processes directly, providing a hands-on approach to learning the fundamentals of process management and shell functionalities.

## Demo
Include a demo video of Slush in action here. Example:
![Slush Demo](out.gif)

## Project Background
Developed as part of a computer science course, Slush focuses on the core aspects of creating and managing processes within a shell environment. It is implemented in C and showcases the ability to spawn new processes, handle user commands, and simulate a basic shell interface.

## Features
- Ability to execute standard shell commands.
- Process management including spawning and terminating processes.
- Custom command interpretation for educational purposes.

## Getting Started

### Prerequisites
- GCC compiler for compiling the C program.
- Make tool for automating the compilation process.

## Installation and Running the Application

### Compilation
Navigate to the project directory and compile Slush using the Makefile:
```bash
make
```
This command generates the executable `slush`.

### Running Slush
To start the shell, run:
```bash
./slush
```

## Usage Instructions
slush supports basic shell commands. Examples include:
- `ls` to list directory contents.
- `cd` to change directories.
- `./slush &` to run Slush in the background (demonstrates process spawning).

slush is a very simple command-line interpreter. It uses a different syntax than shells like bash, and has much more basic functionality. slush executes in a loop in which it displays a prompt, reads in a command line from standard input, and executes the command.

There are two types of commands: built in commands which are executed by slush itself, and program execution commands which are carried out by separate processes.

A built in command must appear on a line by itself. The only built in command is:

cd dir - change current directory to dir

Program execution commands have the form:

prog_n [args] ( ... prog_3 [args] ( prog_2 [args] ( prog_1 [args]

This command runs the programs prog_n, ... , prog_2, prog_1 (each of which may have zero or more arguments) as separate processes in a "pipeline", meaning the standard output of each process is connected to the standard input of the next.

The syntax of slush is backwards from shells you're used to, and is intended to emphasize the functional nature of pipeline commands. As an example, the command line:

more ( sort ( ps aux

produces a paginated, sorted list of processes.

slush catches ^C typed from the keyboard. If a command is running, this interrupts the command. If the user is entering a line of input, slush responds with a new prompt and a fresh input line.

slush will exit when it reads an end-of-file on input (Ctrl+D).

Syntax errors such as:

more ( ( ps au

more ( ps au (

are handled with an error message:

Invalid null command

## Authors
-Nolan Murphy