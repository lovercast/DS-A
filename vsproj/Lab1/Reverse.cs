using System;
using System.Diagnostics;

/* Accept an input string from the user (With no punctuation!) and
 * reverse that string and write it to the console. */
namespace Lab1
{
    public class Reverse
    {

        public static void TestRevString()
        {
            string test = "welcome to the evergreen state college";
            string want = "college state evergreen the to welcome";
            string got = RevString(test);
            Debug.Assert(want == got); // string comparison with ==  in C#

        }

        /* accept an input string from the user, split, reverse the array and join, display to console. */
        public static string RevString(string input)
        {
            string[] words = input.Split(' ');

            int i, j;
            string tem;
            for (i = 0, j = words.Length - 1; i < j; i += 1, j -= 1)
            {
                tem = words[i];
                words[i] = words[j];
                words[j] = tem;
            }

            return String.Join(' ', words);
        }


    }
}
