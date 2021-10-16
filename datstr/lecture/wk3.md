# Data Structures and Algorithms Week 3 lecture

Generics, Unit Testing And Singly Linked Lists

# Unit Testing and Generics

## What Are Unit Tests
Tests for a single small piece of functionality.
> Contrast with integration tests, stress tests, regression testing, functional testing

Test a small piece of code, a method, a small function. AAA -- Arrange, Act, Assert

Arrange an object ot test -- a test case
Act -- run what you are trying to tes
Assert -- check the output is exactly what you expect

## Unit Testing Frameworks
Documentation, Testing -- Makes the code more maintainable, 
### Frameworks
we will use MSTest
> What makes MSTest special?

What is [TestMethod] doing? It's a _decorator_!

# Introduction to Generic programming
> Why is generic programming useful? What are the pitfalls of generic programming?
> How does generic programming work? What is happening when you create a 
> When would you not want to use generic programming? 
it makes the code harder to read and reason about.

Is any type substitutable for T? What if the method invokes methods that are incompatible with the type you pass in?
What if you want some functionality that some types come with? Have to use constraints! Give you new powers and give you some new limitations.
and interfaces.

# Introduction to Lists and Linked Lists

A list is an ordered sequence of values.
> What does that mean? Not necessarily sorted. 
Support predecessor and successor operations.

methods
- initialize
- add an item to the list
- remove an item from the list
- search
- sort
- traverse the list

Difference betwen array list and linked list -- 
linked list has unconditionally fast insert, delete operations
sort is faster in array list because of the contiguously stored memory.



