using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace CrackingLab1
{
    class Program
    {
        static void Main(string[] args)
        {
            Unique.TestUnique();                /* Problem 1 */
            Permutation.TestIsPermutation();    /* Problem 2 */
            Compress.TestStringCompress();      /* Problem 3 */
        }
    }

    // determine if a string has all unique characters
    class Unique
    {

        public static void TestUnique()
        {
            string a, b;
            bool want, got;

            a = "hello world";
            b = "abcdefghijklmnopqrstuvwxyz";

            want = false;
            got = DictUnique(a);
            Debug.Assert(want == got);

            want = true;
            got = DictUnique(b);
            Debug.Assert(want == got);
        }

        /// <summary>
        /// Permutations!
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static bool IsUnique(string input)
        {

            for (int i = 0; i < input.Length; i++) {
                for (int j = 0; j < input.Length; j++) {
                    if (input[i] == input[j] && i != j)
                        return false;
                }
            }
            return true;
        }


        /// <summary>
        /// Uniqueness test using the built-in dictionary data structure.
        /// </summary>
        /// <param name="a"></param>
        /// <returns>true if the string contains only unique characters, else false.</returns>
        public static bool DictUnique(string a)
        {
            // are dictionaries zeroed memory?
            Dictionary<char, int> dict = new Dictionary<char, int>();

            foreach (char c in a) {

                if (dict.ContainsKey(c))
                    return false;
                else
                    dict.Add(c, 1);
            }
            return true;
        }

    }

    class Permutation
    {

        public static void TestIsPermutation()
        {
            string a, b;
            a = "deschutes";
            b = "deutsches";
            Debug.Assert(IsPermutation(a, b));
            a = "hello";
            b = "olleh dlrow";
            Debug.Assert(!IsPermutation(a, b));
            a = "hello world, it's nice to meet you!";
            b = "hello world, it's nice to meet you?";
            Debug.Assert(!IsPermutation(a, b));
        }

        /// <summary>
        /// Test permutation using canonicalization.
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns>true if the strings are permutations, false otherwise.</returns>
        public static bool IsPermutation(string a, string b)
        {
            if (a.Length != b.Length)
                return false;

            char[] arra = a.ToCharArray();
            char[] arrb = b.ToCharArray();

            Array.Sort(arra);
            Array.Sort(arrb);

            for (int i = 0; i < arra.Length; i++) {
                if (arra[i] != arrb[i])
                    return false;
            }

            return true;
        }

    }

    class Compress
    {

        public static void TestStringCompress()
        {
            string test = "aabcccccaaa";
            string want = "a2bc5a3";
            string got = StringCompress(test);
            Debug.Assert(want == got);

        }

        /* Output string will be at least as short since its index
         * only increments twice inside the loop if the index for the input string
         * increments at least twice. */
        /// <summary>
        /// A naive string compression algorithm.
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static string StringCompress(string input)
        {

            char c;
            char[] chars = new char[input.Length];
            int count;
            string intstr; /* helper to store the string representation of count */

            int i = 0, j = 0;

            while (i < input.Length)
            {
                c = input[i];
                chars[j++] = c;
                count = 1;

                while (++i < input.Length && input[i] == c) /* ++i first else buffer overflow */
                    count++;

                if (count > 1)
                {
                    intstr = Convert.ToString(count);
                    foreach (char ch in intstr)
                        chars[j++] = ch;
                }
            }

            char[] result = new char[j];
            for (i = 0; i < j; i++)
                result[i] = chars[i];

            return (result.Length < input.Length) ? string.Join("", result) : input;
        }
    }
}
