using System;
using System.Diagnostics;


// Reverse -- add reverse method to ArrayList class
// Total Runtime: O(arrlist.Count). See Reverse method for details.
namespace Lab2
{
    class Problem3
    {
        public static void TestReverse()
        {
            int[] vals = { 1, 1, 2, 3, 3, 4, 5 };

            ArrayList<int> a = new ArrayList<int>(vals);
            var copy = a.Clone();
            a.Reverse();
            for (int i = 0; i < vals.Length; i++) {
                Debug.Assert(copy[i] == a[a.Count - 1 - i]);
            }
        }
    }
}