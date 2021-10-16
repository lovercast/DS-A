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

This is undesirable when `nthreads` is not a divisor of `m`. The remainder of the division will leave zeroed entries at the bottom of the resultant column vector.

Fortunately, there is a trivial fix.
```c
int i_stop = (thread_id < nthreads-1) ? i_start + local_m : m;
```

Those of us who are unsatisfied by satisfying solutions might go one step further and correct `local_m` so that it is aligned with the cacheline size.

```c
size_t cls = 64/sizeof(long); /* say */
int local_m += cls - (local_m % cls);
```
