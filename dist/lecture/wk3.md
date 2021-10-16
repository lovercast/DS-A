# Algorithms and Parallel and Distributed Computing lecture

agenda
learning goals
big O
questions from lab
singly linked lists
vs code

What are we going to do in class on XSEDE
What is an SU? How much computing does that represent?


Specific learning goals

Announcements
Teams activated
> What is teams?

Finding cycles in a singly linked list

how do you do it? 
Store pointers?
Traversing the list there is no end, so you have to gather pointers to nodes somehow if you want to detect a loop.
The problem is determining which node pointers to collect.

What are the complexities for the various singly linked list methods.
Arrays, methods for arrays, compare the methods for dealing with arrays.

delete -- O(1) for LL, O(n) for array.
sort -- in principal, you can do quicksort on both arrays and linked lists,
searching -- if sorted, then binary search on array, can you do binary search on a linked list? Nope. Jumping to the middle element is O(1) in array,
O(n) in linked list.

big O
linked lists
searching
sorting
tree/graph search : DFS
tree/graph search: BFS

depth and breadth first search

string
compute all of the anagrams -- how big is the set of permutations
n choices for first, n-1 choices for second, n-2 choices for third and so on

O(100)
combinations of a string -- how many sets of characters can you make by selecting letters from the string?

log10 x = log2 x * log2/log10

divide by log_a (b) to convert logarithms

# Patterns

Algorithms unplugged
linear search, eg FindFirstIndex(), look at ArrayList
binary search of a sorted array, suppose we are searching for 9.
What is the order of accessing the array?
what if we were searching for 10?

Reverse a linked list! That's a fun problem
start at head, while not null, reverse the pointers,
need a temporary pointer to hold the next thing after you reverse the node

# reversing a linked list -- O(n)
```c
if empty or one elt, return

next = head->next
this = head
last = null

while next != null, do:
    this->next = last;
    last = this;
    this = next;
    next = next->next;
    
this->next = last;
head = this;
```    
    
in place, recursive, second list

# find the cycle in a linked list
store randomly -- proportional to the time you spend searching
on average, you need to store all the nodes in the list before you find the node that points to the beginning.

upper bound for length of list
list should be no more than n long
probability of collection approaches 1 as number of steps approaches n

```c
hash_table h<listptr, byte>; // 1 if seen so far, 0 if not
rand_generator rng;
int steps = 0;
int prob;

// how do you know to terminate -- if the current node hits null!
// won't have collected n elements by the time we reach n

// probability = steps/(steps - 1) -- too linear!
// prob = e^x/1-e^-x

node = head;
while (true)
    prob = rng.get_random(0, n);
    (prob > steps) && hash_table.add(&node);
    node = node.next;
    ++steps;
    if (node.next = null)
        return NULL;
```

```c
r1, r2 = head;
while r1 != r2, do:
    r1 = r1->next;
    r2 = r2->next;
    
r1 = head;
while r1 != r2, do:
    r1 = r1->next;
    r2 = r2->next;

return r1;
```


# merge two linked lists


# remove duplicates

# find kth node from the end

# find the middle of a list

# swap adjacent pairs of nodes

# find the merge point of two lists -- if two lists are linked, how do you find the link where they join?
traverse list, store pointers.


Math on paper

# Parallel and Distributed Computing

what are we working on today?
Bridges2 -- using OpenMP, OpenACC and MPI on Bridges2

Learning goals for PDC
summary of reading and questions
Amdahl's law
computing the parallel speedup
overview of lab3

Integrating the lab with the lecture.

measuring parallel performance

Threading in C# and OpenMP

learning objectives

