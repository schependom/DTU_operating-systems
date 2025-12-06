# Operating Systems Exam

This document contains a set of questions and answers related to the course 02159 Operating Systems.

## Includes

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
```

## Manual

If you type `man printf`, you will likely get the manual for the Terminal command printf, not the C function. The manual is divided into "Sections" to solve this.

-   Section 1: User Commands (Terminal commands like ls, cd, mkdir)
-   Section 2: System Calls (Kernel functions like fork, pipe, open)
-   Section 3: Library Functions (Standard C library like printf, strcpy, malloc)

To see the C function details, specify the section:

```
man 2 fork
man 3 printf
```

## The principle of locality

A programme accesses a relatively small portion of the address space at any instant of time.

**Temporal locality:** (locality in time): if an item is referenced, it will tend to be referenced again soon.

**Spatial locality:** (locality in space): if an item is referenced, items whose addresses are close to it will also be referenced soon.

## Controllers

Mini computer chip that physically controls the (I/O) device on behalf of the computer. It has its processor, registers and memory. It accepts commands from the OS in the form of interrupts (TODO).

### Accessing memory/registers of I/O device

Two ways:

1. Map the memory/registers of the I/O device to the process's address space.
2. Communicate via special in/out instructions:
    - busy waiting
    - interrupt-based
    - DMA (Direct Memory Access)

## Device drivers

A piece of software in the OS that talks to a controller.

TODO

## Buses

Communication lines between parts of the computer.

-   **Serial Bus**: 1 bit
-   **Parallel Bus**: One word (4 bytes = 32 bits) at a time

Either

-   **Dedicated**: exclusively for two components
-   **Shared**: multiple devices share it

## Modes of operation

### Kernel Mode

-   Cpu can execute **every** instruction and can use **all** hardware

### User Mode

-   Cpu can execute only a **limited** set of instructions
-   Make **system call** to access protected resources.
    -   CPU switches to **kernel mode** (trap)
    -   OS handles the request
    -   CPU switches back to **user mode**

## Stability vs Efficiency in different System Types

### 1. Monolithic OS

_E.g. Linux_

-   Entire OS runs as a **<u>single</u> programme** in **kernel mode**
-   User programmes run in **user mode**
-   User programmes access OS services via **system calls**

Advantage: very efficient
Disadvantages:

-   A bug in one OS service can take down the entire system (e.g. driver bug)
-   System extensions require rebuilding the kernel

### 2. Layered Systems

-   Organised as a hierarchy of layers
-   Inner layers are more privileged than outer layers

Advantage: High flexibility.
Disadvantages:

-   Low efficiency (multiple system calls from outside to inside)
-   Defining layers is not straightforward

### 3. Microkernel

-   Keep kernel very small
-   All non-essential services of the **OS** run in **user mode**
-   Communication between non-essential OS services via **message passing** (e.g. pipes)

Advantage: high stability (>< driver bug in Monolithic OS) and high flexibility (no need to rebuild the kernel for extensions like in Monolithic OS)
Disadvantage: high overhead from message passing

## System Call

-   How user programmes interact with the kernel.
-   They return **status codes**

## POSIX

Portable Operating System Interface = POSIX

-   Standardised API for mainting compatibility between operating systems.
-   POSIX-certified: macOS, Solaris
-   POSIX-compliant: Linux

## Process

A process is **a program in execution**

-   Address space (virtual memory)
-   Other resources:
    -   Registers
    -   Open files
    -   ...
-   Container that holds all the informatino needed to execute a programme
-   Unqiue process identifier (PID)
-   An owner (parent process)

### Sequential Process

The CPU runs one process at a time and maintains a **program counter** to keep track of the current instruction.

Each process has its own **logical program counter** and **virtual CPU**

### Process Termination

1. **VOLUNTARILY** via `exit()`
2. **INVOLUNTARILY** via `kill()` or **fatal error**

### Process States

-   **RUNNING**: Actively using CPU
-   **READY**: Runnable, but suspended
-   **BLOCKED**: Unable to run until some external event (e.g. I/O)
-   **ZOMBIE**: Terminated, but parent has not reaped (via `wait()`)

Go from one state to another using process **state TRANSITION**.

### Bursts and Boundings

A process **alternates** between **bursts** of computing and I/O requests.

-   **CPU-bound** process spends most of its time **computing** (executing instructions)
-   **I/O-bound** process spends most of its time **waiting**

## Scheduling

Multiple **READY** state processes compete for the CPU to transition to **RUNNING** state.
The **scheduler** makes the decision on which process to run...

-   ... (A) after a **fork**
-   ... (A) after the running process **voluntarily** `exit()`s
-   ... (A) when a process gets **blocked** waiting for I/O
-   ... (A) when a process **voluntarily** `yield()`s (Cooperative Scheduling)
-   ... when a process gets **unblocked** -> ready

The (A)'s above indicate when the **CPU becomes Available**.

### Scheduler = a process

The scheduler is a **process itself** and thus **needs to use the CPU** to make decisions.

> How does the scheduler (a process) get the CPU from a running process to choose the next one?

## Preemtive Scheduling

Use **(HARDWARE!) clock interrupts** to limit process time and give the CPU to the scheduler.

### Round-robin

Each process is allowed to run up to some fixed time.

-   All ready processes in a FIFO queue
-   Scheduler defines **time quantum**
-   Quantum exceeded or process blocks:
    -   process is **stopped**
    -   process moved to **end of queue**

How to choose time quantum? $\Rightarrow$ **EFFICIENCY vs DELAY**

-   Short quantum:
    -   High overhead
    -   Short delay
-   Long quantum:
    -   Low overhead
    -   High response time (delay)

### Real-Time processes

Real-time processes do not get **preempted** by lower priority processes.

## Non-preemtive Scheduling = Cooperative Scheduling

Let a process run until:

-   it **blocks** (for I/O)
-   it `exit()`s
-   it voluntarily `yield()`s

### 1. First-come, first-served (FCFS)

-   CPU is given to the processes in the order they request it.
-   Implement using FIFO queue.

Fair? Short jobs wait for long jobs...

### 2. Shortest job first (SJF)

-   Scheduler selects shortest process first
-   Minimize **mean turnaround time** = time from submission to completion

Problem: hard to know running times + long jobs may never run

## Scheduling Algorithms

### Priority Scheduling

#### High Priority Processes

-   Critical system-level processes
-   Interactive processes (user expectations)
-   I/O-bound processes

#### Mechanism

-   Each process gets **priority number**
-   Scheduler selects process with **highest priority**
-   Higher priority -> longer quantum
-   Voluntarily decrease priority level with `nice()`

Set priority level to $1/f$ where $f$ is the % of quantum used.

#### Priority Groups (PG)

-   Round Robin in PG4 (with FIFO queue)
-   If PG4 is empty, round-robin in PG3
-   If PG3 is empty, round-robin in PG2
-   If PG2 is empty, round-robin in PG1

### Lottery Scheduling

-   Each process gets a number of **lottery tickets**.
-   Next process is selected randomly.
-   Cooperative processes may exchange tickets.

### Fair-Share Scheduling

-   Processes have **owners**
-   In multi-user environments, we want fairness at user level.

## Fork

A clone of a process

-   Different (virtual) addres space
-   Exact same contents
-   Different PID
-   Different parent PID

This is the only way a process can be created.

## Communication Between Child and Parent

1. Shared memory with `mmap()`
2. Pipes with `pipe()`
3. Listening to exit status with `wait()`

## Signals

-   Software-based interrupts
-   Way to communicate between processes
-   Interrupt normal execution to **run handler function**
-   Default signal handler is executed unless custom handler is registered
-   Some signals, like SIGKILL, cannot be caught or ignored

## Message Passing (Computer Networking)

-   Another way to communicate
-   Exchange messages
    -   between process within the same machine
    -   between process on different machines

```c
send(destination, &message)
receive(source, &message) // blocks until it receives a message
```

-   No race conditions
-   But new problems:
    -   What if messages don't arrive?
    -   What if messages arrive out of order?

## Executing a new programme

**REMEMBER**: only `fork()` can create a new process.
To execute a new programme, we take two steps:

1. `fork()` -> create a clone child process
2. `exec()` -> replace core image of child process with new programme

## Process table

A list of all the processes

-   **Active**
-   **Suspended**
-   **Terminated** until reaped

Reaping is when the parent process calls `wait()` to retrieve the exit status of the child process.
That's when the process is removed from the process table.

## Files

A file is a **data container** on the **disk** (but not only that, it can also be a pipe, a socket, etc.).
Directories gropu files together.
Files are organised in **trees**

## File descriptor

Each open file (or pipe, socket, etc.) is assigned a **file descriptor**.
This is a **handle** to access the file.

> 'Everything in UNIX is a file(descriptor)'

UNIX has special files:

-   keyboard, screen
-   hard drives, cd-roms, serial ports, IO devices
-   system and hardware properties (e.g. CPU temperature)
-   modems, networking interfaces
-   pipes

## `pid` and `ppid`

-   `pid` will change if the same binary is executed multiple times, as each execution creates a new process with a unique process identifier.
-   `ppid` is the `pid` of the parent. When running the same binary multiple times, the `ppid` will remain the same if run from the same shell or parent process.

## The `init()` Process

-   The `init()` process is the first process started by the kernel during the booting of the system.
-   `pid` of `init()` is always `1`.

## `malloc()`

-   `malloc()` allocates memory on the **heap**.
-   Memory allocated with `malloc()` persists until it is explicitly freed using `free()`, or until the program terminates.

## Forking a Child Process

For is the **ONLY** way to create a new process, so all processes are descendants of `init()`.

```c
// Fork a child process and store its PID in a variable
pid_t pid = fork();

