# Reading Week 2 For Parallel and Distributed Computing

Objectives:
- describe distributed memory with message passing
- define and calculate speedup and efficiency
- identify parts of an algorithm that can be parallelized

What you need ot do

1. Read sections 2.6 and 2.7 in Pacheco
1. Read 14.2 to 14.4 in Dive into Systems.
1. TBD
1. TBD
1. TBD


Pacheco 2.6 

Key Concepts

speedup 
efficiency
amdahl's law
gustafson's law
scalability
resolution
barrier

Speedup and efficiency --
What is Speedup? Speedup is the ratio of the time it takes for a serial program to run to the time it takes for a parallel program to run with equivalent output.

What is efficiency? Efficiency is the ratio of the Speedup to the number of cores.
As the number of cores increases, we expect the efficiency to derease, why?
The overall running time of the program will decrease as the number of cores is increased, but the efficiency of each core will not keep pace with the increase in the number of cores.

What are the specfic causes of this? There is a cost penalty associated with adding cores?

formulae
S = T serial / T parallel
E = S/p where p is the number of cores.

Bigger problem sizes are associated with greater efficiency. Why would that be? Why would a 
larger working set size be associated with a greater efficiency when the program is running n processes than a smaller working set size running n processes?

Time spent coordinating work is lower in larger problem sizes -- the threads are busy working and not doing choreography.

Amdahl's law -- "unless virtually all of a serial program is parallelized -- there is going to be very little speedup associated with making it parallel -- why? Just bottlenexk of the serial portion?

> What's the formula for this?

Speedup is limited to the ratio of the time to execute the serial program and the time to execute the serial portion of the parallelized program - no matter how many cores you have, the speedup will not be greater than the inverse of the fraction of the program which is still serial.

Say the program is 15% serial after a parallelization process -- then the speedup limits to 1/.15 = 6.7

Gustafson's Law -- the larger the problem size, the smaller the fraction will be that is inherently parallel.

> What is scalability?
A technology is scalable if it can handle ever greater problem sizes -- 
More users, more clients, more network connections, more applications, more engineers, more ...





Pacheco 2.7
Designing parallel programs

Key Concepts

Foster's Mehodology
Partitioning
Communication
Agglomeration or Aggregation
Mapping




The return value from a thread can be accessed with the second argument to pthred_join

Dive Into Systems 14.3 Mutual Exclusion -- 

> What is a mutex lock? What is a semaphore? How do you use them in pthreads?

"Only one thread enters and executes the code at a time."

```
pthread_mutex_t /* global */
pthread_mutex_init /* in main */
pthread_mutex_destroy

pthread_mutex_lock /* in thread function */
pthread_mutex_unlock

```

lock and unlock are very expensive operations, use lock and unlock as little as possible, 

reduce the size of the critical section of code that must be locked, 

what if you make the memory regions that the critical code must access disjoint so that the threads don't have to lock in the first place?

Time threads with gettimeofday call -- takes a struct timeval * arg and then can pass NULL as second arg, 

can access the member field tv_usec which contains the epoch microseconds.



lock shared resources and don't lock local resources -- waste of time.
Do local computations and lock the shared resource before updating it.

> What's Deadlock? What's an example of deadlock in code? 

Nested mutex lock can cause deadlock -- concurrent access to locks causes threads to lock each other up, halting execution.

Lock only the code that is relevant to the lock in question.


## 14.3.2 Semaphores

Not "who owns what" but "what resources are available right now?"


Semaphore is a flag -- binary switch, locked or unlocked, like a global boolean.

semaphore.h
```
#include <semaphore.h>

sem_t sem; /* note that sem_t is typedef int sem_t */

sem_open (on macos)
sem_init (on linux)

sem_close (macos)
sem_destroy (linux)


sem_post
sem_wait

```


Questions --- 
> When is a semaphore useful? 
semaphore is used to represent that a resourece is in use right now.
Say there's a widget that can be used by one thread at a time.
That thread pulls the cord on the semaphore for the widget and no other threads can access it until it's available.
> When is a condition useful?
If we want to keep a thread suspended in a mutex lock until a condition has been met, then a condition is useful. 
Why can't we just use a sempahore to suspend a thread?
> When is mutex useful?
When we want to prevent race conditions, our first thought might be to implement a mutex lock.
> Why is a cond used exclusively with mutices?
> How do all of these structures work? 
Well a semaphore is quite simple. It's just an int with meaningful numbers.


# 14.3.3 Barrier 
Blocks all threads until they have all reached that point in the flow of execution.
> When is a barrier useful?
A barrier synchronizes threads.
If we want to synchronize threads, we block them with a barrier until they all reach that point.
What if we want to only conditionally block threads? Is there a conditional barrier type?

```
pthread_barrier_t
pthread_barrier_init
pthread_barrier_destroy

pthread_barrier_wait /* blocks until all threads have reached */

```






