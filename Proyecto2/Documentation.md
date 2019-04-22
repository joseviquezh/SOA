# Project 2: Shared Memory: Producer - Consumer

## Prerequisites

This is what you need to compile and run this project

- Linux / UNIX environment
- gcc compiler installed
- make installed

## User guide

To test this project follow the next steps:

1. Run `make` to compile he program
2. The best way to test the project is through the test.sh script, just
specify the number of producers, consumers and the finalizer timer (how
much time in seconds before finalizer shuts down everything), below is an
usage example:

`./test.sh 1 12 10`

First argument is the number of producers, second argument the number of
consumers and the finally the finalizer timer.

Note: It's possible to test the project without the script. For that you
will need to execute creator, producer (as much instances as you like), 
consumer (as much instances as you like) and finalizer. You can refer to
the script source to understand how to test it manually. 

## Built With

* C

## What extra we added

* **utilities/**: These are different project components for the date 
(needed for the messages), Message struct, the random number generator
(needed for the messages) and the semaphore.
* **circ_buff.h/c**: Source code for the circular buffer implementation.

## What doesn't work
The ring buffer size is hardwired in circ_buffer.h in the macro
BUFFER_SIZE. It's not asked through the CLI.

## Authors
* **Victor Arce**
* **Jafet Chaves**
* **Benjamin Lewis**
* **Jose Viquez**
