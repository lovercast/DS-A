using System;
using System.Collections.Generic;
using System.IO;

// modified so that every print statement is echoed by a streamwriter writeline call

namespace Lab1
{

    /* Problem Statement: Generate a random arithmetic expression and quiz the user.
     * Display a message that fits their result. */
    /* 1 <= a, b <= 15, operators are in {+, -, *, /} */
    public class MathTest
    {

        /*  =================== Structs ==================== */
        
        /* Tracking mistakes. */
        public struct Report {

            public string problem;
            public int answer;
            public int guess;

            public Report(string p, int a, int g)
            {
                problem = p;
                answer = a;
                guess = g;
            }
        }

        /* Convenience struct -- handling op char and answer as one object. */
        public struct opAnswer { 

            public char op;
            public int answer;

            public opAnswer(char op, int answer)
            {
                this.op = op;
                this.answer = answer;
            }
        }

        /*  =================== Important Globals ==================== */

        public static List<Report> mistakes; // keeps track of your mistakes.
        public static StreamWriter writer;
        public static opAnswer O; 


        /* =================== Entry Point ==================== */

        /* DoTest is the main entry point for this class.
         * It initializes the important globals, calls the
         * quiz routine, and generates the report card.
         */
       
        /// <summary>
        /// Quiz the user with twenty questions.
        /// Keep track of how many they got right. 
        /// Output a message afterwards.
        /// </summary>
        public static void DoTest()
        {
            string filepath = "reportcard.txt";

            mistakes = new List<Report>(); 
            writer = new StreamWriter(filepath); 

            int result = 0, numQuestions = 20;
            int i;
           
            for (i = 0; i < numQuestions; i++)
                result += Quiz() ? 1 : 0;

            PrintLine($"Score: {result}"); // special print function

            if (result == numQuestions)
            {
                PrintLine("Wow! Perfect score!");
                PrintLine("Somebody must feel pretty special.");
            }
            else if (result > 5 * numQuestions/6)
                PrintLine("Good job.");
            else if (result > 3 * numQuestions/4)
                PrintLine("Great job if you're a third grader!");
            else if (result > numQuestions/2)
                PrintLine("Almost OK job!");
            else
                PrintLine("You're REALLY improving, buddy!");

            if (result == numQuestions) {
                writer.Close();
                return;
            }

            PrintLine("Here were your mistakes:");
            i = 0;
            foreach (Report r in mistakes) {
                ++i;
                Print($"\t{r.problem}\t=\t\u001b[32m{r.answer}\u001b[0m\t(\u001b[31m{r.guess}\u001b[0m),\t");
                if (i % 4 == 0)
                    PrintLine("");
            }

            writer.Close();
        }


        /* =================== I/O Wrappers ==================== */

        public static void PrintLine(string s)
        {
            Console.WriteLine(s);
            writer.WriteLine(s);
        }

        public static void Print(string s)
        {
            Console.Write(s);
            writer.Write(s);
        }

        public static string ReadLine()
        {
            string input = Console.ReadLine();
            writer.WriteLine(input);
            return input;
        }

        

        /* =================== Verbose Routines ==================== */

        /// <summary>
        /// Switches!
        /// Map a number to an operator and
        /// evaluate the result of the expression,
        /// storing in a global struct.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <param name="op"></param>
        private static void GetResult(int a, int b, int op)
        {
            opAnswer result = new opAnswer();
            switch(op) {
            case 0:
                    result.op = '*';
                    result.answer = a * b;
                    break;
            case 1:
                    if (b == 0)
                        throw new DivideByZeroException("You tried to divide divide by zero!");
                    result.op = '/';
                    result.answer = a / b;
                    break;
            case 2:
                    result.op = '+';
                    result.answer = a + b;
                    break;
            case 3:
                    result.op = '-';
                    result.answer =  a - b;
                    break;
            default:
                    PrintLine("illegal operator");
                    result.op = '\0';
                    return;
            }
            O = result;
        }
        

        /// <summary>
        /// Run a quiz against the user!
        /// </summary>
        /// <returns>true if the user answers correctly, else false.</returns>
        public static bool Quiz()
        {
            Random rng = new Random();
            int a, b;
            a = rng.Next(1, 16);
            b = rng.Next(1, 16);

            int op = rng.Next(4);

            GetResult(a, b, op);

            if (O.op == '\0') {
                return false;
            }

            string problem = $"{a} {O.op} {b}";

            /* Display problem and prompt for input */
            Print($"Problem: {problem} = ");

            int userAnswer;
            while (!int.TryParse(ReadLine(), out userAnswer))
            {
                PrintLine("Enter an INTEGER");
                Print($"Problem: {problem} = ");
            }

            if (userAnswer == O.answer) {
                return true;
            } else {
                Report r = new Report(problem, O.answer, userAnswer);
                mistakes.Add(r);
                return false;
            }
        }
    }
}
