# Lecture 1

Parallel and Distributed Computing

Why is parallel computing interesting? Why do we care? How does it affect the way we design programs?

What parallel programming paradigms are there?

Threaading in C#, Task in C#, OpenMP, pthreads, MPI, OpenACC, OpenCL

OpenMP -- C/C++
pthreads -- OpenMPI -- C

Not doing Cuda or OpenCL

- Explain the difference between parallelism and concurrency
- Write parallel programs in OpenMP and C#
- Explain when and how a serial algorithm can be parallelized to speed up execution
- Identify data dependence in a for-loop
- Explain race condition, lock, reduction, and scheduling
- Identify race condition in a program, debug and fix it
- Explain static vs dynamic scheduling and load balancing
- Explain the benefits of multicore hardware and multithreaded software 
- Identify when a problem can benefit from those techniques.

Quizzes will mostly be on tuesdays

The more questions, the better.
Ask questions on assignments.

What you hand in for the lab --
What you did, whom you worked with, questions, 
Then take the code, 
create a discussion topic with the code.
Doesn't have to be working code.

Within a day or two -- comment on someone's code.
create a rough draft -- show it to others -- get feedback -- update draft.

pick a few labs -- pick one or two programs that I wrote and got feedback on, 
HW assignment to submit edited code that got feedback.

Game -- ask questions, see if can recall information fluently.

Games on tuesdays.



> How does the operating system schedule threads? By assigning priority. How does the operating system assign priority? Availability, ...
image compression -- 

What is parallel hardware?

What if you didn't use multithreading in your application?
People would leave if your program couldn't load.

alarms and timers -- background processes -- 
is a background process using less CPU time than others?
Is a timer not runing all the time? It's interleaved -- concurrency.

> What is concurrency?
Concurrency is 

> What is a race condition? 
Two threads compete for access to a piece of data.
Which thread gets access first?
What if the software doesn't prevent two threads from changing the data at the same time?

Threads is a shared memory model -- threads in distributed memory -- MPI

A race condition is the possibility that two threads can get write access to a piece of memory at the same time. 

The behavior of a system depends on the timing of uncontrollable conditions.
A and B occur at the same time, A and B each have state x at the same time
A precedes B, ie. A has state x while B does not, the A loses state x and B gains it
A follows B, it. B has x, then ~x(B) then x(A).

When is this a problem?
When x means write access to shared data.
If x means read acces or write access to non-shared data, this is irrelevant.

How to get Visual Studio to compile and run programs on the command line?



all threads run same code ---


> what is the difference between physical and virtual memory?
virtual memory is the memory address as seen by a process.
physical address is the address as seen by hardware.

The OS assigns a "virtual address" space to a process -- gives the process the 
ability to forget about memory management. Makes code more portable, maintainable.
Programmers don't have to worry about memory management and page tables and 
things like that
heap -- dynamically allocated memory.


What if two threads compete for one memory address on the heap?
Potential for race conditions.

What does the OS do with the 

More memory than physically available on the machine. Allows programs to ask for as much memory as it likes. What if the process asks for more memory than is physically available on the machine?

ENOMEM. Or swapped out to disk.

When does an ENOMEM happen if the operating system can swap things out to disk?
When no memory can be swapped out to disk.

> loader - what does the loader do? Load into memory at runtime?


Submit reflection about what you did and whom you worked with.

Submit code in discussion forum

Write down the names of the people in your group.
meet with your group and take a break.


