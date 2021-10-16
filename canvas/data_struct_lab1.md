# Data Structures and Algorithms Lab 1

Data Structures and Algorithms Lab - week 01
New Attempt
Due Sunday by 11:59pm  Points 10 Submitting a file upload Available Sep 28 at 10am - Oct 31 at 11:59pm about 1 month
For this assignment you must work in groups of 2-3 students. You will start in class in your group, then finish it for homework

Learning goals:

become familiar with C#:
reading strings from Console and indexing characters,
while-loops,
for-loops,
random number generator,
writing to a file
work collaboratively in a team
Microsoft has lots of documentation, and you could also look at C# for Java Programmers (link on Home page)

Please submit a separate .cs file (C# source file) for each of the problems shown below. At the beginning of each file you must include (as a comment) the name of each member of the group. If you do not include this, or if you only have one student name in it, your work will be considered incomplete.

Note: Do as many as you can, but we hope you will complete at least 4 of them, but be sure to submit something tonight. You can add more files later.

 

Problem 1: Write a C# program that asks the user to enter a string (let's say the user entered “Welcome to The Evergreen State College!”). Your program should count and display the number of “e” or “E” in the given input. In
the example above, the output should 10.

Hint: Console.ReadLine can be used to read user input from the console.

Problem 2: Write a C# program that asks the user to enter a string (think of it as a sentence, without any punctuations such as commas and periods). Then your program should reverse the words in the given string, and display it back to the user. For example, "hello world" -> "world hello"
[source: Glassdoor.com/ Microsoft interview questions]  

Problem 3: Write a function that returns the nth prime number. For example, nthPrime(1) should return 2 since 2 is
the first prime number, nthPrime(2) should return 3, nthPrime(3) should return 5, and so
on. [source: Glassdoor.com/ Microsoft interview questions]

We will write an isPrime method in class.

Problem 4: Write a C# program that randomly generates two integers between 1 and 15 inclusively, let's call them a and b. Then it randomly generates a number between 1 and 4, called operation.

If the value of operation is 1, then prompt the user to enter the result of the expression a+b
If the value of operation is 2, then prompt the user to enter the result of the expression a*b
If the value of operation is 3, then prompt the user to enter the result of the expression a-b
If the value of operation is 4, then prompt the user to enter the result of the expression a/b
If the user answers correctly then output a congratulation message, otherwise output a message indicating the answer entered was wrong.
Hint: use Random to get a random number generator.

 

Problem 5: The previous problem generates a random arithmetic question and tests user's knowledge. In this assignment you will ask the user twenty such questions [you must use a loop!] and keep track of how many correct answers were given. At the end output a message that displays that total.

Hint: use problem 4, but you may need to change some of that code.

 

Problem 6: modify your program 5 so everything seen in the console window (including all twenty random questions and user's answers) is also saved into a file, named output.txt.

Hint:  use StreamWriter to write to a file

