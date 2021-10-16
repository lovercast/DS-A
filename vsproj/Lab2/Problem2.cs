using System;
using System.Diagnostics;

// add RemoveDuplicate method to ArrayList class
// Total Runtime: O(arrlist.Count). See RemoveDuplicate method for details.
namespace Lab2
{
    class Problem2
    {
        public static void TestRemoveDuplicate()
        {
            int[] vals = { 1, 1, 2, 3, 3, 4, 5 };
            ArrayList<int> a = new ArrayList<int>(vals);
            a.RemoveDuplicate();
            Debug.Assert(a.Count == 5);
            for (int i = 0; i < 5; i++) {
                Debug.Assert(a[i] == i + 1);
            }
        }

    }
}