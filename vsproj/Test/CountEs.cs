using System;

namespace Test
{
    public class CountEs
    {

        public static bool TestCount()
        { 
            string testinput = "helloworld E1234eee";
            int want = 5;
            int got = Count(testinput);
            return want == got;
        }

        public static int Count(string input)
        {
            /* count es in string */
            /* E and e */
            string s = input.ToLower();

            int count = 0;
            foreach (char c in s) {
                if (c == 'e')
                    ++count;
            }

            return count;
        }

    }
}
