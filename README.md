# ReadMe for Time Measurement Programs in C

This repository contains two C programs designed for measuring the time elapsed between the execution of a parent and a child process. Both programs utilize different methods of inter-process communication: the first program uses pipes, and the second uses shared memory. The programs are structured to fork a child process and measure the time at which each process (parent and child) executes.

## File Descriptions

### 1. Pipe-Based Time Measurement Program

**Filename:** `time_measurement_pipe.c`

This program uses UNIX pipes for inter-process communication. It involves creating a pipe, forking a child process, and then using the pipe to communicate the child's timestamp to the parent process. After the parent process receives this timestamp, it calculates the elapsed time since the child process started.

**Key Features:**
- Forks a child process using `fork()`.
- Uses `gettimeofday()` to obtain the current time.
- Communicates time information between processes using a pipe.
- Calculates elapsed time using `timersub()`.

### 2. Shared Memory-Based Time Measurement Program

**Filename:** `time_measurement_shm.c`

This program utilizes POSIX shared memory for inter-process communication. It creates a shared memory segment, forks a child process, and then uses the shared memory to communicate the child's timestamp to the parent process. The parent process calculates the elapsed time after retrieving the child's timestamp from the shared memory.

**Key Features:**
- Forks a child process using `fork()`.
- Uses `gettimeofday()` to capture the current time.
- Manages shared memory using `shm_open()`, `ftruncate()`, and `mmap()`.
- Communicates time information between processes using shared memory.
- Calculates elapsed time using `timersub()`.


