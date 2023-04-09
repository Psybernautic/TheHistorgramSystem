# The Historgram System


The Histogram System is a multi-component system implemented in ANSI C for Linux. It consists of three distinct processing components: a "data consumer" (DC) application, and two different "data producer" (DP-1 and DP-2) applications. The system uses shared memory, semaphores, and signals for inter-process communication.

# Installation
To install The Histogram System, clone this repository to your local machine and compile the source code using make:

$ git clone https://github.com/your-username/histo-system.git
$ cd histo-system
$ make

This will compile the three components of the system (DC, DP-1, and DP-2) and place the executables in the bin directory.

# Usage
To use HISTO-SYSTEM, run the DP-1 executable to start the data producer process:

$ ./bin/dp1

This will launch DP-1, which will create and initialize the shared memory segment, launch DP-2 and pass it the necessary command line arguments, and then begin writing random letters to the circular buffer.

To start the data consumer process, run the DC executable in a separate terminal:

$ ./bin/dc <shm_id>

Replace <shm_id> with the shared memory ID that was output by DP-1 when it started. This will launch the DC process, which will consume data from the circular buffer and output a histogram of the frequency of each letter.

To stop any of the processes, press Ctrl-C in the terminal where the process is running. This will send a SIGINT signal to the process, which will detach from the shared memory and exit.


# License
The Histogram System is released under the MIT License.

# Credits
The Histogram System was created by Sebastian Posada.


