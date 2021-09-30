using System;
using System.Collections.Generic;

namespace Lab1
{
    class Program
    { 
        public static void Main(string[] args)
        {
            int probnum;

            Console.WriteLine("Welcome to lab 1");
            Console.WriteLine("Enter the problem number you would like to test.");
            GetInput();
            while (!int.TryParse(input, out probnum) || probnum > 6 || probnum < 1) {
                Console.WriteLine("Try again. Enter an INTEGER between ONE and SIX.");
                GetInput();
            }
            DoProblemNumber(probnum);
        }

        public static string input;

        public static void GetInput()
        { 
            Console.Write("Please enter input: ");
            input = Console.ReadLine();
        }

        public static void DoProblemNumber(int probnum)
        {
            switch (probnum) { 
                case 1:
                    Problem1();
                    break;
                case 2:
                    Problem2();
                    break;
                case 3:
                    Problem3();
                    break;
                case 4:
                    Problem4();
                    break;
                case 5:
                    Problem6();
                    break;
                case 6:
                    Problem6();
                    break;
                default:
                    return;
            }
        }


        /* Problem 1 */
        /* Accept a string from user input 
         * count the number of e or E characters .
        display the output to the user. */
        /* Works! */
        public static void Problem1()
        {
            Console.WriteLine("=========");
            Console.WriteLine("Problem 1");
            Console.WriteLine("=========");
            Console.WriteLine("Commence counting 'e's and 'E's");
            Console.WriteLine("Enter a string that you would like to");
            Console.WriteLine("know how many 'e's or 'E's it contains!");
            GetInput();
            int count = CountEs.Count(input);
            Console.WriteLine($"There were {count} 'e's or 'E's in that string.");
        }

        /* Problem 2 */
        /* Accept a string from the user without any punctuation
         * reverse the words in the string and display the output to the user. */
        /* Works! */
        public static void Problem2(){
            Console.WriteLine();
            Console.WriteLine("=========");
            Console.WriteLine("Problem 2");
            Console.WriteLine("=========");
            Console.WriteLine("Your string will be reversed: ");
            GetInput();
            string output = Reverse.RevString(input);
            Console.WriteLine($"Output String:\n{output}");
        }

        /* Problem 3 */
        /* Write a function that returns the nth prime number. */
        /* Calculate primes and writ primes to a file called primes.txt */
        /* Works! */
        public static void Problem3()
        {
            int n;

            Console.WriteLine();
            Console.WriteLine("=========");
            Console.WriteLine("Problem 3");
            Console.WriteLine("=========");
            Console.WriteLine("Display the nth prime.");
            Console.WriteLine("Please enter an integer n");
            GetInput();
            
            while (!int.TryParse(input, out n)) {
                Console.WriteLine("Please enter an INTEGER");
                GetInput();
            }

            // NthPrimeNumber.TestSieve();
            List<int> primes = NthPrimeNumber.Sieve(n);
            Console.WriteLine(primes[n-1]); // return nth prime.
        }


        /* Problem 4 */
        /* Generate random arithmetic expressions to quiz the user. */
        public static void Problem4()
        {
            Console.WriteLine();
            Console.WriteLine("=========");
            Console.WriteLine("Problem 4");
            Console.WriteLine("=========");
            RandomExpression.Quiz();
        }

        /* Problem 6 */
        /* Includes problem 5 */
        public static void Problem6()
        {
            Console.WriteLine();
            Console.WriteLine("=========");
            Console.WriteLine("Problem 6");
            Console.WriteLine("=========");
            MathTest.DoTest();
        }

       
    }
}
