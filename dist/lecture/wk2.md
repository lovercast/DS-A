# Week 2 Parallel & Distributed Computing Lecture

Conceptual Level stuff

programming in OpenMP, pthreads, MPI, OpenACC, OpenCL

Difference between parallel and distributed computing and concurrency --


concurrency encompasses parallel computing 

What is a race condition a problem in? Is a race condition a problem in parallel computing or in concurrency?

> What is concurrency? 
Concurrency means tasks can be divided and performed independently

How does OpenMP handle race conditions? MPI? pthreads?

Computer architecture 

Identify data dependence in a for-loop

Is shared memory faster or distributed memory? under what circumstances is distributed memory faster? What if the cores don't have to exchange any information? Then in a distributed memory model, the
What if the processes need to communicate a lot?

what's a register -- very fast memory -- connected to the ALUs -- a fixed small number of bits that the fastest form of memory on the CPU?
What's a vector processor -- 

The ALU is divided up into pieces -- the ALU pieces add the result of an arithmetic operation on different lanes of a vector register.


Distributed memory and message passing -- CPUs
