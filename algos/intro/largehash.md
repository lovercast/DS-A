11.1-4 
We wish to implement a dictionary by using direct addressing on a huge array. At the start, the array entries may contain garbage, and initializing the entire array is impractical because of its size. Describe a scheme for implementing a direct- address dictionary on a huge array. Each stored object should use O.1/ space; the operations SEARCH, INSERT, and DELETE should take O.1/ time each; and initializing the data structure should take O.1/ time. (Hint: Use an additional array, treated somewhat like a stack whose size is the number of keys actually stored in the dictionary, to help determine whether a given entry in the huge array is valid or not.)

Search(k):
    Query the direct access table with the key k.
    Let the first field in the data at T[k] be an index i into the check array, C.
    If i is not a valid index into the array, it is a disconfirmation.
    If i is a valid index into the array, and the slot contains NUL, it is a disconfirmation.
    If i is a valid index and the slot i in the array contains the key k, then it is a confirmation.
    O(1)
Delete(&x):
    Go to the slot in C corresponding to index i in x.
    NUL the slot in C.
    Push the index i onto the stack of indices to fill.
    Free x.
    O(1)
Insert(x):
    If the stack S is non empty, pop the stack,
    set index i from the pop as the index into C of x.
    Generate a key k for x and set C[i] = k.
    O(1)
Initialization:
    Initialize a stack pointer S.
    Initialize the direct access table T.
    Initialize the check array C.
    O(1)


What happens when C needs to expand? Let C be a dynamic array. 
Dynamic arrays are amortized constant time.

```c
// implements stretchy buffer
typedef struct check_arr {
    
} check_arr;

typedef struct stack {
    struct stack *next;
    int index;
} stack;

typedef struct object {
    int index;
    int datalen;
    char *data;
} object;

typedef struct big_table {
    object table[];
    int size;
} big_table;
```
