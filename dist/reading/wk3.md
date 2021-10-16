# Reading for Week 3 in Parallel and Distributed Computing


Reading for Thursday: 

goals: reinforce programming with pthreads. 

objectives: students will be able to:

explain how to create pthreads, pass parameters, and use join
define race condition and critical section
define mutex
use a mutex to protect a critical section
What you need to do
Read sections 4.1 through 4.6 in Parallel Programming (Pacheco).
Read 14.3 to 14.6 in Dive Into Systems (review the parts you read before) https://diveintosystems.org/development/antora/diveintosystems/1.0/SharedMemory/index.html (Links to an external site.)
give an example of a race condition. you don't need to show code.
what is void * ?
answer 4.1 in Chapter 4.

4.1 to 4.6 in Pacheco

# Chapter 4 in Pacheco -- Shared Memory Programming with Pthreads.

> What problems can arise in sa shared memory system?
> What advantages does a shared memory system have over a distributed memory system?
> How might a shared memory system be vulnerable to attack in ways that a distributed memory system wouldn't be?
general rule:
limit use of global variables to situations in which they are really needed

be careful with global sums because they can cause race conditions -- 
the value of the global sum is stored in a local variable by each thread, then what happens after the += operation is carried out?
both threads write the result to memory, and the last one in wins (blowhard's debate: last one to get their word in wins)

busy waiting: a thread repeatedly tests a condition aand cannot advance until it is true -- what is the difference between this and a pthread cond?

## critical section
A section of code where a thread will update a shared resource -- watch these sections for race conditions.
What are some examples of critical sections?
The whole Fibonnaci program is a critical section.
> Is a matrix a shared resource? Or does a shared resource have to be one memory address?
In a matrix multiplication program, the critical sections are in the beginning when the dynamic memory is allocated.
In general when shared variables are declared or allocated, that code is critical.


Key words
critical section
process
pthreads
race condition
busy-waiting


Implement the following as an example:
busy waiting 
race condition
semaphore
conditional variable



Major topics --
Amdahl's law
Gustafon-Barsis law 
cace coherence
cache hit, miss
write through, write back
false sharing
Thread safety



# Example of a race condition

```c
void *foo(void *v)
{
    static long boo;        
    boo = *(long *)(v);   /* Writing to a static variable -- Why, that's a critical section. */
    return &boo;
}

int main()
{
    long *arg;

    /*...*/

    for (/*...*/)
        pthread_create(&thread_arr[i],NULL,foo, (void *)&arg[i])

    /*...*/

    long *boo;
    
    for (/*...*/)
        pthread_join(thread_arr[i],(void **)&boo)
        
    do_important_stuff(boo);      // yes this function has to block if I don't malloc boo

    /* blissful ignorance */

    return 0;
}
```

# What is void *?

In C/C++, `void *` is a generic pointer type. It cannot be dereferenced without a runtime cast to a specialized pointer type.

In C, a pointer is just a typed virtual address. All pointers have the same size on a given machine. Consequently, a variable typed `void *` can hold the value of any pointer. `void *` is useful for writing generic APIs in C, a language with limited support for generic programming. 

`void *` became standard with the first ISO standard. 


Question 4.1 in Pacheco
"When we discussed matrix-vector multiplication we assumed that both $m$ and $n$, the number of rows and the number of columns, respectively, were evenly divisible by $t$, the number of threads. How do the formulas for the assignments change if this is not the case?"

The formulas as given:
```c
int local_m = m/nthreads;
int i_start = thread_id * local_m;
int i_stop = i_start + local_m;
```

This is undesirable when $nthreads$ is not a divisor of $m$. The remainder of the division will leave zeroed entries at the bottom of the resultant column vector.

Fortunately, there is a trivial fix.
```c
int i_stop = (thread_id < nthreads-1) ? i_start + local_m : m;
```

Those of us who are unsatisfied by satisfying solutions might go one step further and correct `local_m` so that it is aligned with the cacheline size.

```c
size_t cls = 64/sizeof(long); /* say */
int local_m += cls - (local_m % cls);
```



producer-consumer synchronization