if (pid < 0) {
    // Fork failed
} else if (pid == 0) {
    // Child process
} else {
    // Parent process
}

// Everything outside the if-else is executed by BOTH processes
```

-   The child process is an **exact copy** of the parent process
-   Child and parent have **different address spaces**:

    ```c
    int *A

    int main(void) {

        A = malloc(sizeof(int));
        *A = 10;
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            printf("Child: %d", *A); // 10
            *A = 20;
            printf("Child after change: %d", *A); // 20
        } else {
            // Parent process
            sleep(1); // Ensure child runs first
            printf("Parent: %d", *A); // 10, because different address space
        }

        return 0;
    }

    ```

-   The `fork()` system call returns:
    -   **`0` to the child process**
    -   The child's `pid` to the parent process
    -   **`-1` if the fork failed**

For efficiency, the OS may use **copy-on-write** to share memory between the parent and child processes, and only copy the memory when it is modified.

## Communication between Parent and Child

### 1. Shared Memory

```c
int *A;

// inside main()
A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

// 1. NULL                          - kernel will choose the address
// 2. sizeof(int)                   - size of the mapping
// 3. PROT_READ | PROT_WRITE        - protection flags
// 4. MAP_SHARED | MAP_ANONYMOUS    - mapping flags
// 5. -1                            - file descriptor
// 6. 0                             - offset
```

This creates a memory mapping from the process's address space to a region of shared memory that can be accessed by both the parent and child processes.

Returns generic pointer (`void*`), which is then cast to the desired type (here, `int*`), and points to the start address of the shared memory region.

Sharing memory introduces the risk of **Race Conditions**.
Therefore, we `wait()` for the child process to finish before accessing the shared memory:

```c
if (pid == 0) {
    // --- CHILD PROCESS ---
    printf("Child: Writing 42 to shared memory...\n");
    *shared_memory = 42; // This write is visible to the parent!
    exit(0);
} else {
    // --- PARENT PROCESS ---
    // Wait for the child to finish so we don't read too early (Race Condition prevention)
    wait(NULL);

    printf("Parent: Read value from shared memory: %d\n", *shared_memory);

    // 3. Clean up
    munmap(shared_memory, sizeof(int));
}
```

Another way of preventing such issues is to use **mutexes** or **semaphores**.

### 2. `waitpid()`

Waiting for the return code of a child process is a way to get information from it.

```c
// Fork a child process
pid_t pid = fork();

