using System;
using System.Diagnostics;

// I collaborated with Kaid'dub on problems 1 & 4.

namespace Lab2
{
    class Program
    {
        static void Main(string[] args)
        {
            // Program runtimes: see other files and ArrayList.cs for detailed explanations.
            // Problem 1 Part A runs in O(b) time. 
            //           Part B runs in O(logn) time.

            // Problem 2 runs in O(arrlist.Count)  time.
            Problem2.TestRemoveDuplicate();

            // Problem 3 runs in again O(arrlist.Count) time.
            Problem3.TestReverse();

            // Problem 4 runs in O(min(a.len, b.len)) time.
            Problem4.TestLevenshtein1(); 
        }
    }
}
