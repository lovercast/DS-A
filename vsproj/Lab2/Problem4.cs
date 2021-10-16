using System;
using System.Diagnostics;

// Collaborated with Kaid'dub. Some ideas from Richard.

// Overall running time: O(n), where n = min(a.Length, b.Length).
namespace Lab2
{
    class Problem4
    {
        // Collaborated with Richard and Kaid'dub
        public static void TestLevenshtein1()
        {
            string[] tests = { "pale ple", "pales pale", "pale bale", "pale bake" };
            bool[] want = { true, true, true, false };

            bool result;
            int i = 0;
            foreach (string test in tests) {
                result = Problem4.Levenshtein1(test);
                Debug.Assert(want[i] == result); // success win victory royale!
                i++;
            }
        }

        public static bool Levenshtein1(string s)
        {
            bool diff = false;
            string[] arr = s.Split(' ');
            string a = arr[0];
            string b = arr[1];

            int min_len = Math.Min(a.Length, b.Length);

            int i = 0, j = 0;
            // The outer loop and the inner loop are not independent.
            // The inner loop may only run as long as the outer loop may run.
            // For each iteration of the inner loop, the number of possible
            // additional iterations of the outer loop decreases by one.
            // The total number of iterations of loops in the scope beginning
            // with the outer loop is at most min_len, since i and j both increase by 
            // at least one for each iteration of the outer loop. Furthermore,
            // the remaining instructions in this block are all conditionals, assignment operations,
            // or arithmetic operations, and hence O(1). So the total running time of this block is O(n)
            // where n is the length of the shorter of the two strings.
            while (Math.Max(i, j) < min_len) {
                while (Math.Max(i, j) < min_len && a[i] == b[j]) {
                    ++i;
                    ++j;
                }

                if (Math.Max(i, j) < min_len && a[i] != b[j]) {                     // We've encountered a difference!
                    if (diff)                           // Have we already? If so, quit.
                        return false;
                    diff = true;                        // Else, now we have!

                    // Check cases.
                    if (a[i] == b[j + 1]) {             // Case 1:  There's been a deletion in a.
                        ++j;
                    } else if (a[i + 1] == b[j]) {      // Case 2:  There's been an insertion in a.
                        ++i;
                    } else if (a[i + 1] == b[j + 1]) {  // Case 3:  A character has been swapped.
                        ++j;
                        ++i;
                    }
                }

                // Are we at the end of the short word?
                // If yes, then check the lengths.
                if (i >= min_len - 1 || j >= min_len - 1) {

                    if (Math.Abs(a.Length - b.Length) > 1)     // Trivial.
                        return false;
                    else                                // If the length difference is exactly one or zero, we have already dealt with the case 
                        return true;                    // that there is a second difference here. So return true in those cases. 
                }
            }
            return true;
        }
    }
}