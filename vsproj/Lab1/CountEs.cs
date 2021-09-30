using System;
using System.Diagnostics;

namespace Lab1
{
    public class CountEs
    {

        public static void TestCountEs()
        {
            string test = "Welcome to The Evergreen State College!";
            int want = 10;
            int got = Count(test);
            Debug.Assert(want == got);
        }

        public static string ReadInput()
        {
            Console.Write("Please enter an input string: ");
            string input = Console.ReadLine();

            return input;
        }

        public static int Count(string input)
        {
            int count = 0;
            input = input.ToLower(); // is ToLower an in-place transforming function?
            for (int i = 0; i < input.Length; i++) {
                if (input[i] == 'e')
                    count++;
            }
            return count;
        }
    }
}