if (pid == 0) {
    // Child process
    exit(23); // Exit with return code 23
} else {
    // Parent process
    int child_status;
    waitpid(pid, &child_status, 0); // Wait for child to terminate
    int return_code = WEXITSTATUS(child_status);

    printf("Child exited with return code: %d", return_code); // 23
}
```

### 3. Pipes

## Zombie Processes

A parent process **must** `wait()` for its child processes to terminate.

### Orphan Process

An orphan process is a process whose **parent has terminated.**

### Zombie Process

-   A zombie process is a child process that has **terminated**
-   But **parent has not waited** for it to terminate
-   So it **remains in the process table**

### How to wait

Waiting can be done with:

-   `wait()`
    -   Waits for **any** child process to terminate
-   `waitpid()`
    -   Waits for a **specific** child process to terminate

Both functions remove the terminated child process from the process table, preventing zombie processes.

## Signals

Signals are software-based interrupts: interrupt normal execution to run a handler function.

### Common Signals

Some signals _can_ be caught or ignored by the process (like `SIGINT` and `SIGTERM`)
Some signals **cannot** be caught or ignored by the process (like `SIGKILL` and `SIGSTOP`)

-   `kill [pid]` in terminal:
    -   Sends `SIGTERM` to process with id `[pid]`
-   `kill -9 [pid]` in terminal:
    -   Sends `SIGKILL` to process with id `[pid]`
    -   **`SIGKILL` cannot be caught or ignored**
-   `CTRL+C` in terminal:
    -   Sends `SIGINT` to the foreground process
-   Closing terminal:
    -   Sends `SIGHUP` to all processes in the terminal's process group

### Handlers

The default signal handler is executed, unless a custom handler is set up.

```c
#include <signal.h>

signal(SIGINT, handler);
signal(SIGTERM, handler);
```

## Alarms

An alarm schedules a `SIGALRM` signal to be sent to the process after a specified number of seconds.

```c
signal(SIGALRM, handler); // Set up handler for SIGALRM
alarm(5); // Schedule SIGALRM to be sent in 5 seconds
```

## Executing programs

### 1. `fork()`

The parent process calls `fork()` to create a clone child process.

### 2. `exec()`

The parent process calls `execl()` to replace its code with another executable.

```c
// Execute the "ls -1" command
// execl(const char *path, const char *arg, ..., (char *)0);
// The (char *)0 marks the end of the arguments
execl("/bin/ls", "ls", "-1", (char *)0);

// Only executed if execl fails
printf("execl failed");
```

-   Succes:
    -   `xecl` doesn't fail
    -   the the child process image is replaced by the new program
    -   the **lines after `execl` are not executed.**
-   Failure:
    -   `execl` returns `-1`
    -   the child process continues executing the lines after `execl`.

## File Statistics

The `stat()` system call retrieves information about a file, such as its size, permissions, and timestamps.

```c
#include <sys/stat.h>

struct stat fileStat;
if(stat("filename", &fileStat) < 0)
    return 1;

printf("File size:                %lld bytes", (long long)fileStat.st_size);
printf("Last file access:         %s", ctime(&fileStat.st_atime));
printf("Last file modification:   %s", ctime(&fileStat.st_mtime));
```

## File Descriptors

A file descriptor (FD) is a non-negative integer that uniquely identifies an open file within a process.

| FD  | Description              | Code            |
| --- | ------------------------ | --------------- |
| 0   | Standard Input (stdin)   | `STDIN_FILENO`  |
| 1   | Standard Output (stdout) | `STDOUT_FILENO` |
| 2   | Standard Error (stderr)  | `STDERR_FILENO` |

```c
int fd = open("file.txt", O_RDONLY); // or O_WRONLY, O_RDWR, O_CREAT

if (fd < 0) {
    // Handle error
}

dprintf(fd, "Hello, World!"); // Write to file using file descriptor
```

`O_CREAT`: create the file if it does not exist.

## Pipes

A pipe is a **unidirectional data channel** (special file) that can be used for **interprocess communication**. It connects the output of one process to the input of another process.

```c
int link[2]; // store the read and write FD's in a 2-element array

// Read-end is link[0]
// Write-end is link[1]

// pipe buffer of 4096 bytes
char buf[4096];

if (pipe(link) == -1) {
    // Handle error
}

// --------------------------- PARENT PROCESS ---------------------------- //

close(link[0]);
// write data to the pipe
char msg[] = "Hello, World!";
write(link[1], msg, sizeof(msg)); // size of msg in bytes + null terminator (1 byte)

// --------------------------- CHILD PROCESS ---------------------------- //

close(link[1]);
// read data from the pipe
int nbyes = read(link[0], buf, sizeof(buf));

waitpid(-1, NULL, 0); // wait for child to finish
```

## Duplicating File Descriptors

```c
int saved_stdout = dup(STDOUT_FILENO); // save current stdout
dup2(link[1], STDOUT_FILENO); // Copy pipe write-end to stdout
// -> Everything written to stdout goes to pipe

// Restore original stdout
dup2(saved_stdout, STDOUT_FILENO);
close(saved_stdout); // close the saved fd
```

## Threads

Multithreading is the pseudo-parallel execution of multiple threads within a process. CPU is able to hold the state of two execution threads and switch between them super fast

```c
phtread_t thread_id;
pthread_create(&thread_id, NULL, thread_function, arg);
// 1. &thread_id: pointer to thread identifier
// 2. NULL: default thread attributes
// 3. thread_function: function to be executed by the thread
// 4. arg: argument to be passed to the thread function

pthread_join(thread_id, NULL); // Wait for thread to finish
// 1. thread_id: identifier of the thread to wait for
// 2. NULL: we don't care about the return value of the thread

