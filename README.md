Multi-Threaded Producer-Consumer Simulator

A concurrent real-time simulation of the classic Producer-Consumer synchronization problem, implemented in C using POSIX threads, mutexes, and semaphores. The project includes an interactive terminal-based interface built with ncurses for live monitoring of thread activity and buffer operations.

Features
Concurrent execution of multiple producer and consumer threads
Bounded buffer implemented using a circular queue
Thread synchronization using mutex locks and semaphores
Prevention of buffer overflow and underflow conditions
Adjustable production and consumption speeds
Real-time terminal visualization with color-coded buffer states
Live per-thread statistics and activity tracking
Timestamped event logging to both terminal and log file
Safe and graceful shutdown using Ctrl+C handling
Technologies Used
Programming Language: C
Threading Library: POSIX Threads (pthreads)
Synchronization: Mutexes and Semaphores
Terminal Interface: ncurses
Compiler & Build Tools: GCC and Makefile
Platform: Linux
Installation & Execution
Install Required Libraries
sudo apt-get install libncurses5-dev libncursesw5-dev
Build the Project
make
Run the Simulator
./simulator

To terminate the simulation, press Ctrl+C.
All runtime activities will be stored in simulation.log.

Project Directory Structure
src/main.c        - Program entry point and ncurses interface
src/buffer.c/h    - Shared bounded buffer implementation
src/producer.c/h  - Producer thread functionality
src/consumer.c/h  - Consumer thread functionality
src/logger.c/h    - Logging system for events and activities
src/stats.c/h     - Thread statistics management
Learning Objectives

This project demonstrates core Operating Systems concepts including:

Process and thread synchronization
Critical section management
Semaphore-based coordination
Mutex locking mechanisms
Concurrent programming and resource sharing
Real-time system monitoring
Course Information

Course: Operating Systems
