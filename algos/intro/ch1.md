# Loop Invariants

What is a loop invariant?
A loop invariant is a property of a loop that is tre before and aftter each iterationl

What is an example -- In ther insertion sort algorithm, 

the property that a[1, ... i] is a portion fo the full list in sorted order is a loop invariant.

> How do we prove loop invariants?

What if the property above weren't loop invariant?

Consider how the insertion sort algorithm works, ..
Insertio sort works by swapping te places of entries in the loop uyntil they find one that is smaller than them.

We can prove that this is loop invariant by induction. In the base case, where i = 0, there iso only one element to consider, and therefore the proposition is true in the base case.

Furthermore in a general case, the element under consideration ai will be swapped back until it finds an element in the list that is smaller than itself.

If we suppose the inductive hypoth, namely that the elements a[1,...j] are rel sorted, then the next elt when it finds an elt smaller than itself will be sorted relative to the list, and therefore the list fragment is still sorted relative to tiself.

What must we prove to show loop invariance?

- initialization: What is initialization? The property holds prior to the first loop. Why not after? If the property only holds after the first loop, then the property isn't loop invariant. Why does that matter, because we want to show that a property holds in general.
- Maintenance: What is maintenance -- it holds after a general case.
- Termination: When the loop terminates, the invariant gives us a useful property that helps show that the algorithmn is correct. What does this mean? That's wishy washy.

That the loop terminates. What if it doesn't? But the property still holds? 

Termination -- why is the property useful for proving our algo in the first place?

init, maint, term, init, maint, term.

What's another example algorithm?
Convex hull -- Jarvis march. what loops are there? What are the loop invaraiant properties?
Loop invariant -- from each point on the hull, there is no point in the set S which is left of the line from that point to the previous point on the hull.
This is true in the base case -- initialization -- there is only one point in the set.
What about maintenance? Does the property hold after a general iteration of the loop?
The algorithm is to find a point such that no point in the set is left of the line between it and the previous point, so it holds in the general case.
After termination of the loop, there is a circuit of points that point to eachother, and no points lie outside the polygon that they form. Termination.

init, maint, term, init, maint, term

Linear search -- consider a set S and a value v. Linear search through the set looks like this in C:
```
int linear_sort(int S[], int v) {
    for (int i = 0; i < n ; i++) {
        if (S[i] == v):
            return i;

    }
    return NULL;
}
```

What is the loop invariant? and prove it.

The loop invariant ios that the set of points S[0,..i-1] are not equal to v.
If one of them were, then v would have been returned.

Init: In the base case, if the first elemt is equal to v, then 0 is returned.
Maint:  In the general case, the first i-1 elements are not eequal to v, if next is, return i, else prop holds.
Termination: There are two cases, the list contains at least one value eq to v, or else it doesn't. If yes, then done after idx iterations.. if no, then the prop holds for t hewhole list.

## Analyzing algorithms

> What does analyzing mean? 
> What does it mean to analyze an algorithm?

"assume a generic one processor Random Access Memory model of computation."

> Is there such a thing as concurrent RAM computing?
"In the RAM model, instructions are executed ins equential order."

assume instrs take a const amount of time

# algorithm design --

Iterative approach -- sort the first n and repeat until the end
linked lists? Search in a linked list, linear search

Divide and Conquer -- Recursion -- split the operation up into smaller operations that hae the same structures as the whole and sdo the operations separately on each suboperation.
-- Trees, other recursive structures.

Recursive solutions depend on there being a terminal case where the problem can be solved without resortin to recursion. Must his always be the case?
> Do recursive algorithms always have to hit a base case at some point?
> What if the algorithm did not hit a base case which was evaluable. Then the algorithm would proceed to recurse forever.



