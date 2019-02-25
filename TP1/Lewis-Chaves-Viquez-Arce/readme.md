# System Call Tracker (rastreador)

## Prerequisites

This is what you need to compile and run this project

- Linux / UNIX environment
- gcc compiler installed
- make installed

## User guide

To run this program follow the next steps:

1. Run the command `make` to generate an executable file called **rastreador**
2. Run `./rastreador [-v|-V] Prog [Prog options]` to execute the program
    - -v : Used to print a message everytime a system call is detected
    - -V : Works the same way as the '-v' option with the addition that the program will wait for a keypress to continue its execution

## Built With

* C

## What extra we added

* Utilities directory which contains 4 helper files
    * syscall.h/c : used to know the name of the system call being made
    * table.h/c : used to store and print the system calls information

## What doesn't work
Nothing, everything works

## Authors

* **Benjamin Lewis**
* **Jafet Chaves**
* **Jose Viquez**
* **Victor Arce**
