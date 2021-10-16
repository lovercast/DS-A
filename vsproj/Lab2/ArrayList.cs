using System;
using System.Collections.Generic;
using System.Text;

public class ArrayList<T> /* : IList<T> */ where T : IComparable // needs to implement interface methods to ingerit from IList<T>
{
    public int Count { get; private set; } // what does protected do? // readonly defines a compile time constant
    public int Capacity { get; private set; }
    T[] Array;

    public ArrayList(int Cap = 0) // default value!
    {
        Count = Cap;
        Capacity = Cap;
        Array = new T[Count];
    }

    public ArrayList(T[] init_arr)
    {
        Count = init_arr.Length;
        Capacity = init_arr.Length;
        // Array = init_arr; // because it is a reference -- 
        // must copy the input array into the 
        Array = new T[Capacity];
        for (int i = 0; i < Count; i++)
        {
            Array[i] = init_arr[i];
        }
    }

    public T this[int idx] // indexer
    {
        get => Array[idx];  // cool arrow notation!
        set => Array[idx] = value;
    }

    public void Add(T t)
    {
        if (Count == Capacity)
        {
            ResizeArray();
        }
        Array[Count++] = t;
    }

    public void ResizeArray()
    {
        if (Capacity == 0)
            Capacity = 4;
        else
            Capacity *= 2;
        T[] new_arr = new T[Capacity];
        // Array?.CopyTo(new_arr, 0); 
        for (int i = 0; i < Count; i++)
        {
            new_arr[i] = Array[i];
        }
        Array = new_arr; // working with references
    }

    public void Clear()
    {
        Count = 0; // set count to zero -- then new additions will be overwritten over the top of the old elements.
        Capacity = 0;
        Array = null;
        // Array = new T[Capacity];
    }

    // big O, Clone, insert, delete(int value), delete(int index), find()

    public ArrayList<T> Clone()
    {
        if (this == null)
            return null;
        ArrayList<T> new_list = new ArrayList<T>(Capacity);
        Array.CopyTo(new_list.Array, 0);
        new_list.Count = Count;
        return new_list;
    }

    /// <summary>
    /// The inverse of remove, 
    /// insert a specified value at a specified index, default index 0;
    /// Copy elements over 1 space, put new element at index 1.
    /// O(n) running time.
    /// </summary>
    /// <param name="value"></param>
    public void Insert(T value, int idx = 0)
    {
        if (idx < 0 || idx >= Count)
            throw new IndexOutOfRangeException($"Invalid index {idx}. Index must be less than Count: {Count}.");
        if (Count == Capacity)
            ResizeArray();
        // System.Array.Copy(Array, idx, Array, idx+1, Count-idx);
        for (int i = Count; i > idx; i--)
        {
            Array[i] = Array[i - 1];
        }
        ++Count;
        Array[idx] = value;
    }

    /// <summary>
    /// Remove the first instance of
    /// the specified value from the array.
    /// </summary>
    public void Remove(T value)
    {
        int idx = Find(value);
        RemoveAt(idx);
    }

    /// <summary>
    /// Remove the value in Array at a specified index
    /// Copy elements from after value onto the array.
    /// </summary>
    public void RemoveAt(int idx)
    {
        if (idx < 0 || idx >= Count)
            throw new IndexOutOfRangeException($"Invalid index: {idx}. Index must be less than Count: {Count}.");
        // System.Array.Copy(Array, idx+1, Array, idx, Count-1-idx);
        for (int i = idx; i < Count - 1; i++)
        {
            Array[i] = Array[i + 1];
        }
        --Count;
    }

    public int Find(T value)
    {
        for (int i = 0; i < Count; i++)
        {
            if (Array[i].CompareTo(value) == 0)
                return i;
        }
        return -1;
    }

    public override string ToString() // must 
    {
        StringBuilder s = new StringBuilder();
        s.Append('[');
        int width = 50;
        for (int i = 0; i < Count; i++)
        {
            s.Append($" {Array[i]}");
            if (i % width == width - 1)
                s.Append('\n');

        }
        s.Append(']');
        return s.ToString();
    }

    public void Display()
    {
        Console.WriteLine($"Count: {Count}, Capacity: {Capacity}");
        Console.WriteLine(this);
    }

    // iterate over array -- keep track of how many deletions so far, relocate elements left by
    // number of deletions so far.
    // keep a dictionary of elements seen so far
    // space-time trade off.
    //
    // This program consists mostly of constant time operations: arithmetic, assignment, conditional.
    // The dictionary implementation from the standard library is a dynamic chained hash table.
    // The dictionary operations ContainsKey and Add are thus amortized constant time.
    // The limiting factor for the performance is as usual the for loop, which runs 
    // for a total of Count iterations.
    // Total Runtime: O(n), where n is Count.
    public void RemoveDuplicate()
    {
        Dictionary<T, bool> d = new Dictionary<T, bool>();

        int deletions = 0;
        int i;
        for (i = 0; i < Count; ++i) {  // O(n)
            if (d.ContainsKey(Array[i])) {
                ++deletions;
            } else {
                d.Add(Array[i], true); 
                if (deletions > 0) 
                    Array[i - deletions] = Array[i];
            }
            // if not 0, shift to the left by deletions places
            
        }
        Count -= deletions; // O(1).
    }

    public void Swap(int i, int j)
    {
        T tem = Array[i];
        Array[i] = Array[j];
        Array[j] = tem;
    }

    // Runtime O(n/2) = O(n), where n is the length of the array.
    public void Reverse()
    { 
        for (int i = 0, j = Count-1; i < j; i+=1, j-=1) 
            Swap(i, j); // O(1).
    }
}
