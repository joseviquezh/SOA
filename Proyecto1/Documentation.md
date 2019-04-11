# Proyecto 1: Threads lottery

## Prerequisites

This is what you need to compile and run this project

- Linux / UNIX environment
- gcc compiler installed
- make installed
- GTK 3 installed

## User guide

To run and use this program follow the next steps:

1. Run the command `sudo apt-get install libgtk-3-dev` to download the latest version of GTK 3
2. Run `make` to compile he program
3. Run `./main` to run the program
3. Set the parameters as you wish and then click on **Execute** to start the calculations
    - You need to select a thread from a dropdown menu and assign the number of tickets per thread
    - You need to use the "Enter" key in order to set the parameter modified in each text box

## Built With

* C

## What extra we added

* **timer.h/c**: located under timer/ are used to set and controll the timer and system calls for the quantum
* **gui.h**: used to define the interfaces that will be used by the main program to display the GUI
* **thread_scheduling.glade**: used to define the GUI elements

## What doesn't work
Sometimes, when using the Expropiative mode and a big work unit, the program loops and needs to be killed with ˆC

## Authors
* **Victor Arce**
* **Jafet Chaves**
* **Benjamin Lewis**
* **Jose Viquez**
