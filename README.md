# Operating Systems Laboratory

## What’s here and why it matters
This repo is a mix of classic operating-system lab exercises: thread synchronization problems (like Sleeping Barber, Reader-Writer, Producer-Consumer, Dining Philosophers), deadlock models (Banker’s Algorithm, Deadlock Detection), page replacement strategies (FIFO, LRU, LRU Approximation, Optimal), and foundational Linux utility implementations (`cat`, `ls`, `rm`, `wc`, `ps`, `kill`, `cmd`).  

It’s a practical sandbox to dig into how threads, memory, and OS-level tools really tick.

## Why it’s useful
If you want to see operating system theory in action, maybe for coursework or as a primer, this is it. You can compile, run, and test the most common synchronization and memory management scenarios. And if you’re curious about how basic Linux commands (`ls`, `ps`, etc.) work under the hood, you’ll find basic C implementations of them here.

---

## Getting Started

### 1. Clone the repo
```bash
git clone https://github.com/Mahir3376/Operating-Systems-Laboratory.git
cd Operating-Systems-Laboratory
```
### 2. Compile
- Compile everything in one go:
```bash
gcc *.c -o <program-name>
```
- Or compile per file:
```bash
gcc Producer_Consumer.c -o producer_consumer
```
### 3. Run
```bash
./producer_consumer
```
- Some programs may require inputs or arguments, please check the source comments.

## What each file does

### Deadlock & Synchronization
- **Deadlock_Avoidance (Banker’s Algorithm).c** – Banker’s method for safe resource allocation.
- **Deadlock_Detection.c** – Checks the system state for potential deadlocks.
- **Dining_Philosophers.c** – Classic dining philosophers synchronization problem.
- **Producer_Consumer.c** – Shared buffer producer-consumer synchronization.
- **Readers_Writers.c** – Reader-writer lock problem.
- **Sleeping_Barber.c** – Barber shop synchronization scenario.

### Page Replacement
- **FIFO_Page_Replacement.c** – First-In, First-Out page replacement strategy.
- **LRU_Page_Replacement.c** – Least Recently Used replacement.
- **LRUApproximation_Page_Replacement.c** – LRU approximation.
- **Optimal_Page_Replacement.c** – Optimal (MIN) page replacement.

### Linux-style Commands
- **cat.c** – Minimal version of `cat`.
- **ls.c** – Basic directory listing.
- **rm.c** – File removal.
- **wc.c** – Word/line/byte counter.
- **ps.c** – Displays process info.
- **kill.c** – Sends signals to processes.
- **cmd.c** – Simple shell/command interpreter.

---

## Tips for experimenting
- Change thread counts or buffer sizes to see how synchronization breaks or holds.
- Compare page replacement results with different reference strings.
- Inject edge cases; like deadlocks with all resources taken.
- Extend `cmd.c` into a mini shell by wiring more commands into it.
- These are intended to be used in Native Ubuntu environment
---

## License & Contribution
This repo is intended for learning and experimentation.  
Feel free to use, tweak, or extend. If you add improvements or fix bugs, open a pull request.
