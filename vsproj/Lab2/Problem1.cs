using System;
using System.Diagnostics;

// Collaborated with Kaid'dub.

namespace Lab2
{
    class Problem1
    {
        // compute the product of a and b. What is its runtime?
        // part A
        // TOTAL RUNTIME for part A is the sum O(1) + O(b) + O(1) + O(1) = O(b)
        int product(int a, int b)
        {
            int sum = 0; // O(1)

            // sumation and assignment are O(1).
            for (int i = 0; i < b; i++) {
                sum += a;
            }
            return sum; // O(1)
        }

        // part B
        // TOTAL RUNTIME for part B : O(1) + log10(n) * (O(1) + O(1)) = O(log10(n)) 
        //                                              = O(log n / log10) = O(log n).
        int sumDigits(int n)
        {
            int sum = 0; // O(1)
            while (n > 0) { // total loops is equal to log(n)/log(10) = log10(n)
                sum += n % 10; // O(1)
                n /= 10; //O(1)
            }
            return sum; //O(1)
        }
    }
}