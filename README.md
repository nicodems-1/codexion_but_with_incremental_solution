*This project has been created as part of the 42 curriculum by niverdie.*

# Codexion

## Description
Codexion is an advanced concurrency and multi-threading project inspired by the classic Dining Philosophers problem. The goal is to simulate a working environment where multiple Coders (threads) must share a limited number of Dongles (shared resources) to compile their code. 

To survive the simulation without facing a "burnout," coders must strictly alternate between compiling, debugging, and refactoring. The project introduces complex constraints not found in the original problem, such as hardware cooldowns and advanced scheduling algorithms (FIFO and Earliest Deadline First - EDF), forcing a deep dive into real-time operating system concepts and strict resource management.

## Instructions

**Compilation**
The project includes a robust Makefile. To compile the executable, simply run:
`make`

**Execution**
Run the program with the following arguments:
`./codexion <nb_coders> <time_to_burnout> <time_to_compile> <time_to_refactor> <time_to_debug> [cycles] <cooldown> <scheduler>`

**Arguments Breakdown:**
*   **nb_coders:** The number of coders (and dongles).
*   **time_to_burnout:** Time in milliseconds a coder can survive without compiling.
*   **time_to_compile:** Time in milliseconds it takes to compile (requires 2 dongles).
*   **time_to_refactor:** Time in milliseconds spent refactoring.
*   **time_to_debug:** Time in milliseconds spent debugging.
*   **[cycles]** *(Optional)*: Number of times each coder must compile before the simulation successfully ends.
*   **cooldown:** Time in milliseconds a dongle remains unusable after being released.
*   **scheduler:** The queuing policy used to resolve resource contention (`fifo` or `edf`).

**Example:**
`./codexion 5 3000 200 200 200 10 800 edf`

## Resources
**Classic References:**
*   [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/) - The ultimate guide to pthreads.

**AI Usage:**
AI (Google Gemini) was utilized during this project as a purely educational assistant. It was specifically used to:
*   Clarify the behavior of `pthread` race conditions and analyze complex Valgrind output during the stress-testing phase.
*   Assist in refactoring logic to comply with the strict 42 Norm (e.g., identifying patterns to safely extract static helper functions and resolve 25-line limit constraints).
*   Provide guidance on structuring a robust, wildcard-free Makefile.

## Blocking cases handled
This project tackles several critical concurrency and real-time scheduling challenges:

*   **Deadlock Prevention (Coffman's conditions):** Deadlocks are structurally avoided by ensuring coders request their left and right dongles in a specific order based on their parity (even/odd IDs), breaking the circular wait condition. The edge case of a single coder (`nb_coders == 1`) is intercepted immediately to prevent double-locking the only available dongle.
*   **Starvation Prevention:** A strict internal queue mechanism is implemented for each dongle. By relying on the `edf` (Earliest Deadline First) scheduler, the thread with the most urgent `time_to_burnout` is prioritized, forcefully preventing resource monopolization by faster threads.
*   **Cooldown Handling:** Dongles enforce a strict cooldown period post-compilation. This adds a layer of "Hold and Wait" complexity, requiring coders to accurately gauge resource availability without blocking higher-priority neighbors prematurely.
*   **Precise Burnout Detection:** A dedicated, detached monitor thread constantly iterates over the coders' states. It compares the current timestamp against the `last_compiled` timestamp with sub-millisecond precision to halt the simulation the exact moment a burnout occurs.
*   **Log Serialization:** Console output is strictly protected by a global logging mutex. This ensures that timestamps and actions do not overlap or interleave, maintaining a coherent and chronological output even with hundreds of active threads.
*   **Tie Breaker implementation:** In edf when two deadline are equals, the coder written as the first one on the stack is always gonna be the one with the shortest id

## Thread synchronization mechanisms
To orchestrate the simulation safely, several threading primitives are employed:

*   **`pthread_mutex_t` (Locks):** Used extensively to protect shared memory. Each dongle has its own `dongle_lock` to secure its queue and `is_eating` status. A personal `coder_mutex` protects individual coder states (like `last_compiled` and `times_compiled`) so the monitor thread can read them without causing a data race while the coder writes to them.
*   **`pthread_cond_t` (Condition Variables):** Used to synchronize the chaotic startup of threads. Since `pthread_create` does not guarantee immediate execution, all coders are held at a starting gate using `pthread_cond_wait`. Once all threads are successfully created, the main thread broadcasts a signal, ensuring a perfectly fair and simultaneous start for the simulation.
*   **Race Condition Prevention:** The golden rule of this implementation is strict lock encapsulation. A thread never reads or modifies a variable belonging to another scope without first holding the appropriate mutex. The separation of read and write privileges ensures that the monitor can observe the system without ever altering the deterministic flow of the coders' routines.