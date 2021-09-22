# Key Concepts

## Memory stances
Distributed memory -- MPI is distributed memory -- networks -- islands of memory that send messages to one another.
shared memory -- OpenMP and Pthreads are oriented towards shared memory. One monolithic block of memory that tasks and programs share.
> What if the programs alter each other's memory? What if the memory used by one task is altered by anotyer task? What coordinates the cooperation of tasks on shared memory so that they don't sstep on each others toes?

## Difference between Parallel, Concurrent, Distributed:
- Concurrent means a program can be composed of tasks which are "_in progress_" at a given instant. (single program, "tasks", "in progress", "time")
- Parallel means -- A parallel program is one which is composed of __tasks__ that work together to solve a problem. ()
- Distributed computing means multiple __programs__ work together to solve a problem.

Concurrent, non-parallel -- means a program is composed of tasks that are working on different problems.
concurrent non-distributed -- one program composed of concurrent tasks.
non-concurrent parallel -- means a program is composed of tasks that are working on the same problem, but they operate in strictly disjoint time intervals.
non-concurrent distributed -- not at the same time, many programs working together.
non--parallel distributed -- many programs, not working on the same problem. Uninteresting.
non distributed parallel -- One program, composed of tasks that are working on the same problem.
concurrent, parallel and distributed -- many programs, composed of many tasks, working together to solve a problem in overlapping time intervals.