// inside thread_function
void *thread_function(void *arg) {
    // Thread code here
    pthread_exit(NULL); // Exit the thread
    // 1. NULL: no return value
}
```

### Thread Scheduling

**1. USER-LEVEL THREADING**

-   Process scheduler gives quantum to **process**
-   Process has thread scheduler that gives quantum to **threads**
-   Thread scheduling is **non-preemptive**

**2. KERNEL-LEVEL THREADING**

-   Process scheduler gives quantum to **threads** (not processes!)
-   The (thread) scheduler knows that switching threads **within** a process is much faster than switching threads of different processes

## Ways to communicatate

### 1. Shared Memory

-   Global variables (threads only)
-   Heap memory (threads only)
-   Files (all processes)

### 2. Messages and Data Streams

-   Signals
-   Pipes
-   Sockets (UDP, TCP, UNIX = local)

## Race Conditions

The outcome depends on the timing of process/thread execution (scheduling).
It leads to non-deterministic behavior.

### Critical Region

The parts of the programme where the shared memory, shared file, or other shared resource is accessed

### Mutual Exclusion

-   Avoid race conditions
-   When one process/thread is in the critical region, no other process/thread can enter the critical region
-   Locking (or acquiring) and unlocking (or releasing) the resource

### Problem

-   The process can be preempted between the instruction that checks the lock variable and the instruction that sets the lock variable
-   If checking and setting were indivisible or atomic, all would be fine!

### SOL1: Disabling interrupts

-   No clock interrupts, no preemption, no race conditions
    ```c
    disable_interrupts();
    // critical region
    enable_interrupts();
    ```
-   **Only in <u>single</u>-processor systems**

### SOL2: ATOMIC INSTRUCTIONS

-   `TSL` = Test and Set Lock = assembly instruction
-   Indivisible or atomic
-   The CPU executing TSL **locks the memory bus** to prevent other CPUs from accessing memory

## Spinlock uses Busy Waiting

-   **Busy waiting** = continuously testing a lock
-   Wastes CPU cycles
-   Lock that uses busy waiting is called a **spinlock**

> Can be useful in **multi-core** systems in cases of **very short waiting times**

## Mutual Exclusion opt. 1: Mutex

-   A **mutex** is a **safe lock** provided by the Operating System, based on **atomic instructions**.
-   No busy waiting:
    -   The process is **blocked** if locked.
    -   When the lock is released, the mutex makes a system call.
    -   A waiting process is picked from the mutex's queue.

### a) WIHTIN a process

Protect shared variables among threads of the same process:

-   **Global** variables (1 process = 1 virtual address space)
-   **Heap** memory (1 process = 1 heap)

### b) BETWEEN processes

Protect shared variables among processes:

-   Shared variables from **shared memory** (`mmap`)
-   **Files**

Note that the mutex **must be allocated in the shared memory**:

```c
// define mutex in shared memory
pthread_mutex_t *mutex;
mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

int pid = fork();

if (pid == 0) {
    // child process
    pthread_mutex_lock(mutex);
    // critical region
    pthread_mutex_unlock(mutex);
} else {
    // parent process
    pthread_mutex_lock(mutex);
    // critical region
    pthread_mutex_unlock(mutex);
}
```

### Deadlock

-   A waits for B, B waits for A
-   Lock resources in the same order (based on bank account id for example)

    ```c
    void send_money(int a, int b, int amount) {
        int first, second;

        // 1. Determine the order based on Account ID
        if (a < b) {
            first = a;
            second = b;
        } else {
            first = b;
            second = a;
        }

        // 2. Lock resources in the specific order
        mutex_lock(&mutex[first]);
        mutex_lock(&mutex[second]);

        // --- CRITICAL REGION ---
        // (Perform the transfer)
        account[a] -= amount;
        account[b] += amount;

        // 3. Unlock (Order doesn't matter for preventing deadlock here)
        mutex_unlock(&mutex[second]);
        mutex_unlock(&mutex[first]);
    }
    ```

## Mutual Exclusion opt. 2: File Locks

```c
FILE *file;

// lock file before accessing
lockf(fileno(file), F_LOCK, 0); // Lock the file
// 1. fileno(file):     get file descriptor from FILE*
// 2. F_LOCK:           lock operation (can be F_LOCK, F_ULOCK, F_TLOCK, F_TEST)
// 3. 0:                lock the entire file (can be a byte range)

// If lockf fails, the process is BLOCKED until the lock can be acquired

// --- ACCESS FILE --- //
FILE *file = fopen("data.txt", "r+"); // read or write to the file and create it if it doesn't exist
fscanf(file, "%d", &data); // read data from file
// file: pointer to FILE*
// "%d": format string for reading an integer
// &data: pointer to variable where the read integer will be stored

// unlock file after accessing
lockf(fileno(file), F_ULOCK, 0); // Unlock the file
// 1. fileno(file):     get file descriptor from FILE*
// 2. F_ULOCK:          unlock operation
// 3. 0:                unlock the entire file (can be a byte range)
```

## File permissions

File permissions have the form of a **bitmap** of 9 digits.

```txt
3 bits for owner, 3 bits for group, 3 bits for others

owner | group | others
 xxx     xxx     xxx
 |||     |||     |||
 rwx     rwx     rwx

And translate to octal:
 r = 4
 w = 2
 x = 1

What does 0644 mean?
 Owner:     rw-     = 4 + 2 + 0     = 6
 Group:     r--     = 4 + 0 + 0     = 4
 Others:    r--     = 4 + 0 + 0     = 4
```

```c
fopen("file.txt", string);
// "r"   : read only
// "w"   : write only (create file if it doesn't exist, truncate if it does)
// "a"   : append (create file if it doesn't exist)
// "r+"  : read and write
// "w+"  : read and write (create file if it doesn't exist, truncate if it does)
// "a+"  : read and append (create file if it doesn't exist)
```

## Thread mutexes

```c
pthread_mutex_t lock;

pthread_mutex_init(&lock, NULL); // Initialize mutex
// 1. &lock: pointer to mutex
// 2. NULL: default mutex attributes

pthread_mutex_lock(&lock); // Lock the mutex
// -- ACCESS CRITICAL REGION --
pthread_mutex_unlock(&lock); // Unlock the mutex
```

## Producer-Consumer problem

-   Producer puts items into buffer
-   Consumer takes items out of buffer

Signaling

-   Buffer full $\rightarrow$ producer goes to sleep
-   Empty slot appears $\rightarrow$ consumer wakes up producer
-   Buffer empty $\rightarrow$ consumer goes to sleep
-   Item consumed $\rightarrow$ producer wakes up consumer

## Semaphore

An integer $\geq 0$.

`down`:

-   If $>0$, semaphore is **decremented** and the process can continue.
-   If $=0$, semaphore is **blocked** until it becomes $>0$.

```c
#define N 10
typedef int semaphore;

semaphore mutex = 1; // binary semaphore
semaphore empty = N; // counting semaphore
semaphore full = 0; // counting semaphore

void producer() {
    // produce
    while (true) {
        down(&empty);           // decrement empty semaphore
        down(&mutex);           // enter critical region
        // critical region
        up(&mutex);             // exit critical region
        up(&full);              // increment full slots
    }
}

