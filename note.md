### Process Control Block
* Each process is represented in the operating system by a process control
block (PCB)—also called a task control block. A PCB is shown in Figure 3.3.
It contains many pieces of information associated with a specific process,
including these:
    - Process state. The state may be new, ready, running, waiting, halted, and
so on.
    - Program counter. The counter indicates the address of the next instruction
to be executed for this process.
    - CPU registers. The registers vary in number and type, depending on the
computer architecture. They include accumulators, index registers, stack
pointers, and general-purpose registers, plus any condition-code informa-
tion. Along with the program counter, this state information must be saved
when an interrupt occurs, to allow the process to be continued correctly
afterward when it is rescheduled to run.
    - CPU-scheduling information. This information includes a process prior-
ity, pointers to scheduling queues, and any other scheduling parameters.
(Chapter 5 describes process scheduling.)
    - Memory-management information. This information may include such
items as the value of the base and limit registers and the page tables, or the
segment tables, depending on the memory system used by the operating
system (Chapter 9).
    - Accounting information. This information includes the amount of CPU
and real time used, time limits, account numbers, job or process numbers,
and so on.
    - I/O status information. This information includes the list of I/O devices
allocated to the process, a list of open files, and so on.
In brief, the PCB simply serves as the repository for all the data needed to start,
or restart, a process, along with some accounting data.


