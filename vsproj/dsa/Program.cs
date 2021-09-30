// write a program with 
// loops
// methods
// classes
// breakpoints/debugging

using System;
using System.Diagnostics;

/* 
 * An example class.
 */

namespace dsa
{
    public class Program
    {
        public static void Main(string[] args)
        {

            string instr = Console.ReadLine();
            int numRands = 100;
            int[] randsArray = GetRands(numRands);
            //Console.WriteLine(randsArray.ToString());
            // Console.WriteLine(randsArray.Length);
            Console.WriteLine(instr);
            Console.WriteLine(counte(instr));
        }

        public static int counte(string instr)
        {
            int count = 0;
            instr = instr.ToLower();
            foreach (char letter in instr) {
                if (letter == 'e')
                    count++;
            }
            return count;
        }

        static void GetInput(string input)
        { 

        }

        static int[] GetRands(int numRands)
        {
            try {
                int[] nums = new int[numRands];
                    int n;
                    Random myRand = new Random();
                    for (int i = 0; i < numRands; i++) {
                        n = myRand.Next(0,100);
                        nums[i] = n;
                        // Console.Write("{0} ", myRand.Next(0, 100));
                        if (i % 10 == 9)
                            Console.Write("\n");
                    }
                    return nums;

             }
            catch (Exception e) {
                Console.WriteLine(e.ToString());
            }
            return null;
        }

        /* program that prints over a loop */

        static void PrintStuff()
        {
            int[] collection = { 1, 2, 3, 4, 5 };
            foreach (var i in collection)
            {
                Console.WriteLine(i);
            }

            int j;
            for (j = 0; j < 26; j++)
            {
                switch (j % 3)
                {
                    case 0:
                        Console.WriteLine("{0} mod 3", j % 3);
                        break;
                    case 1:
                        Console.WriteLine("{0} mod 3", j % 3);
                        break;
                    case 2:
                        Console.WriteLine("{0} mod 3", j % 3);
                        break;
                }
            }
        }
    }
}