void consumer() {
    // consume
    while (true) {
        down(&full);            // decoupled from up(&full)!
        down(&mutex);
        // critical region
        up(&mutex);
        up(&empty);
    }
}
```

## Key distinction mutex/semaphore

-   **Mutex**:
    -   used for mutual **exclusion**
    -   only one process/thread can access a critical region at a time
    -   the lock is **owned** by the process that acquired it
    -   only the owner can **up** the mutex
    -   lock (`down`) and unlock (`up`) are **coupled**
-   **Semaphore**:
    -   used for **signaling**/**synchronization**
    -   when you want to sleep until other process/thread wakes you up
    -   the lock is **not** owned by a process
    -   typically, _another_ process/thread is going to `up` the semaphore
    -   sleep (`down`) and wake-up (`up`) are **decoupled**

## Address Space

-   The set of addresses a process can use.
-   When memory is abstracted, programmes do not directly see the physical memory.
-   This is as opposed to
    -   No memory abstraction (embedded systems, smart cards)
    -   Memory Protection Keys (static relocation needed)

## Dynamic relocation

Two registers needed: - **Base** register - **Limit** register

For every memory reference, one extra **addition** and **comparison**:

1. CPU adds base value to memory reference
2. CPU compares result with limit value for protection

## Memory Overload

-   When there is not enough RAM (= main memory) to keep all running processes in memory
-   We need **swapping**
-   Store the address space of processes on the **disk**
-   Base/limit registers need to be adjusted to new memory location when moved away and back again

### Managing free memory

Memory is divided into **allocation units**.

1. **BITMAP**
    - For each unit: `0` if free, `1` if allocated
    - Simple and memory-efficient (1 bit per allocation unit)
    - Slow: to add $k$-unit process, we need to search bitmap for $k$ consecutive 0 bits
2. **(double) LINKED LIST**
    - Linked list keeps track of allocated (`P`) and free (`H`) units
    - In each list:
        - `P` = process, `H` = hole
        - `start` = start of allocation unit
        - `size` = size of allocation unit
    - Quick to search for sufficient space
    - Easy to remove address space (free memory)
    - **Allocation strategies**: first fit, next fit, best fit, worst fit

## Virtual Memory

-   Each process has own **virtual address space**
-   Address space is split into **pages**
-   All pages are stored in the **hard drive / disk**
-   Not all pages need to be in RAM for a process to run
    -   **Page fault** when a process tries to access a page that is not in RAM
-   **MMU** (Memory Management Unit) keeps track of mapping **virtual pages** to **physical frames**

Formula to calculate physical memory address
$$\texttt{PHYSICAL\_ADDRESS} = (\texttt{VIRTUAL\_PAGE\_NO} \bmod \texttt{PAGE\_SIZE}) + (\texttt{PAGE\_FRAME\_NO} \times \texttt{PAGE\_SIZE})$$

## Page Tables

-   MMU converts virtual address into physical addres using page tables:
    $$\texttt{VIRTUAL ADDRESS} \overset{\texttt{PAGE TABLE}}{\longrightarrow} \texttt{PHYSICAL ADDRESS}$$
-   Virtual address has **virtual page number** and **offset**
-   **Offset** is copied to physical address
-   **Virtual page number** is used to look up **physical page frame number** in page table

Page table (sorted by virtual page number) contains:

-   **Physical page frame number**
-   **Present** bit (1 if page is in memory, 0 if page is not in memory)
-   **Read/Write/Execute** bits
-   **Modified** bit
-   **Referenced** bit

For very large page tables (32-bit virtual address), if the page size is 4KiB = $4 \cdot 2^10 = 2^{12}$ bytes per page, we need 12 offset bits and thus $32 - 12 = 20$ virtual page number bits. We split these 20 bits further up into a **multilevel page table**: 10 bits for the first level and 10 bits for the second level.

| 10  | 10  | 12     |
| --- | --- | ------ |
| PT1 | PT2 | Offset |

The top-level page table (PT1) is the **page directory**.

### Size of PT1 = page directory

An entry is a **pointer to a physical address of PT2** (32-bit address space), so one entry is 4 bytes.

$$|\texttt{PT1}| = 2^{10} \text{entries} \times \text{size of entry} = 2^{10} \times 4 \text{ bytes} = 2^{12} \text{ bytes} = 4 \text{ KiB}$$

### Size of PT2

An entry is a **4 bytes**: physical page frame number, present bit, read/write/execute bits, modified bit, referenced bit.

$$|\texttt{PT2}| = 2^{10} \text{entries} \times \text{size of entry} = 2^{10} \times 4 \text{ bytes} = 2^{12} \text{ bytes} = 4 \text{ KiB}$$

### Effiency

**We only keep the PT2's that we actually use in memory**, but PT1 always needs to be in memory.c So, the total size of the page table is always $$|\texttt{PT1}| + |\texttt{PT2}| \times \text{number of PT2's in memory}$$

### If we didn't use a page directory

Without a page directory, we would need to keep the entire page table in memory, which would be $2^{20} \times 4 \text{ bytes} = 2^{22} \text{ bytes} = 4 \text{MiB}$$.

### Example

Let's say we load a program that needs $12 \text{MiB}$ of memory.

With a page directory, we only need to keep $12 \text{MiB} / 4 \text{KiB} = 12 \times 2^{20} / 2^{12} = 12 \times 2^{8} = 12 \times 256$ PT2's in memory, which is $12 \times 256 \times 4 \text{ bytes} = 12 \times 1024 \text{ bytes} = 12 \times 2^{10} \text{ bytes} = 12 \text{KiB}$.

If we didn't use a page directory, we would need to keep the entire page table in memory, which would be $2^{20} \times 4 \text{ bytes} = 2^{22} \text{ bytes} = 4 \text{MiB}$.

### Page Fault

-   When a **page fault** occurs
-   The OS needs to choose which page to remove from memory to make space for the incoming page
-   Removed page was modified $\rightarrow$ hard disk copy must be updated
-   We want to minimise the number of page faults

## Spinning = busy waiting

### Uniprocessor -> doesn't make sense

-   Thread A holds the lock but is currently paused (swapped out). Thread B gets the CPU, sees the lock is taken, and enters a busy-wait loop (`while(locked) {}`).
-   For the lock to be released, Thread A needs to run. However, Thread B is currently using the CPU to do nothing but check the lock.

This is **inefficient** because Thread B is **wasting 100% of the CPU cycles**.

How to do it better?

-   Thread B should immediately Yield/Sleep.
-   This gives the CPU to Thread A, allowing it to finish its work and release the lock much faster.
-   Thread B can then be woken up when Thread A releases the lock.

### Multiprocessor -> makes sense

Spinning makes sense for **short waits.**

-   Threads run in true parallel.
-   Thread A is running on Core 1 and holds the lock. Thread B is running on Core 2 and wants the lock.
-   While Thread B spins on Core 2, Thread A is simultaneously executing instructions on Core 1.
-   If the critical section is **short** (e.g., just updating a counter), Thread A might release the lock in a few nanoseconds. Thread B sees this almost immediately and grabs the lock without ever stopping.

Why not just sleep on Multiprocessors too?

-   **Overhead vs. Latency**.
-   Putting a thread to sleep (blocking) is an **expensive** operation. It involves:

    -   Saving register state.
    -   Switching to kernel mode (trap).
    -   Updating the scheduler queue.
    -   Processing an interrupt to wake it up.

-   This "Context Switch" might take ~2-5 microseconds.
-   If the lock is only held for 50 nanoseconds, sleeping is a waste of time.
-   It is much faster to just spin for those 50ns than to spend 5000ns putting the thread to sleep and waking it up.

SO:

> -   If the average mutex is held for **short** -> **SPIN**
> -   If the average mutex is held for **long** -> **SLEEP**

Modern OSes use a **hybrid** approach:

> Spin for short period and if threshold is exceeded, sleep.

## Multiprocessor Scheduling

### 1. Affinity Scheduling

-   It is cache-efficient to run the same thread on the same core.
-   When a thread is created, it is assigned to a CPU via **load balancing**.
-   CPUs give priorities to the threads assigned to them.
-   If no threads are available for a CPU, it will **steal** a thread from another CPU.

### 2. Gang Scheduling

-   There is a benefit in running collaborating threads together as a group over multiple CPUs at the same time:
-   Groups of related threads are scheduled as a unit
-   Parallel on different CPUs
-   All start and end their quantum at the same time

## Multicomputers

Clusters of nodes, where each node is equipped with a CPU, memory and **networking interface**.
They can be connected in various **topologies** using **switches**.

### Packet Switching

-   Messages broken down into packets
-   The full packed is stored an then forwarded
-   Flexible and efficient, but introduces delay

### Circuit Switching

-   A path is established between source and destination
-   No intermediate buffering
-   Fast, but inefficient use of resources

### Networking Interface

-   A hardware component that enables connectino to network
-   Own **memory**
-   Own **CPU**
-   They connect to the **switch**, which forwards the packets to the correct node

### Message Passing

-   How multicomputers communicate.
-   `send()` and `receive()`
-   The address needs to identify the computer **and the process!**
    -   Multiple processes may communicate in parallel.
    -   The OS needs to know which process to deliver the packets.

#### Blocking call

-   Synchronous
-   The process is blocked until the send/receive is complete

#### Non-blocking call

-   Asynchronous
-   Control is immediately returned to the process
-   Message is copied to a **kernel buffer**
-   Process gets notified when send/receive is done

### Load balancing

-   Assign processes to nodes using processor **allocation algorithms**
    -   Sender-initiated (node is overloaded -> send to another node)
    -   Receiver-initiated (node is underloaded -> receive from another node)

## I/O devices

### Classification based on the type of data

1. **Block Devices**
    - Store information in fix-sized blocks
2. **Character Devices**
    - Store information in bytes
3. **Other**

### Classification based on the number of users

1. **Dedicated Devices**
2. **Shared Devices**

### Controllers

A **controller** is a CHIP/PCB that interfaces hardware of I/O devices with the rest of the system.

-   Knows how to talk with device (via **standard** or **proprietary** protocol)
-   OS communicates with controller via **registers**
-   May handle multiple devices (e.g. interrupt controller)

### Controller Registers

-   Status Registers (read only)
-   Command Registers (write only)
-   Configuration Registers (read/write)
-   Data Registers or Buffer Registers (read/write)

### 1. Separate I/O address space

-   Each I/O register is assigned an **I/O port number**
-   Communication is based on **special CPU instructions**: requires assembly!

### 2. Memory-mapped I/O

-   I/O registers are mapped to **memory space** (compatible with standard memory instructions)
-   Each memory unit or peripheral device **responds** only if the requested address falls within its **range**
-   **Standard** memory **instructions** work on I/O registers: doesn't require assembly, can be done in C!
-   Flexibility in access permission: OS can control: in virtual address space or not

Disadvantages:

-   Caching a status register is a disaster $\Rightarrow$ need for **selectively disabling caching**
-   Challenging in systems with multiple buses
    -   Example:
        1. high-bandwidth bus CPU <-> MEMORY
        2. low-bandwidth bus for everything CPU <-> Memory, I/O, etc.
    -   Send request to memory bus (fast) and try other buses if not successful

### 3. Hybrid

-   **Data Buffers/Registers** mapped to **memory space**
-   **Control** registers mapped to **I/O space**

## Direct Memory Access (DMA)

-   A peripheral (chip) that does 1 thing: **transfer data**
-   CPU asks DMA controller to transfer data
    -   Address
    -   Count (number of bytes)
    -   Direction (read or write)
    -   Control (start, stop, etc.)
-   CPU can continue doing other things while the data is being transferred
-   DMA interrupts the CPU when the transfer is complete

Disadvantages:

-   **Slower** than CPU
-   Makes little sense when **CPU is idle**

Considerations:

-   In **embedded systems**, getting rid of DMA saves money.
-   **BUT**! In **battery-powered embedded systems**, DMA is **more energy-efficient**.

### Bus Arbitration

1. **Cycle stealing mode**
    - DMA acquires bus for **each word**
2. **Burst mode**
    - DMA acquires bus once and transfers multiple words
    - Efficient but may block CPU for long time (because it needs to wait for the bus)

### Data Path

1. **Normal** (**Two**-Cycle) Transfer

    - Cycle 1: DMA reads data into internal **DMA register**
    - Cycle 2: DMA writes data from internal **DMA register** to **main memory (RAM)**

2. **Fly-By** (One-Cycle) Transfer
    - Only one cycle: DMA reads data and it goes **directly to main memory (RAM)**, flying by the DMA controller
    - Doubles the effective transfer speed by cutting bus cycles in half

Why use normal transfer?

-   Supports **memory-to-memory** transfer:
    -   Read from location 1 in RAM and transfer to location 2 in RAM
    -   **Not supported by fly-by transfer!** There is only one address bus. If you want to move data from RAM Address A to RAM Address B, you need the bus to say "Address A" and "Address B" simultaneously.
-   Supports **device-to-device** transfer, unlike **fly-by transfer**:
    -   DMA has **address** bus and **DACK** bus (DMA Acknowledge)
    -   You use DACK to select the first I/O device.
    -   You cannot use the address bus to select the second I/O device.
    -   And the DACK is already used to select the first device.
    -   Impossible to do device-to-device transfer.

## Interrupts

### Interrupt controller

-   Interrupts are implemented by **changing the sate of a bus line**
-   The change is detected by an **interrupt controller**
-   Controller handles interrupt for **multiple devices**
-   Interrupted CPU **acknowledges** the interrupt to indicate that it is ready to accept a new interrupt

### (IM)Precise Interrupt

When an interrupt occurs, many instructions on the CPU can be at various stages (IF, ID, EX, MEM, WB).

1. **PRECISE** interrupt

    - Leaves the system at a well-defined state
    - All instructions before the **PC** have been completed
    - No instructions after the **PC** have been completed
    - The execution state of the instruction pointed by the **PC** is **known**

2. **IMPRECISE** interrupt
    - Does not meet the above criteria
    - Required to save **a lot of information on the stack**

## I/O Software

### 1. Programmed I/O

-   CPU does all work
-   **Blocking**: **polling / busy waiting**
-   Occupies CPU + energy waste

### 2. Interrupted I/O

-   CPU still does all work
-   **Non-Blocking**: **Interrupts**
-   Copy character from memory to printer's data register
-   Down semaphore and sleep
-   Interrupt handler wakes process back up
-   Copy next character ... repeat

### 3. DMA

-   DMA does all work
-   CPU: copy memory address, count, channel to DMA's **control registers**
-   Down a semaphore, sleep
-   When DMA is done, it wakes up the process
-   Reduces number of interrupts **one per character $\rightarrow$ one per buffer**
-   But: DMA is slower than CPU...

## I/O Software Levels

### 1. Device Drivers

-   Code that communicates with device **controller**
-   Writing/reading **device registers**
-   Provides API for **initializing and using** the I/O device
-   Provided by hardware manufacturer
-   One for each supported OS
-   Typically part of the **kernel**

### 2. Device-Independent I/O Software

-   Efficiency, uniformity, avoid code duplication
-   Uniform interface for device **drivers**
    -   OS defines an api for common operations
    -   Device manufactureres must make **drivers** (level 1) that adhere to the API
    -   +: applications use the same API on all devices
    -   -: applications cannot access particular niche features
-   Allocate and release dedicated devices
-   Error handling and reporting
-   Buffering

### 3. User-Space I/O Software

-   Background services (daemons), libraries
-   Spooling
-   Formatting (format I/O)
-   System call APIs (make I/O call)

#### Spooling

-   Only a **daemon process** has permissions to acquire a device
-   User applications print via the **daemon**
-   This prevents users from never releasing the device

## Exam Questions

> **What is the key advantage of monolithic operating systems compared to microkernel operating systems?**

**Monolythic** is better for **efficiency** and **speed**, because everything runs in **kernel mode**.

**Microkernel**: only the core fundamental services run in kernel mode, while other services run in user mode. They have to use system calls (IPC) to communicate with the kernel, which adds overhead and can slow down performance.

> **You are developing an application that is composed of multiple collaborative processes. You wish to implement the following functionality: if a resource is currently unavailable, the process should go to sleep until it receives a wakeup signal from another the process. Which method you would use to avoid a race condition?**

This is similar to the producer-consumer problem we covered in class. The producer puts items into a buffer, and the consumer takes items out of the buffer. If the buffer is empty, the consumer should go to sleep until the producer adds more.

When we want to do this kind of wake-up signal, we can use a **semaphore**. It has two main operations: `wait()` and `signal()`. When a process wants to access a resource, it calls `wait()`. If the resource is unavailable, the process goes to sleep until another process calls `signal()` to wake it up.

> **What is the primary role of the Memory Management Unit (MMU)?**

The primary role of the Memory Management Unit (MMU) is to handle **virtual memory management**. It translates virtual addresses used by a program into physical addresses in the computer's RAM.
This allows us to run programs that are much bigger than the available physical memory, by swapping parts of the program in and out of RAM as needed.

> **Explain the challenges associated with static relocation and Why it is necessary to transition to dynamic relocation. Discuss the advantages of dynamic relocation in overcoming these challenges.**

Static relocation involves assigning fixed memory addresses to programs at compile time. It's **not flexible**. If another program is loaded into memory, it may conflict with the addresses assigned to the first program. This can lead to inefficient memory usage.

Dynamic relocation allows programs to be loaded into different memory locations at runtime. This is done using techniques like paging and segmentation. This allows us to be more flexible and use the memory more efficiently.

> **Describe a bug or error that would trigger a watchdog timer reboot?**

A watchdog timer is a hardware or software timer that reboots the system if it detects that the system is not functioning properly. When it expires, it triggers a system reboot to recover from the unresponsive state.

A common bug that would trigger a watchdog timer reboot is an infinite loop or deadlock in the code. In this case, the application is stuck. If we use a preamptive scheduler, the watchdog timer is not needed, because the OS can interrupt the application and switch to another process.

However, imagine we have an infinite loop in a kernel module: a deadlock or an infinite loop in the scheduler for example. Then the watchdog timer is not reset periodically, and it will eventually expire and trigger a reboot.

> **Which is the key advantage of developing device drivers using interrupt-driven I/O?**

An interrupt-driven I/O is an I/O method where the **CPU is notified by the device when it is ready to transfer data**, rather than the CPU continuously checking the device status (**polling**).

1. **Efficiency**:
    - You don't waste the CPU with busy-waiting. The CPU can perform other tasks while waiting for the I/O operation to complete.
    - The harddrive will for example notify the CPU when it has finished reading or writing data.
2. **Responsiveness**:
    - Let's say we're creating a keyboard driver. When we press a key, the keyboard sends an interrupt to the CPU, which **immediately** handles the key press event. This makes the system more responsive to user input.
    - If we would implement this without interrupt-driven I/O, we would have to periodically check the keyboard status, which is inefficient and can lead to delays.

> **Assuming the current state of the memory is as shown in Figure 5, name a virtual address that will generate a page fault if accessed?**

A page fault occurs when a process attempts to access a virtual page that is currently not mapped to a physical memory page frame.

> **Given that the OS uses FIFO as page replacement algorithm and its current list is `[6,1,3,5,2,4,0,7]` with the right- most element representing the tail of the list. What is the physical address corresponding to the virtual address after the appropriate page is loaded?**

-   The current FIFO list is `[6, 1, 3, 5, 2, 4, 0, 7]`.
-   The left-most element is the oldest page and is the victim to be replaced.
-   The victim page is page 6.

TODO
https://gemini.google.com/share/ca2c1f2f9c30

> **Name a system operation that is not possible without clock interrupts.**

-   A pre-amptive scheduler.
-   An alarm signal with `signal(SIGALRM, handler);` and `alarm(seconds);`

> **Nodes in a distributed system typically use protocols that have been standardised by international standardisation bodies to communicate with each other. Are international standards also necessary when nodes in a multicomputer system communicate with each other?**

The agreement does not need to be internationally standardised, as long as all nodes in the multicomputer system agree on the same protocol.

> **Which is the key disadvantage of the NFU (Not Frequently Used) page replacement algorithm?**

It's like an elephant: it never forgets.

It keeps track of how often each page is used, and when a page needs to be replaced, it selects the page that has been used the least frequently.

If pages are used a lot in the beginning but then not used anymore, they will stay in memory because they were used frequently in the past!

> **Consider preemptive and non-preemptive scheduling. Identify two real-life applications/senarios where:**

1. **Preemptive scheduling is better suited than non-preemptive scheduling.**
2. **Non-preemptive scheduling is more appropriate than preemptive scheduling.**

> **For each example, explain briey why you chose the particular scheduling technique and how it benefits the application.**

1. **Preemptive Scheduling**: The OS gives a process some time and if that time passes, it interrupts the process and gives another process a chance to run.

    - REAL WORLD EXAMPLE: A web server handling multiple requests from different users.
    - WHY: Preemptive scheduling allows the server to handle multiple requests efficiently, ensuring that no single request monopolizes the server's resources. This improves responsiveness and ensures that all users get a fair share of the server's processing time.

2. **Non-Preemptive Scheduling**: Once a process starts running, it continues until it **finishes** or **voluntarily** yields control.

    - There is no fareness
    - There is no responsiveness
    - But there is also no overhead from context switching
    - REAL WORLD EXAMPLE: Embedded systems spend a lot of time idle, waiting for an external event (like a button press). This makes non-preemptive scheduling suitable because the system can run a task to completion without interruption, reducing complexity and overhead.

> **The DMA chip can transfer data from and to the memory without using the CPU. Name a scenario that it is not ecient to use DMA for memory transfer?**

A scenario where it is not efficient to use DMA for memory transfer is when transferring very small amounts of data, such as a few bytes or a single word. This because the overhead of setting up the DMA transfer (configuring the DMA controller, initiating the transfer, and handling interrupts) may outweigh the benefits of offloading the transfer from the CPU. In such cases, it may be more efficient for the CPU to handle the transfer directly, especially if the data transfer is quick and does not significantly impact CPU performance.

> **What's the advantage that in UNIX everything is a file?**

The same tools can be used to access files and devices. The same system call can read data from an actual file, an IO device like a keyboard, or a network socket.

## Long questions

> **Describe, in your own words, how the scheduling algorithm of Linux prioritises the execution of processes that cannot tolerate latency.**

TODO

---

> Let's assume that you are designing a server for the OS Challenge. Arriving requests have 99% probability
> to have priority 1 and 15% probability to have priority 200. How would you design parallelism and prioritisation?
> You shall assume that anything not explicitly specied in this sub-question is as described on the OS Challenge specication document.

As soon as the high priority request arrives, it should be handled immediately. The low priority requests can be queued and processed when there are no high priority requests pending.

---

> **Summarise the advantages and disadvantages of processes and threads.**

Processes offer more stability trough isolation (seperate address spaces), but are more resource-intensive and have higher context-switching overhead.

Threads share the same address space, making them lightweight and efficient for concurrent tasks, but they can lead to stability issues if one thread crashes.

---

> **Give one example of a real-world application where you would choose to implement it with multiple threads and one example that you would choose to implement it with multiple processes. Motivate your design decision. Don't use the browser example.**

1. **Multiple Threads**: A web server handling multiple client requests.

    - **Motivation**: Threads within the same process share the same memory space, which allows for efficient communication and data sharing between threads. This is beneficial for a web server, as it can handle multiple requests concurrently while sharing resources like database connections and caches.

2. **Multiple Processes**: Multiple documents open in a word processor.

    - **Motivation**: Each document can be handled by a separate process, providing isolation between documents. If one document crashes or encounters an error, it does not affect the other open documents. This enhances stability and ensures that users can continue working on other documents without interruption.

---

> **Explain in your own words when using a spinlock (i.e., a busy-waiting mutex) in a multiprocessor system can improve the performance.**

Waiting in a busy-wait loop (spinlock) can be more efficient than putting the thread to sleep and waking it up again, especially when the expected wait time is very short. In a multiprocessor system, if a thread tries to acquire a lock that is held by another thread, it can continuously check (spin) for the lock to become available. This avoids the overhead of context switching and scheduling that occurs when a thread is put to sleep.

---

> **You are using a multiprocessor system. The operating system implements hybrid mutexes. These mutexes operate as follows. If a thread requests to acquire a locked mutex, the thread first continuously polls the mutex (spins) for a period of time, T. After the time T passes, the thread yields (context switch) and retries when it gets rescheduled. The parameter T is congurable, taking values in µs in the range [0,65535]. Setting T = 0 disables spinning.**
>
> **After long-term statistical analysis, you know that the time a thread needs to wait for a locked mutex to be released follows the histogram provided in Figure 6. Moreover, a context switch takes 1000 µs**
>
> **Calculate the optimum value for the conguration parameter T, which minimises the overhead (i.e. the sum of time the CPU wastes spinning and switching). For simplicity, you can consider that when the thread gets rescheduled after the rst context switch, it nds the mutex unlocked.**

To calculate the optimum value for T, we need to analyze the trade-off between spinning and context switching.

-   If T is too low, the thread will frequently yield and incur the overhead of context switching.
-   If T is too high, the thread will waste CPU cycles spinning unnecessarily.

$$P(wait = 50) = 0.6, \quad P(wait = 500) = 0.3, \quad P(wait = 5000) = 0.1$$

Look at the extremes:

-   No context switches, only busy-waiting
-   Always context switch

The right answer should be somewhere in between.

TODO
