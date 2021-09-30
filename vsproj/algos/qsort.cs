using System;
using System.Diagnostics;

namespace algos
{
    public class QuickSort
    {
        private static int partition(int[] arr, int left, int right)
        {
            int i, last;
            i = last = left;

            int pivot = arr[i];

            for (i = left+1; i <= right; i++) {
                if (arr[i] <= pivot)
                    swap(arr, i, ++last);
            }
            swap(arr, left, last);
            return last;
        }

        public static void Qsort(int[] arr, int left, int right)
        {
            int pivot;
            if (left < right) {

                pivot = partition(arr, left, right);
                Qsort(arr, left, pivot - 1);
                Qsort(arr, pivot+1, right);
            }
        }

        private static void swap(int[] arr, int i, int j)
        {
            int tem = arr[i];
            arr[i] = arr[j];
            arr[j] = tem;
        }

        public static void Test_Qsort()
        {
            int[] arr = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
            int[] sorted = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            Qsort(arr, 0, arr.Length-1);
            Console.WriteLine(arr);
            Debug.Assert(arr == sorted);

        }
    }
}
