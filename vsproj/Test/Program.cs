using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.IO;
// recommended byt not required to name the class the same as file

namespace one
{

    public class Boo
    { 
        public int size { 
            get { return 1; }
            set {;}
        }

    }

    namespace two
    {

        public class Boo // this class shadows the upper one
        {
            public int size { 
                get { return 2; }
                set {; }
            }
        }

        public class Program
        {
            public static void Enumerthing()
            {
                List<int> l = new List<int>{ 1, 2, 3, 4, 5 };
                IEnumerator<int> iter = l.GetEnumerator();

                do
                {
                    Console.WriteLine(iter.Current);
                } while (iter.MoveNext());
                // Console.WriteLine(iter.Current);

                Console.WriteLine(l);
                one.Boo b1 = new one.Boo();
                Boo b2 = new Boo();
                Console.WriteLine( b1.size);
                Console.WriteLine(b2.size);
            }

            // do not use Console.Read() -- returns a character.
            public static void Main()
            {

                Console.WriteLine(CountEs.TestCount());

                return;
                

                StringArray vals = new StringArray();

                // how do you initialize a string so that it will pass?
                // just use do while
                string input = new string("");

                do
                {
                    Console.Write("Next String (blank to quit): ");
                    input = Console.ReadLine();
                    if (input == "")
                        break;
                    vals.Append(input);
                } while (input != "");


                Console.WriteLine("Values entered: ");
                vals.Display();

                // Console.WriteLine("Randomize:");
                vals.Randomize();

                Console.WriteLine("currently in vals:");

                vals.Display();


                Console.WriteLine("==============================");

                string filename = "outfile.txt";

                vals.WriteToFile2(filename);

                StringArray newarr = new StringArray();
                newarr.Load2("outfile.txt");

                Console.WriteLine("loaded values:\n");
                newarr.Display();

                Console.WriteLine("Success!");
            }
        }

        // is this public or private by default
        // this is our API -- this is the interface that is available to users.
        class StringArray
        { 
            // access modifiers
            public List<string> Contents { get; set; }
            public int Length { get; set; }
            public int Capacity { get; set; }

            public StringArray()
            {
                this.Contents = new List<string>();
                this.Length = 0;
                this.Capacity = 0;
            }

            public StringArray(string[] list)
            {
                this.Contents = new List<string>(list);
                this.Length = this.Contents.Count;
                this.Capacity = this.Contents.Capacity;
            }

            // is string a nullable type in C# by default? I think no.
            public void Append(string newstr)
            {
                Contents.Add(newstr);
            }

            /// <summary>
            /// Prompt the user for values to enter into a StringArray.
            /// </summary>
            public void Initialize()
            { 
            
            }

            /// <summary>
            /// Print values to stdout.
            /// </summary>
            public void Display()
            { 
                foreach (string s in this.Contents) {
                    Console.WriteLine(s);
                }
            }



            /// <summary>
            /// Shuffle the values in an array. 
            /// Iterate over the indices, choose a random position.
            /// Swap.
            /// </summary>
            public void Randomize()
            {
                int loops = 100;
                //string[] asarray = this.Contents.ToArray();
                Random rng = new Random();
                int i, j;
                for (int iter = 0; iter < loops; iter++) {
                    i = rng.Next(this.Contents.Count);
                    j = rng.Next(this.Contents.Count);
                    Swap(i, j);
                }

                //this.Contents = new List<string>(asarray);
            }

        private static int Max(int a, int b)
        {
            return a >= b ? a : b;
        }

            private void Swap(int i, int j)
            {
                string temp;

                if (this.Contents.Count <= Max(i, j))
                    return; // do nothing
                temp = this.Contents[i];
                this.Contents[i] = this.Contents[j];
                this.Contents[j] = temp;
            }

            /// <summary>
            /// Write the contents of a StringArray to a file 
            /// with path given by filename.
            /// </summary>
            /// <param name="filename">The filepath to write the StringArray to.</param>
            public void WriteToFile(string filename)
            {
                try
                {
                    File.Create(filename);
                }
                catch (Exception e) {
                    Console.WriteLine(e.Message);
                }
                File.WriteAllLines(filename, this.Contents);
            }

            public void WriteToFile2(string filename)
            {
                // StreamWriter sw = new StreamWriter(filename);
                using (StreamWriter sw = new StreamWriter(filename)) {
                    foreach (string line in this.Contents)
                    {
                        sw.WriteLine(line);
                    }

                    // sw.Flush();
                }
            }

            /// <summary>
            /// Append the contents of a StringArray file to the 
            /// end of a StringArray instance.
            /// </summary>
            /// <param name="filename">The file path to load from.</param>
            public void LoadFromFile(string filename)
            {
                try
                {
                    this.Contents = new List<string>(File.ReadAllLines(filename));
                }
                catch (Exception e) {
                    Console.WriteLine(e.Message);
                }
            }

            public void Load2(string filename)
            {
                try
                {
                    StreamReader reader; // = new StreamReader(filename);
                    string line;
                    using (reader = new StreamReader(filename))
                    {
                        while ((line = reader.ReadLine()) != null)
                        {
                            this.Contents.Add(line);
                        }
                    }
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }

            }
        }
    }


}


// solution file and cs code -- what is the difference between the sln
// sln contains links for 
// project code is opened with sln files -- this is not code 



namespace MyNamespace
{
    /// <summary>
    /// </summary>
    /// <param>myint is a good parameter</param>
    // readonly
    // const
    class MyClass
    { 
        public int myint;
        public readonly int ID;
        public const string name = "hello world!";
        public MyClass()
        {
            this.ID = 12345;
            Console.WriteLine("boo");
        }
    
    }
     
}


