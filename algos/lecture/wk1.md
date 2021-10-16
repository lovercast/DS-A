# Lecture 1 Notes

Algorithms

agenda
- announcements
- syllabus
- learning goals 
- questions from lab
- slides on algorithms

Learning Goals --
- describe the concept of a data structure 
- describe the techniques needed for algorithm analysis
- analyze the performance of searching and sorting algorithms
- implement searching algorithms such as sequential searching and binary searching
- implement data structures such as linked lists, stack, queues.
- implement sequential sorting algorithms such as: bubble sort, selection sort, insertion sort, merge sort, heap sort, quick sort
- implement binary tress, binary search trees, graphs, graph algorithms.
- implement hash tables, use various generic and non-generic collection classes
- implement generic classes
- implement graphs
- explain the differences between threads and processes, including advantages and disadvantages of each.
- Generic classes
- compate different data structures in terms of efficiency

Understand how parallel hardware works!
How does our understanding of parallel hardware inform how we develop applications and write programs.

Understanding concurrency -- not just about speeding things up -- speeding things up without breaking things.

What is a data structure? 

multi-processor, vector processor, GPU

parallel patterns -- what are the standard parallel patterns?

Which programming environment ot use for a problem?
OpenMP? Pthreads?

- Debugging parallel programs!!!
How do you do it?!?!

Hurdle to overcome -- see a bug as a challenge to overcome -- something exciting as opposed to something holding you back!

See a bug as a challenge -- a mystery -- how do you investigate it? How do you understand it? What  are the strategies? What's the right attitude?
Debug parallel programs -- find race conditions and bisect them.


Work with other students.

Richard has office hours tuesdays and thursdays -- or by appointment.

================================================

Is there a pattern in stock market
Development and what meet?

opportunity at evergreen -- help the staff do what? The computer center -- 

piazza -- what is this?

algorithms + data structures = programs?

Why do we care about complexity?
If n is very large, the theoretical complexity gives us a handle on how much time the algorithm will take?

The amount of data that we have is very large -- asymptotic behavior really affects
run time.

run time and time complexity -- what is the difference?
run time is empirical -- time complexity is theoretical

harmonic series - compound denominators -- does it diverge?

$$
O(N), \Omega(N), \Theta(N)
$$

$$
f(x) = O(g(x)) if 
f(x) <= c \cdot g(x) \textrm{for some 0 < c for sufficiently large x}
$$
$$
f(x) = \Omega(g(x)) if 
f(x) >= c \cdot g(x) \textrm{for some 0 < c for sufficiently large x}
$$
$$
f(x) = \Theta(g(x)) if 
f(x) <= c_1 \cdot g(x),
f(x) >= c_2 \cdot g(x) \textrm{for some 0 < c for x > k for some k}
$$

